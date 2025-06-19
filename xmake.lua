includes('xpack.lua')
xpack_toolchain('gcc-arm', 'arm-none-eabi-gcc@14.2.1-1.1.1')
set_toolchains('gcc-arm')

includes('defaults.lua')
default_flags('cortex-m0plus')

includes('board/metro_m0.lua')
board:init()

includes('lib_asf.lua')
includes('lib_hardware.lua')
includes('lib_tiny.lua')
includes('lib_tiny-rtt.lua')

target('target') do
  set_kind('binary')
  add_deps('asf', 'hardware', 'tiny', 'tiny-rtt')
  add_files('src/*.c')
  add_includedirs('src')
  add_ldflags(
    '-Wl,-Map,$(buildir)/$(plat)/$(arch)/$(mode)/target.map'
  )
  board:configure()
  after_build(function(target)
    import("core.tool.toolchain")
    os.exec(toolchain.load('gcc-arm'):tool('size') .. ' ' .. target:targetfile())
  end)
end
