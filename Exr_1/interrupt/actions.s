.include "efm32gg.s"

// FUNCTIONS START

		.thumb_func
led_rl:	  // ROTATE LEDS LEFT (shift leds left by shifting bits right)
		  // Get the current value of the LEDs
		  ldr r0, =GPIO_PA_BASE
		  ldr r1, [r0, #GPIO_DOUT]
		  mov r3, #0xff
		  lsl r3, #8
		  and r1, r1, r3

		  // Invert the bits so that 1 = on, 0 = off
		  // and shift them to the right so we can work with immediate values
		  eor r1, r1, r3
		  lsr r1, r1, #8

		  // Check if leftmost bit is on
		  // If it is, turn on the rightmost bit after shifting
		  and r2, r1, #0b00000001
		  lsr r1, r1, #1
		  cmp r2, #0b00000001
		  it eq
		  orreq r1, r1, #0b10000000

		  // Shift bits back, turn those on and others off
		  lsl r1, r1, #8
		  str r3, [r0, #GPIO_DOUTSET]
		  str r1, [r0, #GPIO_DOUTCLR]

		  bx lr

		.thumb_func
led_rr:   // ROTATE LEDS RIGHT (we shift leds right by shifting bits left)
		  // Get the current value of the LEDs
		  ldr r0, =GPIO_PA_BASE
		  ldr r1, [r0, #GPIO_DOUT]
		  mov r3, #0xff
		  lsl r3, #8
		  and r1, r1, r3

		  // Invert the bits so that 1 = on, 0 = off
		  // and shift them to the right so we can work with immediate values
		  eor r1, r1, r3
		  lsr r1, r1, #8

		  // Check if rightmost bit is on
		  // If it is, turn it off before shifting,
		  // and turn on the leftmost bit after shifting
		  and r2, r1, #0b10000000
		  cmp r2, #0b10000000
		  ittte eq
		  andeq r1, r1, #0b01111111
		  lsleq r1, #1
		  orreq r1, r1, #0b00000001
		  lslne r1, r1, #1

		  // Shift bits back, turn those on and others off
		  lsl r1, r1, #8
		  str r3, [r0, #GPIO_DOUTSET]
		  str r1, [r0, #GPIO_DOUTCLR]
		  
		  bx lr

		.thumb_func
led_on:   // TURN ON THE RIGHTMOST LED
		  ldr r1, =GPIO_PA_BASE
		  mov r2, #0x80
		  lsl r2, r2, #8
		  str r2, [r1, #GPIO_DOUTCLR]
		  
		  bx lr

		.thumb_func
led_off:  // TURN OFF THE RIGHTMOST LED
		  ldr r1, =GPIO_PA_BASE
		  mov r2, #0x80
		  lsl r2, r2, #8
		  str r2, [r1, #GPIO_DOUTSET]
		  
		  bx lr

// FUNCTIONS END
