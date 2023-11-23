/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 * #TODO
 */
/**
 * SECTION:element-gstmtkh264dec
 *
 * The mtkh264dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkh264dec ! FIXME ! fakesink
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
#include "gstmtkh264dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkh264dec_debug_category);
#define GST_CAT_DEFAULT gst_mtkh264dec_debug_category

/* prototypes */


static void gst_mtkh264dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkh264dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkh264dec_dispose (GObject * object);
static void gst_mtkh264dec_finalize (GObject * object);

static gboolean gst_mtkh264dec_open (GstVideoDecoder * decoder);
static gboolean gst_mtkh264dec_close (GstVideoDecoder * decoder);
static gboolean gst_mtkh264dec_start (GstVideoDecoder * decoder);
static gboolean gst_mtkh264dec_stop (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkh264dec_parse (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame, GstAdapter * adapter, gboolean at_eos);
static gboolean gst_mtkh264dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state);
static gboolean gst_mtkh264dec_reset (GstVideoDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkh264dec_finish (GstVideoDecoder * decoder);
static GstFlowReturn gst_mtkh264dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame);
static gboolean gst_mtkh264dec_sink_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkh264dec_src_event (GstVideoDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkh264dec_negotiate (GstVideoDecoder * decoder);
static gboolean gst_mtkh264dec_decide_allocation (GstVideoDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkh264dec_propose_allocation (GstVideoDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

static GstStaticPadTemplate gst_mtkh264dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-h264,"
                     "stream-format=byte-stream,"
                     "alignment=au,"
                     "level=(string){ 1, 1b, 1.1, 1.2, 1.3, 2, 2.1, 2.2, 3, 3.1, 3.2, 4, 4.1, 4.2 },"
                     "profile=(string){ baseline, constrained-baseline, main, high }")
    );


static GstStaticPadTemplate gst_mtkh264dec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("video/x-raw,"
                     "format=(string){ I420, YV12, NV12, NV21, RGB16 },"
                     "width=[ 1, 32768 ],"
                     "height=[ 1, 32768 ],"
                     "framerate=[ 0/1, 2147483647/1 ]")
    );

/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkh264dec, gst_mtkh264dec, GST_TYPE_VIDEO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkh264dec_debug_category, "mtkh264dec", 0,
        "debug category for mtkh264dec element"));

static void
gst_mtkh264dec_class_init (GstMtkh264decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstVideoDecoderClass *video_decoder_class = GST_VIDEO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkh264dec_sink_template);
   gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkh264dec_src_template);
 
  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK H.264 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkh264dec_set_property;
  gobject_class->get_property = gst_mtkh264dec_get_property;
  gobject_class->dispose = gst_mtkh264dec_dispose;
  gobject_class->finalize = gst_mtkh264dec_finalize;
  video_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkh264dec_open);
  video_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkh264dec_close);
  video_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkh264dec_start);
  video_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkh264dec_stop);
  video_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkh264dec_parse);
  video_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkh264dec_set_format);
  video_decoder_class->reset = GST_DEBUG_FUNCPTR (gst_mtkh264dec_reset);
  video_decoder_class->finish = GST_DEBUG_FUNCPTR (gst_mtkh264dec_finish);
  video_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkh264dec_handle_frame);
  video_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkh264dec_sink_event);
  video_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkh264dec_src_event);
  video_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkh264dec_negotiate);
  video_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkh264dec_decide_allocation);
  video_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkh264dec_propose_allocation);

}

static void
gst_mtkh264dec_init (GstMtkh264dec * mtkh264dec)
{
}

void
gst_mtkh264dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (object);

  GST_DEBUG_OBJECT (mtkh264dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkh264dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (object);

  GST_DEBUG_OBJECT (mtkh264dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkh264dec_dispose (GObject * object)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (object);

  GST_DEBUG_OBJECT (mtkh264dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkh264dec_parent_class)->dispose (object);
}

void
gst_mtkh264dec_finalize (GObject * object)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (object);

  GST_DEBUG_OBJECT (mtkh264dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkh264dec_parent_class)->finalize (object);
}

static gboolean
gst_mtkh264dec_open (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "open");

  return TRUE;
}

static gboolean
gst_mtkh264dec_close (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "close");

  return TRUE;
}

static gboolean
gst_mtkh264dec_start (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "start");

  return TRUE;
}

static gboolean
gst_mtkh264dec_stop (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "stop");

  return TRUE;
}

static GstFlowReturn
gst_mtkh264dec_parse (GstVideoDecoder * decoder, GstVideoCodecFrame * frame,
    GstAdapter * adapter, gboolean at_eos)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "parse");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkh264dec_set_format (GstVideoDecoder * decoder,
    GstVideoCodecState * state)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "set_format");

  return TRUE;
}

static gboolean
gst_mtkh264dec_reset (GstVideoDecoder * decoder, gboolean hard)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "reset");

  return TRUE;
}

static GstFlowReturn
gst_mtkh264dec_finish (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "finish");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkh264dec_handle_frame (GstVideoDecoder * decoder,
    GstVideoCodecFrame * frame)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "handle_frame");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkh264dec_sink_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkh264dec_src_event (GstVideoDecoder * decoder, GstEvent * event)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkh264dec_negotiate (GstVideoDecoder * decoder)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkh264dec_decide_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkh264dec_propose_allocation (GstVideoDecoder * decoder, GstQuery * query)
{
  GstMtkh264dec *mtkh264dec = GST_MTKH264DEC (decoder);

  GST_DEBUG_OBJECT (mtkh264dec, "propose_allocation");

  return TRUE;
}


