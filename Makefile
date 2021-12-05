CXX := g++
CXXFLAGS := -Wall -MMD -MP
LDFLAGS := -lSDL2 -lGLEW -lGLU -lGL

SRC_DIRS ?= ./src
BUILD_DIR ?= ./build
TARGET_EXEC ?= opengl_engine

SRCS := $(shell find $(SRC_DIRS) -name *.cpp -or -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

all: CXXFLAGS += -O2
all: $(BUILD_DIR)/$(TARGET_EXEC)

debug: DFLAGS := -D DEBUG
debug: $(BUILD_DIR)/$(TARGET_EXEC)

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS) $(DFLAGS)

# c++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	$(MKDIR_P) $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(DFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p