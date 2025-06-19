target('tiny') do
  set_kind('static')
  add_files('lib/tiny/src/*.c')
  add_includedirs('lib/tiny/include', { public = true })
end
