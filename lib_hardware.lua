target('hardware') do
  set_kind('static')
  add_deps('asf', 'tiny')
  add_includedirs(path.join(os.scriptdir(), 'src/hardware'), { public = true })
  add_files(path.join(os.scriptdir(), 'src/hardware/*.c'))
end
