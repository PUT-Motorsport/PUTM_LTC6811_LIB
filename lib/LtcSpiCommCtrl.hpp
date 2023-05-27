/*
 * LtcSpiCommCtrl.hpp
 *
 *  Created on: 25 maj 2023
 *      Author: user
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPICOMMCTRL_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPICOMMCTRL_HPP_

#include "LTC6811Lib.hpp"
#include "LTC6811CmdCodes.hpp"
#include "PEC15.hpp"
#include "SpiWrapper.hpp"

#include <main.h>
#include <array>
#include <type_traits>
#include <math.h>
#include <algorithm>

template < size_t chain_size >
class LtcSpiCommCtrl
{
	public:
		LtcSpiCommCtrl() = delete;
		LtcSpiCommCtrl(SPI_HandleTypeDef* hspi, GPIO_TypeDef* port, uint16_t pin) : hspi(hspi), port(port), pin(pin) { };

		/*
		 * direct read
		 */
		template
		<
			class RdReg,
			std::enable_if_t
			<
				std::is_base_of_v < WrRdRegisterGroup, RdReg > ||
				std::is_base_of_v < RdRegisterGroup, RdReg >
			, bool > = true
		>
		HAL_StatusTypeDef rawRead(RCmd cmd, std::array < RdReg, chain_size > &data);

		template
		<
			class RdReg,
			std::enable_if_t
			<
				std::is_base_of_v < WrRdRegisterGroup, RdReg > ||
				std::is_base_of_v < RdRegisterGroup, RdReg >
			, bool > = true
		>
		HAL_StatusTypeDef rawRead(RCmd cmd, std::array < RdReg, chain_size > *data, std::array < PecStatus, chain_size > *pec_status);

		/*
		 * direct write overrides current mem
		 */
		template < class WrRdReg, std::enable_if_t< std::is_base_of_v < WrRdRegisterGroup, WrRdReg >, bool> = true >
		HAL_StatusTypeDef rawWrite(WCmd cmd, std::array< WrRdReg, chain_size > const &data);
		HAL_StatusTypeDef rawWrite(WCmd cmd);

		void wakeUp();
		/*
		 * the ltc will timeout and will go into idle / sleep mode
		 * use every 2 sec in the case no valid command is scheduled
		 * to be send
		 */
		void handleWatchDog();

	private:

		void startComm();
		void stopComm();

		SPI_HandleTypeDef* hspi;
		GPIO_TypeDef* port;
		uint16_t pin;

		bool lock_cs;

		struct Config
		{
			//twake in ms typ: 100us - 300us
			constexpr static inline float twake = 0.3f;
			constexpr static inline uint32_t twake_full = std::clamp(uint32_t(std::ceil(float(chain_size) * twake)), uint32_t(1), uint32_t(UINT32_MAX));
		};
};

template < size_t chain_size >
void LtcSpiCommCtrl< chain_size >::wakeUp()
{
	uint8_t dummy_cmd = 0;
	HAL_SPI_Transmit(hspi, &dummy_cmd, 1, 100);
	HAL_Delay(Config::twake_full);
	HAL_SPI_Transmit(hspi, &dummy_cmd, 1, 100);
	HAL_Delay(Config::twake_full);
}

template < size_t chain_size >
void LtcSpiCommCtrl< chain_size >::handleWatchDog()
{
	//TODO: also some dummy rdcnfg?
}

template < size_t chain_size >
template < class WrRdReg, std::enable_if_t< std::is_base_of_v < WrRdRegisterGroup, WrRdReg >, bool > >
HAL_StatusTypeDef LtcSpiCommCtrl< chain_size >::rawWrite(WCmd cmd, std::array< WrRdReg, chain_size > const &data)
{
	HAL_StatusTypeDef status = HAL_OK;

	std::array < uint8_t, 2 > scmd = cmd.serialize();
	std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

	std::array < uint8_t, chain_size * 8 + 4 > stxdata;
	auto stdit = stxdata.begin();
	std::copy(scmd.begin(), scmd.end(), stdit);
	stdit += 2;
	std::copy(scmd_pec.begin(), scmd_pec.end(), stdit);
	stdit += 2;

	for(auto dit = data.rbegin(); dit != data.rend(); dit++)
	{
		auto sd = serializeRegisterGroup(*dit);
		auto sd_pec = calcPEC(sd);
		std::copy(sd.begin(), sd.end(), stdit);
		std::copy(sd_pec.begin(), sd_pec.end(), stdit);
		stdit += 8;
	}

	startComm();

	spiTransmit(hspi, stxdata, 100, &status);

	stopComm();

	return status;
}

