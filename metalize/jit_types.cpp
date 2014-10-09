// 用于生成llvm jit可使用的类型
// 最好是在编译时动态生成，否则不同系统结果不一样，如x86_64和x86_32生成的不太一样。
// usage:
// clang++ -S -emit-llvm metalize/jit_types.cpp -I. -I/usr/include/qt/QtCore/ -I/usr/include/qt -fPIC -std=c++11 -I/usr/include/qt/QtGui -I/usr/include/qt/QtWidgets -I/usr/include/qt/QtNetwork

#include "metalize/metar_classes_qtcore.h"
#include "metalize/metar_classes_qtgui.h"
#include "metalize/metar_classes_qtwidgets.h"
#include "metalize/metar_classes_qtnetwork.h"

void __keep_jit_types()
{
    void *v0 = NULL;
    // eg.
    // (YaQString*)v0;
    // (YaQUrl*)v0;
    #include "jit_types_body.cpp"
}











