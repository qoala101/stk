FROM qoala101/stk:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
RUN \
  cmake -B ./build -DBUILD_TESTS=OFF && \
  cmake --build ./build --config Release