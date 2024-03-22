xfa_src = files(
  'main.c',
)

subdir('xfa')

xfa_lib = library('libxfa', libxfa_src,
  c_args: xfa_c_args,
  dependencies: xfa_deps,
  install: true
)

xfa_exe = executable('xfa', xfa_src,
  c_args: xfa_c_args,
  dependencies: xfa_deps,
  link_with: xfa_lib,
  install: true
)
