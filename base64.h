/*

  https://github.com/superwills/NibbleAndAHalf -- Fast base64 encoding and decoding.
  version 1.0.1, Feb 1, 2022 812a

  Copyright (C) 2013 William Sherif

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  William Sherif

  YWxsIHlvdXIgYmFzZSBhcmUgYmVsb25nIHRvIHVz

*/

#ifndef BASE64_H
#define BASE64_H

#include <stdio.h>

// The COMPILE-TIME SETTING SAFEBASE64 is really important.
// You need to decide if PARANOIA is more important to you than speed.
//
// SAFEBASE64: Remove this def to NOT check the validity of base64 ascii strings
// before unbase64'ing that string.  If you don't #define SAFEBASE64,
// then the program assumes that all characters in the string sent to unbase64()
// are in the base64 alphabet.  As such if a character is NOT in the base64 alphabet
// your data will be wrong (it will be turned to 0 (as if it were just a base64 'A')).
// Removing this test greatly speeds up unbase64'ing (by about 3-4x).
#define SAFEBASE64
#define isMultipleOf(a,x) (!((a)%x))

/* PAD. Base64 is all about breaking the input into SEXTETS, or 6-bit inputs.
 * If you have 1 byte of input, that's 8 bits, not 6. So it won't fit.
 * We need the input to be a multiple of 6. So 8 bits would be padded
 * by 2 bytes to make the total input size 24 bits, which is divisible by 6.
 * A 2 byte input is 16 bits, which is not divisible by 6. So we pad it
 * by 1 byte to make it 24 bits, which is now divisible by 6.

 * We use modulus 3 bytes above because that's 24 bits, and 24 bits is
 * the lowest number that is both divisible by 6 and 8. We need the final
 * output data is to both be divisible by 6 and 8.
 * 2 gives 1 and 1 gives 2, but 0 gives 0.
 */
#define BASE64PAD(_n_) ((((_n_)%3)&1)<<1) + ((((_n_)%3)&2)>>1)

/* (len+pad) IS divisible by 3
 * So, final length IS a multiple of 4 for a valid base64 string.
 */
#define BASE64LEN(_len_) 4*((_len_) + BASE64PAD(_len_))/3

/* You take the ascii string len and divide it by 4
 * to get the number of 3 octet groups. You then *3 to
 * get #octets total.
 * If len<4, we makes sure you get a flen of 0, because that's not even
 * a valid base64 string at all.
 */
#define UNBASE64LEN(_len_) 3*((_len_)/4)

// Converts any binary data to base64 characters.
// Length of the resultant string is stored in flen
// (you must pass pointer flen).
int base64(const void* binaryData, int len, char *base64String);

// Convert your base64 string haJIh/+ back to binary data.
// len is the string length and should NOT include the null terminator.
// Final size will be stored in flen
// (you must pass pointer flen).
int unbase64(const char* ascii, int len, unsigned char *bin);

// Checks the integrity of a base64 string to make sure it is
// made up of only characters in the base64 alphabet (array b64)
#define isbase64ValidChr( ch ) ( ('0' <= ch && ch <= '9') || \
  ('A' <= ch && ch <= 'Z') || ('a' <= ch && ch <= 'z') || \
  ch=='+' || ch=='/' )  // other 2 valid chars, + ending chrs
// = is NOT considered a valid base64 chr, it's only valid at the end for padding

// Tells you if a string is valid base64, which means it's length is
// a multiple of 4, and it contains only valid base64 chrs.
int base64integrity(const char *ascii, int len);

#endif
