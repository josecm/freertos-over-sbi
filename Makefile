TARGET := freertos-riscv

LD_FILE:=linker.ld

SRCS:=
SRCS+=$(wildcard ./Source/*.c)
SRCS+=$(wildcard ./Source/*.S)
SRCS+=$(wildcard ./Source/portable/GCC/RISC-V/*.[c,S])
SRCS+=./Source/portable/MemMang/heap_4.c
SRCS+=main.c retarget.c sbi.c uart8250.c boot.S

LIBS:=

INCS:=
INCS+=$(wildcard ./Source/include/*.h)
INCS+=$(wildcard ./inc/*.h)
INCS+=./Source/portable/GCC/RISC-V/portmacro.h
INCS+=./Source/portable/GCC/RISC-V/chip_specific_extensions/RV64I_SBI_no_extensions/freertos_risc_v_chip_specific_extensions.h
INCS+=FreeRTOSConfig.h

##############################################################################

CROSS_COMPILE := riscv64-unknown-elf-
CC:=$(CROSS_COMPILE)gcc
AS:=$(CROSS_COMPILE)as
LD:=$(CROSS_COMPILE)ld
OBJCOPY:=$(CROSS_COMPILE)objcopy
OBJDUMP:=$(CROSS_COMPILE)objdump

ROOT_DIR:=$(abspath .)
BUILD_DIR:=$(ROOT_DIR)/build
DIRS:=

C_OBJS+=$(patsubst  %.c, %.o, $(filter %.c, $(SRCS)))
ASM_OBJS+=$(patsubst  %.S, %.o, $(filter %.S, $(SRCS)))
OBJS:=$(patsubst $(ROOT_DIR)%, $(BUILD_DIR)%, $(abspath $(C_OBJS) $(ASM_OBJS)))
 
BUILD_DIRS:=$(sort $(dir $(OBJS)))
DIRS+=$(BUILD_DIRS)

DEPS:=$(patsubst %.o, %.d, $(OBJS))
DIRS+=$(patsubst %/, %, $(sort $(dir $(DEPS))))

GENERIC_FLAGS:= -march=rv64g -mabi=lp64d -g3\
	$(addprefix -I, $(sort $(dir $(INCS)))) 
ASFLAGS:=$(GENERIC_FLAGS) 
CPPFLAGS:=$(GENERIC_FLAGS)
CFLAGS:=$(GENERIC_FLAGS) -mcmodel=medany -O3
LDFLAGS:=$(GENERIC_FLAGS) -nostartfiles --static 


all: $(TARGET).bin

-include $(DEPS)

$(TARGET).bin: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS)
	$(CC) $(LDFLAGS) -T$(LD_FILE) $(OBJS) -o $@
	$(OBJDUMP) -S $@ > $(TARGET).asm
	$(OBJDUMP) -x -d --wide $@ > $(TARGET).lst

$(BUILD_DIR)/%.d : $(ROOT_DIR)/%.[c,S]
	$(CC) -MM -MG -MT "$(patsubst %.d, %.o, $@) $@"  $(CPPFLAGS) $< > $@

$(BUILD_DIR)%.o: $(ROOT_DIR)%.[c,S]
	@$(CC) $(CFLAGS) -c $< -o $@	

.SECONDEXPANSION:

$(OBJS) $(DEPS): | $$(@D)

$(DIRS):
	mkdir -p $@

clean:
	rm -rf ./build

.PHONY: all clean
