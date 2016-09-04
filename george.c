#include "vga.h"
#include "threads.h"
#include "idt.h"
#include "gdt.h"
#include "util.h"
#include "bits.h"
#include "isr.h"
#include "pic.h"
#include "page.h"
#include "alloc.h"
#include "kbd.h"
#include "fpu.h"

#include "userland/print-a-include.h"
#include "userland/print-b-include.h"
#include "userland/calc-include.h"

void initialize_idt (void) {
  create_idt();
  for (int i = 0; i < 256; i++) {
    register_isr(i, isr_other);
  }
  register_isr(0x35, dumb_isr);
  register_isr(0x36, yield_isr);
  register_isr(0x37, putch_isr);
  register_isr(0x38, exit_isr);
  register_isr(0x39, getch_isr);
  register_isr(0x07, nm_isr);
  register_isr(0x08, doublefault_isr);

  /* A list of syscalls I'm not handling from userspace */
  register_isr(0x00, exit_isr);
  register_isr(0x04, exit_isr);
  register_isr(0x05, exit_isr);
  register_isr(0x06, exit_isr);
  register_isr(0x0D, exit_isr);
  register_isr(0x0E, exit_isr);

  register_isr(0x01, isr_01);
  register_isr(0x02, isr_02);
  register_isr(0x03, isr_03);
  register_isr(0x09, isr_09);
  register_isr(0x0A, isr_0A);
  register_isr(0x0B, isr_0B);
  register_isr(0x0C, isr_0C);
  register_isr(0x0F, isr_0F);
  register_isr(0x10, isr_10);
  register_isr(0x11, isr_11);
  register_isr(0x12, isr_12);
  register_isr(0x13, isr_13);
  register_isr(0x14, isr_14);
  register_isr(0x15, isr_15);
  register_isr(0x16, isr_16);
  register_isr(0x17, isr_17);
  register_isr(0x18, isr_18);
  register_isr(0x19, isr_19);
  register_isr(0x1A, isr_1A);
  register_isr(0x1B, isr_1B);
  register_isr(0x1C, isr_1C);
  register_isr(0x1D, isr_1D);
  register_isr(0x1E, isr_1E);
  register_isr(0x1F, isr_1F);

  register_isr(0x20, timer_isr);
  register_isr(0x21, kbd_isr);
}

void create_test_threads (void) {
  user_thread_create(&userland_print_b_bin[0], userland_print_b_bin_len);
  user_thread_create(&userland_calc_bin[0], userland_calc_bin_len);
}

void kernel_main (void) {
  clear_screen();
  puts("Welcome to GeorgeOS, Multiboot Edition!\r\n");
  initialize_allocator();
  insert_pt(initial_pt());
  initialize_idt();
  insert_idt();
  initialize_gdt();
  remap_pic();
  create_test_threads();
  idle_thread_create();
  init_kbd();
  fpu_init();
  puts("Launching userspace.\r\n");
  schedule(); /* Does not return */
}
