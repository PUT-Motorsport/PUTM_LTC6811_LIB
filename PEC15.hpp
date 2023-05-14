/************************************
Copyright 2012 Linear Technology Corp. (LTC)
Permission to freely use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies:
THIS SOFTWARE IS PROVIDED “AS IS” AND LTC DISCLAIMS ALL WARRANTIES
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO
EVENT SHALL LTC BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM ANY USE OF SAME, INCLUDING
ANY LOSS OF USE OR DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE
OR OTHER TORTUOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
PERFORMANCE OF THIS SOFTWARE.
***********************************************************/

#ifndef INC_PUTM_LTC_6811_LTC6804_LIB_PEC15_HPP_
#define INC_PUTM_LTC_6811_LTC6804_LIB_PEC15_HPP_

int16 pec15_table[256];

constexpr int16 CRC15_POLY = 0x4599;

void initPEC15Table()
{
	for (int i = 0; i < 256; i++)
	{
		remainder = i << 7;
		for (int bit = 8; bit > 0; --bit)
		{
			if (remainder & 0x4000)
			{
				remainder = ((remainder << 1));
				remainder = (remainder ^ CRC15_POLY);
			}
			else
			{
				remainder = ((remainder << 1));
			}
		}
		pec15_table[i] = remainder & 0xffff;
	}
}

uint16_t pec15 (uint8_t *data , int len)
{
	int16 remainder;
	int16 address;
	remainder = 16;
	for (int i = 0; i < len; i++)
	{
		address = ((remainder >> 7) ^ data[i]) & 0xff;
		remainder = (remainder << 8 ) ^ pec15_table[address];
	}
	return (remainder * 2);
}


#endif /* INC_PUTM_LTC_6811_LTC6804_LIB_PEC15_HPP_ */

/*
 * *sees code in doc*
 * immediately slaps ctrl+c
 */
