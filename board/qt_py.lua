board = {
  init = function()
    add_defines(
      '__SAMD21E18A__'
    )
  end,

  configure = function()
    add_ldflags(
      '-T lib/asf4/samd21/gcc/gcc/samd21g18a_flash.ld'
    )
    add_includedirs('src/board/qt_py')
  end,

  svd = function()
    return 'lib/svd/ATSAMD21E18A.svd'
  end,

  device = function()
    return 'ATSAMD21E18A'
  end
}
