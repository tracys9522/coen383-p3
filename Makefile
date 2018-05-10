all: 
	g++ -pthread -o p3 p3.cpp customer.cpp seat.cpp seller.cpp
clean:
	rm -f p3
