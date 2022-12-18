FROM ubuntu:22.04
# Installation of build dependencies
RUN apt-get update && apt-get install build-essential -y
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install cmake -y
# Preparation of build
WORKDIR /src
COPY configure /src/
RUN bash configure --compiler=g++ --no-mpi
# Compilation
WORKDIR /src
COPY . .
WORKDIR /src/build
RUN cmake ..
RUN make
RUN ctest
RUN make install
CMD panda
