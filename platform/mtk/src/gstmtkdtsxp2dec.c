/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkdtsxp2dec
 *
 * The mtkdtsxp2dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkdtsxp2dec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstmtkdtsxp2dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkdtsxp2dec_debug_category);
#define GST_CAT_DEFAULT gst_mtkdtsxp2dec_debug_category

/* prototypes */


static void gst_mtkdtsxp2dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkdtsxp2dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkdtsxp2dec_dispose (GObject * object);
static void gst_mtkdtsxp2dec_finalize (GObject * object);

static gboolean gst_mtkdtsxp2dec_start (GstAudioDecoder * decoder);
static gboolean gst_mtkdtsxp2dec_stop (GstAudioDecoder * decoder);
static gboolean gst_mtkdtsxp2dec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_mtkdtsxp2dec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_mtkdtsxp2dec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_mtkdtsxp2dec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkdtsxp2dec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_mtkdtsxp2dec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkdtsxp2dec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkdtsxp2dec_open (GstAudioDecoder * decoder);
static gboolean gst_mtkdtsxp2dec_close (GstAudioDecoder * decoder);
static gboolean gst_mtkdtsxp2dec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_mtkdtsxp2dec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkdtsxp2dec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_mtkdtsxp2dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-dts")
    );

static GstStaticPadTemplate gst_mtkdtsxp2dec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=F32LE,rate=[4000,96000],"
                     "channels=[1,6],layout=interleaved")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkdtsxp2dec, gst_mtkdtsxp2dec,
    GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkdtsxp2dec_debug_category, "mtkdtsxp2dec", 0,
        "debug category for mtkdtsxp2dec element"));

static void
gst_mtkdtsxp2dec_class_init (GstMtkdtsxp2decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkdtsxp2dec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkdtsxp2dec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK DTSXP2 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkdtsxp2dec_set_property;
  gobject_class->get_property = gst_mtkdtsxp2dec_get_property;
  gobject_class->dispose = gst_mtkdtsxp2dec_dispose;
  gobject_class->finalize = gst_mtkdtsxp2dec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_sink_event);
  audio_decoder_class->src_event =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_close);
  audio_decoder_class->negotiate =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkdtsxp2dec_propose_allocation);

}

static void
gst_mtkdtsxp2dec_init (GstMtkdtsxp2dec * mtkdtsxp2dec)
{
}

void
gst_mtkdtsxp2dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (object);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkdtsxp2dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (object);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkdtsxp2dec_dispose (GObject * object)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (object);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkdtsxp2dec_parent_class)->dispose (object);
}

void
gst_mtkdtsxp2dec_finalize (GObject * object)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (object);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkdtsxp2dec_parent_class)->finalize (object);
}

static gboolean
gst_mtkdtsxp2dec_start (GstAudioDecoder * decoder)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "start");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_stop (GstAudioDecoder * decoder)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "stop");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_mtkdtsxp2dec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkdtsxp2dec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_mtkdtsxp2dec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "flush");

}

static GstFlowReturn
gst_mtkdtsxp2dec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkdtsxp2dec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_open (GstAudioDecoder * decoder)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "open");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_close (GstAudioDecoder * decoder)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "close");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkdtsxp2dec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query)
{
  GstMtkdtsxp2dec *mtkdtsxp2dec = GST_MTKDTSXP2DEC (decoder);

  GST_DEBUG_OBJECT (mtkdtsxp2dec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkdtsxp2dec", GST_RANK_NONE,
      GST_TYPE_MTKDTSXP2DEC);
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
//    mtkdtsxp2dec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
