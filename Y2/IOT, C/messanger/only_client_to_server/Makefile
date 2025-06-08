CC = clang++
LD = clang++

CPPFLAGS = -std=c++17 -I/opt/iot/include -D__DEBUG__=1
LDFLAGS = -lpthread -L/opt/iot/lib -liot

ROOTDIR = ./

BUILD_DIR = made

APP = task1_ser

APP2 = task1

all: $(BUILD_DIR)/$(APP) $(BUILD_DIR)/$(APP2)

$(BUILD_DIR)/$(APP): task1_ser.cpp Makefile
	$(CC) task1_ser.cpp $(CPPFLAGS) $(LDFLAGS) -o $@

$(BUILD_DIR)/$(APP2): task1.cpp Makefile
	$(CC) task1.cpp $(CPPFLAGS) $(LDFLAGS) -o $@

clean:
	-rm -fR $(BUILD_DIR)/$(APP) $(BUILD_DIR)/*.o

-include $(shell mkdir .dep 2>/dev/null) $(wildcard .dep/*)

.PHONY: clean all