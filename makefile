CEDEV = $(shell cedev-config --prefix)
OUTDIR = out

NAME = cedar
SRCS = $(filter-out src/$(NAME).asm,$(wildcard src/*.c))
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
RMDIR = ( rmdir /s /q $1 2>nul || call )
MKDIR = ( mkdir $1 2>nul || call )
else
CC = ez80-clang
RMDIR = rm -rf $1
MKDIR = mkdir -p $1
endif

CFLAGS = -S -Wall -Wextra -Oz -mllvm -profile-guided-section-prefix=false $(addprefix -I ,$(INCLUDES))

all: $(OUTDIR) $(OBJS)

$(OUTDIR)/%.c.src: src/%.c
	$(Q)$(CC) $(CFLAGS) $< -o $@

$(OUTDIR):
	$(Q)$(call MKDIR, $(OUTDIR))

clean:
	$(Q)$(call RMDIR,$(OUTDIR))
