FROM qoala101/stk:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
RUN \
  cd build && \
  cmake .. \
  -DBUILD_TESTS=OFF && \
  cmake --build . --config Release