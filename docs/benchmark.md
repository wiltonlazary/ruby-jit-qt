  0.110000   0.010000   0.120000 (  0.130065) +load ast的时间，70,  模板实例化时间30，执行时间
  0.020000   0.010000   0.030000 (  0.027951)

  0.120000   0.020000   0.140000 (  0.136926)
  0.020000   0.010000   0.030000 (  0.032951)

  0.190000   0.070000   0.260000 (  0.257139)
  0.090000   0.060000   0.150000 (  0.152348)


以下使用qt5.QString.new("abc")测试，源代码在../bench/bc\_001.rb。
在把loadPreparedASTFile放在Init_handby中
加载 .so   0.100000   0.000000   0.100000 (  0.105675)
首次new类  0.040000   0.010000   0.050000 (  0.051636)
二次new类  0.030000   0.000000   0.030000 (  0.025341)

在把loadPreparedASTFile放在CtrlEngine首次使用中
加载 .so   0.020000   0.010000   0.030000 (  0.030760)
首次new类  0.110000   0.020000   0.130000 (  0.122943)
二次new类  0.020000   0.010000   0.030000 (  0.026927)

由以上两个测试，得出的loadPreparedASTFile调用大概用时70ms。
即使使用rb\_thread\_create函数，也无法真正放在另一线程中执行，还是会占用加载.so的时间。
花在模板实例化上的时间大概在30ms。

