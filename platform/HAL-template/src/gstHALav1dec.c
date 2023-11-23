/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALav1dec
 *
 * The HALav1dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALav1dec ! FIXME ! fakesink
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
#include "gstHALav1dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALav1dec_debug_category);
#define GST_CAT_DEFAULT gst_HALav1dec_debug_category

/* prototypes */


static void gst_HALav1dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALav1dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALav1dec_dispose (GObject * object);
static void gst_HALav1dec_finalize (GObject * object);

static gboolean gst_HALav1dec_open (GstVideoDecoder * decoder);
static gboolean gst_HALav1dec_close (GstVideoDecoder * decoder);
static gboolean gst_HALav1dec_start (GstVideoDecoder * decoder);
static gboolean gst_HALav1dec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALav1dec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_HALav1dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_HALav1dec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_HALav1dec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALav1dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_HALav1dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALav1dec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALav1dec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_HALav1dec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALav1dec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_HALav1dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-av1")
    );

static GstStaticPadTemplate gst_HALav1dec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "video/x-raw,"
                      "format={ (string)I420, (string)YV12, (string)Y42B, (string)Y444, (string)I420_10LE, (string)I420_12LE, (string)I422_10LE, (string)I422_12LE, (string)Y444_10LE, (string)Y444_12LE },"
                      "width=[ 1, 2147483647 ],"
                      "height=[ 1, 2147483647 ],"
                      "framerate=[ 0/1, 2147483647/1 ]")
    );

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkav1dec, gst_HALav1dec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALav1dec_debug_category, "HALav1dec", 0,
        "debug category for HALav1dec element"));

static void
gst_HALav1dec_class_init (GstMtkav1decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALav1dec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALav1dec_sink_template);


  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK AV1 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALav1dec_set_property;
  gobject_class->get_property = gst_HALav1dec_get_property;
  gobject_class->dispose = gst_HALav1dec_dispose;
  gobject_class->finalize = gst_HALav1dec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALav1dec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALav1dec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALav1dec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALav1dec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALav1dec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_HALav1dec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_HALav1dec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_HALav1dec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALav1dec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_HALav1dec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_HALav1dec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_HALav1dec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALav1dec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALav1dec_propose_allocation);

}

static void
gst_HALav1dec_init (GstMtkav1dec * HALav1dec)
{
}

void
gst_HALav1dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (object);

  GST_DEBUG_OBJECT (HALav1dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALav1dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (object);

  GST_DEBUG_OBJECT (HALav1dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALav1dec_dispose (GObject * object)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (object);

  GST_DEBUG_OBJECT (HALav1dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALav1dec_parent_class)->dispose (object);
}

void
gst_HALav1dec_finalize (GObject * object)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (object);

  GST_DEBUG_OBJECT (HALav1dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALav1dec_parent_class)->finalize (object);
}

static gboolean
gst_HALav1dec_open (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "open");

  return TRUE;
}

static gboolean
gst_HALav1dec_close (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "close");

  return TRUE;
}

static gboolean
gst_HALav1dec_start (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "start");

  return TRUE;
}

static gboolean
gst_HALav1dec_stop (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_HALav1dec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_HALav1dec_set_format (GstVideoDecoder * decoder, GstVideoCodecState * state)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "set_format");

  return TRUE;
}

static gboolean
gst_HALav1dec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_HALav1dec_finish (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALav1dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_HALav1dec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALav1dec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "src_event");

  return TRUE;
}

static gboolean
gst_HALav1dec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALav1dec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALav1dec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkav1dec *HALav1dec = GST_MTKAV1DEC (decoder);

  GST_DEBUG_OBJECT (HALav1dec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALav1dec", GST_RANK_NONE,
      GST_TYPE_MTKAV1DEC);
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
//    HALav1dec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
