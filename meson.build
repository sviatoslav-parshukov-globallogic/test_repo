project('esplayer', 'c', 'cpp', version : '0.0.0.1', license : 'Commercial')

gst_req = '>= 1.18.0'

glib_dep = dependency('glib-2.0', version: '>= 2.38',
    fallback: ['glib', 'libglib_dep'])
gio_dep = dependency('gio-2.0',
    fallback: ['glib', 'libgio_dep'])
gobject_dep = dependency('gobject-2.0', version: '>= 2.38',
    fallback: ['glib', 'libgobject_dep'])
gmodule_dep = dependency('gmodule-2.0',
    fallback: ['glib', 'libgmodule_dep'])

gst_dep = dependency('gstreamer-1.0', version: gst_req,
    fallback : ['gstreamer', 'gst_dep'])

#gstplay_dep = dependency('gstreamer-play-1.0', version: gst_req,
#    fallback: ['gst-plugins-bad', 'gstplay_dep'])
#gsttag_dep = dependency('gstreamer-tag-1.0', version: gst_req,
#    fallback: ['gst-plugins-base', 'tag_dep'])
#gstwebrtc_dep = dependency('gstreamer-webrtc-1.0', version: gst_req,
#    fallback: ['gst-plugins-bad', 'gstwebrtc_dep'])
#gstsdp_dep = dependency('gstreamer-sdp-1.0', version : gst_req,
#    fallback : ['gst-plugins-base', 'sdp_dep'])
#gstrtp_dep = dependency('gstreamer-rtp-1.0', version : gst_req,
#    fallback : ['gst-plugins-base', 'rtp_dep'])

#add_project_arguments('-O3',  language : 'cpp')
#
build_args = [
]

subdir('esplayerd')
subdir('libesplayer')
subdir('platform')

#test_h264 = find_program('test_h264.sh')
#test('test h264 encoder ', test_h264, env: ['TEST_VIDEOS_DIR=' + meson.project_source_root() + '/video-tests'])
