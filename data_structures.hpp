/*
 * data_structures.hpp
 *
 *  Created on: May 13, 2023
 *      Author: Piotr Lesicki
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_DATA_STRUCTURES_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_DATA_STRUCTURES_HPP_

#include <cstring>
#include <concepts>

union Byte
{
	uint8_t byte;
	struct
	{
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
	};

	constexpr Byte(uint8_t byte) : byte(byte) { };
	constexpr Byte(uint8_t b7, uint8_t b6, uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) : b0(b0), b1(b1), b2(b2), b3(b3), b4(b4), b5(b5), b6(b6), b7(b7) { };

	operator uint8_t() { return this->byte; }
	operator uint16_t() { return uint16_t(this->byte); }
	operator uint32_t() { return uint32_t(this->byte); }
};

union Word
{
	uint16_t word;
	struct
	{
		Byte byte0;
		Byte byte1;
	};
	struct
	{
		uint8_t b0 : 1;
		uint8_t b1 : 1;
		uint8_t b2 : 1;
		uint8_t b3 : 1;
		uint8_t b4 : 1;
		uint8_t b5 : 1;
		uint8_t b6 : 1;
		uint8_t b7 : 1;
		uint8_t b8 : 1;
		uint8_t b9 : 1;
		uint8_t b10 : 1;
		uint8_t b11 : 1;
		uint8_t b12 : 1;
		uint8_t b13 : 1;
		uint8_t b14 : 1;
		uint8_t b15 : 1;
	};

	constexpr Word(uint16_t word) : word(word) { };
	constexpr Word(uint8_t byte1, uint8_t byte0) : byte0(byte0), byte1(byte1) { };
	constexpr Word(uint8_t b15, uint8_t b14, uint8_t b13, uint8_t b12, uint8_t b11, uint8_t b10, uint8_t b9, uint8_t b8,
		 uint8_t b7, uint8_t b6, uint8_t b5, uint8_t b4, uint8_t b3, uint8_t b2, uint8_t b1, uint8_t b0) :
		 b0(b0), b1(b1), b2(b2), b3(b3), b4(b4), b5(b5), b6(b6), b7(b7),
		 b8(b8), b9(b9), b10(b10), b11(b11), b12(b12), b13(b13), b14(b14), b15(b15)
		 { };

	operator uint16_t() { return uint16_t(this->word); }
	operator uint32_t() { return uint32_t(this->word); }
};

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_DATA_STRUCTURES_HPP_ */
