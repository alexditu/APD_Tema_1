all: serial_neoptimizat serial_optimizat paralel_optimizat

serial_neoptimizat:
	g++ serial_neoptimizat.cpp -o serial_neopt -Wall

serial_optimizat:
	g++ serial_optimizat.cpp -o serial_opt -Wall

paralel_optimizat:
	g++ -fopenmp paralel_optimizat.cpp -o paralel_opt -Wall

clean:
	rm serial_neopt serial_opt paralel_opt
