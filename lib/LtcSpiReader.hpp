/*
 * LtcSpiReader.hpp
 *
 *  Created on: May 16, 2023
 *      Author: Piotr Lesicki
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_

#include "LTC6811Lib.hpp"

#include <main.h>

class LtcSpiReader
{
	public:
		LtcSpiReader() = delete;
		LtcSpiReader(SPI_HandleTypeDef* hspi, GPIO_TypeDef* port, uint16_t pin) : hspi(hspi), port(port), pin(pin) { };

	private:
		void startComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_RESET); }
		void stopComm() { HAL_GPIO_WritePin(port, pin, GPIO_PinState::GPIO_PIN_SET); }

		SPI_HandleTypeDef* hspi;
		GPIO_TypeDef* port;
		uint16_t pin;
};

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_LIB_LTCSPIREADER_HPP_ */
