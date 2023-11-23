#ifndef MAINLOOP_H
#define MAINLOOP_H
#include <string>
#include <memory>
#include <map>
#include "GStreamerPipeline.h"


class MainLoop
{
    private:
        struct DaemonClient
        {
            explicit DaemonClient(size_t aclientId) : clientId{aclientId} {}
            const size_t clientId;
            std::map<size_t, std::unique_ptr<GStreamerPipeline>> pipelines;
        };

        std::map<size_t, std::unique_ptr<DaemonClient>> clients;

        void handleNewMessageFromPipeline(size_t clid, size_t pipelineId, const std::string& msg);

    public:

        MainLoop();

        void newClient(const size_t& clid);

        std::string createPipeline(const size_t& clid, std::string metadata);
        int32_t ready(const size_t& clid, const size_t& pipelineId);
        int32_t play(const size_t& clid, const size_t& pipelineId);
        int32_t stop(const size_t& clid, const size_t& pipelineId);
        void cleanup (const size_t& clid, const size_t& pipelineId);

        void loadConfig() {};

};


#endif

