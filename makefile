randwrite: randwrite.o list.o
	g++ -o randwrite randwrite.o list.o

randwrite.o: randwrite.cc list.h
	g++ -c randwrite.cc

list.o: list.cc list.h
	g++ -c list.cc

clean:
	rm -f *.o randwrite *~

auth:   auth.o randwrite.o
	g++ -o auth auth.o randwrite.o

auth.o: auth.cc list.h
	g++ -c auth.cc
