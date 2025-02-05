# Verzeichnisse
INCLUDE_DIR = ./include

# Ziel
TARGET = logfile_auslagern

# Quellen
SRC = ./src/main.cpp

# Compiler und Flags
CXX = g++
CXXFLAGS = -std=c++20
LDFLAGS = -L$(INCLUDE_DIR) -lws2_32

# Rezept für das Ziel
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET) $(LDFLAGS)

clean:
	del $TARGET.exe
