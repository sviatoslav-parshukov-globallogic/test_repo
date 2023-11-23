/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKAACDEC_H_
#define _GST_MTKAACDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKAACDEC   (gst_HALaacdec_get_type())
#define GST_MTKAACDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKAACDEC,GstMtkaacdec))
#define GST_MTKAACDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKAACDEC,GstMtkaacdecClass))
#define GST_IS_MTKAACDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKAACDEC))
#define GST_IS_MTKAACDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKAACDEC))

typedef struct _GstMtkaacdec GstMtkaacdec;
typedef struct _GstMtkaacdecClass GstMtkaacdecClass;

struct _GstMtkaacdec
{
  GstAudioDecoder base_HALaacdec;

};

struct _GstMtkaacdecClass
{
  GstAudioDecoderClass base_HALaacdec_class;
};

GType gst_HALaacdec_get_type (void);

G_END_DECLS

#endif
