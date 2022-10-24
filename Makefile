OUT = timetable
BUILD_DIR = ./build
INSTALL_DIR = /usr/local/bin

OBJECT = ./obj
SOURCE = ./src

SRC := $(shell find $(SOURCE) -name *.cpp)
OBJ := $(SRC:%=$(OBJECT)/%.o)
DEPS := $(OBJ:.o=.d)
INC_DIRS := $(shell find $(SOURCE) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS)) -I$(INC_DIR)

CXX = clang++
CXXFLAGS = -pipe -std=c++20 -pedantic
DEBUG_FLAGS := $(CXXFLAGS) -g -fsanitize=address
RELEASE_FLAGS := $(CXXFLAGS) -O3 -flto -Wall -Wextra -Werror


release: $(OBJ)
	@mkdir -p $(BUILD_DIR)
	@echo "Building the RELEASE binary..."
	@$(CXX) $(OBJ) -o $(BUILD_DIR)/$(OUT) $(RELEASE_FLAGS)
	@echo "The binary was built successfully!"

debug: $(OBJ)
	@mkdir -p $(BUILD_DIR)
	@echo "Building the DEBUG binary..."
	@echo "$(CXX) $(OBJ) -o $(BUILD_DIR)/$(OUT) $(DEBUG_FLAGS)"
	@$(CXX) $(OBJ) -o $(BUILD_DIR)/$(OUT) $(DEBUG_FLAGS)
	@echo "The binary was built successfully!"

install: $(BUILD_DIR)/$(OUT)
	@cp $(BUILD_DIR)/$(OUT) $(INSTALL_DIR)

uninstall:
	@rm $(INSTALL_DIR)/$(OUT)

$(OBJECT)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	@echo "Building $@..."
ifeq ($(MAKECMDGOALS),debug)
	@$(CXX) $(DEBUG_FLAGS) -c $< -o $@
else ifeq ($(MAKECMDGOALS),release)
	@$(CXX) $(RELEASE_FLAGS) -c $< -o $@
else ifeq ($(MAKECMDGOALS),)
	@$(CXX) $(RELEASE_FLAGS) -c $< -o $@
endif

.PHONY: clean install uninstall debug release

clean:
	rm -rf $(OBJECT) $(BUILD_DIR)/$(OUT) $(BUILD_DIR)/groups $(BUILD_DIR)/lecturers

-include $(DEPS)


