/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkvp8dec
 *
 * The mtkvp8dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkvp8dec ! FIXME ! fakesink
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
#include "gstmtkvp8dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkvp8dec_debug_category);
#define GST_CAT_DEFAULT gst_mtkvp8dec_debug_category

/* prototypes */


static void gst_mtkvp8dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkvp8dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkvp8dec_dispose (GObject * object);
static void gst_mtkvp8dec_finalize (GObject * object);

static gboolean gst_mtkvp8dec_open (GstVideoDecoder * decoder);
static gboolean gst_mtkvp8dec_close (GstVideoDecoder * decoder);
static gboolean gst_mtkvp8dec_start (GstVideoDecoder * decoder);
static gboolean gst_mtkvp8dec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkvp8dec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_mtkvp8dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_mtkvp8dec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkvp8dec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkvp8dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_mtkvp8dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkvp8dec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkvp8dec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_mtkvp8dec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkvp8dec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mtkvp8dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-vp8")
    );

/* FIXME: add/remove formats you can handle */
#define VIDEO_SRC_CAPS \
    GST_VIDEO_CAPS_MAKE("{ I420 }")


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkvp8dec, gst_mtkvp8dec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkvp8dec_debug_category, "mtkvp8dec", 0,
        "debug category for mtkvp8dec element"));

static void
gst_mtkvp8dec_class_init (GstMtkvp8decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkvp8dec_sink_template);
  gst_element_class_add_pad_template (GST_ELEMENT_CLASS (klass),
      gst_pad_template_new ("src", GST_PAD_SRC, GST_PAD_ALWAYS,
          gst_caps_from_string (VIDEO_SRC_CAPS)));

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK VP8 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkvp8dec_set_property;
  gobject_class->get_property = gst_mtkvp8dec_get_property;
  gobject_class->dispose = gst_mtkvp8dec_dispose;
  gobject_class->finalize = gst_mtkvp8dec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkvp8dec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkvp8dec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkvp8dec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkvp8dec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkvp8dec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkvp8dec_propose_allocation);

}

static void
gst_mtkvp8dec_init (GstMtkvp8dec * mtkvp8dec)
{
}

void
gst_mtkvp8dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (object);

  GST_DEBUG_OBJECT (mtkvp8dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkvp8dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (object);

  GST_DEBUG_OBJECT (mtkvp8dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkvp8dec_dispose (GObject * object)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (object);

  GST_DEBUG_OBJECT (mtkvp8dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkvp8dec_parent_class)->dispose (object);
}

void
gst_mtkvp8dec_finalize (GObject * object)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (object);

  GST_DEBUG_OBJECT (mtkvp8dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkvp8dec_parent_class)->finalize (object);
}

static gboolean
gst_mtkvp8dec_open (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "open");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_close (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "close");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_start (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "start");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_stop (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_mtkvp8dec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkvp8dec_set_format (GstVideoDecoder * decoder, GstVideoCodecState * state)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "set_format");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_mtkvp8dec_finish (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkvp8dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkvp8dec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkvp8dec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkvp8dec *mtkvp8dec = GST_MTKVP8DEC (decoder);

  GST_DEBUG_OBJECT (mtkvp8dec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkvp8dec", GST_RANK_NONE,
      GST_TYPE_MTKVP8DEC);
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
//    mtkvp8dec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
