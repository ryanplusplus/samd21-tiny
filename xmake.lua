includes('xpack.lua')
xpack_toolchain('gcc-arm', 'arm-none-eabi-gcc@14.2.1-1.1.1')
set_toolchains('gcc-arm')

includes('defaults.lua')
default_flags('cortex-m0plus')

includes('board/metro_m0.lua')
local board = board
board:init()

includes('lib_asf.lua')
includes('lib_hardware.lua')
includes('lib_tiny.lua')
includes('lib_tiny-rtt.lua')

target('target') do
  set_default(true)
  set_kind('binary')
  set_extension('.elf')
  add_deps('asf', 'hardware', 'tiny', 'tiny-rtt')
  add_files('src/*.c')
  add_includedirs('src')
  add_ldflags(
    '-Wl,-Map,$(buildir)/$(plat)/$(arch)/$(mode)/target.map'
  )
  board:configure()

  after_build(function(target)
    import('core.tool.toolchain')
    os.exec(toolchain.load('gcc-arm'):tool('size') .. ' ' .. target:targetfile())
  end)
end

target('upload') do
  set_default(false)
  set_kind('phony')
  add_deps('target')
  on_build(function(target)
    local jlink_cmd = ([=[
      r
      loadfile %s
      r
      g
      exit
    ]=]):format(target:dep('target'):targetfile())
    local cmdfile = os.tmpfile() .. '.jlink'
    io.writefile(cmdfile, jlink_cmd)
    local flash_cmd = ('JLinkExe -device %s -if SWD -speed 4000 -CommanderScript %s'):format(
      board:device(),
      cmdfile
    )
    os.exec(flash_cmd)
    os.rm(cmdfile)
  end)
end
