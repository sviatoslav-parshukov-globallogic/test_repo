/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKFLACDEC_H_
#define _GST_MTKFLACDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKFLACDEC   (gst_mtkflacdec_get_type())
#define GST_MTKFLACDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKFLACDEC,GstMtkflacdec))
#define GST_MTKFLACDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKFLACDEC,GstMtkflacdecClass))
#define GST_IS_MTKFLACDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKFLACDEC))
#define GST_IS_MTKFLACDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKFLACDEC))

typedef struct _GstMtkflacdec GstMtkflacdec;
typedef struct _GstMtkflacdecClass GstMtkflacdecClass;

struct _GstMtkflacdec
{
  GstAudioDecoder base_mtkflacdec;

};

struct _GstMtkflacdecClass
{
  GstAudioDecoderClass base_mtkflacdec_class;
};

GType gst_mtkflacdec_get_type (void);

G_END_DECLS

#endif
