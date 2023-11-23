/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKAC3DEC_H_
#define _GST_MTKAC3DEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKAC3DEC   (gst_mtkac3dec_get_type())
#define GST_MTKAC3DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKAC3DEC,GstMtkac3dec))
#define GST_MTKAC3DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKAC3DEC,GstMtkac3decClass))
#define GST_IS_MTKAC3DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKAC3DEC))
#define GST_IS_MTKAC3DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKAC3DEC))

typedef struct _GstMtkac3dec GstMtkac3dec;
typedef struct _GstMtkac3decClass GstMtkac3decClass;

struct _GstMtkac3dec
{
  GstAudioDecoder base_mtkac3dec;

};

struct _GstMtkac3decClass
{
  GstAudioDecoderClass base_mtkac3dec_class;
};

GType gst_mtkac3dec_get_type (void);

G_END_DECLS

#endif
