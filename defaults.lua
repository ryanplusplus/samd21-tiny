function default_flags(cpu)
  add_asflags(
    '-mcpu=' .. cpu,
    '-mthumb',
    '-g2'
  )

  add_cxflags(
    '--specs=nano.specs',
    '-mcpu=' .. cpu,
    '-mthumb',
    '-g',
    '-g2',
    '-Os',
    '-fdata-sections',
    '-ffunction-sections',
    '-Wall',
    '-Wextra',
    '-Werror',
    '-Wfatal-errors',
    '-Wcast-qual',
    '-Wpedantic',
    '-nostartfiles'
  )

  add_cflags(
    '-std=c11'
  )

  add_cxxflags(
    '-fno-rtti',
    '-fno-exceptions',
    '-fno-unwind-tables',
    '-fno-non-call-exceptions',
    '-fno-threadsafe-statics',
    '-fno-use-cxa-atexit',
    '-Weffc++',
    '-std=c++17'
  )

  add_ldflags(
    '-mcpu=' .. cpu,
    '-mthumb',
    '-g',
    '-g2',
    '-Os',
    '-nostartfiles',
    '-Wl,--gc-sections',
    { force = true }
  )
end
