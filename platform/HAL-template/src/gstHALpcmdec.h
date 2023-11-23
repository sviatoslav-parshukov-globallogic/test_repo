/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKPCMDEC_H_
#define _GST_MTKPCMDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKPCMDEC   (gst_HALpcmdec_get_type())
#define GST_MTKPCMDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKPCMDEC,GstMtkpcmdec))
#define GST_MTKPCMDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKPCMDEC,GstMtkpcmdecClass))
#define GST_IS_MTKPCMDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKPCMDEC))
#define GST_IS_MTKPCMDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKPCMDEC))

typedef struct _GstMtkpcmdec GstMtkpcmdec;
typedef struct _GstMtkpcmdecClass GstMtkpcmdecClass;

struct _GstMtkpcmdec
{
  GstAudioDecoder base_HALpcmdec;

};

struct _GstMtkpcmdecClass
{
  GstAudioDecoderClass base_HALpcmdec_class;
};

GType gst_HALpcmdec_get_type (void);

G_END_DECLS

#endif
