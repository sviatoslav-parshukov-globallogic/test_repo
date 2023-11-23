#!/bin/bash
gst-launch-1.0 filesrc location=$TEST_VIDEOS_DIR/h264/test.h264 ! h264parse ! avdec_h264 ! videoconvert ! videorate drop-only=true ! "video/x-raw, framerate=1/1" ! jpegenc ! multifilesink location="ffmpeg_h264_%03d.jpg"
gst-launch-1.0 filesrc location=$TEST_VIDEOS_DIR/h264/test.h264 ! h264parse ! vaapih264dec ! videoconvert ! videorate drop-only=true ! "video/x-raw, framerate=1/1" ! jpegenc ! multifilesink location="vaapi_h264_%03d.jpg"
result1=$(compare -metric PSNR ffmpeg_h264_006.jpg vaapi_h264_006.jpg img_diff_6.jpg 2>&1)

echo "this is result $result1 "
if [[ "$result1" != "inf"  ]] && [[ $result < 65 ]]; then
	echo "test failed "
	exit -1
fi
echo "test passed"
