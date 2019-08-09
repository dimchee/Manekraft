OUT  := libsqlite.a

CPP	:= $(wildcard *.cpp)
C	:= $(wildcard *.c)
OBJ := $(subst .cpp,.o,$(CPP)) $(subst .c,.o,$(C))


all : $(OUT)

$(OUT) : $(OBJ)
	ar rvs $@ $^

%.o : %.cpp
	gcc -c $^

%.o : %.c
	gcc -c $^

clean:
	rm $(wildcard *.o)
