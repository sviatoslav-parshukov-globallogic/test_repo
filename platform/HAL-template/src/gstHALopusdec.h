/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKOPUSDEC_H_
#define _GST_MTKOPUSDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKOPUSDEC   (gst_HALopusdec_get_type())
#define GST_MTKOPUSDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKOPUSDEC,GstMtkopusdec))
#define GST_MTKOPUSDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKOPUSDEC,GstMtkopusdecClass))
#define GST_IS_MTKOPUSDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKOPUSDEC))
#define GST_IS_MTKOPUSDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKOPUSDEC))

typedef struct _GstMtkopusdec GstMtkopusdec;
typedef struct _GstMtkopusdecClass GstMtkopusdecClass;

struct _GstMtkopusdec
{
  GstAudioDecoder base_HALopusdec;

};

struct _GstMtkopusdecClass
{
  GstAudioDecoderClass base_HALopusdec_class;
};

GType gst_HALopusdec_get_type (void);

G_END_DECLS

#endif
