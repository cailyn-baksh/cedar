
include $(CEDEV)/meta/makefile.mk

CFLAGS = -Wall -Wextra -Oz -I include/
CXXFLAGS = -Wall -Wextra -Oz -I include/

LIB_SRC := src/cedar.asm
LIB_LIB := bin/cedar.lib
LIB_8XV := bin/cedar.8xv
LIB_H := include/cedar.h

all: cedar

cedar: $(LIB_8XV)

$(LIB_8XV): $(LIB_SRC)
	$(call MKDIR,$(@D))
	$(Q)$(FASMG) -v 2 $^ $@

clean:
	$(Q)$(call REMOVE,$(LIB_LIB) $(LIB_8XV))

.PHONY: all clean
