#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include "gstmtkh264dec.h"
#include "gstmtkh265dec.h"
#include "gstmtkh266dec.h"
#include "gstmtkvp8dec.h"
#include "gstmtkvp9dec.h"
#include "gstmtkav1dec.h"
#include "gstmtkmjpegdec.h"
#include "gstmtkdvdec.h"

#include "gstmtkaacdec.h"
#include "gstmtkac3dec.h"
#include "gstmtkeacdec.h"
#include "gstmtkmp3dec.h"
#include "gstmtkflacdec.h"
#include "gstmtkdtsxp2dec.h"
#include "gstmtkopusdec.h"
#include "gstmtkvorbisdec.h"
#include "gstmtkpcmdec.h"

static gboolean plugin_init (GstPlugin * plugin)
{
    if(!gst_element_register (plugin, "mtkh264dec", GST_RANK_NONE, GST_TYPE_MTKH264DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkh265dec", GST_RANK_NONE, GST_TYPE_MTKH265DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkh266dec", GST_RANK_NONE, GST_TYPE_MTKH266DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkvp8dec", GST_RANK_NONE, GST_TYPE_MTKVP8DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkvp9dec", GST_RANK_NONE, GST_TYPE_MTKVP9DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkav1dec", GST_RANK_NONE, GST_TYPE_MTKAV1DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkmjpegdec", GST_RANK_NONE, GST_TYPE_MTKMJPEGDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkdvdec", GST_RANK_NONE, GST_TYPE_MTKDVDEC))
        return FALSE;

    if(!gst_element_register (plugin, "mtkaacdec", GST_RANK_NONE, GST_TYPE_MTKAACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkac3dec", GST_RANK_NONE, GST_TYPE_MTKAC3DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkeacdec", GST_RANK_NONE, GST_TYPE_MTKEACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkmp3dec", GST_RANK_NONE, GST_TYPE_MTKMP3DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkflacdec", GST_RANK_NONE, GST_TYPE_MTKFLACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkdtsxp2dec", GST_RANK_NONE,  GST_TYPE_MTKDTSXP2DEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkopusdec", GST_RANK_NONE, GST_TYPE_MTKOPUSDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkvorbisdec", GST_RANK_NONE, GST_TYPE_MTKVORBISDEC))
        return FALSE;
    if(!gst_element_register (plugin, "mtkpcmdec", GST_RANK_NONE, GST_TYPE_MTKPCMDEC))
        return FALSE;

    return TRUE;
}

GST_PLUGIN_DEFINE (
                   GST_VERSION_MAJOR,
                   GST_VERSION_MINOR,
                   mtk,
                   "MTK hardware decoder plugin",
                   plugin_init,
                   VERSION,
                   "unknown", 
                   "ES Player",
                   "http://vizio.com/"
                  )
