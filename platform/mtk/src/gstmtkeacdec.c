/* Vizio
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */
/**
 * SECTION:element-gstmtkeacdec
 *
 * The mtkeacdec element does FIXME stuff.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch-1.0 -v fakesrc ! mtkeacdec ! FIXME ! fakesink
 * ]|
 * FIXME Describe what the pipeline does.
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gst/gst.h>
#include <gst/audio/gstaudiodecoder.h>
#include "gstmtkeacdec.h"

GST_DEBUG_CATEGORY_STATIC (gst_mtkeacdec_debug_category);
#define GST_CAT_DEFAULT gst_mtkeacdec_debug_category

/* prototypes */


static void gst_mtkeacdec_set_property (GObject * object,
    guint property_id, const GValue * value, GParamSpec * pspec);
static void gst_mtkeacdec_get_property (GObject * object,
    guint property_id, GValue * value, GParamSpec * pspec);
static void gst_mtkeacdec_dispose (GObject * object);
static void gst_mtkeacdec_finalize (GObject * object);

static gboolean gst_mtkeacdec_start (GstAudioDecoder * decoder);
static gboolean gst_mtkeacdec_stop (GstAudioDecoder * decoder);
static gboolean gst_mtkeacdec_set_format (GstAudioDecoder * decoder,
    GstCaps * caps);
static GstFlowReturn gst_mtkeacdec_parse (GstAudioDecoder * decoder,
    GstAdapter * adapter, gint * offset, gint * length);
static GstFlowReturn gst_mtkeacdec_handle_frame (GstAudioDecoder * decoder,
    GstBuffer * buffer);
static void gst_mtkeacdec_flush (GstAudioDecoder * decoder, gboolean hard);
static GstFlowReturn gst_mtkeacdec_pre_push (GstAudioDecoder * decoder,
    GstBuffer ** buffer);
static gboolean gst_mtkeacdec_sink_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkeacdec_src_event (GstAudioDecoder * decoder,
    GstEvent * event);
static gboolean gst_mtkeacdec_open (GstAudioDecoder * decoder);
static gboolean gst_mtkeacdec_close (GstAudioDecoder * decoder);
static gboolean gst_mtkeacdec_negotiate (GstAudioDecoder * decoder);
static gboolean gst_mtkeacdec_decide_allocation (GstAudioDecoder * decoder,
    GstQuery * query);
static gboolean gst_mtkeacdec_propose_allocation (GstAudioDecoder * decoder,
    GstQuery * query);

enum
{
  PROP_0
};

/* pad templates */

/* FIXME add/remove the formats that you want to support */
static GstStaticPadTemplate gst_mtkeacdec_src_template =
GST_STATIC_PAD_TEMPLATE ("src",
    GST_PAD_SRC,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-raw, format=F32LE, layout=non-interleaved")
    );

static GstStaticPadTemplate gst_mtkeacdec_sink_template =
GST_STATIC_PAD_TEMPLATE ("sink",
    GST_PAD_SINK,
    GST_PAD_ALWAYS,
    GST_STATIC_CAPS ("audio/x-eac3")
    );


/* class initialization */

G_DEFINE_TYPE_WITH_CODE (GstMtkeacdec, gst_mtkeacdec, GST_TYPE_AUDIO_DECODER,
    GST_DEBUG_CATEGORY_INIT (gst_mtkeacdec_debug_category, "mtkeacdec", 0,
        "debug category for mtkeacdec element"));

static void
gst_mtkeacdec_class_init (GstMtkeacdecClass * klass)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
  GstAudioDecoderClass *audio_decoder_class = GST_AUDIO_DECODER_CLASS (klass);

  /* Setting up pads and setting metadata should be moved to
     base_class_init if you intend to subclass this class. */
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkeacdec_src_template);
  gst_element_class_add_static_pad_template (GST_ELEMENT_CLASS (klass),
      &gst_mtkeacdec_sink_template);

  gst_element_class_set_static_metadata (GST_ELEMENT_CLASS (klass),
      "MTK EAC decoder", "Generic", "FIXME Description",
      "FIXME <fixme@example.com>");

  gobject_class->set_property = gst_mtkeacdec_set_property;
  gobject_class->get_property = gst_mtkeacdec_get_property;
  gobject_class->dispose = gst_mtkeacdec_dispose;
  gobject_class->finalize = gst_mtkeacdec_finalize;
  audio_decoder_class->start = GST_DEBUG_FUNCPTR (gst_mtkeacdec_start);
  audio_decoder_class->stop = GST_DEBUG_FUNCPTR (gst_mtkeacdec_stop);
  audio_decoder_class->set_format =
      GST_DEBUG_FUNCPTR (gst_mtkeacdec_set_format);
  audio_decoder_class->parse = GST_DEBUG_FUNCPTR (gst_mtkeacdec_parse);
  audio_decoder_class->handle_frame =
      GST_DEBUG_FUNCPTR (gst_mtkeacdec_handle_frame);
  audio_decoder_class->flush = GST_DEBUG_FUNCPTR (gst_mtkeacdec_flush);
  audio_decoder_class->pre_push = GST_DEBUG_FUNCPTR (gst_mtkeacdec_pre_push);
  audio_decoder_class->sink_event =
      GST_DEBUG_FUNCPTR (gst_mtkeacdec_sink_event);
  audio_decoder_class->src_event = GST_DEBUG_FUNCPTR (gst_mtkeacdec_src_event);
  audio_decoder_class->open = GST_DEBUG_FUNCPTR (gst_mtkeacdec_open);
  audio_decoder_class->close = GST_DEBUG_FUNCPTR (gst_mtkeacdec_close);
  audio_decoder_class->negotiate = GST_DEBUG_FUNCPTR (gst_mtkeacdec_negotiate);
  audio_decoder_class->decide_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkeacdec_decide_allocation);
  audio_decoder_class->propose_allocation =
      GST_DEBUG_FUNCPTR (gst_mtkeacdec_propose_allocation);

}

