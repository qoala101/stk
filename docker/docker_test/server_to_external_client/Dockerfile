FROM stonks_dependencies_image:latest
RUN apt -y install curl
WORKDIR /app
COPY ./include       ./include
COPY ./src           ./src
COPY ./test          ./test
COPY CMakeLists.txt .
RUN cd build && \
  cmake .. && \
  cmake --build . --target server_to_external_client --config Release
ENTRYPOINT ["/app/bin/server_to_external_client"]
EXPOSE 6507