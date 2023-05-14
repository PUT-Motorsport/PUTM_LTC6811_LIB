/*
 * other.hpp
 *
 *  Created on: May 14, 2023
 *      Author: user
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_OTHER_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_OTHER_HPP_

#include <concepts>

template < typename E >
	requires std::is_enum_v<E>
uint16_t operator | (uint16_t a, E b)
{
	return a | uint16_t(b);
}

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_OTHER_HPP_ */
