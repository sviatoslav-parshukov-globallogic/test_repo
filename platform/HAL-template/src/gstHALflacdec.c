/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALflacdec
 *
 * The HALflacdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALflacdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstHALflacdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALflacdec_debug_category);
#define GST_CAT_DEFAULT gst_HALflacdec_debug_category

/* prototypes */


static void gst_HALflacdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALflacdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALflacdec_dispose (GObject * object);
static void gst_HALflacdec_finalize (GObject * object);

static gboolean gst_HALflacdec_start (GstAudioDecoder * decoder);
static gboolean gst_HALflacdec_stop (GstAudioDecoder * decoder);
static gboolean gst_HALflacdec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_HALflacdec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_HALflacdec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_HALflacdec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_HALflacdec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_HALflacdec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALflacdec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALflacdec_open (GstAudioDecoder * decoder);
static gboolean gst_HALflacdec_close (GstAudioDecoder * decoder);
static gboolean gst_HALflacdec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_HALflacdec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALflacdec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_HALflacdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw,"
                     "format={ (string)S8, (string)S16LE, (string)S24_32LE, (string)S32LE },"
                     "layout=interleaved,"
                     "rate=[ 1, 655350 ],"
                     "channels=[ 1, 8 ]")
    );

static GstStaticPadTemplate gst_HALflacdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-flac, framed=true, "
                     "rate=[1,655350], channels=[1,8]")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkflacdec, gst_HALflacdec, GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALflacdec_debug_category, "HALflacdec", 0,
        "debug category for HALflacdec element"));

static void
gst_HALflacdec_class_init (GstMtkflacdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALflacdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALflacdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK FLAC decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALflacdec_set_property;
  gobject_class->get_property = gst_HALflacdec_get_property;
  gobject_class->dispose = gst_HALflacdec_dispose;
  gobject_class->finalize = gst_HALflacdec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALflacdec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALflacdec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_HALflacdec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALflacdec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALflacdec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_HALflacdec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_HALflacdec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_HALflacdec_sink_event);
  audio_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_HALflacdec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALflacdec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALflacdec_close);
  audio_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_HALflacdec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALflacdec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALflacdec_propose_allocation);

}

static void
gst_HALflacdec_init (GstMtkflacdec * HALflacdec)
{
}

void
gst_HALflacdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (object);

  GST_DEBUG_OBJECT (HALflacdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALflacdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (object);

  GST_DEBUG_OBJECT (HALflacdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALflacdec_dispose (GObject * object)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (object);

  GST_DEBUG_OBJECT (HALflacdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALflacdec_parent_class)->dispose (object);
}

void
gst_HALflacdec_finalize (GObject * object)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (object);

  GST_DEBUG_OBJECT (HALflacdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALflacdec_parent_class)->finalize (object);
}

static gboolean
gst_HALflacdec_start (GstAudioDecoder * decoder)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "start");

  return TRUE;
}

static gboolean
gst_HALflacdec_stop (GstAudioDecoder * decoder)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "stop");

  return TRUE;
}

static gboolean
gst_HALflacdec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_HALflacdec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALflacdec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_HALflacdec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "flush");

}

static GstFlowReturn
gst_HALflacdec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_HALflacdec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALflacdec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "src_event");

  return TRUE;
}

static gboolean
gst_HALflacdec_open (GstAudioDecoder * decoder)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "open");

  return TRUE;
}

static gboolean
gst_HALflacdec_close (GstAudioDecoder * decoder)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "close");

  return TRUE;
}

static gboolean
gst_HALflacdec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALflacdec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALflacdec_propose_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkflacdec *HALflacdec = GST_MTKFLACDEC (decoder);

  GST_DEBUG_OBJECT (HALflacdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALflacdec", GST_RANK_NONE,
      GST_TYPE_MTKFLACDEC);
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
//    HALflacdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
