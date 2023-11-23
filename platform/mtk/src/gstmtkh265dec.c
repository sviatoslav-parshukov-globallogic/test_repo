/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkh265dec
 *
 * The mtkh265dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkh265dec ! FIXME ! fakesink
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
#include "gstmtkh265dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkh265dec_debug_category);
#define GST_CAT_DEFAULT gst_mtkh265dec_debug_category

/* prototypes */


static void gst_mtkh265dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkh265dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkh265dec_dispose (GObject * object);
static void gst_mtkh265dec_finalize (GObject * object);

static gboolean gst_mtkh265dec_open (GstVideoDecoder * decoder);
static gboolean gst_mtkh265dec_close (GstVideoDecoder * decoder);
static gboolean gst_mtkh265dec_start (GstVideoDecoder * decoder);
static gboolean gst_mtkh265dec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkh265dec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_mtkh265dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_mtkh265dec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkh265dec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkh265dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_mtkh265dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkh265dec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkh265dec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_mtkh265dec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkh265dec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mtkh265dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "video/x-h265,"
                      "stream-format={ (string)hvc1, (string)hev1, (string)byte-stream },"
                      "alignment={ (string)au, (string)nal }")
    );

static GstStaticPadTemplate gst_mtkh265dec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "video/x-raw,"
                      "format=I420,"
                      "width=[ 1, 2147483647 ],"
                      "height=[ 1, 2147483647 ],"
                      "framerate=[ 0/1, 2147483647/1 ]")
    );


/* FIXME: add/remove formats you can handle */
#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420, Y444, Y42B, UYVY, RGBA }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkh265dec, gst_mtkh265dec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkh265dec_debug_category, "mtkh265dec", 0,
        "debug category for mtkh265dec element"));

static void
gst_mtkh265dec_class_init (GstMtkh265decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkh265dec_sink_template);

   gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkh265dec_src_template);
  
  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK H.265 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkh265dec_set_property;
  gobject_class->get_property = gst_mtkh265dec_get_property;
  gobject_class->dispose = gst_mtkh265dec_dispose;
  gobject_class->finalize = gst_mtkh265dec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkh265dec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkh265dec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkh265dec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkh265dec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkh265dec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkh265dec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_mtkh265dec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_mtkh265dec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkh265dec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkh265dec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkh265dec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkh265dec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkh265dec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkh265dec_propose_allocation);

}

static void
gst_mtkh265dec_init (GstMtkh265dec * mtkh265dec)
{
}

void
gst_mtkh265dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (object);

  GST_DEBUG_OBJECT (mtkh265dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkh265dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (object);

  GST_DEBUG_OBJECT (mtkh265dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkh265dec_dispose (GObject * object)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (object);

  GST_DEBUG_OBJECT (mtkh265dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkh265dec_parent_class)->dispose (object);
}

void
gst_mtkh265dec_finalize (GObject * object)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (object);

  GST_DEBUG_OBJECT (mtkh265dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkh265dec_parent_class)->finalize (object);
}

static gboolean
gst_mtkh265dec_open (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "open");

  return TRUE;
}

static gboolean
gst_mtkh265dec_close (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "close");

  return TRUE;
}

static gboolean
gst_mtkh265dec_start (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "start");

  return TRUE;
}

static gboolean
gst_mtkh265dec_stop (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_mtkh265dec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkh265dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "set_format");

  return TRUE;
}

static gboolean
gst_mtkh265dec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_mtkh265dec_finish (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkh265dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkh265dec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkh265dec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkh265dec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkh265dec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkh265dec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkh265dec *mtkh265dec = GST_MTKH265DEC (decoder);

  GST_DEBUG_OBJECT (mtkh265dec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkh265dec", GST_RANK_NONE,
      GST_TYPE_MTKH265DEC);
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
//    mtkh265dec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
