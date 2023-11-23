#ifndef VIZIO_MEDIA_LIB_INCLUDE_MEDIA_CONFIG_H
#define VIZIO_MEDIA_LIB_INCLUDE_MEDIA_CONFIG_H

#include "MediaTypes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Maximum audio bytes per sample.
static const int kMaxBytesPerSample = 4;

// Maximum audio sampling rate.
static const int kMaxSampleRate = 192000;

typedef enum VZM_AudioCodec_ {
  VZM_kAudioCodecUnknown = 0,
  VZM_kCodecAAC,
  VZM_kCodecMP3,
  VZM_kCodecPCM,
  VZM_kCodecPCM_S16BE,
  VZM_kCodecVorbis,
  VZM_kCodecOpus,
  VZM_kCodecEAC3,
  VZM_kCodecAC3,
  VZM_kCodecDTS,
  VZM_kCodecFLAC,
  VZM_kCodecMpegHAudio,
  VZM_kCodecDTSXP2,

  VZM_kAudioCodecMin = VZM_kAudioCodecUnknown,
  VZM_kAudioCodecMax = VZM_kCodecDTSXP2
} VZM_AudioCodec;

typedef enum VZM_ChannelLayout_ {
  VZM_CHANNEL_LAYOUT_UNSUPPORTED,

  // Front C
  VZM_CHANNEL_LAYOUT_MONO,

  // Front L, Front R
  VZM_CHANNEL_LAYOUT_STEREO,

  // Front L, Front R, Front C, LFE, Side L, Side R
  VZM_CHANNEL_LAYOUT_SURROUND_5_1,

  // Actual channel layout is specified in the bitstream and the actual channel
  // count is unknown at Chromium media pipeline level (useful for audio
  // pass-through mode).
  VZM_CHANNEL_LAYOUT_BITSTREAM,

  // Channels are not explicitly mapped to speakers.
  VZM_CHANNEL_LAYOUT_DISCRETE,

  // Max value, must always equal the largest entry ever logged plus one.
  VZM_CHANNEL_LAYOUT_MAX_LIMIT
} VZM_ChannelLayout;

typedef enum VZM_SampleFormat_ {
  VZM_kUnknownSampleFormat = 0,
  VZM_kSampleFormatU8,         // Unsigned 8-bit w/ bias of 128.
  VZM_kSampleFormatS16,        // Signed 16-bit.
  VZM_kSampleFormatS32,        // Signed 32-bit.
  VZM_kSampleFormatF32,        // Float 32-bit.
  VZM_kSampleFormatPlanarS16,  // Signed 16-bit planar.
  VZM_kSampleFormatPlanarF32,  // Float 32-bit planar.
  VZM_kSampleFormatPlanarS32,  // Signed 32-bit planar.
  VZM_kSampleFormatS24,        // Signed 24-bit.

  VZM_kSampleFormatMin = VZM_kUnknownSampleFormat,
  VZM_kSampleFormatMax = VZM_kSampleFormatS24
} VZM_SampleFormat;

typedef enum VZM_PhotoCodec_ {
  VZM_kPhotoCodecUnknown = 0,
  VZM_kCodecJPEG,
  VZM_kCodecPNG,
  VZM_kCodecHEIC,

  VZM_kPhotoCodecMin = VZM_kPhotoCodecUnknown,
  VZM_kPhotoCodecMax = VZM_kCodecHEIC
} VZM_PhotoCodec;

typedef enum VZM_VideoCodec_ {
  VZM_kVideoCodecUnknown = 0,
  VZM_kCodecH264,
  VZM_kCodecVC1,
  VZM_kCodecMPEG2,
  VZM_kCodecMPEG4,
  VZM_kCodecTheora,
  VZM_kCodecVP8,
  VZM_kCodecVP9,
  VZM_kCodecHEVC,
  VZM_kCodecDolbyVisionH264,
  VZM_kCodecDolbyVisionHEVC,
  VZM_kCodecAV1,
  VZM_kCodecMJPEG,

  VZM_kVideoCodecMin = VZM_kVideoCodecUnknown,
  VZM_kVideoCodecMax = VZM_kCodecMJPEG
} VZM_VideoCodec;