static void
gst_mtkeacdec_init (GstMtkeacdec * mtkeacdec)
{
}

void
gst_mtkeacdec_set_property (GObject * object, guint property_id,
    const GValue * value, GParamSpec * pspec)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (mtkeacdec, "set_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkeacdec_get_property (GObject * object, guint property_id,
    GValue * value, GParamSpec * pspec)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (mtkeacdec, "get_property");

  switch (property_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
  }
}

void
gst_mtkeacdec_dispose (GObject * object)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (mtkeacdec, "dispose");

  /* clean up as possible.  may be called multiple times */

  G_OBJECT_CLASS (gst_mtkeacdec_parent_class)->dispose (object);
}

void
gst_mtkeacdec_finalize (GObject * object)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (object);

  GST_DEBUG_OBJECT (mtkeacdec, "finalize");

  /* clean up object here */

  G_OBJECT_CLASS (gst_mtkeacdec_parent_class)->finalize (object);
}

static gboolean
gst_mtkeacdec_start (GstAudioDecoder * decoder)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "start");

  return TRUE;
}

static gboolean
gst_mtkeacdec_stop (GstAudioDecoder * decoder)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "stop");

  return TRUE;
}

static gboolean
gst_mtkeacdec_set_format (GstAudioDecoder * decoder, GstCaps * caps)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "set_format");

  return TRUE;
}

static GstFlowReturn
gst_mtkeacdec_parse (GstAudioDecoder * decoder, GstAdapter * adapter,
    gint * offset, gint * length)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "parse");

  return GST_FLOW_OK;
}

static GstFlowReturn
gst_mtkeacdec_handle_frame (GstAudioDecoder * decoder, GstBuffer * buffer)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "handle_frame");

  return GST_FLOW_OK;
}

static void
gst_mtkeacdec_flush (GstAudioDecoder * decoder, gboolean hard)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "flush");

}

static GstFlowReturn
gst_mtkeacdec_pre_push (GstAudioDecoder * decoder, GstBuffer ** buffer)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "pre_push");

  return GST_FLOW_OK;
}

static gboolean
gst_mtkeacdec_sink_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "sink_event");

  return TRUE;
}

static gboolean
gst_mtkeacdec_src_event (GstAudioDecoder * decoder, GstEvent * event)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "src_event");

  return TRUE;
}

static gboolean
gst_mtkeacdec_open (GstAudioDecoder * decoder)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "open");

  return TRUE;
}

static gboolean
gst_mtkeacdec_close (GstAudioDecoder * decoder)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "close");

  return TRUE;
}

static gboolean
gst_mtkeacdec_negotiate (GstAudioDecoder * decoder)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "negotiate");

  return TRUE;
}

static gboolean
gst_mtkeacdec_decide_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "decide_allocation");

  return TRUE;
}

static gboolean
gst_mtkeacdec_propose_allocation (GstAudioDecoder * decoder, GstQuery * query)
{
  GstMtkeacdec *mtkeacdec = GST_MTKEACDEC (decoder);

  GST_DEBUG_OBJECT (mtkeacdec, "propose_allocation");

  return TRUE;
}

/*
static gboolean
plugin_init (GstPlugin * plugin)
{

//   FIXME Remember to set the rank if it's an element that is meant
//     to be autoplugged by decodebin. 
  return gst_element_register (plugin, "mtkeacdec", GST_RANK_NONE,
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
//    mtkeacdec,
//    "FIXME plugin description",
//    plugin_init, VERSION, "LGPL", PACKAGE_NAME, GST_PACKAGE_ORIGIN)
