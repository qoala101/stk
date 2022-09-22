FROM debian:latest
WORKDIR /app
COPY CMakeLists.txt  .
COPY conanfile.txt  .
RUN apt update && \
  apt -y install \
  cmake \
  git \
  iwyu \
  pip && \
  apt autoclean && \
  apt autoremove && \
  apt clean && \
  rm -rf /var/lib/apt/lists/* &&\
  pip --no-input install conan && \
  conan profile new default || true && \
  conan profile update settings.os=Linux default && \
  conan profile update settings.arch=x86_64 default && \
  conan profile update settings.compiler=clang default && \
  conan profile update settings.compiler.libcxx=libstdc++11 default && \
  conan profile update settings.compiler.version=11 default && \
  conan profile update settings.build_type=Release default && \
  conan profile update env.CC=/usr/bin/clang-11 default && \
  conan profile update env.CXX=/usr/bin/clang++-11 default && \
  mkdir build && \
  cd build && \
  conan install .. --build=missing -pr=default && \
  conan remove "*" -s -b -f && \
  cmake .. -DFETCH_DEPENDENCIES_ONLY=ON