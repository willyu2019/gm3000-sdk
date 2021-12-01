OBJ = $(patsubst %.cpp, %.o, $(wildcard *.cpp))
SHLIBS = ../../x64/libgm3000_pkcs11.dylib 

all: $(OBJ)
	$(CXX) $^ -o pkcsdemo $(SHLIBS)
$(OBJ): %.o : %.cpp
	$(CXX) -c $< -o $@
clean:
	rm -f pkcsdemo *.o
