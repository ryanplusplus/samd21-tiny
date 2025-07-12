rule('jlink-upload') do
  on_run(function(target)
    local binfile do
      local binfile_target = target:values('upload-binfile-target')
      if binfile_target then
        binfile = target:dep(binfile_target):targetfile()
      else
        binfile = target:targetfile()
      end
    end

    local jlink_cmd = ([=[
      r
      loadfile %s
      r
      g
      exit
    ]=]):format(binfile)
    local cmdfile = os.tmpfile() .. '.jlink'
    io.writefile(cmdfile, jlink_cmd)
    local flash_cmd = ('JLinkExe -device %s -if SWD -speed 4000 -CommanderScript %s'):format(
      target:values('upload-device'),
      cmdfile
    )
    os.exec(flash_cmd)
    os.rm(cmdfile)
  end)
end
