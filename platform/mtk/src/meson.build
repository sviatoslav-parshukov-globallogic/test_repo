# add plugin specific dependencies here, example:
# liquid_dep = cc.find_library('liquid', required : true)
cc = meson.get_compiler('c')   

gstmtk_sources = ['gstmtk.c',
  'gstmtkh264dec.c',
  'gstmtkh265dec.c',
  'gstmtkh266dec.c',
  'gstmtkvp8dec.c',
  'gstmtkvp9dec.c',
  'gstmtkav1dec.c',
  'gstmtkmjpegdec.c',
  'gstmtkdvdec.c',

  'gstmtkaacdec.c',
  'gstmtkaacdec.c',
  'gstmtkac3dec.c',
  'gstmtkeacdec.c',
  'gstmtkmp3dec.c',
  'gstmtkflacdec.c',
  'gstmtkdtsxp2dec.c',
  'gstmtkopusdec.c',
  'gstmtkvorbisdec.c',
  'gstmtkpcmdec.c'
  ]

video_dep = dependency('gstreamer-video-1.0', version: gst_req,
    fallback : ['gstreamer', 'gst_dep'])
audio_dep = dependency('gstreamer-audio-1.0', version: gst_req,
    fallback : ['gstreamer', 'gst_dep'])

library('gstmtk',
  gstmtk_sources,
  c_args :  ['-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
  '-DVERSION="0.0.1"', 
  '-DPACKAGE="gstmtk"'
  ],
  #include_directories : [configinc, libsinc],
  dependencies : [gst_dep, video_dep, audio_dep],
  install : true,
  install_dir : plugins_install_dir,
)
