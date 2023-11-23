/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKAV1DEC_H_
#define _GST_MTKAV1DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKAV1DEC   (gst_mtkav1dec_get_type())
#define GST_MTKAV1DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKAV1DEC,GstMtkav1dec))
#define GST_MTKAV1DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKAV1DEC,GstMtkav1decClass))
#define GST_IS_MTKAV1DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKAV1DEC))
#define GST_IS_MTKAV1DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKAV1DEC))

typedef struct _GstMtkav1dec GstMtkav1dec;
typedef struct _GstMtkav1decClass GstMtkav1decClass;

struct _GstMtkav1dec
{
  GstVideoDecoder base_mtkav1dec;

};

struct _GstMtkav1decClass
{
  GstVideoDecoderClass base_mtkav1dec_class;
};

GType gst_mtkav1dec_get_type (void);

G_END_DECLS

#endif
