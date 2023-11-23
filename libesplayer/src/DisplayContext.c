#include "DisplayContext.h"

// Create display context with number,
// where num is 0 for primary stream,
// and num > 0 for secondary streams.
VZM_DisplayContext VZM_CreateDisplayContext(int num)
{
    return VZM_DisplayNoContext;
}

// Destroy display context
void VZM_DestroyDisplayContext(VZM_DisplayContext dctx)
{

}

void VZM_SetGeometry(VZM_DisplayContext dctx, VZM_RectF display_rect)
{

}

int VZM_GetZOrder(VZM_DisplayContext dctx)
{
    return 0;
}

int VZM_SetZOrder(VZM_DisplayContext dctx, int z_order)
{
    return 0;
}

int VZM_SetVideoMute(VZM_DisplayContext dctx, int muted)
{
    return 0;
}