template < size_t chain_size >
HAL_StatusTypeDef LtcSpiCommCtrl< chain_size >::rawWrite(WCmd cmd)
{
	HAL_StatusTypeDef status = HAL_OK;

	std::array < uint8_t, 2 > scmd = cmd.serialize();
	std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

	std::array < uint8_t,  4 > stxdata;
	auto stdit = stxdata.begin();
	std::copy(scmd.begin(), scmd.end(), stdit);
	stdit += 2;
	std::copy(scmd_pec.begin(), scmd_pec.end(), stdit);

	startComm();

	spiTransmit(hspi, stxdata, 100, &status);

	stopComm();

	return status;
}


template < size_t chain_size >
template
<
	class RdReg,
	std::enable_if_t
	<
		std::is_base_of_v < WrRdRegisterGroup, RdReg > ||
		std::is_base_of_v < RdRegisterGroup, RdReg >
	, bool >
>
HAL_StatusTypeDef LtcSpiCommCtrl< chain_size >::rawRead(RCmd cmd, std::array < RdReg, chain_size > &data)
{
	HAL_StatusTypeDef status = HAL_OK;

	std::array < uint8_t, 2 > scmd = cmd.serialize();
	std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

	std::array < uint8_t, 4 > stxdata;
	auto stdit = stxdata.begin();
	std::copy(scmd.begin(), scmd.end(), stdit);
	stdit += 2;
	std::copy(scmd_pec.begin(), scmd_pec.end(), stdit);
	stdit += 2;

	startComm();

	spiTransmit(hspi, stxdata, 100, &status);



	std::array < uint8_t, 6 > rdata;
	std::array < uint8_t, 2 > rpec;

	for(auto& reg : data)
	{
		//for now skip clearing
		//rdata.fill(0);
		//rpec.fill(0);

		spiReceive(hspi, rdata, 100, &status);
		spiReceive(hspi, rpec, 100, &status);
		reg = deserializeRegisterGroup < RdReg >(rdata);
	}


	startComm();

	spiTransmit(hspi, scmd, 100, &status);
	spiTransmit(hspi, scmd_pec, 100, &status);
	stopComm();

	return status;
}

template < size_t chain_size >
template
<
	class RdReg,
	std::enable_if_t
	<
		std::is_base_of_v < WrRdRegisterGroup, RdReg > ||
		std::is_base_of_v < RdRegisterGroup, RdReg >
	, bool >
>
HAL_StatusTypeDef LtcSpiCommCtrl< chain_size >::rawRead(RCmd cmd, std::array < RdReg, chain_size > *data, std::array < PecStatus, chain_size > *pec_status)
{
	HAL_StatusTypeDef status = HAL_OK;

	std::array < uint8_t, 2 > scmd = cmd.serialize();
	std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

	//TODO?: still not sure if the communication should be performed in batches
	//or in bulk

	startComm();

	spiTransmit(hspi, scmd, 100, &status);
	spiTransmit(hspi, scmd_pec, 100, &status);

	std::array < uint8_t, 6 > rdata;
	std::array < uint8_t, 2 > rpec;

	auto pit = pec_status->begin();

	for(auto& reg : *data)
	{
		//for now skip clearing
		//rdata.fill(0);
		//rpec.fill(0);

		spiReceive(hspi, rdata, 100, &status);
		spiReceive(hspi, rpec, 100, &status);

		auto pec = calcPEC(rdata);
		if (pec[0] != rpec[0] && pec[1] != rpec[1])
		{
			*pit = PecStatus::Error;
			rdata = { 0, 0, 0, 0, 0, 0 };
		}
		else
		{
			*pit = PecStatus::Ok;
		}
		pit++;

		reg = deserializeRegisterGroup < RdReg >(rdata);
	}

	stopComm();

	return status;
}


template < size_t chain_size >
void LtcSpiCommCtrl< chain_size >::startComm() { if(!lock_cs) HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_RESET); }

template < size_t chain_size >
void LtcSpiCommCtrl< chain_size >::stopComm() { if(!lock_cs) HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_SET); }

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPICOMMCTRL_HPP_ */
