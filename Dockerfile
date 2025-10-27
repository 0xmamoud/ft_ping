FROM debian:bullseye

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    gdb \
    valgrind \
    inetutils-ping \
    net-tools \
    vim \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /ft_ping
CMD ["/bin/bash"]
