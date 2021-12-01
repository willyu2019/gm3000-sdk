OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
SHLIBS = ../../x86/libgm3000_pkcs11.so
all: $(OBJ)
	g++ $^  $(SHLIBS) -o enumobj 
$(OBJ) : %.o : %.cpp
	g++ -c $< -o $@ 
clean:
	rm -f enumobj  *.o
