# add plugin specific dependencies here, example:
# liquid_dep = cc.find_library('liquid', required : true)
cc = meson.get_compiler('c')   

gstHAL_sources = ['gstHAL.c',
  'gstHALh264dec.c',
  'gstHALh265dec.c',
  'gstHALh266dec.c',
  'gstHALvp8dec.c',
  'gstHALvp9dec.c',
  'gstHALav1dec.c',
  'gstHALmjpegdec.c',
  'gstHALdvdec.c',

  'gstHALaacdec.c',
  'gstHALaacdec.c',
  'gstHALac3dec.c',
  'gstHALeacdec.c',
  'gstHALmp3dec.c',
  'gstHALflacdec.c',
  'gstHALdtsxp2dec.c',
  'gstHALopusdec.c',
  'gstHALvorbisdec.c',
  'gstHALpcmdec.c'

  ]


video_dep = dependency('gstreamer-video-1.0', version: gst_req,
    fallback : ['gstreamer', 'gst_dep'])
audio_dep = dependency('gstreamer-audio-1.0', version: gst_req,
    fallback : ['gstreamer', 'gst_dep'])




library('gstHAL',
  gstHAL_sources,
  c_args :  ['-DPROJECT_NAME=' + meson.project_name(),
  '-DPROJECT_VERSION=' + meson.project_version(),
  '-DVERSION="0.0.1"', 
  '-DPACKAGE="gstHAL"'
  ],
  #include_directories : [configinc, libsinc],
  dependencies : [gst_dep, video_dep, audio_dep],
  install : true,
  install_dir : plugins_install_dir,

)
