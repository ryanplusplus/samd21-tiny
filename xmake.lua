includes('xpack.lua')
xpack_toolchain('gcc-arm', 'arm-none-eabi-gcc@14.2.1-1.1.1')
set_toolchains('gcc-arm')

includes('defaults.lua')
add_default_flags('cortex-m0plus')

includes('board/metro_m0.lua')
local board = board
board:configure()

includes('jlink.lua')

includes('lib_asf.lua')
includes('lib_hardware.lua')
includes('lib_tiny.lua')
includes('lib_tiny-rtt.lua')

target('target') do
  set_kind('binary')
  set_extension('.elf')
  add_deps('asf', 'hardware', 'tiny', 'tiny-rtt')
  add_files('src/*.c')
  add_includedirs('src')
  on_load(function(target)
    board:configure_target(target)
    target:add('ldflags', '-Wl,-Map,$(builddir)/$(plat)/$(arch)/$(mode)/' .. target:name() .. '.map')
  end)

  after_build(function(target)
    import('core.tool.toolchain')
    os.exec(toolchain.load('gcc-arm'):tool('size') .. ' ' .. target:targetfile())
  end)
end

target('upload') do
  set_kind('phony')
  add_deps('target')
  add_rules('jlink-upload')
  set_values('upload-device', board:device())
  set_values('upload-binfile-target', 'target')
end
