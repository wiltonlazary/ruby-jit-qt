#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

a = File.absolute_path(File.dirname(__FILE__))
ret = require "#{a}/test_init.rb"

app = Qt5::QCoreApplication.new(ARGV.count(), ARGV)

puts app

# Signal.trap("INT") do
#   $debug = !$debug
#   puts "Debug now: #$debug"
# end

# Thread.new {
#   puts self
#   # app.exec # QCoreApplication::exec: Must be called from the main thread
#   # puts "execed"
# #   sleep(5);
# #   app.quit
#   100.times {
#     sleep(1)
#     puts "kjoiefwaf"
#     app.exec
#   }
# };

# sleep(5)
# app.quit
# app.exec  # 为什么不能接收控制台的Ctrl+c信号呢?

# 5.times {
#   sleep(1);
# }
# app.quit






