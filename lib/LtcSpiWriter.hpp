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
#include <type_traits>

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
		template < class WrRdReg, std::enable_if_t< std::is_base_of_v < WrRdRegisterGroup, WrRdReg >, bool> = true >
		void rawWrite(WCmd cmd, std::array< WrRdReg, chain_size > data)
		{
			std::array < uint8_t, 2 > scmd = cmd.serialize();
			std::array < uint8_t, 2 > scmd_pec = calcPEC(scmd);

			std::array < std::array < uint8_t, 6 >, chain_size > sdata;
			for (size_t i = 0; i < chain_size; i++) sdata[i] = serializeRegisterGroup(data[i]);
			std::array < std::array < uint8_t, 2 >, chain_size > sdata_pec;
			for(size_t i = 0; i < chain_size; i++)
			{
				sdata_pec[i] = calcPEC(sdata[i]);
			}
			//TODO?: omit placing data into one array and use tx a few times?
			//cmd + cmd_pec + reg_group + reg_pec = 2 + 2 + chain_size * 6 + chain_size * 2;
			std::array < uint8_t, chain_size * 8 + 4 > tdata;
			tdata[0] = scmd[0];
			tdata[1] = scmd[1];
			tdata[2] = scmd_pec[0];
			tdata[3] = scmd_pec[1];
			for (size_t i = 0; i < chain_size; i++)
			{
				//TODO?: memcpy xd?
				for(size_t j = 0; j < 6; j++)
				{
					//addr element_index + register_row + shift_4 cuz cmd and cmd_pec
					tdata[i + j + 4] = sdata[i][j];
				}
				//addr element_index + shift_9 cuz prev shit
				tdata[i + 9 ] = sdata_pec[i][0];
				tdata[i + 10] = sdata_pec[i][1];
			}
			startComm();
			HAL_SPI_Transmit(hspi, tdata.data(), chain_size * 8 + 4, 1000);
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
