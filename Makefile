driver.x: passwordserver.o driver.o
	g++ -std=c++11 -o driver.x driver.o passwordserver.o -lcrypt

passwordserver.o: passwordserver.h passwordserver.cpp hash.h hash.hpp
	g++ -std=c++11 -c passwordserver.cpp

driver.o: passwordserver.h hash.h driver.cpp
	g++ -std=c++11 -c driver.cpp

clean:
	rm driver.x *.o
