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
    # Cross-compilation tools
    crossbuild-essential-armhf \
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

# Set up cross-compilation environment
RUN mkdir -p /opt/cross-pi-gcc

# Install Raspberry Pi cross-compilation tools
RUN apt-get update && apt-get install -y \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && wget -q https://github.com/Pro/raspi-toolchain/releases/latest/download/raspi-toolchain.tar.gz \
    && tar -xzf raspi-toolchain.tar.gz -C /opt \
    && rm raspi-toolchain.tar.gz

# Set up environment variables for cross-compilation
ENV PATH="/opt/cross-pi-gcc/bin:${PATH}" \
    RASPBERRY_VERSION=3 \
    TOOLCHAIN_PREFIX=/opt/cross-pi-gcc

# Default command
CMD ["bash"]
