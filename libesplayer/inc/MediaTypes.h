#ifndef VIZIO_MEDIA_LIB_INCLUDE_MEDIA_TYPES_H
#define VIZIO_MEDIA_LIB_INCLUDE_MEDIA_TYPES_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum VZM_MediaSyncType_ {
    // Default operation, synchronize playback using PTS with higher latency.
    VZM_kModeSyncPts = 0,
    // With this mode, synchronization is disabled and audio/video frames are
    // rendered "right away",
    // - for audio, frames are still rendered based on the sampling frequency
    // - for video, frames are rendered as soon as available at the output of
    //   the video decoder.
    //   The assumption is that no B frames are used when synchronization is
    //   disabled, otherwise B frames would always be skipped.
    VZM_kModeIgnorePts = 1,
    // In addition to the constraints above, also do not wait for vsync.
    VZM_kModeIgnorePtsAndVSync = 2
} VZM_MediaSyncType;

typedef enum VZM_AudioStreamType_ {
    // "Real" audio stream. If this stream underruns, all audio output may pause
    // until more real stream data is available.
    VZM_kAudioStreamNormal = 0,
    // Sound-effects audio stream. May be interrupted if a real audio stream
    // is created with a different sample rate. Underruns on an effects stream
    // do not affect output of real audio streams.
    VZM_kAudioStreamSoundEffects = 1
} VZM_AudioStreamType;

typedef enum VZM_StreamId_ {
    VZM_kStreamPrimary = 0,
    VZM_kStreamSecondary
} VZM_StreamId;

typedef enum VZM_ContentType_ {
    VZM_kContentAudio = 1,
    VZM_kContentVideo = 2,
    VZM_kContentAudioVideo = 3
} VZM_ContentType;

typedef enum VZM_PushBufferStatus_E_ {
    VZM_kBufferUnavailable = -3,
    VZM_kBufferPending = -2,
    VZM_kBufferFailed = -1,
    VZM_kBufferSuccess = 0
} VZM_PushBufferStatus_E;

typedef enum VZM_LogLevel_ {
    VZM_LOG_LEVEL_ERROR = 1,
    VZM_LOG_LEVEL_INFO = 2,
    VZM_LOG_LEVEL_DEBUG = 5
} VZM_LogLevel;

typedef void (*VZM_LoggerCallback)(VZM_LogLevel severity, const char* filename, int line, const char* log_message);

typedef struct VZM_RectF_ {
    float x;
    float y;
    float width;
    float height;
} VZM_RectF;

typedef struct VZM_Rect_
{
    uint32_t u4X; ///< X Position, possible value [0:9999]
    uint32_t u4Y; ///< Y position, possible value [0:9999]
    uint32_t u4W; ///< Width, possible value [1:10000]
    uint32_t u4H; ///< Height, possible value [1:10000]
} VZM_Rect;

typedef struct VZM_Size_ {
    int width;
    int height;
} VZM_Size;

typedef struct VZM_MediaParams_ {
    VZM_MediaSyncType sync_type;
    VZM_AudioStreamType audio_type;
} VZM_MediaParams;

#ifdef __cplusplus
}
#endif

#endif
