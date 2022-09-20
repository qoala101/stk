FROM stonks-dependencies AS builder
WORKDIR /app
COPY ./include       ./include
COPY ./src           ./src
COPY ./test          ./test
COPY CMakeLists.txt .
RUN cd build && \
  cmake .. && \
  cmake --build . --target persistent_db_server --config Release

FROM debian:latest
WORKDIR /app
COPY --from=builder /app/bin/persistent_db_server .
ENTRYPOINT ["/app/persistent_db_server"]
EXPOSE 6506