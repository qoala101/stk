FROM qoala101/stonks:base
WORKDIR /app
COPY --from=qoala101/stonks:binaries \
  /app/bin/service_ngrok_uri_exposer ./bin/
ENTRYPOINT "/app/bin/service_ngrok_uri_exposer"