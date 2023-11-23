/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstHALeacdec
 *
 * The HALeacdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! HALeacdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstHALeacdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_HALeacdec_debug_category);
#define GST_CAT_DEFAULT gst_HALeacdec_debug_category

/* prototypes */


static void gst_HALeacdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_HALeacdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_HALeacdec_dispose (GObject * object);
static void gst_HALeacdec_finalize (GObject * object);

static gboolean gst_HALeacdec_start (GstAudioDecoder * decoder);
static gboolean gst_HALeacdec_stop (GstAudioDecoder * decoder);
static gboolean gst_HALeacdec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_HALeacdec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_HALeacdec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_HALeacdec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_HALeacdec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_HALeacdec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALeacdec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_HALeacdec_open (GstAudioDecoder * decoder);
static gboolean gst_HALeacdec_close (GstAudioDecoder * decoder);
static gboolean gst_HALeacdec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_HALeacdec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_HALeacdec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_HALeacdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw, format=F32LE, layout=non-interleaved")
    );

static GstStaticPadTemplate gst_HALeacdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-eac3")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkeacdec, gst_HALeacdec, GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_HALeacdec_debug_category, "HALeacdec", 0,
        "debug category for HALeacdec element"));

static void
gst_HALeacdec_class_init (GstMtkeacdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALeacdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_HALeacdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK EAC decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_HALeacdec_set_property;
  gobject_class->get_property = gst_HALeacdec_get_property;
  gobject_class->dispose = gst_HALeacdec_dispose;
  gobject_class->finalize = gst_HALeacdec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_HALeacdec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_HALeacdec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_HALeacdec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_HALeacdec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_HALeacdec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_HALeacdec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_HALeacdec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_HALeacdec_sink_event);
  audio_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_HALeacdec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_HALeacdec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_HALeacdec_close);
  audio_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_HALeacdec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_HALeacdec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_HALeacdec_propose_allocation);

}

static void
gst_HALeacdec_init (GstMtkeacdec * HALeacdec)
{
}

void
gst_HALeacdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (HALeacdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALeacdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (HALeacdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_HALeacdec_dispose (GObject * object)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (HALeacdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_HALeacdec_parent_class)->dispose (object);
}

void
gst_HALeacdec_finalize (GObject * object)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (HALeacdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_HALeacdec_parent_class)->finalize (object);
}

static gboolean
gst_HALeacdec_start (GstAudioDecoder * decoder)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "start");

  return TRUE;
}

static gboolean
gst_HALeacdec_stop (GstAudioDecoder * decoder)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "stop");

  return TRUE;
}

static gboolean
gst_HALeacdec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_HALeacdec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_HALeacdec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_HALeacdec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "flush");

}

static GstFlowReturn
gst_HALeacdec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_HALeacdec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "sink_event");

  return TRUE;
}

static gboolean
gst_HALeacdec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "src_event");

  return TRUE;
}

static gboolean
gst_HALeacdec_open (GstAudioDecoder * decoder)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "open");

  return TRUE;
}

static gboolean
gst_HALeacdec_close (GstAudioDecoder * decoder)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "close");

  return TRUE;
}

static gboolean
gst_HALeacdec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "negotiate");

  return TRUE;
}

static gboolean
gst_HALeacdec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_HALeacdec_propose_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkeacdec *HALeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (HALeacdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "HALeacdec", GST_RANK_NONE,
      GST_TYPE_MTKEACDEC);
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
//    HALeacdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
