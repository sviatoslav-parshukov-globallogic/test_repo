/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKMJPEGDEC_H_
#define _GST_MTKMJPEGDEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKMJPEGDEC   (gst_mtkmjpegdec_get_type())
#define GST_MTKMJPEGDEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKMJPEGDEC,GstMtkmjpegdec))
#define GST_MTKMJPEGDEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKMJPEGDEC,GstMtkmjpegdecClass))
#define GST_IS_MTKMJPEGDEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKMJPEGDEC))
#define GST_IS_MTKMJPEGDEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKMJPEGDEC))

typedef struct _GstMtkmjpegdec GstMtkmjpegdec;
typedef struct _GstMtkmjpegdecClass GstMtkmjpegdecClass;

struct _GstMtkmjpegdec
{
  GstVideoDecoder base_mtkmjpegdec;

};

struct _GstMtkmjpegdecClass
{
  GstVideoDecoderClass base_mtkmjpegdec_class;
};

GType gst_mtkmjpegdec_get_type (void);

G_END_DECLS

#endif
