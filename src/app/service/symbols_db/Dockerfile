FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/service_symbols_db ./bin/
RUN mkdir VOVA
ENTRYPOINT ["/app/bin/service_symbols_db", "--db_file_path", "./bin/VOVA/vova.db"]
EXPOSE 6506