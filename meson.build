project('Xfa', 'c',
  version: '0.1',
  meson_version: '>=0.63.0'
)

cc = meson.get_compiler('c')

xfa_c_args = [
  '-Wall',
  '-Wextra',

  '-Werror=assume',
  '-Werror=implicit-function-declaration',
  '-Werror=incompatible-pointer-types',
  '-Werror=return-type',

  '-Wno-unused-function',
  '-Wno-unused-parameter',
]

if host_machine.system() == 'windows'
  xfa_c_args += '-D_CRT_SECURE_NO_WARNINGS'
endif

xfa_c_args = cc.get_supported_arguments(xfa_c_args)

xfa_deps = [

]

if host_machine.system() == 'windows'
  xfa_deps += cc.find_library('msvcrt', required: false)
endif

subdir('src')
