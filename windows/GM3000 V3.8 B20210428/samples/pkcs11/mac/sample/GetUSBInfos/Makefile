OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SHLIBS = ../../x64/libgm3000_pkcs11.dylib -ldl

all: $(OBJ)
	$(CXX) $^ -o getusbinfos $(SHLIBS)
$(OBJ) : %.o : %.cpp
	$(CXX) -c $< -o $@
clean:
	rm  getusbinfos *.o
