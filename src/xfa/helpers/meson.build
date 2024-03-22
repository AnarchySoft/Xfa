helpers_src = files(
  'io.c',
)

if host_machine.system() == 'windows'
  helpers_src += files('fsys.win32.c')
else
  helpers_src += files('fsys.unix.c')
endif