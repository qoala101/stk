FROM debian:11
WORKDIR /app
COPY ./thirdparty ./thirdparty
COPY CMakeLists.txt .
COPY conanfile.txt .
COPY conanprofile.txt .
RUN \
  apt-get update && \
  apt-get install -y \
  \
  # Required to fetch git dependencies by cmake.
  git \
  \
  # Required to install latest clang.
  lsb-release software-properties-common gnupg wget \
  \
  # Required to install conan and latest cmake.
  pip && \
  \
  # Installs Clang.
  wget -nv https://apt.llvm.org/llvm.sh && \
  bash ./llvm.sh 15 && \
  \
  # Reduces image size.
  apt-get autoclean && \
  apt-get autoremove && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/* && \
  \
  pip --no-cache-dir --no-input install cmake conan==1.60.0 && \
  \
  # Installs dependencies.
  conan config set general.revisions_enabled=True && \
  conan install . -if ./build --build=missing -pr=./conanprofile.txt -s build_type=Release && \
  \
  conan remove "*" -s -b -f && \
  \
  # Fetches dependencies from git.
  export CC=/usr/bin/clang-15 && \
  export CXX=/usr/bin/clang++-15 && \
  cmake -B ./build -DTHIRD_PARTY_ONLY=ON