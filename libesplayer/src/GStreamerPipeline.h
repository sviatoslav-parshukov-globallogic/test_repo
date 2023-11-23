#include <string>
#include <gst/gst.h>
#include <gst/app/gstappsrc.h>


class GStreamerPipeline
{
    private:
        GstElement *pipeline;
        GstElement *appsrc;
        GstElement *shmsink;

        static void needData (GstElement *appsrc, guint unused_size, gpointer user_data);    

    public:
        GStreamerPipeline(std::string shmname, std::string meta);
        ~GStreamerPipeline();
};
