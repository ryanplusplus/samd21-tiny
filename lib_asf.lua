target('asf') do
  set_kind('headeronly')
  add_includedirs(path.join(os.scriptdir(), 'lib/asf4/samd21/include'), { public = true })
  add_sysincludedirs(path.join(os.scriptdir(), 'lib/asf4/samd21/CMSIS/Include'), { public = true })
end
