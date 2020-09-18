.include "efm32gg.s"

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