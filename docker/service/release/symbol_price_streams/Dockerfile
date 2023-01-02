FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/service_symbol_price_streams ./bin/
ENTRYPOINT /app/bin/service_symbol_price_streams