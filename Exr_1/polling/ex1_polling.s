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
          bl setup_clock
		  bl setup_leds
		  bl setup_buttons
		  b polling

		.thumb_func
polling:
          // Loop initialization
	      ldr r4, =GPIO_PC_BASE
	      ldr r5, [r4, #GPIO_DIN]
	      and r5, r5, #0xff

loop:
		  ldr r6, [r4, #GPIO_DIN]
          and r6, r6, #0xff 
	      eor r0, r5, r6 // find what bits have changed
		  and r0, r0, r5 // find what bits have changed AND were high at last poll

          // Check for button SW5 (left)
          and r1, r0, #0x10
          cmp r1, #0
		  it ne
          blne led_rl // Rotate LEDs left

          // Check for button SW7 (right)
          and r1, r0, #0x40
          cmp r1, #0
		  it ne
          blne led_rr // Rotate LEDs right

		  // Check for button SW6 (left)
		  and r1, r0, #0x20
		  cmp r1, #0
		  it ne
		  blne led_on  // Turn on the rightmost LED

		  // Check for button SW8 (down)
		  and r1, r0, #0x80
		  cmp r1, #0
		  it ne
		  blne led_off // Turn off the rightmost LED
	
          mov r5, r6 // update 'previous value'
          b loop

		.thumb_func
setup_clock:
		  // setup GPIO clock
		  ldr r1, =CMU_BASE
		  ldr r2, [r1, #CMU_HFPERCLKEN0]
		  mov r3, #1
		  lsl r3, r3, #CMU_HFPERCLKEN0_GPIO
		  orr r2, r2, r3
		  str r2, [r1, #CMU_HFPERCLKEN0]

		  bx lr

		.thumb_func
setup_leds:
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

		  bx lr

		.thumb_func
setup_buttons:
		  // Set button pins to input
		  ldr r1, =GPIO_PC_BASE
		  mov r2, #0x33333333
		  str r2, [r1, #GPIO_MODEL]

          // Enable internal pull-up for buttons
		  mov r2, #0xff
		  str r2, [r1, #GPIO_DOUT]

		  bx lr

	/////////////////////////////////////////////////////////////////////////////
	//
  // GPIO handler
  // The CPU will jump here when there is a GPIO interrupt
	//
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
gpio_handler:  
		  b .  // do nothing
	
	/////////////////////////////////////////////////////////////////////////////
	
        .thumb_func
dummy_handler:  
		  b .  // do nothing
