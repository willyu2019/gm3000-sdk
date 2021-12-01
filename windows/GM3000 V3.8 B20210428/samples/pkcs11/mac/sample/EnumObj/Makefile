OBJ := $(patsubst %.cpp,%.o,$(wildcard *.cpp))
SHLIBS = ../../x64/libgm3000_pkcs11.dylib
all: $(OBJ)
	g++ $^  $(SHLIBS) -o enumobj 
$(OBJ) : %.o : %.cpp
	g++ -c $< -o $@ 
clean:
	rm -f enumobj  *.o