// Profile for Video codec.
typedef enum VZM_VideoProfile_ {
  VZM_kVideoProfileUnknown = 0,
  VZM_kH264Baseline,
  VZM_kH264Main,
  VZM_kH264Extended,
  VZM_kH264High,
  VZM_kH264High10,
  VZM_kH264High422,
  VZM_kH264High444Predictive,
  VZM_kH264ScalableBaseline,
  VZM_kH264ScalableHigh,
  VZM_kH264StereoHigh,
  VZM_kH264MultiviewHigh,
  VZM_kVP8ProfileAny,
  VZM_kVP9Profile0,
  VZM_kVP9Profile1,
  VZM_kVP9Profile2,
  VZM_kVP9Profile3,
  VZM_kDolbyVisionCompatible_EL_MD,
  VZM_kDolbyVisionCompatible_BL_EL_MD,
  VZM_kDolbyVisionNonCompatible_BL_MD,
  VZM_kDolbyVisionNonCompatible_BL_EL_MD,
  VZM_kHEVCMain,
  VZM_kHEVCMain10,
  VZM_kHEVCMainStillPicture,
  VZM_kAV1ProfileMain,
  VZM_kAV1ProfileHigh,
  VZM_kAV1ProfilePro,
  VZM_kMJPEGProfileHuffmanBaselineDCT,
  VZM_kMJPEGProfileHuffmanExtendedDCT,
  VZM_kMJPEGProfileHuffmanProgressiveDCT,
  VZM_kMJPEGProfileHuffmanLossless,
  VZM_kMJPEGProfileJpegLS,

  VZM_kVideoProfileMin = VZM_kVideoProfileUnknown,
  VZM_kVideoProfileMax = VZM_kMJPEGProfileJpegLS
} VZM_VideoProfile;

typedef enum VZM_PrimaryID_ {
  VZM_PRIMARY_ID_INVALID = 0,
  VZM_PRIMARY_ID_BT709 = 1,
  VZM_PRIMARY_ID_UNSPECIFIED = 2,
  VZM_PRIMARY_ID_BT470M = 4,
  VZM_PRIMARY_ID_BT470BG = 5,
  VZM_PRIMARY_ID_SMPTE170M = 6,
  VZM_PRIMARY_ID_SMPTE240M = 7,
  VZM_PRIMARY_ID_FILM = 8,
  VZM_PRIMARY_ID_BT2020 = 9,
  VZM_PRIMARY_ID_SMPTEST428_1 = 10,
  VZM_PRIMARY_ID_SMPTEST431_2 = 11,
  VZM_PRIMARY_ID_SMPTEST432_1 = 12,
  VZM_PRIMARY_ID_EBU_3213_E = 22
} VZM_PrimaryID;

// Table 3
typedef enum VZM_TransferID_ {
  VZM_TRANSFER_ID_INVALID = 0,
  VZM_TRANSFER_ID_BT709 = 1,
  VZM_TRANSFER_ID_UNSPECIFIED = 2,
  VZM_TRANSFER_ID_GAMMA22 = 4,
  VZM_TRANSFER_ID_GAMMA28 = 5,
  VZM_TRANSFER_ID_SMPTE170M = 6,
  VZM_TRANSFER_ID_SMPTE240M = 7,
  VZM_TRANSFER_ID_LINEAR = 8,
  VZM_TRANSFER_ID_LOG = 9,
  VZM_TRANSFER_ID_LOG_SQRT = 10,
  VZM_TRANSFER_ID_IEC61966_2_4 = 11,
  VZM_TRANSFER_ID_BT1361_ECG = 12,
  VZM_TRANSFER_ID_IEC61966_2_1 = 13,
  VZM_TRANSFER_ID_BT2020_10 = 14,
  VZM_TRANSFER_ID_BT2020_12 = 15,
  VZM_TRANSFER_ID_SMPTEST2084 = 16,
  VZM_TRANSFER_ID_SMPTEST428_1 = 17,
  VZM_TRANSFER_ID_ARIB_STD_B67 = 18
} VZM_TransferID;

// Table 4
typedef enum VZM_MatrixID_ {
  VZM_MATRIX_ID_RGB = 0,
  VZM_MATRIX_ID_BT709 = 1,
  VZM_MATRIX_ID_UNSPECIFIED = 2,
  VZM_MATRIX_ID_FCC = 4,
  VZM_MATRIX_ID_BT470BG = 5,
  VZM_MATRIX_ID_SMPTE170M = 6,
  VZM_MATRIX_ID_SMPTE240M = 7,
  VZM_MATRIX_ID_YCOCG = 8,
  VZM_MATRIX_ID_BT2020_NCL = 9,
  VZM_MATRIX_ID_BT2020_CL = 10,
  VZM_MATRIX_ID_YDZDX = 11,
  VZM_MATRIX_ID_INVALID = 255
} VZM_MatrixID;

