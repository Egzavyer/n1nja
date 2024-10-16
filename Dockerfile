FROM debian:bookworm-20220316-slim

WORKDIR /home/debian/n1nja/build

RUN apt-get update && apt-get install -y \
    cmake \
    build-essential \
    wget \
    git

COPY . .

EXPOSE 8080

