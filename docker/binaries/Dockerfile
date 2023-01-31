FROM qoala101/stk:dependencies
WORKDIR /app
COPY ./include ./include
COPY ./src ./src
RUN cd build && \
  cmake .. \
  -DTHIRD_PARTY_ONLY=OFF \
  -DBUILD_TESTS=OFF && \
  cmake --build . --config Release