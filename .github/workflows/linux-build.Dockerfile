FROM ubuntu:22.04

# Build arguments
ARG BUILD_TYPE=Debug
ARG PLATFORM=DESKTOP

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive \
    BUILD_TYPE=${BUILD_TYPE} \
    PLATFORM=${PLATFORM}

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

# Install Faust
RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    && rm -rf /var/lib/apt/lists/* \
    && git clone https://github.com/grame-cncm/faust.git \
    && cd faust \
    && make \
    && make install \
    && cd .. \
    && rm -rf faust

# Set up Lua environment and install common testing dependencies
RUN luarocks install busted && \
    luarocks install luacov && \
    luarocks install luacheck

# Set up working directory
WORKDIR /norns

# Copy the entire project into the container
COPY . .

# Configure the build
RUN ./build.sh configure --platform=${PLATFORM} --type=${BUILD_TYPE}

# Build the project
RUN ./build.sh build

# Default command
CMD ["bash"]
