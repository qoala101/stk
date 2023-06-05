FROM debian:11
RUN \
  apt-get update && \
  apt-get install -y --no-install-recommends \
  \
  # Required for networking to work.
  libcurl3-gnutls \
  \
  # Required for AWS.
  libatomic1 && \
  \
  # Reduces image size.
  apt-get autoclean && \
  apt-get autoremove && \
  apt-get clean && \
  rm -rf /var/lib/apt/lists/*