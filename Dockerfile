# Build Stage
FROM --platform=linux/amd64 ubuntu:20.04 as builder

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y cmake

## Install build dependencies.
RUN apt-get update && \
    DEBIAN_FRONTEND=noninteractive apt-get install -y autoconf

## Add source code to the build stage.
ADD . /semblance
WORKDIR /semblance

## TODO: ADD YOUR BUILD INSTRUCTIONS HERE.
RUN ./autogen.sh && ./configure && make

#Package Stage
FROM --platform=linux/amd64 ubuntu:20.04

## TODO: Change <Path in Builder Stage>
COPY --from=builder /semblance/dump /
