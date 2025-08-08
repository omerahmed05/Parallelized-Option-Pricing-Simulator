FROM python:3.11-slim

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential g++ libomp-dev && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

# Build your C++ simulator
RUN g++ -fopenmp -o simulator simulator.cpp math.cpp

# Install Python dependencies inside container (if any)
RUN python -m venv venv
RUN . venv/bin/activate && pip install --upgrade pip && pip install -r requirements.txt

# Default command to run the simulator and plot
CMD ./simulator && ./venv/bin/python plotter.py