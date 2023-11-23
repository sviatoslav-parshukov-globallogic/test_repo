/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkac3dec
 *
 * The mtkac3dec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkac3dec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstmtkac3dec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkac3dec_debug_category);
#define GST_CAT_DEFAULT gst_mtkac3dec_debug_category

/* prototypes */


static void gst_mtkac3dec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkac3dec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkac3dec_dispose (GObject * object);
static void gst_mtkac3dec_finalize (GObject * object);

static gboolean gst_mtkac3dec_start (GstAudioDecoder * decoder);
static gboolean gst_mtkac3dec_stop (GstAudioDecoder * decoder);
static gboolean gst_mtkac3dec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_mtkac3dec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_mtkac3dec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_mtkac3dec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkac3dec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_mtkac3dec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkac3dec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkac3dec_open (GstAudioDecoder * decoder);
static gboolean gst_mtkac3dec_close (GstAudioDecoder * decoder);
static gboolean gst_mtkac3dec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_mtkac3dec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkac3dec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_mtkac3dec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-ac3, framed=true, channels=[1,6],"
                     "rate=[8000,48000]," 
                     "alignment={(string)iec61937,(string)frame }")
    );

static GstStaticPadTemplate gst_mtkac3dec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,format=S16LE,rate=[1,max],"
        "channels=[1,max],layout=interleaved")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkac3dec, gst_mtkac3dec, GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkac3dec_debug_category, "mtkac3dec", 0,
        "debug category for mtkac3dec element"));

static void
gst_mtkac3dec_class_init (GstMtkac3decClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkac3dec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkac3dec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK AC3 decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkac3dec_set_property;
  gobject_class->get_property = gst_mtkac3dec_get_property;
  gobject_class->dispose = gst_mtkac3dec_dispose;
  gobject_class->finalize = gst_mtkac3dec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkac3dec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkac3dec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkac3dec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkac3dec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkac3dec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_mtkac3dec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_mtkac3dec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkac3dec_sink_event);
  audio_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkac3dec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkac3dec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkac3dec_close);
  audio_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkac3dec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkac3dec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkac3dec_propose_allocation);

}

static void
gst_mtkac3dec_init (GstMtkac3dec * mtkac3dec)
{
}

void
gst_mtkac3dec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (object);

  GST_DEBUG_OBJECT (mtkac3dec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkac3dec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (object);

  GST_DEBUG_OBJECT (mtkac3dec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkac3dec_dispose (GObject * object)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (object);

  GST_DEBUG_OBJECT (mtkac3dec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkac3dec_parent_class)->dispose (object);
}

void
gst_mtkac3dec_finalize (GObject * object)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (object);

  GST_DEBUG_OBJECT (mtkac3dec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkac3dec_parent_class)->finalize (object);
}

static gboolean
gst_mtkac3dec_start (GstAudioDecoder * decoder)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "start");

  return TRUE;
}

static gboolean
gst_mtkac3dec_stop (GstAudioDecoder * decoder)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "stop");

  return TRUE;
}

static gboolean
gst_mtkac3dec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_mtkac3dec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkac3dec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_mtkac3dec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "flush");

}

static GstFlowReturn
gst_mtkac3dec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkac3dec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkac3dec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkac3dec_open (GstAudioDecoder * decoder)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "open");

  return TRUE;
}

static gboolean
gst_mtkac3dec_close (GstAudioDecoder * decoder)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "close");

  return TRUE;
}

static gboolean
gst_mtkac3dec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkac3dec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkac3dec_propose_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkac3dec *mtkac3dec = GST_MTKAC3DEC (decoder);

  GST_DEBUG_OBJECT (mtkac3dec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkac3dec", GST_RANK_NONE,
      GST_TYPE_MTKAC3DEC);
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
//    mtkac3dec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
