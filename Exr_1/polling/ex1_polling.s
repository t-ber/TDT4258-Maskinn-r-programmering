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

          // Loop initialization
		  // Merk at vi bruker registre r4+ for "variabler" som må 
		  // beholde sin verdi etter funksjonskall
	      ldr r4, =GPIO_PC_BASE
	      ldr r5, [r4, #GPIO_DIN]
	      and r5, r5, #0xff // bryr oss kun om de 8 LSB

loop:	  ldr r6, [r4, #GPIO_DIN]
          and r6, r6, #0xff // bryr oss kun om de 8 LSB
	      eor r0, r5, r6 // r0 viser nå hvilke bit som har endret seg siden sist
		  and r0, r0, r5 // r0 viser nå hvilke bit som har endret seg til å være lave

          // Sjekk om venstre-knapp har blitt trykket ned (SW5)
          and r1, r0, #0x10 // ikke null hvis biten endret seg // til å være lav
          // and r1, r1, r5 // fortsatt ikke null hvis biten forrige gang var 1
          cmp r1, #0
		  it ne
          blne led_rl // Roter ledene mot venstre

          // Sjekk om høyre-knapp har blitt trykket ned (SW7)
          and r1, r0, #0x40 // ikke null hvis biten endret seg // til å være lav
          // and r1, r1, r5 // fortsatt ikke null hvis biten forrige gang var 1
          cmp r1, #0
		  it ne
          blne led_rr // Roter ledene mot høyre

		  // Sjekk om opp-knapp har blitt trykket ned (SW6)
		  and r1, r0, #0x20 // ikke null hvis biten har endret seg // til å være lav
		  // and r1, r1, r5 // fortsatt ikke null hvis biten forrige gang var 1
		  cmp r1, #0
		  it ne
		  blne led_on  // Skru på led lengst til høyre

		  // Sjekk om ned-knapp har blitt trykket ned (SW8)
		  and r1, r0, #0x80 // ikke null hvis biten har endret seg // til å være lav
		  // and r1, r1, r5 // fortsatt ikke null hvis biten forrige gang var 1
		  cmp r1, #0
		  it ne
		  blne led_off // Skru av led lengst til høyre
	
          mov r5, r6 // oppdaterer "forrige verdi" i r5
          b loop


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
