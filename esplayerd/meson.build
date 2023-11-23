
grpc_dep = [ 
   dependency('grpc++', version : '>=1.29.0'), 
   dependency('grpc', version : '>=10.0.0'), 
   dependency('protobuf', version : '>=3.12.0'), 
  ]
  
protoc = find_program('protoc', required: true)
esplayer_plugin = generator(protoc, \
   output    : ['@BASENAME@.grpc.pb.cc', '@BASENAME@.grpc.pb.h'],
   arguments : ['--grpc_out=@BUILD_DIR@', '--plugin=protoc-gen-grpc=/usr/bin/grpc_cpp_plugin', '--proto_path=@CURRENT_SOURCE_DIR@', 'esplayerd.proto'])
generated_esplayer_plugin = esplayer_plugin.process('esplayerd.proto')
esplayer = generator(protoc, \
  output    : ['@BASENAME@.pb.cc', '@BASENAME@.pb.h'],
  arguments : [ '-I./', '--cpp_out=@BUILD_DIR@', '--proto_path=@CURRENT_SOURCE_DIR@', 'esplayerd.proto'])
generated_esplayer = esplayer.process('esplayerd.proto')

sources = [
  'src/main.cpp',
  'src/mainLoop.cpp',
  'src/gRPCServer.cpp',
  'src/GStreamerPipeline.cpp'
  ]

executable('esplayerd',
 sources,
 generated_esplayer_plugin,
 generated_esplayer,
 dependencies: [grpc_dep, gst_dep],
 install : true,
 link_args : ['-lgrpc++_reflection']
)
