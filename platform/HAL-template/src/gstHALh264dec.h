/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKH264DEC_H_
#define _GST_MTKH264DEC_H_

#include <gst/video/video.h>
#include <gst/video/gstvideodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKH264DEC   (gst_HALh264dec_get_type())
#define GST_MTKH264DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKH264DEC,GstMtkh264dec))
#define GST_MTKH264DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKH264DEC,GstMtkh264decClass))
#define GST_IS_MTKH264DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKH264DEC))
#define GST_IS_MTKH264DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKH264DEC))

typedef struct _GstMtkh264dec GstMtkh264dec;
typedef struct _GstMtkh264decClass GstMtkh264decClass;

struct _GstMtkh264dec
{
  GstVideoDecoder base_HALh264dec;

};

struct _GstMtkh264decClass
{
  GstVideoDecoderClass base_HALh264dec_class;
};

GType gst_HALh264dec_get_type (void);

G_END_DECLS

#endif
