OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SHLIBS = ../../x64/libgm3000_pkcs11.dylib -ldl

all: $(OBJ)
	$(CXX) $^ -o pkcstest $(SHLIBS)
$(OBJ): %.o : %.cpp
	$(CXX) -c $< -o $@
clean:
	rm -f pkcstest *.o
