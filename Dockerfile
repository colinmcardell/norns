FROM ubuntu:22.04

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive

# Install basic build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    libasound2-dev \
    libavahi-compat-libdnssd-dev \
    libcairo2-dev \
    libevdev-dev \
    libgpiod-dev \
    libjack-jackd2-dev \
    liblo-dev \
    libmonome-dev \
    libnanomsg-dev \
    libsdl2-dev \
    libsndfile1-dev \
    libudev-dev \
    lua5.3-dev \
    pkg-config \
    python3 \
    python3-pip \
    # Add Lua development tools
    luarocks \
    # Add testing and documentation tools
    ldoc \
    && rm -rf /var/lib/apt/lists/*

# Set up working directory
WORKDIR /norns

# Copy the entire project into the container
COPY . .

# Set up Lua environment and install common testing dependencies
RUN luarocks install busted && \
    luarocks install luacov && \
    luarocks install luacheck

# Create a script to build the project
RUN echo '#!/bin/bash\n\
    cd /norns/matron && make clean && make "$@"' > /usr/local/bin/build-norns && \
    chmod +x /usr/local/bin/build-norns

# Default command
CMD ["build-norns"]