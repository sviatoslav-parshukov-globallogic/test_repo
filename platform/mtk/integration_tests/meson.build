dep_gstreamer = dependency('gstreamer-1.0')
dep_gstreamer_check = dependency('gstreamer-check-1.0')

gst_build_path = meson.project_build_root()

e1 = executable( 'skeleton_integration', 'skeleton_integration.c',
    dependencies:[dep_gstreamer, dep_gstreamer_check ])
test('skeleton_integration', e1,
    env: ['GST_PLUGIN_PATH='+ gst_build_path + '/platform/mtk/src'])
