#include <iostream>
#include "GStreamerPipeline.h" 


GStreamerPipeline::GStreamerPipeline(std::string shmname, std::string meta)
{
//    gst_init(NULL, NULL);
    pipeline = gst_pipeline_new ("pipeline");
    appsrc = gst_element_factory_make ("appsrc", "source");
    shmsink = gst_element_factory_make ("shmsink", "sink");

    std::cout << "GST:: caps set" << std::endl;

    g_object_set (G_OBJECT (appsrc), "caps",
                  gst_caps_new_simple (meta.c_str(),
                                       "format", G_TYPE_STRING, "RGB16",
                                       "width", G_TYPE_INT, 384,
                                       "height", G_TYPE_INT, 288,
                                       "framerate", GST_TYPE_FRACTION, 0, 1,
                                       NULL), NULL);

    std::cout << "GST:: control set to " << shmname << std::endl;

    g_object_set (G_OBJECT (shmsink), "socket-path", 
                  shmname.c_str(), NULL);

    std::cout << "GST:: shm size set to 1000000" << std::endl;

    g_object_set (G_OBJECT (shmsink), "shm-size", 
                  1000000, NULL);

    std::cout << "GST:: add elements" << std::endl;

    gst_bin_add_many (GST_BIN (pipeline), appsrc, shmsink, NULL);

    std::cout << "GST:: link pipeline" << std::endl;

    gst_element_link_many (appsrc, shmsink, NULL);

    /* setup appsrc */
    g_object_set (G_OBJECT (appsrc),
                  "stream-type", 0, // GST_APP_STREAM_TYPE_STREAM
                  "format", GST_FORMAT_TIME,
                  "is-live", TRUE,
                  NULL);

    g_signal_connect (appsrc, "need-data", G_CALLBACK (GStreamerPipeline::needData), NULL);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}

void GStreamerPipeline::needData (GstElement *appsrc, guint unused_size, gpointer user_data)    
{
     std::cout << "GST:: need data callback" << std::endl;
};


GStreamerPipeline::~GStreamerPipeline()
{

}
