VERSION := 1.1.0

# Directories
SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj
PLUGINS_DIR := $(BIN_DIR)/plugins

# Output
LIB_NAME := libEGL.dylib
LIB_OUT_PATH := $(BIN_DIR)/$(LIB_NAME)
LIB_BACKUP_DIR := $(BIN_DIR)/backup
LIB_BACKUP_PATH := $(LIB_BACKUP_DIR)/old_$(LIB_NAME)

# LoL paths
LOL_ROOT_DIR := "/Applications/League of Legends.app/Contents/LoL"
LOL_CEF_DIR := $(LOL_ROOT_DIR)/"League of Legends.app/Contents/Frameworks"
TARGET_LIB_DIR := $(LOL_CEF_DIR)/"Chromium Embedded Framework.framework/Libraries"
TARGET_LIB_PATH := $(TARGET_LIB_DIR)/$(LIB_NAME)

# "/Applications/League of Legends.app/Contents/LoL/League of Legends.app/Contents/Frameworks/Chromium Embedded Framework.framework/Libraries"

# Compiler
CC := clang
CXX := clang++
CXXFLAGS := -std=c++17 -fPIC -arch x86_64 -I./ -I./src/ -fvisibility=hidden

# Linker
LDFLAGS := -shared -dynamiclib -arch x86_64 -current_version $(VERSION) -compatibility_version 1.0.0
LDLIBS := -framework cocoa,foundation,corefoundation -F $(LOL_CEF_DIR) -framework "Chromium Embedded Framework" "$(LIB_BACKUP_PATH)"

# Source files
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cc) $(wildcard $(SRC_DIR)/**/*.cc)
OBJCXX_SRCS := $(wildcard $(SRC_DIR)/**/*.mm)
INC_HEADERS := $(wildcard $(SRC_DIR)/*.h)

# Object files
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cc,$(OBJ_DIR)/%.o,$(CPP_SRCS))
OBJCXX_OBJS := $(patsubst $(SRC_DIR)/%.mm,$(OBJ_DIR)/%.o,$(OBJCXX_SRCS))

# Default target
all: $(LIB_OUT_PATH)

# Rule to compile C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cc ${INC_HEADERS}
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# Rule to compile Objective-C++ source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.mm
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -x objective-c++ -c -o $@ $<

# Create backup dylib
$(LIB_BACKUP_PATH):
	@mkdir -p $(LIB_BACKUP_DIR)
	@cp -n $(TARGET_LIB_PATH) $(LIB_BACKUP_PATH)

# Rule to link the dylib
$(LIB_OUT_PATH): $(CPP_OBJS) $(OBJCXX_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -o $@ $(CPP_OBJS) $(OBJCXX_OBJS) $(LDLIBS)
	@install_name_tool -change "@executable_path/../Frameworks/Chromium Embedded Framework.framework/Chromium Embedded Framework" "@loader_path/../Chromium Embedded Framework" $@
	@install_name_tool -change "./$(LIB_NAME)" "$(abspath $(LIB_BACKUP_PATH))" $@

# Target to install the dylib
install: $(LIB_OUT_PATH)
	cp $(LIB_OUT_PATH) $(TARGET_LIB_PATH)

restore:
	cp $(LIB_BACKUP_PATH) $(INSTALL_DIR)

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(LIB_OUT_PATH)

# Open plugins folder
open:
	@mkdir -p $(PLUGINS_DIR)
	@open $(PLUGINS_DIR)

.PHONY: all install clean