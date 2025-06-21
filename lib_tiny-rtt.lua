target('tiny-rtt') do
  set_kind('static')
  add_files('lib/tiny-rtt/src/*.c')
  add_cxflags('-Wno-cast-qual')
  add_includedirs('lib/tiny-rtt/Config')
  add_includedirs('lib/tiny-rtt/include', { public = true })
end
