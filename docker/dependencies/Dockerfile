FROM debian:latest
WORKDIR /app
COPY CMakeLists.txt .
COPY conanfile.txt .
COPY conanprofile.txt .
RUN apt update && \
  apt -y install \
  cmake \
  \
  # Required to fetch git dependencies by cmake.
  git \
  \
  # Required to install latest clang.
  lsb-release software-properties-common gnupg wget \
  \
  # Required to install conan.
  pip && \
  \
  # Installs latest clang.
  bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)" && \
  \
  # Reduces image size.
  apt autoclean && \
  apt autoremove && \
  apt clean && \
  rm -rf /var/lib/apt/lists/* &&\
  pip --no-input install conan && \
  \
  mkdir build && \
  cd build && \
  \
  # Installs dependencies.
  conan install .. --build=missing -pr=../conanprofile.txt -s build_type=Release && \
  \
  conan remove "*" -s -b -f && \
  \
  # Fetches dependencies from git.
  cmake .. -DTHIRD_PARTY_ONLY=ON