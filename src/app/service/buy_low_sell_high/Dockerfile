FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/service_buy_low_sell_high ./bin/
ENTRYPOINT ["/app/bin/service_buy_low_sell_high"]
EXPOSE 6507