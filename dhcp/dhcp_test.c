#include <stdio.h> /* for print() */

#include "dhcp.h"

#define FALSE 0
#define TRUE 1
#define ADRESSES_AMOUNT 6
#define BITS_IN_BYTE 8
#define BITS_IN_BIT_ARRAY (sizeof(ip_addr_t) * sizeof(size_t))
#define SUBNET_SIZE (BITS_IN_BYTE * 2)
#define SYSTEM_ADDRESSES 3
#define UNABLE_TO_REMOVE_SYSTEM_ADDRESS 3 
#define OP_SUCCESS 0
#define INVALID_IP 2

void DHCPTest();
void PrintBitArray(ip_addr_t bit_array);
size_t TwoInPowerOfNum(size_t exp);

size_t g_err_count = 0;

int main()
{
	DHCPTest();

	if (!g_err_count)
	{
		printf("\n\33[1;32mAll tests were successful!\33[0;0m\n\n");
	}

	return 0;
}

void DHCPTest()
{
	size_t i = 0;
	size_t init_num_of_free = TwoInPowerOfNum(SUBNET_SIZE) - SYSTEM_ADDRESSES;
	ip_addr_t subnet = 0x82780000;
	ip_addr_t ip_address[ADRESSES_AMOUNT] = {0x82781100, 0x82781000,
	                                         0x82781700, 0x82781C00,
	                                         0x82782800, 0x82783C00};
	ip_addr_t illegal_address = 0x8278FFFF;
	ip_addr_t non_existing_ip = 0x82781200;
	ip_addr_t returned_ip = 0;
	dhcp_t *dhcp = DHCPCreate(subnet, SUBNET_SIZE);

	if (NULL == dhcp)
	{
		printf("\33[1;31mDHCPCreate failed in line %d\33[0;0m\n", __LINE__);
    	++g_err_count;
	}

	printf("DHCP consumption after create: %lu bytes\n", DHCPMemoryConsumption(dhcp));

	if ((DHCPCountFree(dhcp)) != init_num_of_free)
	{
		printf("\33[1;31mDHCPCountFree failed in line %d\33[0;0m\n", __LINE__);
    	++g_err_count;
	}

	/******************************Alloc Test*********************************/
	for (i = 0; i < ADRESSES_AMOUNT; ++i)
	{
		if (DHCPAllocIp(dhcp, ip_address[i], &returned_ip) != OP_SUCCESS)
		{
			printf("\33[1;31mDHCPAllocIp failed in line %d\33[0;0m\n", __LINE__);
	    	++g_err_count;
		}
	}

	/***************************Duplication Test******************************/
	for (i = 0; i < ADRESSES_AMOUNT; ++i)
	{
		if (DHCPAllocIp(dhcp, ip_address[i], &returned_ip) != OP_SUCCESS)
		{
			printf("\33[1;31mDHCPAllocIp failed in line %d\33[0;0m\n", __LINE__);
	    	++g_err_count;
		}
	}

	/*******************************Free Test*********************************/
	for (i = 1; i < 4; ++i)
	{
		int res = 0;
		if ((res = DHCPFreeIp(dhcp, &ip_address[i])) != 0)
		{
			printf("\33[1;31mDHCPFreeIp failed in line %d res was %d\33[0;0m\n", __LINE__, res);
	    	++g_err_count;
		}
	}

	/***************************Free of invalid Test***************************/
	if (DHCPFreeIp(dhcp, &non_existing_ip) != 4)
	{
		printf("\33[1;31mDHCPFreeIp didn't recognize request to remove non existing ip in line %d\33[0;0m\n", __LINE__);
    	++g_err_count;
	}

	/********************Alloc & Free system address test*********************/
	DHCPAllocIp(dhcp, illegal_address, &returned_ip);
	printf("ret %x\n", returned_ip);
	if (returned_ip == illegal_address)
	{
		printf("\33[1;31mDHCPAllocIp failed in line %d\33[0;0m\n", __LINE__);
    	++g_err_count;
	}

	if (DHCPFreeIp(dhcp, &illegal_address) != 4)
	{
		printf("\33[1;31mDHCPFreeIp failed in line %d\33[0;0m\n", __LINE__);
    	++g_err_count;
	}

	printf("DHCP consumption before destroy: %lu bytes\n", 
    DHCPMemoryConsumption(dhcp));

	DCHPDestroy(dhcp);
}

void PrintBitArray(ip_addr_t bit_array)
{
	ip_addr_t mask = 1;

	mask <<= (BITS_IN_BIT_ARRAY - 1);

	while (mask)
	{
		if ((bit_array & mask) == 0)
		{
			printf("0");
		}
		else
		{
			printf("1");
		}

		mask >>= 1;
	}

	printf("\n");
}

size_t TwoInPowerOfNum(size_t exp)
{
	size_t result = 1;

	for ( ; exp > 0; --exp)
	{
		result *= 2;
	}

	return result;
}