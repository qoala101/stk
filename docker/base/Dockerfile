FROM debian:latest
RUN apt update && \
  apt -y install \
  \
  # Required for networking to work.
  libcurl3-gnutls \
  \
  # Required for AWS.
  libatomic1 && \
  \
  # Reduces image size.
  apt autoclean && \
  apt autoremove && \
  apt clean && \
  rm -rf /var/lib/apt/lists/*