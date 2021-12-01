OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SHLIBS = ../../x86/libgm3000_pkcs11.so

all: $(OBJ)
	$(CXX) $^ -o exportcert $(SHLIBS)

$(OBJ): %.o: %.cpp
	$(CXX) -c $< -o $@
clean:
	rm -f *.o exportcert
