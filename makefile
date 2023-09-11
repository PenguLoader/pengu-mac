# Directories
SRC_DIR := src
BUILD_DIR := build
LIB_DIR := bin

LOL_ROOT_DIR := "/Applications/League of Legends.app/Contents/LoL"
INSTALL_DIR := $(LOL_ROOT_DIR)/"League of Legends.app/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries"
# "/Applications/League of Legends.app/Contents/LoL/League of Legends.app/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries"

# Compiler
CC := clang
CXX := clang++
CXXFLAGS := -std=c++17 -fPIC -arch x86_64 -I./ -I./src/

# Linker
LDFLAGS := -shared -dynamiclib -arch x86_64
LDLIBS := -framework cocoa,foundation,corefoundation -F $(LOL_ROOT_DIR)/"League of Legends.app/Contents/Frameworks" -framework "Chromium Embedded Framework"

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/**/*.cc)
OBJCXX_SRCS := $(wildcard $(SRC_DIR)/**/*.mm)
INC_HEADERS := $(wildcard $(SRC_DIR)/*.h)

# Object files
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cc,$(BUILD_DIR)/%.o,$(CPP_SRCS))
OBJCXX_OBJS := $(patsubst $(SRC_DIR)/%.mm,$(BUILD_DIR)/%.o,$(OBJCXX_SRCS))

# Output dylib name
LIB_NAME := libvk_swiftshader.dylib

# Final target
LIB_PATH := $(LIB_DIR)/$(LIB_NAME)

# Default target
all: $(LIB_PATH)

# Rule to compile C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cc ${INC_HEADERS}
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule to compile Objective-C++ source files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.mm
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -x objective-c++ -c -o $@ $<

# Rule to link the dylib
$(LIB_PATH): $(CPP_OBJS) $(OBJCXX_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -o $@ $(CPP_OBJS) $(OBJCXX_OBJS) $(LDLIBS)
	@install_name_tool -change "@executable_path/../Frameworks/Chromium Embedded Framework.framework/Chromium Embedded Framework" "@loader_path/../Chromium Embedded Framework" $@

# Target to install the dylib
install: $(LIB_PATH)
	@mkdir -p $(INSTALL_DIR)
	cp $(LIB_PATH) $(INSTALL_DIR)

restore:
	cp $(LIB_NAME) $(INSTALL_DIR)

clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)

# Open plugins folder
open:
	@mkdir -p $(INSTALL_DIR)/plugins
	@open $(INSTALL_DIR)/plugins

.PHONY: all install clean