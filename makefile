####### Variables
OS        = $(shell uname)
BIN_DIR   = bin_$(OS)
OBJ_DIR   = obj_$(OS)
SRC_DIR   = src
CXXFLAGS  = -std=c++11 -Wall -Wextra -g # 
INCPATH   = -Isrc -I/usr/include/opencv4/opencv -I/usr/include/opencv4
LDFLAGS   = -lopencv_highgui -lopencv_imgcodecs -lopencv_imgproc -lopencv_core

ifdef OMP
	ifeq ($(OS),Darwin)
		CXX 	  = /usr/local/opt/llvm/bin/clang++
		CXXFLAGS += --ld-path=/usr/local/opt/llvm/bin/ld64.lld -mlinker-version=450 -I/usr/local/opt/llvm/include -I/usr/local/include
		LDFLAGS  += -v -L/usr/local/opt/llvm/lib -L/usr/local/lib
	else
		CXXFLAGS += -fopenmp
		LDFLAGS += -lgomp
		CXXOPT    = -O2 -ftree-vectorize -funroll-loops
	endif
#	ifeq ($(OS),Linux)
#		CXX       = clang++
#	endif
else
	CXXFLAGS += -Wno-unknown-pragmas
	CXXOPT    = -O3 -ftree-vectorize -funroll-loops#
endif

####### Files
# source files without extension:
SRC_FILES    = $(patsubst $(SRC_DIR)/%.cpp,%,$(shell find $(SRC_DIR)/ -name \
			     '*.cpp' -type f))
SRC_FILES   += $(patsubst $(SRC_DIR)/%.c,%,$(shell find $(SRC_DIR)/ -name \
			     '*.c' -type f))
OBJ_FILES    = $(addprefix $(OBJ_DIR)/,$(addsuffix .o, $(SRC_FILES)))

# name of the application:
TARGET       = $(BIN_DIR)/film_grain_rendering_main

####### Build rules
.PHONY: all clean

all: $(TARGET)

$(BIN_DIR)/film_grain_rendering_main: $(OBJ_FILES)
	@echo "===== Link $@ ====="
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXOPT) -o $@ $^ $(LIBS) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo "===== Compile $< ====="
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXOPT) $(INCPATH) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "===== Compile $< ====="
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(CXXOPT) $(INCPATH) -c $< -o $@

clean:
	@echo "===== Clean $< ====="
	@rm -rf $(BIN_DIR) $(OBJ_DIR)
