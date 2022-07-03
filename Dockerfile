FROM ubuntu:20.04

RUN apt-get update -y \
&& apt-get upgrade -y \
&& apt-get install -y clang-10 clang++-10 make cmake
