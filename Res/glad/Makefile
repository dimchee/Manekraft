OUT  := libglad.a

CPP	:= $(wildcard *.cpp)
C	:= $(wildcard *.c)
OBJ := $(subst .cpp,.o,$(CPP)) $(subst .c,.o,$(C))


all : $(OUT)

$(OUT) : $(OBJ)
	ar rvs $@ $^

%.o : %.cpp
	g++ -c $^

%.o : %.c
	g++ -c $^

clean:
	rm $(wildcard *.o)
