#include <llprint.h>
#include <cos_debug.h>
#include <cos_types.h>
#include <hypercall.h>
#include "pci_lib.h"

void print_bits(unsigned int num);
void print_addr_bits(unsigned int num);
void unpack_addr(unsigned int addr, unsigned *bus, unsigned *dev, unsigned *fun, unsigned *reg);
void wheels_outl(unsigned short port, unsigned int value);
unsigned int wheels_inl(unsigned short port);

struct data_port {
	u32_t value;
	compcap_t kernel_comp_cap;
};
