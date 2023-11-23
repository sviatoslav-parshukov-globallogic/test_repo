#include <gst/gst.h>

#include "gRPCInterface.h"
#include "GStreamerPipeline.h"

int main(int argc, char** argv) 
{
    gst_init (&argc, &argv);

    //VZM_MediaContext VZM_CreateMediaContext(VZM_MediaParams params, VZM_MediaCallback callback)

    gRPCInterface server(
                         grpc::CreateChannel(
                                             "localhost:50051", 
                                             grpc::InsecureChannelCredentials()));

    size_t reply = server.registerClient();

    std::cout << "RPC::Received clientId:" << reply << std::endl;

    //int VZM_SetConfig(VZM_MediaContext ctx, const VZM_Config* config);

    std::string config=("video/x-h264");
    auto streamNameAndId = server.getStreamNameAndId(config);
    std::cout << "RPC::Received control name " << streamNameAndId.first << std::endl;

    GStreamerPipeline gst(streamNameAndId.first, config);

    std::cout << "RPC::Send READY for channel " << streamNameAndId.first << std::endl;

    server.streamReady(streamNameAndId.first, streamNameAndId.second);

    auto streamNameAndId2 = server.getStreamNameAndId(config);
    std::cout << "RPC::Received control name " << streamNameAndId2.first << std::endl;

    GStreamerPipeline gst2(streamNameAndId2.first, config);

    std::cout << "RPC::Send READY for channel " << streamNameAndId2.first << std::endl;

    server.streamReady(streamNameAndId2.first, streamNameAndId2.second);


    //VZM_PushBufferStatus_E VZM_PushBuffer(VZM_MediaContext ctx, uint8_t* buffer, int64_t timestamp);

    //int VZM_Start(VZM_MediaContext ctx, uint64_t start_pts);

    while(true) {};

    return 0;
}
