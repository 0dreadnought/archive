CC = clang++
LD = clang++
CPPFLAGS = -std=c++17  -I./include

LDFLAGS = -lpthread

ROOTDIR = ./

CP = cp
ECHO = echo

BUILD_DIR = build

CPP_SOURCES = src/log.cpp simpletest/simpletest.cpp
CPP_HEADERS = src/log.hpp simpletest/simpletest.h
C_SOURCES = 

APP = app3

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))

vpath %.cpp $(sort $(dir $(CPP_SOURCES)))
vpath %.cpp src

# OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
# vpath %.c $(sort $(dir $(C_SOURCES)))

$(BUILD_DIR)/%.o: %.cpp $(CPP_HEADERS) Makefile | $(BUILD_DIR)
	$(ECHO) compiling $<
	$(CC) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	$(ECHO) compiling $<
	clang -c $(CFLAGS) $< -o $@

all: $(BUILD_DIR) $(BUILD_DIR)/$(APP)  $(BUILD_DIR)/$(APP2)

run: $(BUILD_DIR)/$(APP)
	$(BUILD_DIR)/$(APP)

$(BUILD_DIR)/$(APP): main3.cpp $(OBJECTS) Makefile
	$(ECHO) linking $<
	$(CC) $(CPPFLAGS) $(LDFLAGS) -o $@ ./src/main3.cpp $(OBJECTS)
	$(ECHO) success

$(BUILD_DIR):
	mkdir $@

#######################################
# clean up
#######################################
clean:
	-rm -fR $(BUILD_DIR)/$(APP) $(BUILD_DIR)/*.o

#######################################
# dependencies
#######################################
-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all

