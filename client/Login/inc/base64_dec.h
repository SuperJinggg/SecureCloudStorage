/* base64_dec.h */
/*
 *   This file is part of the AVR-Crypto-Lib.
 *   Copyright (C) 2006, 2007, 2008  Daniel Otte (daniel.otte@rub.de)
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#ifndef BASE64_DEC_H_
#define BASE64_DEC_H_

#ifdef  __cplusplus
extern "C" {
#endif

int base64_binlength(char* str, unsigned char strict);
int base64dec(void* dest, char* b64str, unsigned char strict);
#ifdef  __cplusplus
}
#endif
#endif /*BASE64_DEC_H_*/
