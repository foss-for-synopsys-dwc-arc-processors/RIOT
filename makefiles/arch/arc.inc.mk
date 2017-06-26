# Target triple for the build.
export TARGET_ARCH ?= arc-elf32


LMAP_OPTION = -Wl,-M,-Map=$(APPLICATION).map


# define build specific options
export CFLAGS_CPU = 
export CFLAGS_DBG ?= -g3 -gdwarf-2
export CFLAGS_OPT ?= -O2 -mno-sdata -std=gnu99

export CFLAGS +=  @$(COMPILE_OPT_ARGFILE) $(CFLAGS_CPU) $(CFLAGS_DBG) $(CFLAGS_OPT)
#$(CFLAGS_DBG)

export ASFLAGS += $(CFLAGS_CPU) $(CFLAGS_DBG)
export CCASFLAGS += $(CFLAGS_CPU) $(CFLAGS_DBG) -x assembler-with-cpp

export LINKFLAGS += @$(COMPILE_OPT_ARGFILE) -mno-sdata -nostartfiles \
		$(LMAP_OPTION) -Wl,--script=$(LINK_SCRIPT) -Wl,--start-group -lm -lc -lgcc -Wl,--end-group


# This CPU implementation is using the new core/CPU interface:
export CFLAGS += -DCOREIF_NG=1

# Rule to generate assembly listings from ELF files:
%.lst: %.elf
	$(OBJDUMP) $(OBJDUMPFLAGS) $< > $@

