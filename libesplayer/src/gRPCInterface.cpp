#include <string> 
#include "gRPCInterface.h"

size_t gRPCInterface::registerClient()
{
    ::google::protobuf::Empty request;
//    request.set_name(user);

    // Container for the data we expect from the server.
    RegisterClientReply reply;

    // Context for the client. It could be used to convey extra information to
    // the server and/or tweak certain RPC behaviors.
    ClientContext context;

    // The actual RPC.
    Status status = stub_->RegisterClient(&context, request, &reply);

    // Act upon its status.
    if (status.ok()) {
      return reply.clientid();
    } else {
      std::cout << status.error_code() << ": " << status.error_message()
                << std::endl;
      return 0;
    }
}

std::pair<std::string, size_t> gRPCInterface::getStreamNameAndId(std::string meta) 
{
   MediaType request;
   request.set_mediatype(meta);

   ShmNameReply reply;

   ClientContext context;

    // The actual RPC.
   Status status = stub_->ShmNameRequest(&context, request, &reply);

   // Act upon its status.
   if (status.ok()) {
       return {reply.stream().Get(0).shmname(), reply.stream().Get(0).pipelineid()};
   } else {
       std::cout << status.error_code() << ": " << status.error_message()
           << std::endl;
       return {};
   }
}

int32_t gRPCInterface::streamReady(std::string name, size_t pipelineId) 
{
   Stream request;
   request.set_shmname(name);
   request.set_pipelineid(pipelineId);

   Result reply;

   ClientContext context;

    // The actual RPC.
   Status status = stub_->ShmReady(&context, request, &reply);

   // Act upon its status.
   if (status.ok()) {
       return reply.error();
   } else {
       std::cout << status.error_code() << ": " << status.error_message()
           << std::endl;
       return 0;
   }
}
