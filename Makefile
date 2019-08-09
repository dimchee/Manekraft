CC			:= g++
C_FLAGS		:= -O2 -std=c++17 -Wall -Wextra

BUILD		:= Build
BIN			:= bin
LIB			:= lib

SRC			:= Src

MLIB		:= libglad.a liblua.a libsqlite.a

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= main.exe
LIBRARIES	:= -lglfw3 -lglad -lopengl32 -lwinmm -lgdi32 -llua -lsqlite
BUILD		:= $(BUILD)/Win32
# MLIB		:= $(MLIB) libglfw3
else
EXECUTABLE	:= main
LIBRARIES	:= -lglfw3 -lglad -lGLU -lGL -lX11 -lpthread -ldl -llua -lsqlite
BUILD		:= $(BUILD)/Linux
endif

BIN			:= $(BUILD)/$(BIN)
LIB			:= $(BUILD)/$(LIB)
LIBS		:= $(addprefix $(LIB)/,$(MLIB))

all : $(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE) : $(LIB) $(BIN)  $(LIBS) $(wildcard $(SRC)/Core/*) $(wildcard $(SRC)/Scripts/*) main.cpp
	$(CC) $(C_FLAGS)  -I. -L$(LIB) main.cpp -o $@ $(LIBRARIES)

$(LIB) :
	mkdir -p $(BUILD)
	mkdir -p $(LIB)

$(BIN) : $(LIB)
	mkdir -p $(BIN)

$(SRC) :
	mkdir -p $@

%.a :
	cd Res/$(subst lib,,$(notdir $(basename $@)))/ && make
	mv Res/$(subst lib,,$(notdir $(basename $@)))/$(notdir $@) $(LIB)
	cd Res/$(subst lib,,$(notdir $(basename $@)))/ && make clean
	   
# Additional commands

run: all
	./$(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)