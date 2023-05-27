/*
 * SpiWrapper.hpp
 *
 *  Created on: 24 maj 2023
 *      Author: user
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_LIB_SPIWRAPPER_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_LIB_SPIWRAPPER_HPP_

#include <main.h>

enum struct SpiStatus
{
	Ok 			= 0x00,
	Timeout		= 0x01,
	//PecError 	= 0x02,
	Error		= 0x03,
};

enum struct PecStatus
{
	Ok,
	Error
};

template < size_t size >
inline HAL_StatusTypeDef spiTransmit(SPI_HandleTypeDef *hspi, std::array < uint8_t, size > &darray, uint32_t Timeout)
{
	return HAL_SPI_Transmit(hspi, darray.data(), size, Timeout);
}

template < size_t size >
inline void spiTransmit(SPI_HandleTypeDef *hspi, std::array < uint8_t, size > &darray, uint32_t Timeout, HAL_StatusTypeDef *pstatus)
{
	auto st = HAL_SPI_Transmit(hspi, darray.data(), size, Timeout);
	if(*pstatus < st) *pstatus = st;
}

template < size_t size >
inline HAL_StatusTypeDef spiReceive(SPI_HandleTypeDef *hspi, std::array < uint8_t, size > &darray, uint32_t Timeout)
{
	return HAL_SPI_Receive(hspi, darray.data(), size, Timeout);
}

template < size_t size >
inline void spiReceive(SPI_HandleTypeDef *hspi, std::array < uint8_t, size > &darray, uint32_t Timeout, HAL_StatusTypeDef *pstatus)
{
	auto st = HAL_SPI_Receive(hspi, darray.data(), size, Timeout);
	if(*pstatus < st) *pstatus = st;
}

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_LIB_SPIWRAPPER_HPP_ */
