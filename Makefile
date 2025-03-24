CXX=clang++
CXXFLAGS=-std=c++20 -O3 \
		 -I./include/ \
		 -I./include/external
LDFLAGS=./lib/scarablib/libscarablib.a -lGL

BUILD_DIR  = build
OBJS_DIR   = $(BUILD_DIR)/objs

SRC_FILES = $(wildcard src/*.cpp src/*/*/*.cpp src/**/*.cpp)
OBJS      = $(patsubst %.cpp, $(OBJS_DIR)/%.o, $(SRC_FILES))

TARGET=vakraft

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BUILD_DIR)/$@ $(LDFLAGS)

$(OBJS_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_DIR) $(TARGET)

.PHONY: all clean
