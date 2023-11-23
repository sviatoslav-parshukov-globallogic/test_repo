/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKH265DEC_H_
#define _GST_MTKH265DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKH265DEC   (gst_mtkh265dec_get_type())
#define GST_MTKH265DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKH265DEC,GstMtkh265dec))
#define GST_MTKH265DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKH265DEC,GstMtkh265decClass))
#define GST_IS_MTKH265DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKH265DEC))
#define GST_IS_MTKH265DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKH265DEC))

typedef struct _GstMtkh265dec GstMtkh265dec;
typedef struct _GstMtkh265decClass GstMtkh265decClass;

struct _GstMtkh265dec
{
  GstVideoDecoder base_mtkh265dec;

};

struct _GstMtkh265decClass
{
  GstVideoDecoderClass base_mtkh265dec_class;
};

GType gst_mtkh265dec_get_type (void);

G_END_DECLS

#endif
