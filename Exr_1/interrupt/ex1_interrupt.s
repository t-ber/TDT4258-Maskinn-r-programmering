        .syntax unified
	
	      .include "efm32gg.s"
		  .include "actions.s"

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
          // Set up GPIO Clock
		  ldr r1, =CMU_BASE
		  ldr r2, [r1, #CMU_HFPERCLKEN0]
		  mov r3, #1
		  lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		  orr r2, r2, r3
		  str r2, [r1, #CMU_HFPERCLKEN0]

		  // Set high drive strenght
		  ldr r1, =GPIO_PA_BASE
		  mov r2, #0x2
		  str r2, [r1, #GPIO_CTRL]

		  // Set LED Pins to output
		  mov r2, #0x55555555
		  str r2, [r1, #GPIO_MODEH]

		  // Set pins low, except the far right one
		  mov r2, #0b01111111
		  lsl r2, r2, #8
		  str r2, [r1, #GPIO_DOUTSET]

          // Set button pins to input
		  ldr r1, =GPIO_PC_BASE
		  mov r2, #0x33333333
		  str r2, [r1, #GPIO_MODEL]

          // Enable internal pull-up for buttons
		  mov r2, #0xff
		  str r2, [r1, #GPIO_DOUT]

          // Set up interrupts
          mov r2, #0x22222222
          str r2, [r1, #GPIO_EXTIPSELL]

          mov r2, #0xff
          str r2, [r1, #GPIO_EXTIFALL]

          mov r2, #0xff
          str r2, [r1, #GPIO_IEN]

          ldr r1, =ISER0
          mov r2, #0x80
          lsl r2, #4
          orr r2, r2, #0x2
          str r2, [r1]

          // energy mode
          ldr r1, =SCR
          mov r2, #6
          str r2, [r1]
          
          wfi

	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  

	      ldr r4, =GPIO_PC_BASE
          ldr r5, [r4, #GPIO_IF]

          and r6, r5, #0x10
          cmp r6, #0
          it ne
          blne led_rl

          and r6, r5, #0x40
          cmp r6, #0
          it ne
          blne led_rr

          and r6, r5, #0x20
          cmp r6, #0
          it ne
          blne led_on

          and r6, r5, #0x80
          cmp r6, #0
          it ne
          blne led_off

          str r5, [r4, #GPIO_IFC]

          bx lr
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
          bx lr
