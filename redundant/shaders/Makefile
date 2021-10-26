CXX=g++

main: main.o loadShader.o
	$(CXX) -o main main.o loadShader.o  -lGL -lGLU -lglut -lGLEW -lm -lpthread
	
main.o: main.cpp
	$(CXX) -c -o main.o main.cpp

loadShader.o: loadShader.cpp
	$(CXX) -c -o loadShader.o loadShader.cpp
	
.PHONY: clean
clean:
	rm -f *.o main