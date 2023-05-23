/*
 * LtcSpiReader.hpp
 *
 *  Created on: May 16, 2023
 *      Author: Piotr Lesicki
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_

#include "LTC6811Lib.hpp"
#include "PEC15.hpp"

#include <main.h>
#include <array>
#include <type_traits>

//chain size - number of ltc in the chain
template < size_t chain_size >
class LtcSpiReader
{
	public:
		LtcSpiReader() = delete;
		LtcSpiReader(SPI_HandleTypeDef* hspi, GPIO_TypeDef* port, uint16_t pin) : hspi(hspi), port(port), pin(pin) { };

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
		std::array < RdReg, chain_size > rawRead(RCmd cmd)
		{
			std::array < RdReg, chain_size > buff;

			std::array < uint8_t, 2 > scmd = cmd.serialize();
			std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);
			std::array < uint8_t, 4 > tdata { scmd[0], scmd[1], scmd_pec[0], scmd_pec[1] };

			std::array < uint8_t, chain_size * 8 > rdata;
			startComm();
			HAL_SPI_Transmit(hspi, tdata.data(), 4, 1000);
			HAL_SPI_Receive(hspi, rdata.data(), chain_size * 8, 1000);
			stopComm();

			auto rdit = rdata.begin();

			for(auto& reg : buff)
			{
				reg = deserializeRegisterGroup(rdit, rdit + 7);
				//skip 6 cuz reg group has six 8bit subregs + 2 cuz pec
				rdit += 8;
			}

			return buff;
		}

	private:
		void startComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_RESET); }
		void stopComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_SET); }

		SPI_HandleTypeDef* hspi;
		GPIO_TypeDef* port;
		uint16_t pin;
};

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_ */
