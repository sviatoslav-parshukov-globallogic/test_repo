/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKMP3DEC_H_
#define _GST_MTKMP3DEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKMP3DEC   (gst_HALmp3dec_get_type())
#define GST_MTKMP3DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKMP3DEC,GstMtkmp3dec))
#define GST_MTKMP3DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKMP3DEC,GstMtkmp3decClass))
#define GST_IS_MTKMP3DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKMP3DEC))
#define GST_IS_MTKMP3DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKMP3DEC))

typedef struct _GstMtkmp3dec GstMtkmp3dec;
typedef struct _GstMtkmp3decClass GstMtkmp3decClass;

struct _GstMtkmp3dec
{
  GstAudioDecoder base_HALmp3dec;

};

struct _GstMtkmp3decClass
{
  GstAudioDecoderClass base_HALmp3dec_class;
};

GType gst_HALmp3dec_get_type (void);

G_END_DECLS

#endif
