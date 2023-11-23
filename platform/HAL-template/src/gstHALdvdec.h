/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKDVDEC_H_
#define _GST_MTKDVDEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKDVDEC   (gst_HALdvdec_get_type())
#define GST_MTKDVDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKDVDEC,GstMtkdvdec))
#define GST_MTKDVDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKDVDEC,GstMtkdvdecClass))
#define GST_IS_MTKDVDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKDVDEC))
#define GST_IS_MTKDVDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKDVDEC))

typedef struct _GstMtkdvdec GstMtkdvdec;
typedef struct _GstMtkdvdecClass GstMtkdvdecClass;

struct _GstMtkdvdec
{
  GstVideoDecoder base_HALdvdec;

};

struct _GstMtkdvdecClass
{
  GstVideoDecoderClass base_HALdvdec_class;
};

GType gst_HALdvdec_get_type (void);

G_END_DECLS

#endif
