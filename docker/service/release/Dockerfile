FROM qoala101/stonks:base
ARG executable_name
ENV EXECUTABLE_NAME=$executable_name
WORKDIR /app
COPY --from=qoala101/stonks:binaries /app/bin/$executable_name ./bin/
ENTRYPOINT exec /app/bin/$EXECUTABLE_NAME