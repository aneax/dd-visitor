FROM ubuntu:focal

ARG DEBIAN_FRONTEND=noninteractive

ENV TZ=Europe/Berlin

RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

RUN apt-get update && apt-get install -y wget build-essential git cmake ca-certificates software-properties-common

RUN wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key| apt-key add - # Fingerprint: 6084 F3CF 814B 57C1 CF12 EFD5 15CF 4D18 AF4F 7421

RUN apt-get update && apt-add-repository "deb http://apt.llvm.org/focal/ llvm-toolchain-focal-15 main" 

RUN apt-get update

RUN apt-get install -y clang-15 clang-tools-15 lldb-15 clang-format-15 python3-clang-15 clangd-15 lld-15

RUN apt-get update && apt-get autoclean && apt-get autoremove && apt-get clean && rm -rf /var/lib/apt/lists/*

ENV CC=/usr/bin/clang-14 \
    CXX=/usr/bin/clang++-14

# These commands copy your files into the specified directory in the image
# and set that as the working location
COPY . /usr/src/docker_test
WORKDIR /usr/src/docker_test

# # This command compiles your app using GCC, adjust for your source code
# RUN mkdir build && cd build

# RUN cmake .. -DCMAKE_BUILD_TYPE=Release -g make

# RUN make -j

# CMD ["/usr/src/docker_test/build/tests/tests"]

# # This command runs your application, comment out this line to compile only
# CMD ["./dr"]

