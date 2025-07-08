all: install-python-packages simulator

# install-python-packages is a marker file:
# compares timestamp of install-python-packages and requirements.txt
# if requirements.txt has been updated, a new marker file is generated and pip install -r requirements.txt is ran
install-python-packages: requirements.txt
	pip install -r requirements.txt
	touch install-python-packages

simulator:
	g++ simulator.cpp math.cpp -o simulator

clean:
	rm -f simulator install-python-packages
 