/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALdvdec
 *
 * The HALdvdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALdvdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>
#include "gstHALdvdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALdvdec_debug_category);
#define GST_CAT_DEFAULT gst_HALdvdec_debug_category

/* prototypes */


static void gst_HALdvdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALdvdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALdvdec_dispose (GObject * object);
static void gst_HALdvdec_finalize (GObject * object);

static gboolean gst_HALdvdec_open (GstVideoDecoder * decoder);
static gboolean gst_HALdvdec_close (GstVideoDecoder * decoder);
static gboolean gst_HALdvdec_start (GstVideoDecoder * decoder);
static gboolean gst_HALdvdec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALdvdec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_HALdvdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_HALdvdec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_HALdvdec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALdvdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_HALdvdec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALdvdec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALdvdec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_HALdvdec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALdvdec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_HALdvdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw,"
                     "format={ (string)YUY2, (string)BGRx, (string)RGB },"
                     "framerate=[ 1/1, 60/1 ],"
                     "width=720,"
                     "height={ (int)576, (int)480 }")
    );

/* FIXME: add/remove formats you can provide */
#define VIDEO_SINK_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")


static GstStaticPadTemplate gst_HALdvdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-dv, systemstream=false")
    );

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkdvdec, gst_HALdvdec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALdvdec_debug_category, "HALdvdec", 0,
        "debug category for HALdvdec element"));

static void
gst_HALdvdec_class_init (GstMtkdvdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALdvdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALdvdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK DV decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALdvdec_set_property;
  gobject_class->get_property = gst_HALdvdec_get_property;
  gobject_class->dispose = gst_HALdvdec_dispose;
  gobject_class->finalize = gst_HALdvdec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALdvdec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALdvdec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALdvdec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALdvdec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALdvdec_parse);
  video_decoder_class->set_format = GST_DEBUG_FUNCPTR (gst_HALdvdec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_HALdvdec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_HALdvdec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALdvdec_handle_frame);
  video_decoder_class->sink_event = GST_DEBUG_FUNCPTR (gst_HALdvdec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_HALdvdec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_HALdvdec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALdvdec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALdvdec_propose_allocation);

}

static void
gst_HALdvdec_init (GstMtkdvdec * HALdvdec)
{
}

void
gst_HALdvdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (HALdvdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALdvdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (HALdvdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALdvdec_dispose (GObject * object)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (HALdvdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALdvdec_parent_class)->dispose (object);
}

void
gst_HALdvdec_finalize (GObject * object)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (HALdvdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALdvdec_parent_class)->finalize (object);
}

static gboolean
gst_HALdvdec_open (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "open");

  return TRUE;
}

static gboolean
gst_HALdvdec_close (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "close");

  return TRUE;
}

static gboolean
gst_HALdvdec_start (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "start");

  return TRUE;
}

static gboolean
gst_HALdvdec_stop (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_HALdvdec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_HALdvdec_set_format (GstVideoDecoder * decoder, GstVideoCodecState * state)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "set_format");

  return TRUE;
}

static gboolean
gst_HALdvdec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_HALdvdec_finish (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALdvdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_HALdvdec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALdvdec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "src_event");

  return TRUE;
}

static gboolean
gst_HALdvdec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALdvdec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALdvdec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkdvdec *HALdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (HALdvdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALdvdec", GST_RANK_NONE,
      GST_TYPE_MTKDVDEC);
}
*/

/* FIXME: these are normally defined by the GStreamer build system.
   If you are creating an element to be included in gst-plugins-*,
   remove these, as they're always defined.  Otherwise, edit as
   appropriate for your external plugin package. */
#ifndef VERSION
#define VERSION "0.0.FIXME"
#endif
#ifndef PACKAGE
#define PACKAGE "FIXME_package"
#endif
#ifndef PACKAGE_NAME
#define PACKAGE_NAME "FIXME_package_name"
#endif
#ifndef GST_PACKAGE_ORIGIN
#define GST_PACKAGE_ORIGIN "http://FIXME.org/"
#endif

//GST_PLUGIN_DEFINE (GST_VERSION_MAJOR,
//    GST_VERSION_MINOR,
//    HALdvdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
