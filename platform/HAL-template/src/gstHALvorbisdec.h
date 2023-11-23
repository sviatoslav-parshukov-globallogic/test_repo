/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKVORBISDEC_H_
#define _GST_MTKVORBISDEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKVORBISDEC   (gst_HALvorbisdec_get_type())
#define GST_MTKVORBISDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKVORBISDEC,GstMtkvorbisdec))
#define GST_MTKVORBISDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKVORBISDEC,GstMtkvorbisdecClass))
#define GST_IS_MTKVORBISDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKVORBISDEC))
#define GST_IS_MTKVORBISDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKVORBISDEC))

typedef struct _GstMtkvorbisdec GstMtkvorbisdec;
typedef struct _GstMtkvorbisdecClass GstMtkvorbisdecClass;

struct _GstMtkvorbisdec
{
  GstAudioDecoder base_HALvorbisdec;

};

struct _GstMtkvorbisdecClass
{
  GstAudioDecoderClass base_HALvorbisdec_class;
};

GType gst_HALvorbisdec_get_type (void);

G_END_DECLS

#endif
