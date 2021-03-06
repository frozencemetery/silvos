#include "util.h"

#define MASTER_PIC 0x20
#define MASTER_PIC_COMMAND MASTER_PIC
#define MASTER_PIC_DATA (MASTER_PIC+1)
#define SLAVE_PIC 0xA0
#define SLAVE_PIC_COMMAND SLAVE_PIC
#define SLAVE_PIC_DATA (SLAVE_PIC+1)

void remap_pic (void) {
  outb(0x20, 0x11); /* Initialize and require 4th byte. */
  outb(0x21, 0x20); /* Offset ISRs 0 - 7 to interrupt lines 0x20 - 0x27. */
  outb(0x21, 0x04); /*  */
  outb(0x21, 0x01); /* Enter 8086 mode */

  outb(0xA0, 0x11);
  outb(0xA1, 0x40); /* Offset to 0x48-0x4F, I think */
  outb(0xA1, 0x02);
  outb(0xA1, 0x01); /* 8086 mode */

  outb(0x21, 0xFC); /* Block non-keyboard, non-timer lines */
  outb(0xA1, 0xFF); /* block everything */
}

void eoi (void) {
  outb(0x20, 0x20); /* End of interrupt for master PIC */
}
