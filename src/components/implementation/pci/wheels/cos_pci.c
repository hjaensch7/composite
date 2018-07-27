#include <llprint.h>
#include <cos_debug.h>
#include <cos_types.h>
#include <hypercall.h>
#include "pci_lib.h"
#include "cos_pci.h"

u32_t addr;

void
print_bits(unsigned int num)
{
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1<<(size*8-1);
	for(unsigned int i = 0; i<size*8; ++i){
    	printc("%u", num&maxPow ? 1 : 0);
    	num = num<<1;
	}
	printc("\n");
	return;
}


void
print_addr_bits(unsigned int num)
{
    unsigned int size = sizeof(unsigned int);
    unsigned int maxPow = 1<<(size*8-1);
	for(unsigned int i = 0; i<size*8; ++i){
		switch(i){
			case 30:
				printc("|");
				break;
			case 24:
				printc("reg");
				break;
			case 21:
				printc("func");
				break;
			case 16:
				printc("dev");
				break;
			case 8:
				printc("bus");
				break;
			case 1:
				printc("|");
				break;
		}
    	printc("%u", num&maxPow ? 1 : 0);
    	num = num<<1;
	}
	printc("\n");
}

unsigned
shift(unsigned bits, int len, int pos)
{
	unsigned mask =  (1<<len)-1;
	return (bits>>pos)&mask;
}

void
unpack_addr(unsigned int addr, unsigned *bus, unsigned *dev, unsigned *fun, unsigned *reg)
{
	//printc("cos: ");
	//print_bits(addr);
	*bus = shift(addr, 8, 16);
	*dev = shift(addr, 5, 11);
	*fun = shift(addr, 3, 8);
	*reg = shift(addr, 6, 2);
}


void
print_port(int port)
{
	if(port == 3320){
		printc("CONFIG_ADDRESS\n");
	}else{
		printc("CONFIG_DATA\n");
	}
}
/*
void
parse_outl(unsigned int addr)
{
	printc("Parsing Request:\n");
//	Print below prints sorted bits
//	print_addr_bits(addr);
	unsigned bus, dev, fun, reg;
	unpack_addr(addr, &bus, &dev, &fun, &reg);
	printc("reg: %d fun: %d dev: %d bus: %d\n", reg, fun, dev, bus);
	printc("config_data: ");
	print_bits(get_config_data(bus, dev, fun, reg));
}
*/

void
wheels_outl(unsigned short port, unsigned int value)
{
	printc("rk called %s: ", __func__);
	print_port(port);
	addr = value;
}

unsigned int
wheels_inl(unsigned short port)
{
	printc("rk called %s: ", __func__);
	print_port(port);
	//Unpacking Address
	unsigned bus, dev, fun, reg;
	unpack_addr(addr, &bus, &dev, &fun, &reg);
	printc("bus: %d dev: %d fun: %d reg: %d\n", bus, dev, fun, reg<<2);
	//Searching for config data
	printc("cos data: ");
	u32_t data = get_config_data(bus, dev, fun, reg);
	print_bits(data);

	return data;
}

void
cos_init(void)
{
	PRINTLOG(PRINT_DEBUG, "Wheels Spinning up\n");
	cos_pci_scan();
	hypercall_comp_init_done();
	assert(0);
}
