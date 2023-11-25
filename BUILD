cc_library(
    name = "simple_bitcask",
    srcs = [
      'simple_bitcask.cpp',
      'crc32.cpp',
    ],
    hdrs = [
      'simple_bitcask.h',
      'crc32.h',
    ],
    includes = [],
    deps = [
    ],
    copts = [
        "-std=c++11",
    ],
    linkopts = [
    ],
    visibility = ["//visibility:public"],
)

cc_binary(
  name = 'main',
  srcs = [
    'main.cpp',
  ],
  deps = [
    ':simple_bitcask',
  ],
  copts = [
   '--std=c++11',
  ],
  linkopts = [
  ],
)