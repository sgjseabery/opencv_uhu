FROM ubuntu:25.04
ARG dev_user

LABEL description="OpenCV Course UHU - Seabery"

ENV DEBIAN_FRONTEND=noninteractive

RUN userdel -r ubuntu || true

# Basic packages
RUN apt-get update \
    && apt-get -y install \
    bash-completion \
    cmake \
    build-essential \
    g++-14 \
    gcc-14 \
    init \
    libudev-dev \
    libv4l-dev \
    libturbojpeg0-dev \
    pkg-config \
    libgtk-4-1 \
    libgstreamer1.0-0 \
    libgstreamer-gl1.0-0 \
    libglew2.2 \
    usbutils \
    libopencv-dev \
    python3-opencv

# Enable bash completion
RUN echo '[[ -r "/usr/share/bash-completion/bash_completion" ]] && . "/usr/share/bash-completion/bash_completion"' >> ~/.bashrc

# Codecs and highgui
RUN apt-get -y --no-install-recommends install \
    ffmpeg \
    libavcodec61 \
    libavformat61 \
    libavutil59 \
    libgtkglext1-dev \
    libswscale8

# Set locales and timezone
RUN apt-get -y install locales
RUN sed -i '/es_ES.UTF-8/s/^# //g' /etc/locale.gen && \
    locale-gen
ENV LANG=es_ES.UTF-8
ENV LANGUAGE=es_ES:es
ENV LC_ALL=es_ES.UTF-8
ENV TZ=Europe/Madrid
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install sudo
RUN apt-get -y --no-install-recommends install sudo

# Add the development user and add them to the sudo group
RUN useradd -m -s /bin/bash "$dev_user"
RUN usermod -aG sudo "$dev_user"
RUN usermod -aG video "$dev_user"

# Configure passwordless sudo for the development user
RUN echo "$dev_user ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/"$dev_user" && \
chmod 0440 /etc/sudoers.d/"$dev_user"

# Install OpenCV 4.10
RUN echo "deb http://archive.ubuntu.com/ubuntu plucky-proposed multiverse restricted main universe" | tee -a /etc/apt/sources.list
RUN apt-get update
RUN apt install -y libopencv-dev/plucky-proposed
RUN apt install -y python3-opencv/plucky-proposed


ENTRYPOINT ["tail", "-f", "/dev/null"]