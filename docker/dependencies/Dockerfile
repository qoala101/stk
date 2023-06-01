FROM debian:11
WORKDIR /app
COPY ./thirdparty ./thirdparty
COPY CMakeLists.txt .
COPY conanfile.txt .
COPY conanprofile.txt .
RUN \
  apt-get update && \
  apt-get -y install \
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
  # Installs latest clang.
  bash -c "$(wget -nv -O - https://apt.llvm.org/llvm.sh)" && \
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
  conan install . -if ./build --build=missing -pr=../conanprofile.txt -s build_type=Release && \
  \
  conan remove "*" -s -b -f && \
  \
  # Fetches dependencies from git.
  cmake -B ./build -DTHIRD_PARTY_ONLY=ON