run: sequential
	./sequential

sequential: main.cpp registro/next.cpp registro/registro.cpp sequential/sequential.cpp
	g++ -o sequential main.cpp registro/next.cpp registro/registro.cpp sequential/sequential.cpp -std=c++17 -lstdc++fs
