FROM stonks-dependencies:latest
WORKDIR /app
COPY ./include       ./include
COPY ./src           ./src
COPY ./test          ./test
COPY CMakeLists.txt .
RUN cd build && \
  cmake .. && \
  cmake --build . --target persistent_db_server --config Release
ENTRYPOINT ["/app/bin/persistent_db_server"]
EXPOSE 6506