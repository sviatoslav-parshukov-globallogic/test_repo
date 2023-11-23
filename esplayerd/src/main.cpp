#include <iostream>
#include <gst/gst.h>
#include "gRPCServer.h"

int main(int argc, char * argv[])
{

    gst_init(&argc, &argv);

    RunServer(50051);
    return 0;
}
