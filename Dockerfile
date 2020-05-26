# created by jakub
FROM ubuntu as builder
RUN apt-get update
RUN apt-get -y install g++ cmake libboost-test-dev protobuf-compiler libprotobuf-dev
WORKDIR /app
COPY . .
RUN cmake .
ENTRYPOINT make