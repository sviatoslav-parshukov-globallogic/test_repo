#ifndef GRPCSERVER_H
#define GRPCSERVER_H

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "esplayerd.grpc.pb.h"

void RunServer(uint16_t port);

#endif

