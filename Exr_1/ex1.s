        .syntax unified
	
	      .include "efm32gg.s"

	/////////////////////////////////////////////////////////////////////////////
	//
  // Exception vector table
  // This table contains addresses for all exception handlers
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .section .vectors
	
	      .long   stack_top               /* Top of Stack                 */
	      .long   _reset                  /* Reset Handler                */
	      .long   dummy_handler           /* NMI Handler                  */
	      .long   dummy_handler           /* Hard Fault Handler           */
	      .long   dummy_handler           /* MPU Fault Handler            */
	      .long   dummy_handler           /* Bus Fault Handler            */
	      .long   dummy_handler           /* Usage Fault Handler          */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* SVCall Handler               */
	      .long   dummy_handler           /* Debug Monitor Handler        */
	      .long   dummy_handler           /* Reserved                     */
	      .long   dummy_handler           /* PendSV Handler               */
	      .long   dummy_handler           /* SysTick Handler              */

	      /* External Interrupts */
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO even handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   gpio_handler            /* GPIO odd handler */
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler
	      .long   dummy_handler

	      .section .text

	/////////////////////////////////////////////////////////////////////////////
	//
	// Reset handler
  // The CPU will start executing here after a reset
	//
	/////////////////////////////////////////////////////////////////////////////

	      .globl  _reset
	      .type   _reset, %function
        .thumb_func
_reset: 
		ldr r1, cmu_base_addr
		ldr r2, [r1, #CMU_HFPERCLKEN0]

		mov r3, #1
		lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		orr r2, r2, r3

		str r2, [r1, #CMU_HFPERCLKEN0]

		ldr r1, =GPIO_PA_BASE
		mov r2, #0x2
		str r2, [r1, #GPIO_CTRL] 

		mov r2, #0x55555555
		str r2, [r1, #GPIO_MODEH]
		//sett leds low
		mov r3, #0b11111111
		lsl r3, r3, #0x8
		str r3, [r1, #GPIO_DOUTSET]

		//Set buttons
		ldr r1, =GPIO_PC_BASE
		mov r2, #0x33333333
		str r2, [r1, #GPIO_MODEL]

		mov r3, #0xff
		str r3, [r1, #GPIO_DOUT]

		//Set up Interrupts

		ldr r1, =GPIO_BASE
		mov r2, #0x22222222
		str r2, [r1, #GPIO_EXTISPELL]

		mov r2, #0xff
		str r2, [r1, #GPIO_EXTIFALL]

		str r2, [r1, #GPIO_EXTRISE]

		str r2, [r1, #GPIO_IEN]

		mov r2, #0x802
		ldr r3, =ISER0
		str r2, [r3]
		loop:
			B loop





		//Lights corresponding leds when a button is pressed
		/*ldr r1, =GPIO_PC_BASE
		ldr r2, =GPIO_PA_BASE
		mov r3, #0b00000000
		lsl r3, r3, #0x8
		str r3, [r2, #GPIO_DOUT]*/
		/*loop: 
			ldr r3, [r1, #GPIO_DIN]
			lsl r3, r3, #0x8
			str r3, [r2, #GPIO_DOUT]
			B loop*/

	      b .  // do nothing
		

cmu_base_addr: 
			.long CMU_BASE	
	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	    mov r3, #0b00000000
		lsl r3, r3, #0x8
		str r3, [r1, #GPIO_DOUTSET]
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
        b .  // do nothing

