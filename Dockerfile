FROM debian:latest

RUN apt-get update && \
    apt-get install -y \
    make \
    build-essential \
    manpages-dev \
    iputils-ping \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app