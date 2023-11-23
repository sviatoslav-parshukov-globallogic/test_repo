#ifndef VIZIO_MEDIA_LIB_INCLUDE_MEDIA_CALLBACK_H
#define VIZIO_MEDIA_LIB_INCLUDE_MEDIA_CALLBACK_H

#include "MediaTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct VZM_MediaCallback_ {
    void* user_data;
    void (*onVideoResolutionChanged)(void* user_data, VZM_Size size);
    void (*onEndOfStream)(void* user_data);
    void (*onPushComplete)(void* user_data);
    void (*onPlaying)(void* user_data);
    void (*onError)(void* user_data, int error_code);
    void (*onOutputBufferAvailable)(void* user_data, VZM_ContentType content_type);
    // TODO: subtitle stucture will be specified later
    void (*onSubtitle)(void* user_data, void* data);
} VZM_MediaCallback;

#ifdef __cplusplus
}
#endif

#endif
