/****************************************/
/*										*/
/*				  OL 61					*/
/*				  DHCP		     		*/
/*										*/
/****************************************/

#ifndef OL61_DHCP
#define OL61_DHCP

#include <stddef.h> /* size_t */

typedef struct dhcp_t dhcp_t;
typedef unsigned int ip_addr_t;

/*
* create a dhcp.
* Arguments: subnet, number of constant bits on the ip.
* Return value: dhcp, or NULL in case of failure.
*/
dhcp_t *DHCPCreate(unsigned int subnet, size_t num_of_pre_determined);

/*
* Destroy the dhcp.
* Arguments: dhcp.
* Return value: none.
*/
void DCHPDestroy(dhcp_t *dhcp);

/*
* alloc an ip to a user, if the ip is free alloc it otherwise alloc close one.
* Arguments: dhcp, wanted ip, retunred ip.
* Returned value: success / fail, and returned ip.
*/
int DHCPAllocIp(dhcp_t *dhcp, ip_addr_t const requested_ip,
	            ip_addr_t *returned_ip);

/*
* free specific ip from use.
* Arguments: dhcp, ip to release.
* Returned value: success / fail status.
*/
int DHCPFreeIp(dhcp_t *dhcp, ip_addr_t *ip);

/*
* count the free ips avliable.
* Arguments: dhcp.
* Returned value: free ip number.
*/
size_t DHCPCountFree(dhcp_t *dhcp);

/*
* return the memory consumption of the dhcp.
* Arguments: dhcp.
* Returned value: memory consumption size.
*/
size_t DHCPMemoryConsumption(dhcp_t *dhcp);

#endif /* OL61_DHCP */