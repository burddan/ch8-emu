FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libsdl2-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

RUN cd /usr/src/gtest && \
    cmake . && \
    make && \
    cp lib/*.a /usr/lib

WORKDIR /app

COPY . .

RUN make

CMD ["make", "test"]
