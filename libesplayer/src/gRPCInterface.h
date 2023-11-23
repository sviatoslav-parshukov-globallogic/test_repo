#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>

#include "esplayerd.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using ESPlayer::Server;
using ESPlayer::RegisterClientReply;
using ESPlayer::MediaType;
using ESPlayer::ShmNameReply;
using ESPlayer::Result;
using ESPlayer::Stream;

class gRPCInterface {
 public:
  gRPCInterface(std::shared_ptr<Channel> channel)
      : stub_(Server::NewStub(channel)) {}

  size_t registerClient(); 
  std::pair<std::string, size_t> getStreamNameAndId(std::string meta); 
  int32_t streamReady(std::string, size_t pipelineId);

 private:
  std::unique_ptr<Server::Stub> stub_;
};
