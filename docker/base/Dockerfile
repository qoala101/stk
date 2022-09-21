FROM debian:latest
RUN apt update && \
  apt -y install libcurl3-gnutls && \
  apt autoclean && \
  apt autoremove && \
  apt clean && \
  rm -rf /var/lib/apt/lists/*