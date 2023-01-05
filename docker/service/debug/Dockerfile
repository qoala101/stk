FROM qoala101/stonks:base
ARG executable_name
ENV EXECUTABLE_NAME=$executable_name
WORKDIR /app
COPY ./bin/$executable_name ./bin/service
ENTRYPOINT ["/app/bin/service"]