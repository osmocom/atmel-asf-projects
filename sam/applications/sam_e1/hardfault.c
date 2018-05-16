#include <asf.h>

/**
 * \brief Default HardFault interrupt handler.
 */
struct hardfault_args {
	unsigned long r0;
	unsigned long r1;
	unsigned long r2;
	unsigned long r3;
	unsigned long r12;
	unsigned long lr;
	unsigned long pc;
	unsigned long psr;
};

void hard_fault_handler_c(struct hardfault_args *args)
{
    printf("\r\nHardFault\r\n");
    printf("R0=%08x, R1=%08x, R2=%08x, R3=%08x, R12=%08x\r\n",
	   args->r0, args->r1, args->r2, args->r3, args->r12);
    printf("LR[R14]=%08x, PC[R15]=%08x, PSR=%08x\r\n",
	   args->lr, args->pc, args->psr);
    printf("BFAR=%08x, CFSR=%08x, HFSR=%08x\r\n",
	   SCB->BFAR, SCB->CFSR, SCB->HFSR);
    printf("DFSR=%08x, AFSR=%08x, SHCSR=%08x\r\n",
	   SCB->DFSR, SCB->CFSR, SCB->SHCSR);

    if (SCB->HFSR & 0x40000000)
	    printf("FORCED ");
    if (SCB->HFSR & 0x00000002)
	    printf("VECTTBL ");

    uint32_t ufsr = SCB->CFSR >> 16;
    if (ufsr & 0x0200)
	    printf("DIVBYZERO ");
    if (ufsr & 0x0100)
	    printf("UNALIGNED ");
    if (ufsr & 0x0008)
	    printf("NOCP ");
    if (ufsr & 0x0004)
	    printf("INVPC ");
    if (ufsr & 0x0002)
	    printf("INVSTATE ");
    if (ufsr & 0x0001)
	    printf("UNDEFINSTR ");

    uint32_t bfsr = (SCB->CFSR >> 8) & 0xff;
    if (bfsr & 0x80)
	    printf("BFARVALID ");
    if (bfsr & 0x10)
	    printf("STKERR ");
    if (bfsr & 0x08)
	    printf("UNSTKERR ");
    if (bfsr & 0x04)
	    printf("IMPRECISERR ");
    if (bfsr & 0x02)
	    printf("PRECISERR ");
    if (bfsr & 0x01)
	    printf("IBUSERR ");

    uint32_t mmfsr = (SCB->CFSR & 0xff);
    if (mmfsr & 0x80)
	    printf("MMARVALID ");
    if (mmfsr & 0x10)
	    printf("MSTKERR ");
    if (mmfsr & 0x08)
	    printf("MUNSTKERR ");
    if (mmfsr & 0x02)
	    printf("DACCVIOL ");
    if (mmfsr & 0x01)
	    printf("IACCVIOL ");

    while ( 1 ) ;
}

__attribute__((naked))
void HardFault_Handler( void )
{
	__asm volatile(
		".syntax unified	\n"
		" tst lr, #4		\n"
		" ite eq		\n"
		" mrseq r0, msp		\n"
		" mrsne r0, psp		\n"
		//" ldr r1, [r0, #24] 	\n"
		" b hard_fault_handler_c\n"
		".syntax divided	\n");
}
