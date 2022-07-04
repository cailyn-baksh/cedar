CEDEV = $(shell cedev-config --prefix)
OUTDIR = out

NAME = cedar
SRCS = $(wildcard src/*.c src/*.asm)
OBJS = $(addsuffix .src,$(patsubst src/%,$(OUTDIR)/%,$(SRCS)))
INCLUDES = include/ $(CEDEV)/include


# verbosity
V ?= 0
ifeq ($(V),0)
Q = @
FASMG_V := -n
else
Q =
FASMG_V := -v$(V)
endif

# os specific
ifeq ($(OS),Windows_NT)
CC = ez80-clang.exe
NATIVEPATH ?= $(subst /,\,$1)
RMDIR = ( rmdir /s /q $1 2>nul || call )
MKDIR = ( mkdir $1 2>nul || call )
COPY = ( echo F | xcopy $1 $2 /Q /Y /I /K 1>nul 2>nul || call )
else
CC = ez80-clang
NATIVEPATH ?= $(subst \,/,$1) $1
RMDIR = rm -rf
MKDIR = mkdir -p $1
COPY = cp $1 $2
endif

CFLAGS = -S -Wall -Wextra -Oz -mllvm -profile-guided-section-prefix=false $(addprefix -I ,$(INCLUDES))

all: $(OUTDIR) $(OBJS)

$(OUTDIR)/%.asm.src: src/%.asm
	$(Q)echo [copying] $(call NATIVEPATH,$<)
	$(Q)$(call COPY,$(call NATIVEPATH,$<),$(call NATIVEPATH,$@))

$(OUTDIR)/%.c.src: src/%.c
	$(Q)echo [compiling] $(call NATIVEPATH,$<)
	$(Q)$(CC) $(CFLAGS) $< -o $@

$(OUTDIR):
	$(Q)$(call MKDIR,$(OUTDIR))

clean:
	$(Q)$(call RMDIR,$(OUTDIR))

rebuild: clean all

test:
	$(MAKE) -C test

.PHONY: all clean rebuild test
