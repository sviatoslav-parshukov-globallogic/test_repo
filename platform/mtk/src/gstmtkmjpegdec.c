/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkmjpegdec
 *
 * The mtkmjpegdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkmjpegdec ! FIXME ! fakesink
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
#include "gstmtkmjpegdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkmjpegdec_debug_category);
#define GST_CAT_DEFAULT gst_mtkmjpegdec_debug_category

/* prototypes */


static void gst_mtkmjpegdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkmjpegdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkmjpegdec_dispose (GObject * object);
static void gst_mtkmjpegdec_finalize (GObject * object);

static gboolean gst_mtkmjpegdec_open (GstVideoDecoder * decoder);
static gboolean gst_mtkmjpegdec_close (GstVideoDecoder * decoder);
static gboolean gst_mtkmjpegdec_start (GstVideoDecoder * decoder);
static gboolean gst_mtkmjpegdec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkmjpegdec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_mtkmjpegdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_mtkmjpegdec_reset (GstVideoDecoder * decoder,
    gboolean hard);
static GstFlowReturn gst_mtkmjpegdec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkmjpegdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_mtkmjpegdec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkmjpegdec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkmjpegdec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_mtkmjpegdec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkmjpegdec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mtkmjpegdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "image/jpeg,"
                      "parsed=true")
    );

static GstStaticPadTemplate gst_mtkmjpegdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "video/x-raw,"
                      "format={ (string)I420, (string)YUY2, (string)RGB, (string)BGR,"
                      "(string)Y42B, (string)Y444, (string)YUV9, (string)Y41B, "
                      "(string)GRAY8, (string)RGB8P, (string)I420, (string)Y42B, "
                      "(string)Y444, (string)UYVY, (string)NV12, (string)NV21, "
                      "(string)ARGB, (string)RGBA, (string)ABGR, (string)BGRA, "
                      "(string)GRAY16_BE, (string)GRAY16_LE, (string)A420, "
                      "(string)RGB16, (string)RGB15, (string)I420_10BE, "
                      "(string)I420_10LE, (string)I422_10BE, (string)I422_10LE, "
                      "(string)Y444_10BE, (string)Y444_10LE, (string)GBR, "
                      "(string)GBR_10BE, (string)GBR_10LE, (string)A420_10BE, "
                      "(string)A420_10LE, (string)A422_10BE, (string)A422_10LE, "
                      "(string)A444_10BE, (string)A444_10LE, (string)GBRA, (string)xRGB, "
                      "(string)RGBx, (string)xBGR, (string)BGRx, (string)I420_12BE, "
                      "(string)I420_12LE, (string)I422_12BE, (string)I422_12LE, "
                      "(string)Y444_12BE, (string)Y444_12LE, (string)GBR_12BE, "
                      "(string)GBR_12LE, (string)GBRA_12BE, (string)GBRA_12LE }"
                      )
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkmjpegdec, gst_mtkmjpegdec,
    GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkmjpegdec_debug_category, "mtkmjpegdec", 0,
        "debug category for mtkmjpegdec element"));

static void
gst_mtkmjpegdec_class_init (GstMtkmjpegdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkmjpegdec_sink_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkmjpegdec_src_template);


  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK MJPEG decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkmjpegdec_set_property;
  gobject_class->get_property = gst_mtkmjpegdec_get_property;
  gobject_class->dispose = gst_mtkmjpegdec_dispose;
  gobject_class->finalize = gst_mtkmjpegdec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_sink_event);
  video_decoder_class->src_event =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_src_event);
  video_decoder_class->negotiate =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkmjpegdec_propose_allocation);

}

static void
gst_mtkmjpegdec_init (GstMtkmjpegdec * mtkmjpegdec)
{
}

void
gst_mtkmjpegdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (mtkmjpegdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkmjpegdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (mtkmjpegdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkmjpegdec_dispose (GObject * object)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (mtkmjpegdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkmjpegdec_parent_class)->dispose (object);
}

void
gst_mtkmjpegdec_finalize (GObject * object)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (mtkmjpegdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkmjpegdec_parent_class)->finalize (object);
}

static gboolean
gst_mtkmjpegdec_open (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "open");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_close (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "close");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_start (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "start");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_stop (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_mtkmjpegdec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkmjpegdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "set_format");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_mtkmjpegdec_finish (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkmjpegdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkmjpegdec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkmjpegdec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkmjpegdec *mtkmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (mtkmjpegdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkmjpegdec", GST_RANK_NONE,
      GST_TYPE_MTKMJPEGDEC);
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
//    mtkmjpegdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