typedef enum VZM_RangeID_ {
  VZM_RANGE_ID_INVALID = 0,
  // Limited Rec. 709 color range with RGB values ranging from 16 to 235.
  VZM_RANGE_ID_LIMITED = 1,
  // Full RGB color range with RGB valees from 0 to 255.
  VZM_RANGE_ID_FULL = 2,
  // Range is defined by TransferID/MatrixID.
  VZM_RANGE_ID_DERIVED = 3,
  VZM_RANGE_ID_LAST = VZM_RANGE_ID_DERIVED
} VZM_RangeID;

typedef struct VZM_MasteringMetadata_ {
  float primary_r_chromaticity_x;
  float primary_r_chromaticity_y;
  float primary_g_chromaticity_x;
  float primary_g_chromaticity_y;
  float primary_b_chromaticity_x;
  float primary_b_chromaticity_y;
  float white_point_chromaticity_x;
  float white_point_chromaticity_y;
  float luminance_max;
  float luminance_min;
} VZM_MasteringMetadata;

// HDR metadata common for HDR10 and WebM/VP9-based HDR formats.
typedef struct VZM_HDRMetadata_ {
  VZM_MasteringMetadata mastering_metadata;
  unsigned max_content_light_level;
  unsigned max_frame_average_light_level;
} VZM_HDRMetadata;

typedef struct VZM_AudioConfig_ {
  // Stream Id (kPrimary of kSecondary)
  VZM_StreamId id;
  // Audio codec.
  VZM_AudioCodec codec;
  // Audio channel layout.
  VZM_ChannelLayout channel_layout;
  // The format of each audio sample.
  VZM_SampleFormat sample_format;
  // Number of bytes in each channel.
  int bytes_per_channel;
  // Number of channels in this audio stream.
  int channel_number;
  // Number of audio samples per second.
  int samples_per_second;
  // If encrypted: 1 = true, 0 = false
  int is_encrypted;
  // Extra data buffer for certain codec initialization.
  int extra_data_size;
  uint8_t extra_data[];
} VZM_AudioConfig;

typedef struct VZM_VideoConfig_ {
  // Stream Id (kPrimary of kSecondary)
  VZM_StreamId id;
  // Video codec.
  VZM_VideoCodec codec;
  // Video codec profile.
  VZM_VideoProfile profile;
  // If encrypted: 1 = true, 0 = false
  int is_encrypted;
  // Live stream: 1 = true, 0 = false
  int is_live_stream;

  // ColorSpace info
  VZM_PrimaryID primaries;
  VZM_TransferID transfer;
  VZM_MatrixID matrix;
  VZM_RangeID range;

  int have_hdr_metadata;
  VZM_HDRMetadata hdr_metadata;

  // Extra data buffer for certain codec initialization.
  int extra_data_size;
  uint8_t extra_data[];
} VZM_VideoConfig;

typedef enum VZM_PhotoStretchMode_ {
  VZM_PHOTO_STRETCH_MODE_FULL_SCREEN_STRETCH = 0,    // Photo stretched on full screen
  VZM_PHOTO_STRETCH_MODE_FULL_SCREEN,                // Photo displayed on full screen with original aspect ratio
  VZM_PHOTO_STRETCH_MODE_SOURCE_SIZE,                // Photo displayed on screen with source size if the resolution is not larger than output timing
  VZM_PHOTO_STRETCH_MODE_CUSTOMIZE,                  // Photo display is fully controlled by user.
  VZM_PHOTO_STRETCH_MODE_MAX
} VZM_PhotoStretchMode;

typedef struct VZM_PhotoConfig_ {
  // Phot codec.
  VZM_PhotoCodec codec;

  // Photo stretch mode
  VZM_PhotoStretchMode stretch_mode;

  // Pointer to photo and size
  uint8_t* data;
  size_t size;

  // Extra data buffer for certain codec initialization.
  int extra_data_size;
  uint8_t extra_data[];
} VZM_PhotoConfig;

#ifdef __cplusplus
}
#endif

#endif
