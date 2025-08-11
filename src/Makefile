all:
	# build the simulator
	g++ -fopenmp -o simulator simulator.cpp math.cpp
	# run the simulator
	./simulator
	# plot the results
	../venv/bin/python plotter.py

clean:
	rm -f simulator 
	rm -f ./dist/Data.csv ./dist/plot.html