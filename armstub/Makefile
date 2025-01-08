BINS=armstub8-gic.bin

CC8?=aarch64-linux-gnu-gcc
LD8?=aarch64-linux-gnu-ld
OBJCOPY8?=aarch64-linux-gnu-objcopy
OBJDUMP8?=aarch64-linux-gnu-objdump -maarch64



BIN2C=./bin2c

all : $(BINS)

clean :
	rm -f *.o *.out *.tmp *.bin *.elf *.ds *.C armstubs.h bin2c *~

%8.o: %8.S
	$(CC8) -c $< -o $@

%8-gic.o: %8.S
	$(CC8) -DGIC=1 -DBCM2711=1 -c $< -o $@

%8-gic-highperi.o: %8.S
	$(CC8) -DGIC=1 -DHIGH_PERI=1 -DBCM2711=1 -c $< -o $@

%8-gic.elf: %8-gic.o
	$(LD8) --section-start=.text=0 $< -o $@

%8-gic-highperi.elf: %8-gic-highperi.o
	$(LD8) --section-start=.text=0 $< -o $@

%8.elf: %8.o
	$(LD8) --section-start=.text=0 $< -o $@

%8-gic.tmp: %8-gic.elf
	$(OBJCOPY8) $< -O binary $@

%8-gic-highperi.tmp: %8-gic-highperi.elf
	$(OBJCOPY8) $< -O binary $@

%8.tmp: %8.elf
	$(OBJCOPY8) $< -O binary $@

%.bin: %.tmp
	dd if=$< ibs=256 of=$@ conv=sync

%8.ds: %8.bin
	$(OBJDUMP8) -D --target binary $< > $@
