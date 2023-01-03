FROM qoala101/stonks:base
ARG executable_name
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/$executable_name ./bin/
ENTRYPOINT /app/bin/$executable_name