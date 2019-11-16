#include <assert.h> /* assert     */
#include <sys/resource.h>
#include <stdlib.h> /* allocation */
#include "trie.h"   /* trie       */
#include "dhcp.h"   /* my header  */

#define MAX_NUM_OF_BITS 32
#define RESTRICED_IPS_NUM 3
#define CANNOT_REMOVE_RESTRICTED_IP 4
/*** the unrestricted ips are always the same? it doesnt depend on the subnet size? ***/
unsigned int restricted_ips[3] = {0x00000000, 0xFFFFFFFF, 0xFFFFFFFE};
                 /*** use here the macro RESTRICED_IPS_NUM***/
struct dhcp_t
{
	trie_t *adress_pool;
	unsigned char prefix;
	unsigned int subnet;
};

static void InsertRestrictedIps(dhcp_t *dhcp)
{
	int i = 0;
	int status = 0;

	assert(dhcp);

	for (i = 0; i < RESTRICED_IPS_NUM; ++i)
	{
		status = TrieInsert(dhcp->adress_pool, restricted_ips[i]);
		assert(0 == status);
	}
}

/*
* create a dhcp.
* Arguments: subnet, number of constant bits on the ip.
* Return value: dhcp, or NULL in case of failure.
*/
dhcp_t *DHCPCreate(unsigned int subnet, size_t prefix)
{
	dhcp_t *dhcp = malloc(sizeof(dhcp_t));
	/*** unecessary space ***/
	if (!dhcp)
	{
		return NULL;
	}

	dhcp->adress_pool = TrieCreateRoot(MAX_NUM_OF_BITS - 
		                                         prefix);
	if (!dhcp->adress_pool)
	{
		free(dhcp);
		dhcp = NULL;

		return NULL;
	}

	dhcp->prefix = prefix;
	dhcp->subnet = subnet;
	InsertRestrictedIps(dhcp); /*** check for fail? ***/

	return dhcp;
}

/*
* Destroy the dhcp.
* Arguments: dhcp.
* Return value: none.
*/
void DCHPDestroy(dhcp_t *dhcp)
{
	assert(dhcp);

	TrieDestory(dhcp->adress_pool);
	free(dhcp);
	dhcp = NULL;
}

/*
* free specific ip from use.
* Arguments: dhcp, ip to release.
* Returned value: success / fail status.
*/
int DHCPFreeIp(dhcp_t *dhcp, ip_addr_t *ip)
{
	ip_addr_t new_ip = 0;
	int num_of_shifts = 0;
	int i = 0;

	assert(dhcp);
	assert(ip);

	num_of_shifts = MAX_NUM_OF_BITS - dhcp->prefix;
	for (i = 0; i < 3; ++i)
	{
		new_ip = restricted_ips[i];
		new_ip <<= num_of_shifts;
		new_ip >>= num_of_shifts;
		new_ip |= dhcp->subnet;

		if (new_ip == *ip)
		{
			return (CANNOT_REMOVE_RESTRICTED_IP);
		}
	}

	return (TrieRemove(dhcp->adress_pool, *ip));
}

/*
* count the free ips avliable.
* Arguments: dhcp.
* Returned value: free ip number.
*/
size_t DHCPCountFree(dhcp_t *dhcp)
{
	unsigned int max_possible_addresses = 0;

	assert(dhcp);

	max_possible_addresses = MAX_NUM_OF_BITS - dhcp->prefix;
	max_possible_addresses = 1 << max_possible_addresses;

	return (max_possible_addresses - TrieCountInsertedNums(dhcp->adress_pool));
}

/*
* alloc an ip to a user, if the ip is free alloc it otherwise alloc close one.
* Arguments: dhcp, wanted ip, retunred ip.
* Returned value: success / fail, and returned ip.
*/
int DHCPAllocIp(dhcp_t *dhcp, ip_addr_t const requested_ip,
	            ip_addr_t *returned_ip)
{
	int res = 0;

	assert(dhcp);
	assert(returned_ip);

	*returned_ip = TrieNextUnusedNumber(dhcp->adress_pool, requested_ip);
	res = TrieInsert(dhcp->adress_pool, *returned_ip);

	return ((*returned_ip == 0 || res != 0) ? (FAIL) : (SUCCESS));
}

/*
* return the memory consumption of the dhcp.
* Arguments: dhcp.
* Returned value: memory consumption size.
*/
size_t DHCPMemoryConsumption(dhcp_t *dhcp)
{
	assert(dhcp);

	return (TrieCalcSize(dhcp->adress_pool) + sizeof(dhcp_t));
}