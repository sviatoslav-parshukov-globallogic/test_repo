/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALvorbisdec
 *
 * The HALvorbisdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALvorbisdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstHALvorbisdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALvorbisdec_debug_category);
#define GST_CAT_DEFAULT gst_HALvorbisdec_debug_category

/* prototypes */


static void gst_HALvorbisdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALvorbisdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALvorbisdec_dispose (GObject * object);
static void gst_HALvorbisdec_finalize (GObject * object);

static gboolean gst_HALvorbisdec_start (GstAudioDecoder * decoder);
static gboolean gst_HALvorbisdec_stop (GstAudioDecoder * decoder);
static gboolean gst_HALvorbisdec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_HALvorbisdec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_HALvorbisdec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_HALvorbisdec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_HALvorbisdec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_HALvorbisdec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALvorbisdec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALvorbisdec_open (GstAudioDecoder * decoder);
static gboolean gst_HALvorbisdec_close (GstAudioDecoder * decoder);
static gboolean gst_HALvorbisdec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_HALvorbisdec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALvorbisdec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_HALvorbisdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ( "audio/x-raw,"
                      "format=F32LE,"
                      "rate=[ 1, 2147483647 ],"
                      "channels=[ 1, 256 ]")
    );

static GstStaticPadTemplate gst_HALvorbisdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-vorbis")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkvorbisdec, gst_HALvorbisdec,
    GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALvorbisdec_debug_category, "HALvorbisdec", 0,
        "debug category for HALvorbisdec element"));

static void
gst_HALvorbisdec_class_init (GstMtkvorbisdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALvorbisdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALvorbisdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK Vorbis decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALvorbisdec_set_property;
  gobject_class->get_property = gst_HALvorbisdec_get_property;
  gobject_class->dispose = gst_HALvorbisdec_dispose;
  gobject_class->finalize = gst_HALvorbisdec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_sink_event);
  audio_decoder_class->src_event =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALvorbisdec_close);
  audio_decoder_class->negotiate =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALvorbisdec_propose_allocation);

}

static void
gst_HALvorbisdec_init (GstMtkvorbisdec * HALvorbisdec)
{
}

void
gst_HALvorbisdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (object);

  GST_DEBUG_OBJECT (HALvorbisdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALvorbisdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (object);

  GST_DEBUG_OBJECT (HALvorbisdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALvorbisdec_dispose (GObject * object)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (object);

  GST_DEBUG_OBJECT (HALvorbisdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALvorbisdec_parent_class)->dispose (object);
}

void
gst_HALvorbisdec_finalize (GObject * object)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (object);

  GST_DEBUG_OBJECT (HALvorbisdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALvorbisdec_parent_class)->finalize (object);
}

static gboolean
gst_HALvorbisdec_start (GstAudioDecoder * decoder)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "start");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_stop (GstAudioDecoder * decoder)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "stop");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_HALvorbisdec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALvorbisdec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_HALvorbisdec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "flush");

}

static GstFlowReturn
gst_HALvorbisdec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_HALvorbisdec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "src_event");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_open (GstAudioDecoder * decoder)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "open");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_close (GstAudioDecoder * decoder)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "close");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALvorbisdec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query)
{
  GstMtkvorbisdec *HALvorbisdec = GST_MTKVORBISDEC (decoder);

  GST_DEBUG_OBJECT (HALvorbisdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALvorbisdec", GST_RANK_NONE,
      GST_TYPE_MTKVORBISDEC);
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
//    HALvorbisdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
