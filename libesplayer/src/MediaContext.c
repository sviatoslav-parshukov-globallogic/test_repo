#include "MediaContext.h"

// Create context and initialize media subsystem.
// Returns pointer to media context, or VZM_MediaNoContext
// if media initialization failed.
VZM_MediaContext VZM_CreateMediaContext(VZM_MediaParams params, VZM_MediaCallback callback)
{
    return VZM_MediaNoContext;
}

// Deinitialize media and destroy context
void VZM_DestroyMediaContext(VZM_MediaContext ctx)
{

}

// Get current playing audio and video timestamps in microseconds
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_GetCurrentPTS(VZM_MediaContext ctx, uint64_t *video_pts, uint64_t *audio_pts)
{
    return VZM_MediaStateFail;
}

// Get display context of currently playing video.
// Return pointer to context if success, or VZM_DisplayNoContext if failed.
VZM_DisplayContext VZM_GetDisplayContext(VZM_MediaContext ctx)
{
    return VZM_DisplayNoContext;
}

// Play sound clip once
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_PlaySoundClip(VZM_MediaContext ctx, uint8_t* buffer, size_t size)
{
    return VZM_MediaStateFail;
}

// Get pointer to media buffer
// if audio/video configured for encrypted content (is_encrypted = 1),
// secure memory which is only accessible by TEE will be allocated
uint8_t* VZM_GetBuffer(VZM_MediaContext ctx, size_t size, VZM_ContentType content_type)
{
    return (uint8_t*)NULL;
}

// Push buffer into media subsystem.
// Return:
//    VZM_kBufferSuccess: success, ready for next
//    VZM_kBufferPending: buffer accepted, but rendering is pendiing. Need to
//        wait for corresponding callback before pushing next buffer.
//    VZM_kBufferFailed: failed
VZM_PushBufferStatus_E VZM_PushBuffer(VZM_MediaContext ctx, uint8_t* buffer, int64_t timestamp)
{
    return VZM_kBufferFailed;
}

// Get pointer to decoded buffer of audio/video content
// TODO: pixel / audio sample format must be negotiated
uint8_t* GetDecodedBuffer(VZM_MediaContext ctx, VZM_ContentType content_type)
{
    return (uint8_t*)NULL;
}

// Release pointer to decoded buffer when no longer needed
void ReleaseDecodedBuffer(VZM_MediaContext ctx, uint8_t* buffer)
{

}

// Initialize audio subsystem with provided parameters.
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_SetAudioConfig(VZM_MediaContext ctx, const VZM_AudioConfig* config)
{
    return VZM_MediaStateFail;
}

// Initialize video subsystem with provided parameters.
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_SetVideoConfig(VZM_MediaContext ctx, const VZM_VideoConfig* config)
{
    return VZM_MediaStateFail;
}

// Notify media subsystem that source stream is ended.
// If stream has both audio and video substreams, this method for
// each corresponding substream.
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_SetEndOfStream(VZM_MediaContext ctx, VZM_ContentType content_type)
{
    return VZM_MediaStateFail;
}

// Set media speed (2019 TV only)
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_SetSpeed(VZM_MediaContext ctx, float rate)
{
    return VZM_MediaStateFail;
}

// Set audio volume
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_SetVolume(VZM_MediaContext ctx, float multiplier)
{
    return VZM_MediaStateFail;
}

// Start media playback with initial timestamp in microseconds
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_Start(VZM_MediaContext ctx, uint64_t start_pts)
{
    return VZM_MediaStateFail;
}

// Stop media playback
int VZM_Stop(VZM_MediaContext ctx)
{
    return 0;
}

// Flush media buffers
// content_type is a bitmask of buffers to flush
// ex.: VZM_kContentAudio | VZM_kContentVideo
// Return VZM_MediaStateOK if success, or VZM_MediaStateFail if failed
int VZM_Flush(VZM_MediaContext ctx, VZM_ContentType content_type)
{
    return VZM_MediaStateFail;
}

// Pause media playback
int VZM_Pause(VZM_MediaContext ctx)
{
    return 0;
}

// Resume media playback
int VZM_Resume(VZM_MediaContext ctx)
{
    return 0;
}

// Set log level and callback
void VZM_SetLogger(VZM_LogLevel log_level, VZM_LoggerCallback cb)
{

}

// Check if audio config supported
// Return VZM_MediaStateSupported if supported,
//        or VZM_MediaStateNotSupported if no supported
int VZM_IsSupportedAudioConfig(const VZM_AudioConfig* config)
{
    return VZM_MediaStateNotSupported;
}

// Check if video config supported
// Return VZM_MediaStateSupported if supported,
//        or VZM_MediaStateNotSupported if no supported
int VZM_IsSupportedVideoConfig(const VZM_VideoConfig* config)
{
    return VZM_MediaStateNotSupported;
}
