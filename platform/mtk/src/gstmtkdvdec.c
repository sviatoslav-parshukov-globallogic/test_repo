/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkdvdec
 *
 * The mtkdvdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkdvdec ! FIXME ! fakesink
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
#include "gstmtkdvdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkdvdec_debug_category);
#define GST_CAT_DEFAULT gst_mtkdvdec_debug_category

/* prototypes */


static void gst_mtkdvdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkdvdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkdvdec_dispose (GObject * object);
static void gst_mtkdvdec_finalize (GObject * object);

static gboolean gst_mtkdvdec_open (GstVideoDecoder * decoder);
static gboolean gst_mtkdvdec_close (GstVideoDecoder * decoder);
static gboolean gst_mtkdvdec_start (GstVideoDecoder * decoder);
static gboolean gst_mtkdvdec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkdvdec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_mtkdvdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_mtkdvdec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkdvdec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkdvdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_mtkdvdec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkdvdec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkdvdec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_mtkdvdec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkdvdec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mtkdvdec_src_template =
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


static GstStaticPadTemplate gst_mtkdvdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-dv, systemstream=false")
    );

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkdvdec, gst_mtkdvdec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkdvdec_debug_category, "mtkdvdec", 0,
        "debug category for mtkdvdec element"));

static void
gst_mtkdvdec_class_init (GstMtkdvdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkdvdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkdvdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK DV decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkdvdec_set_property;
  gobject_class->get_property = gst_mtkdvdec_get_property;
  gobject_class->dispose = gst_mtkdvdec_dispose;
  gobject_class->finalize = gst_mtkdvdec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkdvdec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkdvdec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkdvdec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkdvdec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkdvdec_parse);
  video_decoder_class->set_format = GST_DEBUG_FUNCPTR (gst_mtkdvdec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_mtkdvdec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_mtkdvdec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkdvdec_handle_frame);
  video_decoder_class->sink_event = GST_DEBUG_FUNCPTR (gst_mtkdvdec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkdvdec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkdvdec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkdvdec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkdvdec_propose_allocation);

}

static void
gst_mtkdvdec_init (GstMtkdvdec * mtkdvdec)
{
}

void
gst_mtkdvdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (mtkdvdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkdvdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (mtkdvdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkdvdec_dispose (GObject * object)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (mtkdvdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkdvdec_parent_class)->dispose (object);
}

void
gst_mtkdvdec_finalize (GObject * object)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (object);

  GST_DEBUG_OBJECT (mtkdvdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkdvdec_parent_class)->finalize (object);
}

static gboolean
gst_mtkdvdec_open (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "open");

  return TRUE;
}

static gboolean
gst_mtkdvdec_close (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "close");

  return TRUE;
}

static gboolean
gst_mtkdvdec_start (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "start");

  return TRUE;
}

static gboolean
gst_mtkdvdec_stop (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_mtkdvdec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkdvdec_set_format (GstVideoDecoder * decoder, GstVideoCodecState * state)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "set_format");

  return TRUE;
}

static gboolean
gst_mtkdvdec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_mtkdvdec_finish (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkdvdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkdvdec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkdvdec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkdvdec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkdvdec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkdvdec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkdvdec *mtkdvdec = GST_MTKDVDEC (decoder);

  GST_DEBUG_OBJECT (mtkdvdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkdvdec", GST_RANK_NONE,
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
//    mtkdvdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
