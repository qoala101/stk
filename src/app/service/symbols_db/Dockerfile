FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/service_symbols_db ./bin/
ENTRYPOINT /app/bin/service_symbols_db