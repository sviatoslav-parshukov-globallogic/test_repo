#include <memory>
#include <map>
#include <iostream>
#include "mainLoop.h"

MainLoop::MainLoop()
{
};

void MainLoop::newClient(const size_t& clid)
{
    clients[clid] = std::make_unique<DaemonClient>(clid);
}

std::string MainLoop::createPipeline(const size_t& clid, std::string metadata)
{
    size_t pipelineId = clients[clid]->pipelines.size();
   
    std::string shmname = std::string("/tmp/esply.") + std::to_string(clid) + "." + std::to_string(pipelineId);

    clients[clid]->pipelines[pipelineId] = std::make_unique<GStreamerPipeline>(shmname, metadata, [this, clid, pipelineId](const std::string& msg)
    {
        handleNewMessageFromPipeline(clid, pipelineId, msg);
    });

    return shmname;
}

void MainLoop::handleNewMessageFromPipeline(size_t clid, size_t pipelineId, const std::string& msg)
{
    
}

int32_t MainLoop::ready(const size_t& clid, const size_t& pipelineId)
{
    clients[clid]->pipelines[pipelineId]->ready();

    return 0;
}

int32_t MainLoop::play(const size_t& clid, const size_t& pipelineeId)
{
    return 0;
};

int32_t MainLoop::stop(const size_t& clid, const size_t& pipelineId)
{
    return 0;
}

void MainLoop::cleanup (const size_t& clid, const size_t& pipelineId)
{
}

//void loadConfig() {};

