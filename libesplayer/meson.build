sources = [
  'src/gRPCInterface.cpp',
  'src/GStreamerPipeline.cpp',
  'src/main.cpp'
  ]

interface_lib_sources = [
    'src/DisplayContext.c',
    'src/MediaContext.c',
    'src/PhotoPlayer.c'
]

library('esplayer',
  interface_lib_sources,
  include_directories : 'inc',
  install : true
)

static_library('esplayer',
  interface_lib_sources,
  include_directories : 'inc',
  install : true
)

executable('libesplayer',
 sources,
 generated_esplayer_plugin,
 generated_esplayer,
 dependencies: [grpc_dep, gst_dep],
 install : true,
 link_args : ['-lgrpc++_reflection']
)
