CEDEV = $(shell cedev-config --prefix)
OUTDIR = out

NAME = cedar
CSRCS = $(wildcard src/*.c)
ASMSRCS = $(wildcard src/*.asm)
OBJS = $(addsuffix .ll,$(patsubst src/%,$(OUTDIR)/%,$(CSRCS)))
INCLUDES = include/ $(CEDEV)/include
DEFINES = DEBUG

CFLAGS = -Oz -Wall -Wextra -Wno-incompatible-function-pointer-types $(addprefix -I ,$(INCLUDES)) $(addprefix -D,$(DEFINES))

# verbosity
V ?= 0
ifeq ($(V),0)
Q = @
else
Q =
endif

# os specific
ifeq ($(OS),Windows_NT)
CC = ez80-clang.exe
LINK = ez80-link.exe
FASMG = fasmg.exe
NATIVEPATH ?= $(subst /,\,$1)
QUOTE_ARG = "$(subst ",',$1)"#'
RM = ( del /f /q $1 2>nul || call )
RMDIR = ( rmdir /s /q $1 2>nul || call )
MKDIR = ( mkdir $1 2>nul || call )
COPY = ( echo F | xcopy $1 $2 /Q /Y /I /K 1>nul 2>nul || call )
CAT = type
NUL = nul
else
CC = ez80-clang
LINK = ez80-link
FASMG = fasmg
NATIVEPATH ?= $(subst \,/,$1) $1
QUOTE_ARG = '$(subst ','\'',$1)'#'
RM = rm -f $1
RMDIR = rm -rf $1
MKDIR = mkdir -p $1
COPY = cp $1 $2
CAT = cat
NUL = /dev/null
endif

all: $(OUTDIR) $(OBJS)
	@echo [linking] out/lib$(NAME).ll
	$(Q)$(LINK) -S $(OBJS) -o out/lib$(NAME).ll
	@echo [assemblifying] out/cout.src
	$(Q)$(CC) out/lib$(NAME).ll -Oz -S -mllvm -profile-guided-section-prefix=false -o out/cout.src
	@echo [combining assembly] out/lib$(NAME).src
	$(Q)$(CAT) $(call NATIVEPATH,out/cout.src) $(call NATIVEPATH,$(ASMSRCS)) > $(call NATIVEPATH,out/lib$(NAME).src) 2>$(NUL)
	$(Q)$(call RM,$(call NATIVEPATH,out/cout.src))
	@echo [done]

$(OUTDIR)/%.asm.src: src/%.asm
	@echo [copying] $(call NATIVEPATH,$<)
	$(Q)$(call COPY,$(call NATIVEPATH,$<),$(call NATIVEPATH,$@))

$(OUTDIR)/%.c.ll: src/%.c
	@echo [compiling] $(call NATIVEPATH,$<)
	$(Q)$(CC) $(CFLAGS) -S -emit-llvm $< -o $@

$(OUTDIR):
	$(Q)$(call MKDIR,$(OUTDIR))

clean:
	$(Q)$(call RMDIR,$(OUTDIR))
	$(MAKE) -C test clean

rebuild: clean all

test:
	$(MAKE) -C test

.PHONY: all clean rebuild test
