CC			:= g++
C_FLAGS		:= -O2 -std=c++17 -Wall -Wextra

ifeq ($(OS),Windows_NT)
EXECUTABLE	:= app.exe
else
EXECUTABLE	:= app
endif

all : $(EXECUTABLE)

$(EXECUTABLE) : main.cpp
	$(CC) $(C_FLAGS) main.cpp -o $@
	$(EXECUTABLE)

run: all
	$(EXECUTABLE)

clean:
	$(RM) $(EXECUTABLE)