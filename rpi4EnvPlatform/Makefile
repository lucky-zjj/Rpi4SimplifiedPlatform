CONFIGFILE = Makefile.config

# config
include ${CONFIGFILE}

# local settings
-include Makefile.local

# toolchain
include Makefile.toolchain


# common definitions
# ---------------------------------
OUTDIR  = output
NAME	= ${OUTDIR}/program.elf

rwildcard=$(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2) $(filter $(subst *,%,$2),$d))

CODE_DIRS     = all arch/$(PROGPLAT_ARCH) board/$(PROGPLAT_BOARD) ${CODE_DIRS_EXTRA}

SOURCES_C     = $(foreach d,$(CODE_DIRS),$(call rwildcard, $d/src/, *.c))
SOURCES_S     = $(foreach d,$(CODE_DIRS),$(call rwildcard, $d/src/, *.S))
INCLUDE_FILES = $(foreach d,$(CODE_DIRS),$(call rwildcard, $d/inc/, *.h)) all/inc/config_input.h

OBJECTS       = $(SOURCES_C:.c=.o) $(SOURCES_S:.S=.o)

BOARDCONFIG   = $(PROGPLAT_BOARD)
LINKERFILE    = board/$(PROGPLAT_BOARD)/$(BOARDCONFIG).ld


# compiler flags
# ---------------------------------
LDFLAGS_PRE  = -Bstatic  


CFLAGS_EXTRA  = -nostartfiles -nostdlib


INCFLAGS     = $(foreach d,$(CODE_DIRS),-I$d/inc)
SFLAGS       = ${SFLAGS_EXTRA} ${INCFLAGS}
CFLAGS	     = -std=gnu99 -Wall -fno-builtin -fno-stack-protector -O0 ${INCFLAGS} ${CFLAGS_EXTRA}



# compilation and linking
# ---------------------------------
all: $(NAME)

all/inc/config_input.h: ${CONFIGFILE}
	./scripts/gen_config_input.py

%.o: %.S ${INCLUDE_FILES}
	${CROSS}cpp ${INCFLAGS} $< | ${CROSS}as ${SFLAGS} -o $@ -

%.o: %.c ${INCLUDE_FILES}
	${CROSS}gcc ${CFLAGS} -c -o $@ $<

$(NAME): ${OBJECTS} ${INCLUDE_FILES}
	mkdir -p ${OUTDIR}
	${CROSS}ld $(LDFLAGS_PRE) -o $@ -T $(LINKERFILE) ${OBJECTS} $(LDFLAGS_POST)
	${CROSS}objcopy -O binary ${NAME}  output/kernel8.img


clean:
	rm -rf ${OUTDIR}
	rm -f $(call rwildcard, , *.o) all/inc/config_input.h


.PHONY: all clean


# running and debugging
include Makefile.run

