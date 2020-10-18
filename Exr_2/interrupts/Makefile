# Example Makefile
#
# Exercise 2, TDT4258

CC=arm-none-eabi-gcc
LD=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy

CFLAGS=-mcpu=cortex-m3 -mthumb -g -std=c99 -Wall
LDFLAGS=-mcpu=cortex-m3 -mthumb -g -lgcc -lm -lc -lcs3 -lcs3unhosted -lefm32gg -Llib
ASFLAGS=-mcpu=cortex-m3 -mthumb -g
LINKERSCRIPT=lib/efm32gg.ld

SCRIPTFOLDER=src
HEADERFOLDER=inc
BUILDFOLDER=build

${BUILDFOLDER}/main.bin : ${BUILDFOLDER}/main.elf
	${OBJCOPY} -O binary $< $@

${BUILDFOLDER}/main.elf : ${BUILDFOLDER}/main.o ${BUILDFOLDER}/timer.o ${BUILDFOLDER}/dac.o ${BUILDFOLDER}/gpio.o ${BUILDFOLDER}/interrupt_handlers.o ${BUILDFOLDER}/rtc.o ${BUILDFOLDER}/music.o
	${LD} -T ${LINKERSCRIPT} $^ -o $@ ${LDFLAGS} 

${BUILDFOLDER}/%.o : ${SCRIPTFOLDER}/%.c
	${CC} ${CFLAGS} -c $< -o $@

.PHONY : pretty
pretty :
	-indent ${SCRIPTFOLDER}/*.c ${HEADERFOLDER}/*.h

.PHONY : upload
upload :
	-eACommander.sh -r --address 0x00000000 -f "${BUILDFOLDER}/main.bin" -r

.PHONY : clean
clean :
	-rm -rf ${BUILDFOLDER}/*.o ${BUILDFOLDER}/*.elf ${BUILDFOLDER}/*.bin ${BUILDFOLDER}/*.hex 
