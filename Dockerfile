FROM gcc:latest as build
RUN apt-get update && apt-get install -y cmake make gdb
WORKDIR /web_server
COPY . .
WORKDIR /web_server/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --parallel 8 \

EXPOSE 6969

ENTRYPOINT [ "./app/simplehttpserver" ]




FROM gcc:latest as builder

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      build-essential \
      cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir build && \
    cd build && \
    cmake -DCMAKE_BUILD_TYPE=Release .. && \
    cmake --build . --config Release


FROM ubuntu:22.04

RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends \
      libstdc++6 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/web_server .

EXPOSE 6969

CMD ["./web_server"]