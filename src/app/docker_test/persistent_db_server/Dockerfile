FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:libraries \
  /app/bin/persistent_db_server ./bin/
ENTRYPOINT ["/app/bin/persistent_db_server"]
EXPOSE 6506