/* GStreamer
 * Copyright (C) 2023 FIXME <fixme@example.com>
 */

#ifndef _GST_MTKDTSXP2DEC_H_
#define _GST_MTKDTSXP2DEC_H_

#include <gst/audio/gstaudiodecoder.h>

G_BEGIN_DECLS

#define GST_TYPE_MTKDTSXP2DEC   (gst_HALdtsxp2dec_get_type())
#define GST_MTKDTSXP2DEC(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),GST_TYPE_MTKDTSXP2DEC,GstMtkdtsxp2dec))
#define GST_MTKDTSXP2DEC_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),GST_TYPE_MTKDTSXP2DEC,GstMtkdtsxp2decClass))
#define GST_IS_MTKDTSXP2DEC(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),GST_TYPE_MTKDTSXP2DEC))
#define GST_IS_MTKDTSXP2DEC_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),GST_TYPE_MTKDTSXP2DEC))

typedef struct _GstMtkdtsxp2dec GstMtkdtsxp2dec;
typedef struct _GstMtkdtsxp2decClass GstMtkdtsxp2decClass;

struct _GstMtkdtsxp2dec
{
  GstAudioDecoder base_HALdtsxp2dec;

};

struct _GstMtkdtsxp2decClass
{
  GstAudioDecoderClass base_HALdtsxp2dec_class;
};

GType gst_HALdtsxp2dec_get_type (void);

G_END_DECLS

#endif
