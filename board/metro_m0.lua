board = {
  init = function()
    add_defines(
      '__SAMD21G18A__'
    )
  end,

  configure = function()
    add_ldflags(
      '-T lib/asf4/samd21/gcc/gcc/samd21g18a_flash.ld'
    )
    add_includedirs('src/board/metro_m0')
  end,

  svd = function()
    return 'lib/svd/ATSAMD21G18A.svd'
  end,

  device = function()
    return 'ATSAMD21G18A'
  end
}
