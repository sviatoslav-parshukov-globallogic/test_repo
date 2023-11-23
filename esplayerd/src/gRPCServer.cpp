#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>

#include "esplayerd.pb.h"    
#include "esplayerd.grpc.pb.h"    
#include "gRPCServer.h"
#include "mainLoop.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using ESPlayer::RegisterClientReply;
using ESPlayer::MediaType;
using ESPlayer::ShmNameReply;
using ESPlayer::Result;
using ESPlayer::Stream;


class ESPlayerServiceImpl final : public ESPlayer::Server::Service 
{
    private: 
        std::shared_ptr<MainLoop> mainLoop;

        size_t convertClientID(std::string clientId)
        {
            std::hash<std::string> hasher;
            return hasher(clientId);
        }

    public:

        ESPlayerServiceImpl() : ESPlayer::Server::Service()
        {
            mainLoop = std::make_shared<MainLoop>();
        }

        Status RegisterClient(ServerContext* context, const ::google::protobuf::Empty* request,
                  RegisterClientReply* reply) override 
        {
            auto id = convertClientID(context->peer());

            std::cout << "RPC::RegisterClient: clientId " << id << std::endl;

            mainLoop->newClient(id);
            reply->set_clientid(id);
            return Status::OK;
        }

        Status ShmNameRequest(ServerContext* context, const MediaType* request,
                  ShmNameReply* reply) override 
        {
            auto id = convertClientID(context->peer());

            std::cout << "RPC::ShmNameRequest: clientId " << id << ", " << request->mediatype() << std::endl;

            auto res = mainLoop->createPipeline(id, request->mediatype());

            std::cout << "RPC::ShmNameRequest: clientId " << id << ", created pipeline" << std::endl;

            std::remove(res.c_str());

            Stream* stream = reply->add_stream();
            stream->set_shmname(res);
            Result* result = reply->add_result();
            result->set_error(1);

            return Status::OK;
        }


        Status ShmReady(ServerContext* context, const Stream* request,
                  Result* reply) override 
        {
            auto id = convertClientID(context->peer());

            std::cout << "RPC::ShmReady: clientId " << id << ", " << request->shmname() << std::endl;
            mainLoop->ready(id, request->pipelineid());

            reply->set_error(1);

            return Status::OK;
        }




};

void RunServer(uint16_t port)
{
    std::string server_address = std::string("localhost:") + std::to_string(port);
    ESPlayerServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "RPC::Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

