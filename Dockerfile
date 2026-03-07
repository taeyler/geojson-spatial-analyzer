FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    git \
    cmake \
    ninja-build \
    g++ \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*

RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    /vcpkg/bootstrap-vcpkg.sh

ENV VCPKG_ROOT=/vcpkg
ENV VCPKG_DEFAULT_TRIPLET=arm64-linux

WORKDIR /app
COPY . .

RUN /vcpkg/vcpkg install cpr crow nlohmann-json

RUN cmake -B build \
    -G Ninja \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    cmake --build build

FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    ca-certificates \
    libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/geojson-spatial-analyzer-api .

COPY --from=builder /app/data ./data

COPY --from=builder /app/static ./static

EXPOSE 8080

CMD ["./geojson-spatial-analyzer-api"]