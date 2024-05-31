project('BLM5102_Computer_Systems_and_Network_Security', 'c', 'cpp',
  version : '0.1',
  default_options : ['c_std=c89', 'cpp_std=c++17', 'warning_level=3'])

add_global_arguments('-fno-stack-protector', language : 'c')

message('meson.build file find successfully in: ' + meson.current_source_dir())

executable('stack_canaries_app',
           'main.c',
           install : false)
