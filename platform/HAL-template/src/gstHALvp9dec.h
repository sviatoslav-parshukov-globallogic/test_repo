/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKVP9DEC_H_
#define _GST_MTKVP9DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKVP9DEC   (gst_HALvp9dec_get_type())
#define GST_MTKVP9DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKVP9DEC,GstMtkvp9dec))
#define GST_MTKVP9DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKVP9DEC,GstMtkvp9decClass))
#define GST_IS_MTKVP9DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKVP9DEC))
#define GST_IS_MTKVP9DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKVP9DEC))

typedef struct _GstMtkvp9dec GstMtkvp9dec;
typedef struct _GstMtkvp9decClass GstMtkvp9decClass;

struct _GstMtkvp9dec
{
  GstVideoDecoder base_HALvp9dec;

};

struct _GstMtkvp9decClass
{
  GstVideoDecoderClass base_HALvp9dec_class;
};

GType gst_HALvp9dec_get_type (void);

G_END_DECLS

#endif
