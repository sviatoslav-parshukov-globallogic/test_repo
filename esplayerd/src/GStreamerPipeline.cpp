#include <iostream>
#include "GStreamerPipeline.h" 
#include <stdio.h>

GStreamerPipeline::GStreamerPipeline(std::string shmname, std::string meta,
                                     std::function<void(const std::string&)>&& amsgCallback)
    : shmname(shmname)
    , meta(meta)
    , clientId(atoi(shmname.substr(shmname.find(".") + 1, shmname.rfind(".") - shmname.find(".")).c_str()))
    , pipelineId(atoi(shmname.substr(shmname.rfind(".") + 1, shmname.length()).c_str()))
    , msgCallback{amsgCallback} {}

void GStreamerPipeline::ready()
{
    pipeline = gst_pipeline_new ("pipeline");
    shmsrc = gst_element_factory_make ("shmsrc", "src");
    fakesink = gst_element_factory_make ("fakesink", "sink");

    std::cout << "GST:: control name set to" << shmname << std::endl;

    g_object_set (G_OBJECT (shmsrc), "socket-path", 
                  shmname.c_str(), NULL);

    std::cout << "GST:: adding elements to pipeline" << std::endl;

    gst_bin_add_many (GST_BIN (pipeline), shmsrc, fakesink, NULL);

    std::cout << "GST:: linking pipeline " << std::endl;

    gst_element_link_many (shmsrc, fakesink, NULL);

    /* play */
    gst_element_set_state (pipeline, GST_STATE_PLAYING);
}


GStreamerPipeline::~GStreamerPipeline()
{

}
