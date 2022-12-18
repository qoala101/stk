add_executable(service_ngrok_exposer
  main.cc
)

target_link_libraries(service_ngrok_exposer
  PRIVATE libaws
  PRIVATE libcli
  PRIVATE libcore_app_uri_exposer
  PRIVATE libnetwork
  PRIVATE librestsdk
  PRIVATE libservice
  PRIVATE libspdlog
)