CEDEV = $(shell cedev-config --prefix)
OUTDIR = out

NAME = cedar
OBJS = $(addsuffix .ll,$(patsubst src/%,$(OUTDIR)/%,$(wildcard src/*.c)))
ASM = $(wildcard src/*.asm)
ASSETS = $(addsuffix .ll,$(patsubst assets/generated/%,$(OUTDIR)/assets/%,$(wildcard assets/generated/*.c)))
#ASSETS = $(wildcard assets/generated/*.c)
INCLUDES = include/ $(CEDEV)/include/ assets/generated/
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
CONVIMG = convimg.exe
LINKASM = powershell.exe .\\tools\\linkasm.ps1
NATIVEPATH ?= $(subst /,\,$1)
QUOTE_ARG = "$(subst ",',$1)"#'
RM = ( del /f /q $(call NATIVEPATH,$1) 2>nul || call )
RMDIR = ( rmdir /s /q $(call NATIVEPATH,$1) 2>nul || call )
MKDIR = ( mkdir $(call NATIVEPATH,$1) 2>nul || call )
COPY = ( echo F | xcopy $1 $2 /Q /Y /I /K 1>nul 2>nul || call )
CAT = type
NUL = nul
ARROW = --
else
CC = ez80-clang
LINK = ez80-link
FASMG = fasmg
CONVIMG = convimg
LINKASM = ./tools/linkasm.pl
NATIVEPATH ?= $(subst \,/,$1)
QUOTE_ARG = '$(subst ','\'',$1)'#'
RM = rm -f $(call NATIVEPATH,$1)
RMDIR = rm -rf $(call NATIVEPATH,$1)
MKDIR = mkdir -p $(call NATIVEPATH,$1)
COPY = cp $1 $2
CAT = cat
NUL = /dev/null
ARROW = --
endif

all: $(ASSETS) $(OBJS) $(ASM)
	@echo [linking C] $(call NATIVEPATH,$(OBJS) $(ASSETS)) $(call NATIVEPATH,out/lib$(NAME).ll)
	$(Q)$(LINK) $(OBJS) $(ASSETS) -f -o out/lib$(NAME).ll
	@echo [assemblifying] $(call NATIVEPATH,out/lib$(NAME).ll) $(ARROW) $(call NATIVEPATH,out/Cout.tmp)
	$(Q)$(CC) out/lib$(NAME).ll -Oz -S -mllvm -profile-guided-section-prefix=false -o out/Cout.tmp
	@echo [linking asm] $(call NATIVEPATH,out/Cout.tmp $(ASM)) $(ARROW) out/mixed.src
	$(Q)$(CAT) $(call NATIVEPATH,out/Cout.tmp) $(call NATIVEPATH,$(ASM)) > $(call NATIVEPATH,out/mixed.src) 2>$(NUL)
	$(Q)$(LINKASM) $(call NATIVEPATH,out/mixed.src) $(call NATIVEPATH,out/lib$(NAME).src) >$(NUL)
	@echo [done]

$(OUTDIR)/%.c.ll: src/%.c
	@echo [compiling] $(call NATIVEPATH,$<) $(ARROW) $(call NATIVEPATH,$@)
	$(Q)$(CC) $(CFLAGS) -S -emit-llvm $< -o $@

$(OBJS): | out

$(ASSETS): | out

out:
	$(Q)$(call MKDIR,$(OUTDIR))
	$(Q)$(call MKDIR,$(OUTDIR)/assets/)

$(OUTDIR)/assets.ll: assets $(ASSETS)
	@echo [compiling] $(call NATIVEPATH,$@)

assets:
	$(Q)$(call MKDIR,assets/generated)
	(cd assets && $(CONVIMG) && cd ..)

$(OUTDIR)/assets/%.c.ll: assets/generated/%.c
	@echo [compiling] $(call NATIVEPATH,$<) $(ARROW) $(call NATIVEPATH,$@)
	$(Q)$(CC) $(CFLAGS) -S -emit-llvm $< -o $@

clean:
	$(Q)$(call RMDIR,$(OUTDIR))
	(cd assets && $(CONVIMG) --clean && cd ..)
	$(MAKE) -C test clean

rebuild: clean all

test:
	$(MAKE) -C test

.PHONY: all clean rebuild test assets out
