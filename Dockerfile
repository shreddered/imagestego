FROM arm64v8/ubuntu
ENV DEBIAN_FRONTEND noninteractive
RUN apt update && \
    apt install -y cmake libopencv-dev gcc g++
