{
  'targets' : [
    {
      'target_name' : 'node-gl',
      'cflags': [
        '-std=c++11',
      ],
      'include_dirs' : [
        "<!(node -e \"require('nan')\")",
        "src/nanovg",
        "/Users/romanliutikov/git/yoga/yoga/"
      ],
      'link_settings': {
        'libraries': [
          '${PWD}/src/libs/libyoga.a',
          '-lglfw',
          '-framework OpenGL',
          '-framework Cocoa',
          '-framework IOKit',
          '-framework CoreVideo',
          '-framework Carbon'
        ],
      },
      'sources' : [
        'src/init.cpp',
        'src/nanovg/nanovg.c',
      ]
    },
  ],
}
