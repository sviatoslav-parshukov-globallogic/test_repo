/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKVP8DEC_H_
#define _GST_MTKVP8DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKVP8DEC   (gst_HALvp8dec_get_type())
#define GST_MTKVP8DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKVP8DEC,GstMtkvp8dec))
#define GST_MTKVP8DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKVP8DEC,GstMtkvp8decClass))
#define GST_IS_MTKVP8DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKVP8DEC))
#define GST_IS_MTKVP8DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKVP8DEC))

typedef struct _GstMtkvp8dec GstMtkvp8dec;
typedef struct _GstMtkvp8decClass GstMtkvp8decClass;

struct _GstMtkvp8dec
{
  GstVideoDecoder base_HALvp8dec;

};

struct _GstMtkvp8decClass
{
  GstVideoDecoderClass base_HALvp8dec_class;
};

GType gst_HALvp8dec_get_type (void);

G_END_DECLS

#endif
