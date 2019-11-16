/************************************/
/*    DS worksheet / matan keler    */
/*  bit wise worksheet, functions   */
/************************************/

#include "barr.h" /* my header file */

/* count set bits */
size_t BitArrayCountOn(unsigned long bit_array)
{
	size_t i = 0, mask = 1, count = 0;

	for (i = 0; i < LENGTH; i++)
	{
		if (bit_array & mask)
		{
			++count;
		}

		mask <<= 1;
	}

	return count;
}

/* count unset bits */
size_t BitArrayCountOff(unsigned long bit_array)
{
	return (LENGTH - (BitArrayCountOn(bit_array) ) );
}

/* check if indexed bit is set, index can be number between 0 to 64 */
int BitArrayIsOn(unsigned long bit_array, size_t index)
{
	assert(index < LENGTH);

	return ((bit_array & (1UL << index)) != 0) ? 1 : 0;
}

/* check if indexed bit is set off, index can be number between 0 to 64 */
int BitArrayIsOff(unsigned long bit_array, size_t index)
{
	assert(index < LENGTH);

	return (bit_array & ((unsigned long)1 << index)) ? 1 : 0;	
}

/* set indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetOn(unsigned long bit_array, size_t index)
{
	assert(index < LENGTH);

	return (bit_array | ((unsigned long)1 << index) );
}

/* set off indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetOff(unsigned long bit_array, size_t index)
{
	assert(index < LENGTH);

	return (bit_array & ~((unsigned long)1 << index) );
}

/* set on/off(by choise) indexed bit, index can be number between 0 to 64 */
unsigned long BitArraySetBit(unsigned long bit_array, size_t index, int value)
{
	assert(index < LENGTH);
	assert((0 == value) || (1 == value));

	return ((1 == value) ? BitArraySetOn(bit_array, index) : BitArraySetOff(bit_array, index) );
}

/* flip indexed bit value, index can be number between 0 to 64 */
unsigned long BitArrayFlipBit(unsigned long bit_array, size_t index)
{
	assert(index < LENGTH);

	return (bit_array ^ ( (unsigned long)1 << index) );
}

/* rotate left, number_of_shifts can be number between 0 to 8 */
unsigned long BitArrayRotL(unsigned long bit_array, size_t number_of_shifts)
{
	bit_array = (bit_array >> (LENGTH - number_of_shifts)) | (bit_array << number_of_shifts);

	return bit_array;
}

/* rotate right, number_of_shifts can be number between 0 to 8 */
unsigned long BitArrayRotR(unsigned long bit_array, size_t number_of_shifts)
{
	bit_array = (bit_array << (LENGTH - number_of_shifts) ) | (bit_array >> number_of_shifts);

	return bit_array;
}

/* return the mirror version of the number bits */
unsigned long BitArrayMirror(unsigned long bit_array)
{
    unsigned long reverse_num = 0;  /* will hold the value in the end */
    size_t i = 0; 

    /* iterate over num's bits */
    for (i = 0; i < LENGTH; i++) 
    { 
    	/* if the current bit is on we will set the opposite bit on, 
    	                              on the reverse num variable */
        if ((bit_array & ( (unsigned long)1 << i) ) )
        {
           reverse_num |= (unsigned long)1 << ( (LENGTH - 1) - i);   
        }
    } 

    return reverse_num;
}

/* print number bit represantition */
void BitsArrayPrintBinary(unsigned long bit_array)
{
	int i = 0;
	unsigned long  buffer = 0;

    /* iterate over array from the end to print it correctly to the screen(right to left) */                                                                   
    for (i = (LENGTH - 1); i >= 0; i--)
    {
    	buffer = bit_array & (unsigned long)1 << i;

    	if (buffer == 0)
    	{ 
        	printf("0");
    	}

    	else
    	{
        	printf("1");
 		}

    }

    printf("\n");
}

/* init LUT for counting set bits */
void InitCountLUT()
{
	int i = 0;

	/* count flag need to be unset */
	assert (0 == is_count_lut_init);

	for (; i < LUT_LENGTH; i++)
	{
		count_lut[i] = BitArrayCountOn(i);
	}

	count_lut[LUT_LENGTH] = '\0';
	is_count_lut_init = 1;
}

/* init LUT for mirror fucntion */
void InitMirrorLUT()
{
	int i = 0;
	unsigned long number_after_mirror = 0;
	char *ptr = NULL;

	/* count flag need to be unset */
	assert (0 == is_mirror_lut_init);

	for (; i < LUT_LENGTH; i++)
	{	
		number_after_mirror = BitArrayMirror(i);
		ptr = (char*)&number_after_mirror;
		ptr += BYTE_BITS - 1; /* to get the value */
		mirror_lut[i] = (char)*ptr; 
	}

	/* end of string */
	mirror_lut[LUT_LENGTH] = '\0';

	/* set mirror flag */
	is_mirror_lut_init = 1;
}

/* count set bits with lut */
size_t BitArrayCountOnLUT(unsigned long bit_array)
{
	size_t sum = 0;
	unsigned char *ptr = NULL;

	/* count init flag need to be set */
	assert(1 == is_count_lut_init);

	/* sum the counts from the lut */
	while (bit_array)
	{
		ptr = (unsigned char*)&bit_array;
		sum += count_lut[*ptr];

		/* shift left to keep check */
		bit_array >>= BYTE_BITS;
	}

	return sum;

}

/* mirror with lut */
unsigned long BitArrayMirrorLUT(unsigned long bit_array)
{
	unsigned char *ptr_bit_array = NULL;
	unsigned char *ptr_new_number = NULL;
	unsigned long new_number = 0;
	unsigned long i = 0;

	/* mirror init flag need to be set */
	assert(1 == is_mirror_lut_init);

	/* pointers to the numbers */
	ptr_bit_array = (unsigned char*)&bit_array;
	ptr_new_number = (unsigned char*)&new_number;

	/* put mirrored value from bit array in new number */
	for (; i < ((LENGTH) / (BYTE_BITS)) ; i++)
	{
		*(ptr_new_number + BYTE_BITS - 1 - i) = (unsigned long)(mirror_lut[*(ptr_bit_array + i) ] );
	}

	return new_number;
}