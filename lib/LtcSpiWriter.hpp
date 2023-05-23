/*
 * LtcSpiWriter.hpp
 *
 *  Created on: May 16, 2023
 *      Author: Piotr Lesicki
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIWRITER_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIWRITER_HPP_

#include "LTC6811Lib.hpp"
#include "PEC15.hpp"

#include <main.h>
#include <array>

//chain size - number of ltc in the chain
template < size_t chain_size >
class LtcSpiWriter
{
	public:
		LtcSpiWriter() = delete;
		/*
		 *	hspi -
		 *	port - spi chip select port
		 *	pin - spi chip select pin
		 */
		LtcSpiWriter(SPI_HandleTypeDef* hspi, GPIO_TypeDef* port, uint16_t pin) : hspi(hspi), port(port), pin(pin) { };

		/*
		 * HALTING FUNCTION
		 * generates wake up signals using CS and HAL_delay(chain_size)
		 */
		void wakeUp()
		{
			//TODO: wakeup using rdcnfg
		}
		/*
		 * use every 2 sec in the case no valid command is scheduled
		 * to be send
		 */
		void handleWatchDog()
		{
			//TODO: also some dummy rdcnfg?
		}
		/*
		 * direct write overrides current mem
		 */
		void rawWrite(WCmd cmd, std::array< WrRdRegisterGroup*, chain_size > data)
		{
			std::array < uint8_t, 2 > scmd = cmd.serialize();
			std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

			std::array < std::array < uint8_t, 6 >, chain_size > sdata;
			for (size_t i = 0; i < chain_size; i++) sdata[i] = data[i]->serialize();
			std::array < std::array < uint8_t, 2 >, chain_size > sdata_pec;
			for(size_t i = 0; i < chain_size; i++)
			{
				sdata_pec[i] = calcPEC(sdata[i]);
			}
			//cmd + cmd_pec + reg_group + reg_pec = 2 + 2 + chain_size * 6 + chain_size * 2;
			std::array < uint8_t, chain_size * 8 + 4 > rdata;
			rdata[0] = scmd[0];
			rdata[1] = scmd[1];

			startComm();
			//HAL_SPI_Transmit(hspi, , , 1000);
			stopComm();
		}

	private:
		void startComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_RESET); }
		void stopComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_SET); }

		SPI_HandleTypeDef* hspi;
		GPIO_TypeDef* port;
		uint16_t pin;
};

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIWRITER_HPP_ */
