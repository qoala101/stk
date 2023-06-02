FROM qoala101/stk:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
COPY ./test ./test
RUN \
  export CC=/usr/bin/clang-15 && \
  export CXX=/usr/bin/clang++-15 && \
  cmake -B ./build --fresh -DFAIL_ON_WARNINGS=ON -DBUILD_TESTS=ON && \
  cmake --build ./build && \
  ./bin/unittests