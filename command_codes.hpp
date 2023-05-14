/*
 * command_codes.hpp
 *
 *  Created on: May 13, 2023
 *      Author: Piotr Lesicki
 */

/*
 * Notes
 *
 * 1. 	Commands using optional parameters are nicely structured
 * 		in way that all optional args have the same shift across
 * 		all commands ex.
 * 		ADCV:  0 1 MD[1] MD[0] 1   1 DCP 0 CH[2] CH[1] CH[0]
 * 		ADOW:  0 1 MD[1] MD[0] PUP 1 DCP 1 CH[2] CH[1] CH[0]
 * 		so all enums use that feature out of convenience. Also
 * 		for easier management and clearer look all enums use the
 * 		following format:precedeing_bits'arg_bits'following_bits
 * 		ex.
 * 		0b00'00'0000000 or 0b00'01'0000000 or 0b00000000'000
 * 		in a case that arg_bits are last
 */

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_COMMAND_CODES_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_COMMAND_CODES_HPP_

#include "data_structures.hpp"
#include "other.hpp"

/*
 * MD[1:0] - ADC - mode
 *
 */
enum struct Mode : uint16_t
{
	Slow 		= 0b00'00'0000000,
	Fast 		= 0b00'01'0000000,
	Normal 		= 0b00'10'0000000,
	Filtered 	= 0b00'11'0000000
};

/*
 * DSP - Discharge Permitted
 */
enum struct Discharge : uint16_t
{
	NotPermited = 0b000000'0'0000,
	Permitted	= 0b000000'1'0000
};

/*
 * CH[2:0] - Cell Selection for ADC Conversion
 */
enum struct Cell : uint16_t
{
	All 		= 0b00000000'000,
	_1_7 		= 0b00000000'001,
	_2_8 		= 0b00000000'010,
	_3_9 		= 0b00000000'011,
	_4_10 		= 0b00000000'100,
	_5_11 		= 0b00000000'101,
	_6_12	 	= 0b00000000'110,
};

/*
 * PUP - Pull-Up/Pull-Down Current for Open Wire
 * Conversions
 */
enum struct Pull : uint16_t
{
	down 		= 0b0000'0'000000,
	up 			= 0b0000'1'000000
};

/*
 * ST[1:0] - Self Test Mode Selection
 */
enum struct TestMode : uint16_t
{
	_1 			= 0b0000'01'00000,
	_2 			= 0b0000'10'00000
};

/*
 * CHG[2:0] - GPIO Selection for ADC Conversion
 */
enum struct Gpio : uint16_t
{
	All 		= 0b00000000'000,
	_1 			= 0b00000000'001,
	_2 			= 0b00000000'010,
	_3 			= 0b00000000'011,
	_4 			= 0b00000000'100,
	_5		 	= 0b00000000'101,
	Ref2 		= 0b00000000'110
};

/*
 * CHST[2:0] - Status Group Selection
 */
enum struct StatusGroup : uint16_t
{
	All 		= 0b00000000'000,
	SC 			= 0b00000000'001,
	ITMP 		= 0b00000000'010,
	VA 			= 0b00000000'011,
	VD 			= 0b00000000'100
};

/*
 * Read/Write Config Register A - B
 */
constexpr Word WRCFGA{ 0b00000000001 };
constexpr Word WRCFGB{ 0b00000100100 };
constexpr Word RDCFGA{ 0b00000000010 };
constexpr Word RDCFGB{ 0b00000100110 };
/*
 * Read Cell Voltage Register A - F
 */
constexpr Word RDCVA{ 0b00000000100 };
constexpr Word RDCVB{ 0b00000000110 };
constexpr Word RDCVC{ 0b00000001000 };
constexpr Word RDCVD{ 0b00000001010 };
constexpr Word RDCVE{ 0b00000001001 };
constexpr Word RDCVF{ 0b00000001011 };
/*
 * Read Auxiliary Register A - D
 */
constexpr Word RDAUXA{ 0b00000001100 };
constexpr Word RDAUXB{ 0b00000001110 };
constexpr Word RDAUXC{ 0b00000001101 };
constexpr Word RDAUXD{ 0b00000001111 };
/*
 * Read Status Register A - B
 */
constexpr Word RDSTATA{ 0b00000010000 };
constexpr Word RDSTATB{ 0b00000010010 };
/*
 * Write S Control Register
 */
