#include <llprint.h>
#include <pci.h>
#include <cos_debug.h>
#include <cos_types.h>
#include <hypercall.h>
#include "pci_lib.h"


void
print_bits(unsigned int num)
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

//could be useful in decoding addresses sent from kernels
static unsigned int
makeaddr(unsigned bus, unsigned dev, unsigned fun, int reg)
{
	printc("%s, %s\n", __FILE__, __func__);
    return (1<<31) | (bus<<16) | (dev <<11) | (fun<<8) | (reg & 0xfc);
}

void
wheels_outl(unsigned short port, unsigned int value)
{
	printc("rk called %s: ", __func__);
	printc("port %x\n", port);
	printc("value ");
	print_bits(value);
}

unsigned int
wheels_inl(unsigned short port)
{
	printc("rk called %s: ", __func__);
	printc("port %x\n", port);
	return 0;
}

void
endian_check()
{
   unsigned int i = 1;
   char *c = (char*)&i;
   if (*c)
       printc("Little endian\n");
   else
       printc("Big endian\n");
}
void
cos_init(void)
{
	PRINTLOG(PRINT_DEBUG, "Wheels Spinning up\n");
	printc("Edianness: ");
	endian_check();
	cos_pci_scan();
	hypercall_comp_init_done();
	assert(0);
}
