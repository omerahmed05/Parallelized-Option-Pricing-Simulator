FROM python:3.11-slim

# Install build dependencies
RUN apt-get update && apt-get install -y build-essential g++ libomp-dev

# Copy source code into container
WORKDIR /app
COPY . /app

# Install Python dependencies inside container
RUN python -m venv venv # create virtual environment
RUN . venv/bin/activate # activate virtual environment
RUN pip install -r requirements.txt # install python dependencies

# Run the Makefile to build the simulator and plot the results
CMD make