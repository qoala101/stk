FROM qoala101/stonks:base
ARG executable_name
WORKDIR /app
COPY ./bin/$executable_name ./bin/
ENTRYPOINT /app/bin/$executable_name