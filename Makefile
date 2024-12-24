
CXX = g++
CXXFLAGS = -std=c++17 -g -Wall -O0
LDFLAGS = -lm -lSDL2 -lSDL2_image -lSDL2_gfx

SRC_DIR = src
BUILD_DIR = build
PHYSICS_DIR = $(SRC_DIR)/Physics

SRC_FILES = $(SRC_DIR)/Application.cpp $(SRC_DIR)/Graphics.cpp $(SRC_DIR)/Main.cpp \
            $(PHYSICS_DIR)/Force.cpp $(PHYSICS_DIR)/Particle.cpp $(PHYSICS_DIR)/Vec2.cpp

OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, 	 $(BUILD_DIR)/%.o, $(SRC_FILES))

TARGET = $(BUILD_DIR)/app

all: $(TARGET)


$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p build/Physics

run: $(TARGET)
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

# Mark special targets as phony
.PHONY: all run clean
