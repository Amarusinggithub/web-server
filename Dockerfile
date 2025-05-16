FROM gcc:latest
RUN apt-get update && apt-get install -y cmake make gdb
WORKDIR /web_server
COPY . .
EXPOSE 6969