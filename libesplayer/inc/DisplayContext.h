#ifndef VIZIO_MEDIA_LIB_INCLUDE_DISPLAY_CONTEXT_H
#define VIZIO_MEDIA_LIB_INCLUDE_DISPLAY_CONTEXT_H

#include "MediaTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* VZM_DisplayContext;
static const VZM_DisplayContext VZM_DisplayNoContext = NULL;

// Create display context with number,
// where num is 0 for primary stream,
// and num > 0 for secondary streams.
VZM_DisplayContext VZM_CreateDisplayContext(int num);

// Destroy display context
void VZM_DestroyDisplayContext(VZM_DisplayContext dctx);

void VZM_SetGeometry(VZM_DisplayContext dctx, VZM_RectF display_rect);

int VZM_GetZOrder(VZM_DisplayContext dctx);

int VZM_SetZOrder(VZM_DisplayContext dctx, int z_order);

int VZM_SetVideoMute(VZM_DisplayContext dctx, int muted);

#ifdef __cplusplus
}
#endif

#endif
