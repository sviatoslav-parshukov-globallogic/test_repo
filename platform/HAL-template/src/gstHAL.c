#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gst/gst.h>
#include "gstHALh264dec.h"
#include "gstHALh265dec.h"
#include "gstHALh266dec.h"
#include "gstHALvp8dec.h"
#include "gstHALvp9dec.h"
#include "gstHALav1dec.h"
#include "gstHALmjpegdec.h"
#include "gstHALdvdec.h"

#include "gstHALaacdec.h"
#include "gstHALac3dec.h"
#include "gstHALeacdec.h"
#include "gstHALmp3dec.h"
#include "gstHALflacdec.h"
#include "gstHALdtsxp2dec.h"
#include "gstHALopusdec.h"
#include "gstHALvorbisdec.h"
#include "gstHALpcmdec.h"

static gboolean plugin_init (GstPlugin * plugin)
{
    if(!gst_element_register (plugin, "HALh264dec", GST_RANK_NONE, GST_TYPE_MTKH264DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALh265dec", GST_RANK_NONE, GST_TYPE_MTKH265DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALh266dec", GST_RANK_NONE, GST_TYPE_MTKH266DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALvp8dec", GST_RANK_NONE, GST_TYPE_MTKVP8DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALvp9dec", GST_RANK_NONE, GST_TYPE_MTKVP9DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALav1dec", GST_RANK_NONE, GST_TYPE_MTKAV1DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALmjpegdec", GST_RANK_NONE, GST_TYPE_MTKMJPEGDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALdvdec", GST_RANK_NONE, GST_TYPE_MTKDVDEC))
        return FALSE;

    if(!gst_element_register (plugin, "HALaacdec", GST_RANK_NONE, GST_TYPE_MTKAACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALac3dec", GST_RANK_NONE, GST_TYPE_MTKAC3DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALeacdec", GST_RANK_NONE, GST_TYPE_MTKEACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALmp3dec", GST_RANK_NONE, GST_TYPE_MTKMP3DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALflacdec", GST_RANK_NONE, GST_TYPE_MTKFLACDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALdtsxp2dec", GST_RANK_NONE,  GST_TYPE_MTKDTSXP2DEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALopusdec", GST_RANK_NONE, GST_TYPE_MTKOPUSDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALvorbisdec", GST_RANK_NONE, GST_TYPE_MTKVORBISDEC))
        return FALSE;
    if(!gst_element_register (plugin, "HALpcmdec", GST_RANK_NONE, GST_TYPE_MTKPCMDEC))
        return FALSE;

    return TRUE;
}

GST_PLUGIN_DEFINE (
                   GST_VERSION_MAJOR,
                   GST_VERSION_MINOR,
                   HAL,
                   "MTK hardware decoder plugin",
                   plugin_init,
                   VERSION,
                   "unknown", 
                   "ES Player",
                   "http://vizio.com/"
                  )
