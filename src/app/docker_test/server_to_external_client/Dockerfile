FROM stonks-dependencies AS builder
WORKDIR /app
COPY ./include       ./include
COPY ./src           ./src
COPY ./test          ./test
COPY CMakeLists.txt .
RUN cd build && \
  cmake .. && \
  cmake --build . --target server_to_external_client --config Release

FROM debian:latest
WORKDIR /app
COPY --from=builder /app/bin/server_to_external_client .
ENTRYPOINT ["/app/server_to_external_client"]
EXPOSE 6507