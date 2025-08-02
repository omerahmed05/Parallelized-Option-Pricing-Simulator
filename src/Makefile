all:
	g++ -fopenmp -o simulator simulator.cpp math.cpp
	./simulator
	../venv/bin/python plotter.py

clean:
	rm -f simulator 