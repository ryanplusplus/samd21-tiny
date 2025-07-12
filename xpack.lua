function xpack_toolchain(name, which)
  local toolchain_type = which:match('([^@]+)')
  local toolchain_version = which:match('@(.+)')

  local loaded = false

  on_load(function()
    if loaded then return end
    loaded = true
    os.exec('xpm install @xpack-dev-tools/' .. toolchain_type .. '@' .. toolchain_version .. ' -g -s')
  end)

  toolchain(name)
  set_kind('standalone')
  set_toolset('size', toolchain_type:gsub('%-[^%-]+$', '-size'))
  set_sdkdir('~/' .. (os.host() == 'macosx' and 'Library' or '.local') .. '/xPacks/@xpack-dev-tools/' .. toolchain_type .. '/' .. toolchain_version .. '/.content/')
  toolchain_end()
end
