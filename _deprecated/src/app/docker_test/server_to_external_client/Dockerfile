FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:libraries \
  /app/bin/server_to_external_client ./bin/
ENTRYPOINT ["/app/bin/server_to_external_client"]
EXPOSE 6507