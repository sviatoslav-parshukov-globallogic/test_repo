/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKH266DEC_H_
#define _GST_MTKH266DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKH266DEC   (gst_mtkh266dec_get_type())
#define GST_MTKH266DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKH266DEC,GstMtkh266dec))
#define GST_MTKH266DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKH266DEC,GstMtkh266decClass))
#define GST_IS_MTKH266DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKH266DEC))
#define GST_IS_MTKH266DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKH266DEC))

typedef struct _GstMtkh266dec GstMtkh266dec;
typedef struct _GstMtkh266decClass GstMtkh266decClass;

struct _GstMtkh266dec
{
  GstVideoDecoder base_mtkh266dec;

};

struct _GstMtkh266decClass
{
  GstVideoDecoderClass base_mtkh266dec_class;
};

GType gst_mtkh266dec_get_type (void);

G_END_DECLS

#endif
