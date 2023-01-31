FROM qoala101/stk:base
ARG executable_name
ENV EXECUTABLE_NAME=$executable_name
WORKDIR /app
COPY --from=qoala101/stk:binaries /app/bin/$executable_name ./bin/service
ENTRYPOINT ["/app/bin/service"]