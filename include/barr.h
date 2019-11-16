/************************************/
/*    DS worksheet / matan keler    */
/*  bit wise worksheet, header file */
/*  		bitwise APIs			*/ 
/************************************/

#ifndef _BARR /* preproccessor will not copy it again if its already set */
#define _BARR

#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdio.h>  /* IO fucntions */

#define LENGTH sizeof(unsigned long) * 8 /* length of unsigned long in bits */
#define LENGTH_INT sizeof(int) * 8 /* size of int in bits */
#define LUT_LENGTH 256 /* length of lut */
#define BYTE_BITS 8 /* byte is 8 bits */

static unsigned char count_lut[LUT_LENGTH + 1];
static unsigned int is_count_lut_init = 0;
static unsigned char mirror_lut[LUT_LENGTH + 1];
static unsigned int is_mirror_lut_init = 0;

/* count set bits */
size_t BitArrayCountOn(unsigned long bit_array);
/* count unset bits */
size_t BitArrayCountOff(unsigned long bit_array);
/* check if indexed bit is set, index can be number between 0 to 64 */
int BitArrayIsOn(unsigned long bit_array, size_t index);
/* check if indexed bit is set off, index can be number between 0 to 64 */
int BitArrayIsOff(unsigned long bit_array, size_t index);
/* set indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetOn(unsigned long bit_array, size_t index);
/* set off indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetOff(unsigned long bit_array, size_t index);
/* set on/off(by choise) indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetBit(unsigned long bit_array, size_t index, int value);
/* flip indexed bit value, index can be number between 0 to 64 */
unsigned long BitArrayFlipBit(unsigned long bit_array, size_t index);
/* rotate left, number_of_shifts can be number between 0 to 8 */
unsigned long BitArrayRotL(unsigned long bit_array, size_t number_of_shifts);
/* rotate right, number_of_shifts can be number between 0 to 8 */
unsigned long BitArrayRotR(unsigned long bit_array, size_t number_of_shifts);
/* return the mirror version of the number bits, number_of_shifts can be number between 0 to 8 */
unsigned long BitArrayMirror(unsigned long bit_array);
/* print bit represantition */
void BitsArrayPrintBinary(unsigned long bit_array);

/* lut fucntions */
void InitCountLUT();
void InitMirrorLUT();
size_t BitArrayCountOnLUT(unsigned long bit_array);
unsigned long BitArrayMirrorLUT(unsigned long bit_array);

#endif