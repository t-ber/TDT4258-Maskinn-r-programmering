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

// FUNCTIONS START

		  // ROTATE LEDS LEFT (shift leds left by shifting bits right)
led_rl:	  ldr r0, =GPIO_PA_BASE
		  ldr r1, [r0, #GPIO_DOUT]
		  mov r3, #0xff // lager maske for led-bitene
		  lsl r3, #8
		  and r1, r1, r3 // "henter ut" led-bitene
		  eor r1, r1, r3 // inverterer led-bitene slik at 1 = på, 0 = av
		  lsr r1, r1, #8 // shifter de til høyre så vi kan arbeide med immediate values

		  // Need to check if leftmost bit is on
		  // If it is, we need to turn on the rightmost bit after the shifting
		  and r2, r1, #0b00000001
		  lsr r1, r1, #1
		  cmp r2, #0b00000001
		  it eq
		  orreq r1, r1, #0b10000000

		  lsl r1, r1, #8 // shifter de tilbake på riktig plass
		  str r3, [r0, #GPIO_DOUTSET] // skru av alle leds
		  str r1, [r0, #GPIO_DOUTCLR] // skru på de som skal være på
		  mov r15, r14 // return

		  // ROTATE LEDS RIGHT (we shift leds right by shifting bits left)
led_rr:   ldr r0, =GPIO_PA_BASE
		  ldr r1, [r0, #GPIO_DOUT]
		  mov r3, #0xff // lager maske for led-bitene
		  lsl r3, #8
		  and r1, r1, r3 // "henter ut" led-bitene
		  eor r1, r1, r3 // inverterer led-bitene slik at 1 = på, 0 = av
		  lsr r1, r1, #8 // shifter de til høyre så vi kan arbeide med immediate values

		  // Need to check if rightmost bit is on
		  // If it is, we need to turn it off before the shifting,
		  // and turn on the leftmost bit after the shifting

		  // the next block is basically:
		  //
		  // and r2, r1, #0b10000000
		  // if (r2 == 0b10000000) {
		  //     and r1, r1, #0b01111111
		  //     lsl r1, #1
		  //     orr r1, r1, #0b00000001
		  // } else {
		  //     lsl r1, #1
		  // }

		  and r2, r1, #0b10000000
		  cmp r2, #0b10000000
		  ittte eq
		  andeq r1, r1, #0b01111111
		  lsleq r1, #1
		  orreq r1, r1, #0b00000001
		  lslne r1, r1, #1

		  lsl r1, r1, #8 // shifter de tilbake på riktig plass
		  str r3, [r0, #GPIO_DOUTSET] // skrur av alle leds
		  str r1, [r0, #GPIO_DOUTCLR] // skru på de som skal være på
		  mov r15, r14 // return

		  // TURN ON THE RIGHTMOST LED
led_on:   ldr r1, =GPIO_PA_BASE
		  mov r2, #0x80
		  lsl r2, r2, #8
		  str r2, [r1, #GPIO_DOUTCLR]
		  mov r15, r14 // return

		  // TURN OFF THE RIGHTMOST LED
led_off:  ldr r1, =GPIO_PA_BASE
		  mov r2, #0x80
		  lsl r2, r2, #8
		  str r2, [r1, #GPIO_DOUTSET]
		  mov r15, r14 // return

// FUNCTIONS END
	
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
