#include <llprint.h>
#include <cos_debug.h>
#include <cos_types.h>
#include <hypercall.h>
#include "pci_lib.h"
#include "cos_pci.h"

u32_t addr;
int BAR_conf = 0; //Bool 1 = attempting bar config, 0 = not

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

int
dev_exists(unsigned bus, unsigned dev, unsigned fun)
{
	if(0 == get_config_data(bus, dev, fun, 1))
		return 0;
	else
		return 1;
}

int
is_bar(unsigned int addr)
{
	if(shift(addr, 6, 2) >= 4 && shift(addr, 6, 2) <= 9)
		return 1;
	else
		return 0;
}

void
wheels_outl(unsigned short port, unsigned int value)
{
	addr = value;
	if (value == PCI_BITMASK_32){ //this should be casting then comparison
		BAR_conf = 1;
	}
}

unsigned int
wheels_inl(unsigned short port)
{
	//Unpacking Address
	unsigned bus, dev, fun, reg;
	unpack_addr(addr, &bus, &dev, &fun, &reg);
	printc("bus: %d dev: %d fun: %d reg: %d\n", bus, dev, fun, reg<<2);

	//Searching for config data
	printc("cos data: ");
	u32_t data = PCI_BITMASK_32;
	if(!dev_exists(bus, dev, fun))
		return data;

	if (BAR_conf == 1 && is_bar(addr)){ //Getting size of mapped memory
		data = get_alt_bar(bus, dev, fun, reg);
		BAR_conf = 0;
	}else if(is_bar(addr)) //Getting address of mapped memory
		data = get_raw_bar(bus, dev, fun, reg);
	else
		data = get_config_data(bus, dev, fun, reg); //Standard config register
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
