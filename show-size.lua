rule('show-size') do
  after_build(function(target)
    os.exec(target:toolchains()[1]:tool('size') .. ' ' .. target:targetfile())
  end)
end
