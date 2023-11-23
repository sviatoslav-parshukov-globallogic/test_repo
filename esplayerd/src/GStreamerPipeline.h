#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <functional>

class GStreamerPipeline
{
    private:
        GstElement *pipeline;
        GstElement *shmsrc;
        GstElement *fakesink;

        std::string shmname;
        std::string meta;

        const size_t clientId;
        const size_t pipelineId;

        std::function<void(const std::string&)> msgCallback;
//        static void needData (GstElement *appsrc, guint unused_size, gpointer user_data);    

    public:
        GStreamerPipeline(std::string shmname, std::string meta, std::function<void(const std::string&)>&& amsgCallback);
        void ready();
        ~GStreamerPipeline();
};
