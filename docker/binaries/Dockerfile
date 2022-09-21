FROM qoala101/stonks:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
RUN cd build && \
  cmake .. \
  -DFETCH_DEPENDENCIES_ONLY=OFF \
  -DBUILD_TESTS=OFF && \
  cmake --build . --config Release