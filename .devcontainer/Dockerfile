FROM debian:bookworm

# Avoid prompts from apt
ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C.UTF-8 \
    PATH="/usr/local/go/bin:$PATH" \
    GOLANG_VERSION=1.21.4 \
    JACK2_VERSION=1.9.22 \
    LIBMONOME_VERSION=1.4.7 \
    NANOMSG_VERSION=1.1.5 \
    SUPERCOLLIDER_VERSION=3.13.0

# Install base dependencies
RUN apt-get update && apt-get install -y \
    apt-transport-https \
    apt-utils \
    build-essential \
    ca-certificates \
    cmake \
    curl \
    git \
    jackd \
    libasound2-dev \
    libavahi-client-dev \
    libavahi-compat-libdnssd-dev \
    libcairo2-dev \
    libcairo-script-interpreter2 \
    libevdev-dev \
    libfontconfig1-dev \
    libfreetype-dev \
    libfreetype6-dev \
    libglib2.0-dev \
    libgpiod-dev \
    libicu-dev \
    libjack-jackd2-dev \
    liblo-dev \
    liblua5.3-dev \
    libsdl2-dev \
    libsndfile1-dev \
    libudev-dev \
    lua5.3 \
    luarocks \
    pkg-config \
    python3 \
    python3-dev \
    python3-pip \
    sudo \
    unzip \
    wget \
    && rm -rf /var/lib/apt/lists/* \
    && ln -sf /usr/include/lua5.3 /usr/include/lua \
    && ls -la /usr/include/lua5.3 \
    && echo "Verifying lauxlib.h exists:" \
    && test -f /usr/include/lua5.3/lauxlib.h && echo "lauxlib.h found"

# Install Python-is-Python3 for libmonome
RUN apt-get update && apt-get install -y python-is-python3 && rm -rf /var/lib/apt/lists/*

# Install Go
RUN wget https://golang.org/dl/go$GOLANG_VERSION.linux-amd64.tar.gz -O /tmp/go.tar.gz && \
    tar -C /usr/local -xzf /tmp/go.tar.gz && \
    rm -f /tmp/go.tar.gz && \
    go version

# Install libmonome
RUN cd /tmp/ && \
    wget https://github.com/monome/libmonome/archive/v$LIBMONOME_VERSION.tar.gz -O libmonome.tar.gz && \
    tar -xzf libmonome.tar.gz && \
    cd libmonome-$LIBMONOME_VERSION && \
    ./waf configure --disable-udev --disable-osc && \
    ./waf && \
    ./waf install && \
    cd / && \
    rm -rf /tmp/libmonome-$LIBMONOME_VERSION && \
    ldconfig

# Install nanomsg
RUN mkdir -p /tmp/nanomsg && \
    cd /tmp/nanomsg && \
    wget https://github.com/nanomsg/nanomsg/archive/$NANOMSG_VERSION.tar.gz -O nanomsg.tar.gz && \
    tar -xzf nanomsg.tar.gz && \
    cd nanomsg-$NANOMSG_VERSION && \
    mkdir build && \
    cd build && \
    cmake .. && \
    cmake --build . && \
    cmake --build . --target install && \
    cd / && \
    rm -rf /tmp/nanomsg && \
    ldconfig

# Create non-root user
RUN groupadd --gid 1000 vscode && \
    useradd --uid 1000 --gid 1000 -m vscode && \
    echo vscode ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/vscode && \
    chmod 0440 /etc/sudoers.d/vscode

# Set up development tools
RUN apt-get update && apt-get install -y \
    gdb \
    valgrind \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Set up Lua environment and install common testing dependencies
RUN luarocks install busted && \
    luarocks install luacov && \
    luarocks install luacheck && \
    luarocks install ldoc

# Set the default user
USER vscode
WORKDIR /workspace