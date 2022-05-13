FROM ubuntu:22.04 
WORKDIR /src
COPY . .
RUN bash configure --compiler=g++ --no-mpi
RUN apt-get update && apt-get install build-essential -y
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install cmake -y
WORKDIR /src/build
RUN cmake ..
RUN make
RUN ctest
RUN make install
CMD panda
