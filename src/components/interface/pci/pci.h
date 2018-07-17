#ifndef PCI_H
#define PCI_H

void
wheels_outl(unsigned short port, unsigned int value);

unsigned int
wheels_inl(unsigned short port);

#endif /* PCI_H */
