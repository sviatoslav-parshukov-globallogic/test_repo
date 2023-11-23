#ifndef VIZIO_MEDIA_LIB_INCLUDE_PHOTO_PLAYER_H
#define VIZIO_MEDIA_LIB_INCLUDE_PHOTO_PLAYER_H

#include "MediaContext.h"
#include "MediaConfig.h"

#ifdef __cplusplus
extern "C" {
#endif

// Initialize photo subsystem with provided parameters.
int VZM_SetPhotoConfig(VZM_MediaContext ctx, const VZM_PhotoConfig* config);

// Draw photo
int VZM_DrawPhoto(VZM_MediaContext ctx);

// Clear photo
int VZM_ClearPhoto(VZM_MediaContext ctx);

// Rotate photo
int VZM_RotatePhoto(VZM_MediaContext ctx, int angle);

// Zoom photo
int VZM_ZoomPhoto(VZM_MediaContext ctx, float factor);

// Move photo
int VZM_MovePhoto(VZM_MediaContext ctx, int offset_x, int offset_y);

#ifdef __cplusplus
}
#endif

#endif