constexpr Word WRSCTRL{ 0b00000010100 };
/*
 * Write PWM Register Group
 */
constexpr Word WRPWM{ 0b00000100000 };
/*
 * Write PWM/S Control Register Group B
 */
constexpr Word WRPSB{ 0b00000011100 };
/*
 * Read S Control Register Group
 */
constexpr Word RDSCTRL{ 0b00000010110 };
/*
 * Read PWM Register Group
 */
constexpr Word RDPWM{ 0b00000100010 };
/*
 * Read PWM/S Control Register Group B
 */
constexpr Word RDPSB{ 0b00000111110 };
/*
 * Start S Control Pulsing and Poll Status
 */
constexpr Word STSCTRL{ 0b00000011001 };
/*
 * Clear S Control Register Group
 */
constexpr Word CLRSCTRL{ 0b00000011001 };
/*
 * Clear Cell Voltage Register Groups
 */
constexpr Word CLRCELL{ 0b11100010001 };
/*
 * Clear Auxiliary Register Groups
 */
constexpr Word CLRAUX{ 0b11100010010};
/*
 * Clear Status Register Groups
 */
constexpr Word CLRSTAT{ 0b11100010011 };
/*
 * Poll ADC Conversion Status
 */
constexpr Word PLADC{ 0b11100010100 };
/*
 * Diagnose MUX and Poll Status
 */
constexpr Word DIAGN{ 0b11100010101 };
/*
 * Write COMM Register Group
 */
constexpr Word WRCOMM{ 0b11100100001 };
/*
 * Read COMM Register Group
 */
constexpr Word RDCOMM{ 0b11100100010 };
/*
 * Start I2C / SPI Communication
 */
constexpr Word STCOMM{ 0b11100100011 };
/*
 * Start Cell Voltage ADC Conversion and Poll Status
 */
constexpr inline auto ADCV(Mode mode, Discharge discharge, Cell cell)
{
	return Word{ 0b01001100000 | mode | discharge | cell };
}
/*
 * Start Open Wire ADC Conversion and Poll Status
 */
constexpr inline auto ADOW(Mode mode, Pull pull, Discharge discharge, Cell cell)
{
	return Word{ 0b01000101000 | mode | pull | discharge | cell };
}
/*
 * Start Self Test Cell Voltage Conversion and Poll Status
 */
constexpr inline auto CVST(Mode mode, TestMode tmode)
{
	return Word{ 0b01000000111 | mode | tmode };
}
/*
 * Start Overlap Measurement of Cell 7 Voltage
 */
constexpr inline auto ADOL(Mode mode, Discharge discharge)
{
	return Word{ 0b01000000001 | mode | discharge };
}
/*
 * Start GPIOs ADC Conversion and Poll Status
 */
constexpr inline auto ADAX(Mode mode, Gpio gpio)
{
	return Word{ 0b10001100000 | mode | gpio };
}
/*
 * Start GPIOs ADC Conversion With Digital Redundancy and
 * Poll Status
 */
constexpr inline auto ADAXD(Mode mode, Gpio gpio)
{
	return Word{ 0b10000000000 | mode | gpio };
}
/*
 * Start Self Test GPIOs Conversion and Poll Status
 */
constexpr inline auto AXST(Mode mode, TestMode tmode)
{
	return Word{ 0b10000000111 | mode | tmode };
}
/*
 * Start Self Test GPIOs Conversion and Poll Status
 */
constexpr inline auto ADSTAT(Mode mode, StatusGroup group)
{
	return Word{ 0b10000001000 | mode | group };
}
/*
 * Start Self Test Status Group Conversion and Poll Status
 */
constexpr inline auto STATST(Mode mode, TestMode tmode)
{
	return Word{ 0b10001100111 | mode | tmode };
}
/*
 * Start Combined Cell Voltage and GPIO1, GPIO2
 * Conversion and Poll Status
 */
constexpr inline auto ADCVAX(Mode mode, Discharge discharge)
{
	return Word{ 0b10001101111 | mode | discharge };
}
/*
 * Start Combined Cell Voltage and SC Conversion and Poll
 * Status
 */
constexpr inline auto ADCVSC(Mode mode, Discharge discharge)
{
	return Word{ 0b10001100111 | mode | discharge };
}

#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_COMMAND_CODES_HPP_ */
