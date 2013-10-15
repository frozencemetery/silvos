#include "isr.h"
#include "util.h"

struct IDT_entry {
  unsigned short offset_low;
  unsigned short selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short offset_high;
} __attribute__ ((packed));

struct IDT_addr {
  unsigned short size;
  unsigned int base;
} __attribute__ ((packed));

struct IDT_entry idt[256];

void dumbisr (void) {
  panic("wefwefwefwe");
}

extern void yield_isr(void);

void initialize_idt (void) {
  idt[0x35].type_attr = 0x8E; /* present 32-bit interrupt gate in ring 3 */
  idt[0x35].offset_high = 0xFFFF & ((unsigned int) dumbisr >> 16);
  idt[0x35].offset_low = 0xFFFF & ((unsigned int) dumbisr);
  idt[0x35].zero = 0;
  idt[0x35].selector = 8;
  idt[0x36].type_attr = 0x8E; /* present 32-bit interrupt gate in ring 3 */
  idt[0x36].offset_high = 0xFFFF & ((unsigned int) yield_isr >> 16);
  idt[0x36].offset_low = 0xFFFF & ((unsigned int) yield_isr);
  idt[0x36].zero = 0;
  idt[0x36].selector = 8;
}

void insert_idt (void) {
  const struct IDT_addr k = { 0x1000, (unsigned int) idt };
  __asm__("lidt %0" : : "m"(k) : );
}

unsigned int get_idt (void) {
  struct IDT_addr k;
  __asm__("sidt %0" : "=m"(k) : :);
  return k.base;
}
