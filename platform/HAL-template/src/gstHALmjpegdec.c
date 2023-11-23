/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALmjpegdec
 *
 * The HALmjpegdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALmjpegdec ! FIXME ! fakesink
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
#include "gstHALmjpegdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALmjpegdec_debug_category);
#define GST_CAT_DEFAULT gst_HALmjpegdec_debug_category

/* prototypes */


static void gst_HALmjpegdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALmjpegdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALmjpegdec_dispose (GObject * object);
static void gst_HALmjpegdec_finalize (GObject * object);

static gboolean gst_HALmjpegdec_open (GstVideoDecoder * decoder);
static gboolean gst_HALmjpegdec_close (GstVideoDecoder * decoder);
static gboolean gst_HALmjpegdec_start (GstVideoDecoder * decoder);
static gboolean gst_HALmjpegdec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALmjpegdec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_HALmjpegdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_HALmjpegdec_reset (GstVideoDecoder * decoder,
    gboolean hard);
static GstFlowReturn gst_HALmjpegdec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_HALmjpegdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_HALmjpegdec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALmjpegdec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALmjpegdec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_HALmjpegdec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALmjpegdec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_HALmjpegdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "image/jpeg,"
                      "parsed=true")
    );

static GstStaticPadTemplate gst_HALmjpegdec_src_template =
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

G_DEFINE_TYPE_WITH_CODE (GstMtkmjpegdec, gst_HALmjpegdec,
    GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALmjpegdec_debug_category, "HALmjpegdec", 0,
        "debug category for HALmjpegdec element"));

static void
gst_HALmjpegdec_class_init (GstMtkmjpegdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALmjpegdec_sink_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALmjpegdec_src_template);


  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK MJPEG decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALmjpegdec_set_property;
  gobject_class->get_property = gst_HALmjpegdec_get_property;
  gobject_class->dispose = gst_HALmjpegdec_dispose;
  gobject_class->finalize = gst_HALmjpegdec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_HALmjpegdec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_sink_event);
  video_decoder_class->src_event =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_src_event);
  video_decoder_class->negotiate =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALmjpegdec_propose_allocation);

}

static void
gst_HALmjpegdec_init (GstMtkmjpegdec * HALmjpegdec)
{
}

void
gst_HALmjpegdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (HALmjpegdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALmjpegdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (HALmjpegdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALmjpegdec_dispose (GObject * object)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (HALmjpegdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALmjpegdec_parent_class)->dispose (object);
}

void
gst_HALmjpegdec_finalize (GObject * object)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (object);

  GST_DEBUG_OBJECT (HALmjpegdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALmjpegdec_parent_class)->finalize (object);
}

static gboolean
gst_HALmjpegdec_open (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "open");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_close (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "close");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_start (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "start");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_stop (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_HALmjpegdec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_HALmjpegdec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "set_format");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_HALmjpegdec_finish (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALmjpegdec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_HALmjpegdec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "src_event");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALmjpegdec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkmjpegdec *HALmjpegdec = GST_MTKMJPEGDEC (decoder);

  GST_DEBUG_OBJECT (HALmjpegdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALmjpegdec", GST_RANK_NONE,
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
//    HALmjpegdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
