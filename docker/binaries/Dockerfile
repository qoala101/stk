FROM qoala101/stk:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
RUN \
  export CC=/usr/bin/clang-15 && \
  export CXX=/usr/bin/clang++-15 && \
  cmake -B ./build --fresh && \
  cmake --build ./build --config Release