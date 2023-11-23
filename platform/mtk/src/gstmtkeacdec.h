/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKEACDEC_H_
#define _GST_MTKEACDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKEACDEC   (gst_mtkeacdec_get_type())
#define GST_MTKEACDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKEACDEC,GstMtkeacdec))
#define GST_MTKEACDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKEACDEC,GstMtkeacdecClass))
#define GST_IS_MTKEACDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKEACDEC))
#define GST_IS_MTKEACDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKEACDEC))

typedef struct _GstMtkeacdec GstMtkeacdec;
typedef struct _GstMtkeacdecClass GstMtkeacdecClass;

struct _GstMtkeacdec
{
  GstAudioDecoder base_mtkeacdec;

};

struct _GstMtkeacdecClass
{
  GstAudioDecoderClass base_mtkeacdec_class;
};

GType gst_mtkeacdec_get_type (void);

G_END_DECLS

#endif
