
/* i386 initialization routines */

/* Machine dependant */
#include <arch/idt.h>
#include <arch/intrstub.h>
#include <arch/misc.h>

/* Machine independent */
#include <vga.h>
#include <console.h>
#include <init.h>

void init_interrupts(void) {
    /* Initialize interrupt table */
    init_idt();

    static void (*handlers[INTR_CRITICAL_VECTORS])(void) = {
        [0]  = &intr_handler_stub_0, /* Divide by zero (DE) */
        [1]  = &intr_handler_stub_1, /* Intel reserved */
        [2]  = &intr_handler_stub_2, /* Non-maskable external interrupt (NMI) */
        [3]  = &intr_handler_stub_3, /* Breakpoint (BP) */
        [4]  = &intr_handler_stub_4, /* Overflow (OF) */
        [5]  = &intr_handler_stub_5, /* Bound range exceeded (BR) */
        [6]  = &intr_handler_stub_6, /* Invalid opcode (UD) */
        [7]  = &intr_handler_stub_7, /* Device not available (no math
                                       coprocessor) (NM) */
        [8]  = &intr_handler_stub_8, /* Double fault (DF) */
        [9]  = &intr_handler_stub_9, /* FPU copocessor segment overrun (only
                                       generated by i386) */
        [10] = &intr_handler_stub_10, /* Invalid TSS (TS) */
        [11] = &intr_handler_stub_11, /* Segment not present (NP) */
        [12] = &intr_handler_stub_12, /* Stack-segment fault (SS) */
        [13] = &intr_handler_stub_13, /* General protection (GP) */
        [14] = &intr_handler_stub_14, /* Page fault (PF) */
        [15] = &intr_handler_stub_15, /* Intel reserved */
        [16] = &intr_handler_stub_16, /* FPU error (MF) */
        [17] = &intr_handler_stub_17, /* Alignment check (AC) */
        [18] = &intr_handler_stub_18, /* Machine check (MC) */
        [19] = &intr_handler_stub_19, /* SIMD floating-point exception (XM) */

        /* Interrupts 20-31 are Intel reserved, but unused */
        [20] = &intr_handler_stub_20,
        [21] = &intr_handler_stub_21,
        [22] = &intr_handler_stub_22,
        [23] = &intr_handler_stub_23,
        [24] = &intr_handler_stub_24,
        [25] = &intr_handler_stub_25,
        [26] = &intr_handler_stub_26,
        [27] = &intr_handler_stub_27,
        [28] = &intr_handler_stub_28,
        [29] = &intr_handler_stub_29,
        [30] = &intr_handler_stub_30,
        [31] = &intr_handler_stub_31 
    };
    
    /* Ready descriptors for exceptions */
    for(int i = 0; i < INTR_CRITICAL_VECTORS; i++) {
        /* Perform baseline initialization for an interrupt gate */
        init_descriptor((union descriptor*) &idt.vectors[i], DESC_INTERRUPT_GATE32, 0);

        /* Store code segment selector */
        idt.vectors[i].selector = CODE_SEGMENT;

        /* Store offset to the associated handler */
        DESC_GATE_SET_OFFSET(idt.vectors[i], handlers[i]);

        /* Interrupt handlers all run in privilege level 0 */
        idt.vectors[i].dpl = 0;

        /* Set gate as present (enabled) */
        idt.vectors[i].present = 1;
    }

    /* Load IDT */
    load_idt(&idt);

    /* Enable interrupts */
    enable_interrupts();
}

void init_console(void) {
    /* Initialize VGA */
    vga_init();
    
    /* Preinitialize console driver */
    console_preinit();

    /* Bind the console driver to the VGA driver */
    console.puts = &vga_puts;
    console.putchar = &vga_putc;
}
