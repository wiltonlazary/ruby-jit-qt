
#include "fix_clang_undef_eai.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtNetwork>
// #include <QtWebSockets>

#include <cassert>
#include <functional>
#include <typeinfo>


#include <llvm/ExecutionEngine/GenericValue.h>

#include "debugoutput.h"

#include "qtregister.h"
#include "ruby_cxx.h"
#include "qtobjectmanager.h"

#include "utils.h"
#include "entry.h"

#include "clvm.h"

#include "marshallruby.h"
#include "callargument.h"

#include "qtruby.h"

// extern "C" {



// #define VariantOrigValue(v)                         
#define VOValue(v)                            \
    (                                                  \
     (v.type() == QVariant::Invalid) ? QVariant() :    \
     (v.type() == QVariant::Bool) ? v.toBool() :       \
     (v.type() == QVariant::Int) ? v.toInt() :         \
     (v.type() == QVariant::Double) ? v.toDouble() :   \
     (v.type() == QVariant::Char) ? v.toChar() :       \
     (v.type() == QVariant::String) ? v.toString() :   \
     (v.type() == QVariant::Point) ? v.toPoint() :     \
     QVariant())


static void abc()
{
    QVariant v;

    // error, error: non-pointer operand type 'QPoint' incompatible with nullptr
    // 同一表达式的返回值类型应该相同啊。
    //*
    (
     (v.type() == QVariant::Invalid) ? QVariant() : 
     (v.type() == QVariant::Bool) ? v.toBool() :
     (v.type() == QVariant::Int) ? v.toInt() :
     (v.type() == QVariant::Double) ? v.toDouble() :
     (v.type() == QVariant::Char) ? v.toChar() :
     (v.type() == QVariant::String) ? v.toString() :
     (v.type() == QVariant::Point) ? v.toPoint() : QVariant()
     );
        // */
}

typedef struct {
    int iretval;
    bool bretval;
    QString sretval;
} ReturnStorage;

typedef struct {
    int iretval;
    bool bretval;
    QString sretval;

    // 最大支持10个参数，参数的临时值放在这
    int ival[10];
    bool bval[10];
    QString sval[10];
} InvokeStorage;

static QGenericReturnArgument makeRetArg(int type, ReturnStorage &rs)
{
    QGenericReturnArgument retval;

    switch (type) {
    case QMetaType::Int: retval = Q_RETURN_ARG(int, rs.iretval); break;
    case QMetaType::Bool: retval = Q_RETURN_ARG(bool, rs.bretval); break;
    case QMetaType::QString: retval = Q_RETURN_ARG(QString, rs.sretval); break;
    }

    return retval;
}

static VALUE retArg2Value(int type, QGenericReturnArgument retarg)
{
    VALUE retval = Qnil;
    ReturnStorage rs;

    switch (type) {
    case QMetaType::Int:
        rs.iretval = *((int*)retarg.data());
        retval = INT2NUM(rs.iretval);
        break;
    case QMetaType::QString:
        rs.sretval = *((QString*)retarg.data());
        retval = rb_str_new2(rs.sretval.toLatin1().data());
        break;
    case QMetaType::Bool:
        rs.bretval = *((bool*)retarg.data());
        retval = rs.bretval ? Qtrue : Qfalse;
        break;
    case QMetaType::Void:
        
        break;
    default:
        qDebug()<<"unknown ret type:"<<type<<retarg.name()<<retarg.data();
        break;
    };

    return retval;
}

static QGenericArgument Variant2Arg(int type, QVariant &v, int idx, InvokeStorage &is)
{
    QGenericArgument valx;

    switch (type) {
    case QMetaType::Int: 
        is.ival[idx] = v.toInt();
        valx = Q_ARG(int, is.ival[idx]);
        break;
    case QMetaType::QString: 
        is.sval[idx] = v.toString();
        valx = Q_ARG(QString, is.sval[idx]); 
        break;
    case QMetaType::Bool:
        is.bval[idx] = v.toBool();
        valx = Q_ARG(bool, is.bval[idx]);
        break;
    }

    return valx;
}


#include "ctrlengine.h"
#include "frontengine.h"
#include "tests.cpp"

// static CtrlEngine *gce = new CtrlEngine();
static CtrlEngine *gce = NULL;

/*
  统一的to_s方法
  
 */
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_to_s(int argc, VALUE *argv, VALUE obj)
{
    qDebug()<<argc;
    QString klass_name = QString(rb_class2name(RBASIC_CLASS(obj)));
    klass_name = klass_name.split("::").at(1);
    void *ci = Qom::inst()->getObject(obj);

    QString jitstr = gce->vm_qdebug(ci, klass_name);
    
    QString stc; // stream container
    QDebug dm(&stc);
    // dm << "tsed:" << klass_name << "Obj:"<< obj << "Hash:" << rb_hash(obj) << "C:"<< ci;
    dm << "RBO:"<<obj << "["<< jitstr << "]";
    // 怎么能解引用呢 *ci, 当前void*无法解。
    // dm << "hehe has newline???"; // 这种方式要手动换行。

    // 简单方法，
    /*
    if (klass_name == "Qt5:QString") dm << *(YaQString*)ci;
    else if (klass_name == "Qt5::QUrl") dm << *(YaQUrl*)ci;

    if (klass_name == "Qt5::QWidget") {
        QWidget *w = (QWidget*)ci;
    }
    */
    
    // qDebug()<<stc;
    VALUE rv = rb_str_new2(stc.toLatin1().data());
    return rv;
}

/*
  解决类中的enum的处理
  TODO:
  使用FrontEngine查找
 */
VALUE x_Qt_class_const_missing_jit(int argc, VALUE *argv, VALUE obj)
{
    qDebug()<<argc<<TYPE(obj)<<TYPE(argv[0]);
    qDebug()<<QString(rb_id2name(SYM2ID(argv[0])))<<rb_class2name(obj);
    QString klass_name = QString(rb_class2name(obj));
    klass_name = klass_name.split("::").at(1);
    QString const_name = QString(rb_id2name(SYM2ID(argv[0])));
    QString yconst_name = "y" + const_name;
    qDebug()<<klass_name<<const_name;

    int enum_val = gce->vm_enum(klass_name, const_name);
    qDebug()<<enum_val;

    return INT2NUM(enum_val);
    return Qnil;
}

/*
  解决类中的enum的处理
  TODO:
  使用staticMetaObject检测enum变量。
 */
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_const_missing(int argc, VALUE *argv, VALUE obj)
{
    qDebug()<<argc<<TYPE(obj)<<TYPE(argv[0]);
    qDebug()<<QString(rb_id2name(SYM2ID(argv[0])))<<rb_class2name(obj);
    QString klass_name = QString(rb_class2name(obj));
    QString const_name = QString(rb_id2name(SYM2ID(argv[0])));
    QString yconst_name = "y" + const_name;
    // qDebug()<<klass_name<<const_name;

    if (klass_name == "Qt5::QHostAddress") {
        //  if (const_name == "AnyIPv4") return INT2NUM(QHostAddress::AnyIPv4);
    }
    
    VALUE *targv = NULL;
    int targc = 0;
    VALUE self = rb_class_new_instance(targc, targv, obj);
    GET_CI0();
    const QMetaObject *mo = ci->metaObject();
    // qDebug()<<"===="<<ci<<mo<<"----";

    // qDebug()<<"ec:"<<mo->enumeratorCount();
    for (int i = 0; i < mo->enumeratorCount(); i++) {
        QMetaEnum me = mo->enumerator(i);
        bool ok = false;
        // qDebug()<<"enum:"<<i<<""<<me.name();
        // qDebug()<<"enum2:"<<me.keyCount()<<me.keyToValue(const_name.toLatin1().data(), &ok)<<ok;
        int enum_val = me.keyToValue(yconst_name.toLatin1().data(), &ok);
        if (ok) {
            rb_gc_mark(self);
            return INT2NUM(enum_val);
        }

        continue; // below for debug/test purpose
        for (int j = 0; j < me.keyCount(); j++) {
            qDebug()<<"enum2:"<<j<<me.key(j);

            if (QString(me.key(i)) == yconst_name) {
                
            }
        }
    }

    // not found const
    VALUE exception = rb_eException;
    rb_raise(exception, "NameError: uninitialized yconstant %s::%s.\n",
             klass_name.toLatin1().data(), const_name.toLatin1().data());
    /*
    rb_fatal("NameError: uninitialized constant %s::%s.\n",
             klass_name.toLatin1().data(), const_name.toLatin1().data());
    */

    return Qnil;
}

[[deprecated("pregen proto not needed now")]]
static VALUE x_Qt_meta_class_dtor_jit(VALUE id)
{
    VALUE os = rb_const_get(rb_cModule, rb_intern("ObjectSpace"));
    VALUE self = rb_funcall(os, rb_intern("_id2ref"), 1, id);
    QString klass_name = QString(rb_class2name(RBASIC_CLASS(self)));
    klass_name = klass_name.split("::").at(1);
    qDebug()<<"dtor:"<<klass_name;

    void *ci = Qom::inst()->getObject(self);
    qDebug()<<"herhe:"<<ci;

    // TODO
    // 函数为void*时程序崩溃，这是llvm的bug还是使用有问题。
    QString code_src = QString("#include <QtCore>\n"
                               "void jit_main(int a, void *ci) {"
                               "qDebug()<<\"test int:\"<<a;"
                               "qDebug()<<\"in jit:\"<<ci;\n"
                               "delete (%1*)ci;}").arg(klass_name);
    QVector<llvm::GenericValue> gvargs;
    llvm::GenericValue ia;
    ia.IntVal = llvm::APInt(32, 567);
    gvargs.push_back(ia);
    gvargs.push_back(llvm::PTOGV(ci));
    qDebug()<<"view conv back:"<<llvm::GVTOP(gvargs.at(0));

    // delete (QString*)ci;
    llvm::GenericValue gvret = vm_execute(code_src, gvargs);

    return Qnil;
}

/*
  类实例的析构函数  
  TODO:
  使用通用方法之后，宏SAVE_XXX和GET_XXX就可以不需要了。
 */
[[deprecated("pregen proto not needed now")]]
static VALUE x_Qt_meta_class_dtor(VALUE id)
{
    VALUE os = rb_const_get(rb_cModule, rb_intern("ObjectSpace"));
    VALUE self = rb_funcall(os, rb_intern("_id2ref"), 1, id);

    GET_CI0();
    delete ci;

    return Qnil;
}

/////////////////////////////////////// jitttttt
/*
  类实例的析构函数  
  TODO:
  使用通用方法之后，宏SAVE_XXX和GET_XXX就可以不需要了。
 */
static VALUE x_Qt_class_dtor(VALUE id)
{
    VALUE os = rb_const_get(rb_cModule, rb_intern("ObjectSpace"));
    VALUE self = rb_funcall(os, rb_intern("_id2ref"), 1, id);

    // GET_CI0();
    // delete ci;

    // void* now, can not delete, need dtor and free
    void *qo = Qom::inst()->getObject(self);
    qDebug()<<qo<<rb_class2name(RBASIC_CLASS(self));

    return Qnil;
}

/*
// 这应该叫统一的方法，获取qt class名称
// get_qt_class(VALUE self)
// 判断是否是qt类，如果不是，很可能是在Ruby中继承Qt类
// 如果是继承类，取到需要实例化的Qt类名
TODO 还需要考虑继承层次大于2的情况
@param self T_OBJECT | T_CLASS
 */
static QString get_qt_class(VALUE self)
{
    if (TYPE(self) == T_CLASS) {
        QString klass_name = rb_class2name(self);
        if (klass_name.startsWith("Qt5::Q")) return klass_name.split("::").at(1);

        // 如果是继承类，取到需要实例化的Qt类名
        VALUE pcls = RCLASS_SUPER(self);
        klass_name = rb_class2name(pcls);
        return klass_name.split("::").at(1);
    }
    // T_OBJECT
    else {
        QString klass_name = rb_class2name(RBASIC_CLASS(self));
        if (klass_name.startsWith("Qt5::Q")) return klass_name.split("::").at(1);

        // 如果是继承类，取到需要实例化的Qt类名
        VALUE pcls = RCLASS_SUPER(RBASIC_CLASS(self));
        klass_name = rb_class2name(pcls);
        return klass_name.split("::").at(1);
    }
}

static QString get_rb_class(VALUE self)
{
    if (TYPE(self) == T_CLASS) {
        QString klass_name = rb_class2name(self);
        if (klass_name.startsWith("Qt5::Q")) return QString();
        else return klass_name;
    }
    // T_OBJECT
    else {
        QString klass_name = rb_class2name(RBASIC_CLASS(self));
        if (klass_name.startsWith("Qt5::Q")) return QString();
        else return klass_name;
    }
}

/*
  通过Qt类的初始化函数
  获取要实例化的类名，从staticMetaObject加载类信息，
  使用Qt的QMetaObject::newInstance创建新的实例对象。
  TODO:
  处理初始化时的参数。
 */
VALUE x_Qt_class_init_jit(int argc, VALUE *argv, VALUE self)
{
    qDebug()<<argc<<TYPE(self);
    qDebug()<<rb_class2name(RBASIC_CLASS(self));
    qDebug()<<"has block:"<<rb_block_given_p();

    QString klass_name = get_qt_class(self);
    qDebug()<<"class name:"<<klass_name;

    // test_fe();
    // test_parse_class();
    // test_parse_ast();
    // test_piece_compiler();
    // test_tpl_piece_compiler();
    // exit(-1);
    // assert(1==2);
    
    QVector<QVariant> args;
    for (int i = 0; i < argc; i ++) {
        // if (i == 0) continue; // for ctor 0 is arg0
        if (i >= argc) break;
        qDebug()<<"i == "<< i << (i<argc) << (i>argc);
        args << MarshallRuby::VALUE2Variant(argv[i]);
        qDebug()<<"i == "<< i << (i<argc) << (i>argc)<<MarshallRuby::VALUE2Variant(argv[i]);
    }
    
    void *jo = gce->vm_new(klass_name, args);
    qDebug()<<jo<<self<<rb_hash(self);    

    Qom::inst()->addObject(self, jo);

    VALUE free_proc = rb_proc_new(FUNVAL x_Qt_class_dtor, 0);
    rb_define_finalizer(self, free_proc);

    // Qxxx.new() do {|i| }
    if (rb_block_given_p()) {
        rb_yield(self);
    }
    
    // test slot_ruby_space
    /*
    ID rbsym = rb_intern("slot_ruby_space");
    qDebug()<<"rb func:"<<(rbsym == Qnil);
    rb_funcall(rb_cObject, rbsym, 0);
    exit(0);
    */
    
    return self;
}

/*
  通过Qt类的初始化函数
  获取要实例化的类名，从staticMetaObject加载类信息，
  使用Qt的QMetaObject::newInstance创建新的实例对象。
  TODO:
  处理初始化时的参数。
 */
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_init_jit(int argc, VALUE *argv, VALUE self)
{
    assert(1==2);
    return self;
}

[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_init(int argc, VALUE *argv, VALUE self)
{
    qDebug()<<argc<<TYPE(self);
    QString klass_name = QString(rb_class2name(RBASIC_CLASS(self)));
    klass_name = klass_name.split("::").at(1);
    QString yklass_name = QString("y%1").arg(klass_name);
    qDebug()<<"class name:"<<klass_name;
    
    // if (!__rq_metas.contains(yklass_name)) {
    //     qDebug()<<"not supported class:"<<klass_name;
    //     return Qnil;
    // }

    // const QMetaObject *mo = __rq_metas.value(yklass_name);
    // QObject * ci = mo->newInstance();
    // SAVE_CI0(ci);

    VALUE free_proc = rb_proc_new(FUNVAL x_Qt_meta_class_dtor, 0);
    rb_define_finalizer(self, free_proc);

    return self;
    return Qnil;
}


/*
  stack structure:
  [0] => SYM function name
  [1] => arg0
  [2] => arg1
  [3] => arg2
  ...
 */
VALUE x_Qt_class_method_missing_jit(int argc, VALUE *argv, VALUE self)
{
    void *jo = Qom::inst()->getObject(self);
    void *ci = jo;
    qDebug()<<ci<<argc;
    assert(ci != 0);
    QString klass_name = get_qt_class(self);
    QString rbklass_name = get_rb_class(self);
    QString method_name = QString(rb_id2name(SYM2ID(argv[0])));
    qDebug()<<"calling:"<<rbklass_name<<klass_name<<method_name<<argc<<(argc > 1);
    assert(argc >= 1);

    QVector<QVariant> args;
    args = MarshallRuby::ARGV2Variant(argc, argv, 1);
    CallArgument cargs(argc, argv, self);

    // fix try_convert(obj) → array or nil
    if (method_name == "to_ary") {
        return Qnil;
    }
    else if (method_name == "to_s") {
        return Qnil;
    }
    // for emit keyword, just as method 
    else if (method_name == "emit") {
        return argv[0];
    }
    
    // property assign
    if (method_name.endsWith('=')) {
        // TODO 需要更多的详细处理，通过类的property定义找到指定的赋值方法。
        QString set_method_name = QString("set%1%2")
            .arg(method_name.left(1).toUpper())
            .arg(method_name.mid(1, method_name.length()-2));
        gce->vm_call(ci, klass_name, set_method_name, args);
        return Qnil;
    }

    if (method_name.endsWith('?')) {
        qDebug()<<"need rubyfier adjust";
    }

    // for rbsignals
    QString signature = Qom::inst()->getSignature(rbklass_name, method_name);
    if (!signature.isEmpty()) {
        qDebug()<<"handling rbsignal..."<<signature;
        // 查找这个信号连接的所有slots列表，并依次执行。
        // QVector<Qom::RubySlot*> rbslots = Qom::inst()->getConnections(rbklass_name, method_name);
        QVector<Qom::RubySlot*> rbslots = Qom::inst()->getConnections(self, method_name);
        qDebug()<<rbslots<<rbslots.count();
        if (rbslots.count() == 0) {
            qDebug()<<"no slot connected from signal:"<<rbklass_name<<method_name<<signature;
            return Qnil;
        }
        for (int i = 0; i < rbslots.count(); i ++) {
            auto slot = rbslots.at(i);
            // run slot now
            int rargc = argc - 1;
            VALUE rargv[10] = {0};
            for (int i = 1; i < argc; i++) {
                rargv[i-1] = argv[i];
            }
            qDebug()<<"invoking..."<<slot->receiver<<slot->slot;
            VALUE fcret = rb_funcall3(slot->receiver, slot->slot, rargc, rargv);
        }
        return Qnil;
    }

    // others, call as a static member function
    qDebug()<<ci<<klass_name<<method_name<<args;
    QVariant gv = gce->vm_call(ci, klass_name, method_name, args);
    qDebug()<<"vv:"<<gv;
    // TODO 使用真实的返回值
    if (method_name == "height" || method_name == "width"
        || method_name == "x" || method_name == "y") {
        // return INT2NUM(gv.toInt());
    }
    if (gv.isValid()) {
        return gv.toULongLong();
    }
    return Qnil;
}

/*
  stack structure:
  self => CLASS
  [0] => SYM function name
  [1] => arg0
  [2] => arg1
  [3] => arg2
  ...
 */
VALUE x_Qt_class_singleton_method_missing_jit(int argc, VALUE *argv, VALUE self)
{
    qDebug()<<argc;
    QString klass_name = get_qt_class(self);
    QString rbklass_name = get_rb_class(self);
    QString method_name = QString(rb_id2name(SYM2ID(argv[0])));
    qDebug()<<"calling:"<<rbklass_name<<klass_name<<method_name<<argc<<(argc > 1);        
    
    QVector<QVariant> args;
    args = MarshallRuby::ARGV2Variant(argc, argv, 1);

    // fix try_convert(obj) → array or nil
    if (method_name == "to_ary") {
        return Qnil;
    }
    else if (method_name == "to_s") {
        return Qnil;
    }
    // 处理signals方法
    else if (method_name == "signals") {
        // qDebug()<<args;
        Qom::inst()->addSignals(rbklass_name, args);
        return Qtrue;
    }
    
    QVariant gv = gce->vm_static_call(klass_name, method_name, args);
    qDebug()<<"vv:"<<gv;
    if (gv.isValid()) {
        return gv.toULongLong();
    }
    return Qnil;
}

/*
  stack structure:
  [0] => SYM function name
  [1] => arg0
  [2] => arg1
  [3] => arg2
  ...
 */
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_method_missing_jit(int argc, VALUE *argv, VALUE self)
{
    void *jo = Qom::inst()->getObject(self);
    void *ci = jo;
    qDebug()<<ci;
    assert(ci != 0);
    QString klass_name = QString(rb_class2name(RBASIC_CLASS(self)));
    klass_name = klass_name.split("::").at(1);
    QString method_name = QString(rb_id2name(SYM2ID(argv[0])));
    qDebug()<<"calling:"<<klass_name<<method_name<<argc<<(argc > 1);
    assert(argc >= 1);

    QVector<QVariant> args;
    for (int i = 0; i < argc; i ++) {
        if (i == 0) continue;
        if (i >= argc) break;

        qDebug()<<"i == "<< i << (i<argc) << (i>argc);

        args << MarshallRuby::VALUE2Variant(argv[i]);
        qDebug()<<"i == "<< i << (i<argc) << (i>argc)<<MarshallRuby::VALUE2Variant(argv[i]);
    }

    // fix try_convert(obj) → array or nil
    if (method_name == "to_ary") {
        return Qnil;
    }

    if (method_name == "to_s") {
        return Qnil;
    }

    /*
    YaQString *ts = (YaQString*)ci;
    ts->append("1234abc");
    qDebug()<<(*ts)<<ts->toUpper()<<ts->startsWith(QChar('r'))
            <<ts->lastIndexOf("876");
    */

    QVariant gv = jit_vm_call(ci, klass_name, method_name, args);
    qDebug()<<"gv:"<<gv;

    return Qnil;
}

/*
  stack structure:
  [0] => SYM function name
  [1] => arg0
  [2] => arg1
  [3] => arg2
  ...
 */
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_method_missing(int argc, VALUE *argv, VALUE self)
{
    GET_CI0();
    qDebug()<<ci;
    assert(ci != 0);
    const QMetaObject *mo = ci->metaObject();
    QString klass_name = "QString";
    QString method_name = QString(rb_id2name(SYM2ID(argv[0])));
    qDebug()<<"calling:"<<klass_name<<method_name<<argc<<(argc > 1);
    assert(argc >= 1);

    QVector<QVariant> args;
    for (int i = 0; i < argc; i ++) {
        if (i == 0) continue;
        if (i >= argc) break;

        qDebug()<<"i == "<< i << (i<argc) << (i>argc);

        args << MarshallRuby::VALUE2Variant(argv[i]);
        qDebug()<<"i == "<< i << (i<argc) << (i>argc)<<MarshallRuby::VALUE2Variant(argv[i]);
    }

    // fix try_convert(obj) → array or nil
    if (method_name == "to_ary") {
        return Qnil;
    }

    if (method_name == "to_s") {
        return Qnil;
    }
    
    // auto meta invoke
    QString msig;
    for (QVariant &item : args) {
        msig.append(item.typeName()).append(',');
    }

    msig[msig.length()-1] = msig[msig.length()-1] == ',' ? QChar(' ') : msig[msig.length()-1];
    msig = msig.trimmed();
    msig = QString("%1(%2)").arg(method_name).arg(msig);

    int midx = mo->indexOfMethod(msig.toLatin1().data());
    if (midx == -1) {
        for (int i = 0; i < mo->methodCount(); i ++) {
            QMetaMethod mm = mo->method(i);
            qDebug()<<"mmsig:"<<mm.returnType()<<mm.typeName()<<mm.methodSignature();
            if (QString(mm.methodSignature()).replace("const ", "") == msig) {
                midx = i;
                break;
            }
        }
    } 

    if (midx == -1) {
        qDebug()<<"method not found:"<<msig;
    } else {        
        QMetaMethod mm = mo->method(midx);
        // qDebug()<<"mmsig:"<<mm.methodSignature();
        ReturnStorage rs;
        InvokeStorage is;
        QGenericReturnArgument retval;
        QGenericArgument val0, val1, val2, val3, val4, val5, val6, val7, val8, val9;
        int rargc = argc - 1;
        qDebug()<<"retun type:"<<mm.returnType()<<mm.typeName()<<mm.parameterCount()<<rargc;

        if (argc - 1 > mm.parameterCount()) {
            qDebug()<<"maybe you passed too much parameters:"
                    <<QString("need %1, given %2.").arg(mm.parameterCount()).arg(rargc);
        }

        retval = makeRetArg(mm.returnType(), rs);

        bool bret = false;
        // QString tmpv0;
        switch (rargc) {
        case 0:
            // ci->append("123");
            bret = mm.invoke(ci, retval);//Q_RETURN_ARG(int, retval)); 
            break;
        case 1:
            val0 = Variant2Arg(mm.parameterType(0), args[0], 0, is);
            bret = mm.invoke(ci, retval, val0);
            
            // tmpv0 = args[0].toString(); // 如果使用在Q_ARG中直接使用这个，这个临时对象地址会消失
            // val0 = Q_ARG(QString, tmpv0);
            // bret = QMetaObject::invokeMethod(ci, "append", val0);
            // bret = mm.invoke(ci, retval, val0);
            break;
        case 2:
            val0 = Variant2Arg(mm.parameterType(0), args[0], 0, is);
            val1 = Variant2Arg(mm.parameterType(1), args[1], 1, is);
            bret = mm.invoke(ci, retval, val0, val1);
            break;
        case 3:
            val0 = Variant2Arg(mm.parameterType(0), args[0], 0, is);
            val1 = Variant2Arg(mm.parameterType(1), args[1], 1, is);
            val2 = Variant2Arg(mm.parameterType(2), args[2], 2, is);
            bret = mm.invoke(ci, retval, val0, val1, val2);
            break;
        case 4:
            qDebug()<<"invokkkk:"<<rargc<<4;
            val0 = Variant2Arg(mm.parameterType(0), args[0], 0, is);
            val1 = Variant2Arg(mm.parameterType(1), args[1], 1, is);
            val2 = Variant2Arg(mm.parameterType(2), args[2], 2, is);
            val3 = Variant2Arg(mm.parameterType(3), args[3], 3, is);
            bret = mm.invoke(ci, retval, val0, val1, val2, val3);
            break;
        case 5:
            val0 = Variant2Arg(mm.parameterType(0), args[0], 0, is);
            val1 = Variant2Arg(mm.parameterType(1), args[1], 1, is);
            val2 = Variant2Arg(mm.parameterType(2), args[2], 2, is);
            val3 = Variant2Arg(mm.parameterType(3), args[3], 3, is);
            val4 = Variant2Arg(mm.parameterType(4), args[4], 4, is);
            bret = mm.invoke(ci, retval, val0, val1, val2, val3, val4);
            break;
        default:
            qDebug()<<"not impled"<<argc;
            break;
        };
        qDebug()<<bret<<","<<retval.name()<<retval.data();

        VALUE my_retval = retArg2Value(mm.returnType(), retval);
        return my_retval;
    }

    return Qnil;
}


// like class's static method
[[deprecated("pregen proto not needed now")]]
VALUE x_Qt_meta_class_singleton_method_missing(int argc, VALUE *argv, VALUE obj)
{
    qDebug()<<argc<<TYPE(argv[0])<<TYPE(obj);

    // const QMetaObject *mo = &yQObject::staticMetaObject;
    
    // qDebug()<<"qApp:"<<qApp;
    // for (int i = 0; i < mo->methodCount(); i++) {
    //     QMetaMethod mi = mo->method(i);
    //     qDebug()<< i<< mo->method(i).methodSignature()
    //             <<mi.returnType();
    // }

    // QString klass_name = QString(rb_class2name(obj));
    // QString const_name = QString(rb_id2name(SYM2ID(argv[0])));
    // qDebug()<<klass_name<<const_name;
    // qDebug()<<SLOT(on_emu_signal(int))<<SLOT(on_emu_signal())
    //         <<SIGNAL(emu_signal(int))<<SIGNAL(emu_signal());
    // // 1on_emu_signal(int) 1on_emu_signal()

    // yQObject *to = new yQObject;
    // if (klass_name == "Qt5::QObject") {
    //     if (const_name == "connect") {
    //         // standard connect
    //         QString slot_sig = QString("1%1").arg(RSTRING_PTR(argv[4]));
    //         QString signal_sig = QString("2%1").arg(RSTRING_PTR(argv[2]));
    //         QObject *slot_o = Qom::inst()->objs[rb_hash(argv[3])];
    //         QObject *signal_o = Qom::inst()->objs[rb_hash(argv[1])];
    //         qDebug()<<"slotooo:"<<slot_sig<<signal_sig<<slot_o<<signal_o;

    //         QObject::connect(signal_o, signal_sig.toLatin1().data(),
    //                          slot_o, slot_sig.toLatin1().data());

    //         // 
    //         // 
    //         // emit(signal_o->emu_signal(3));
    //         // another
    //     } else { // emit
    //         qDebug()<<"calling emit";
    //         const QMetaObject * mo = &yQObject::staticMetaObject;
    //         QObject *io = mo->newInstance();
    //         QObject *io2 = (decltype(io))io;
    //         qDebug()<<io<<io2;
    //     }
    // }

    return Qnil;
}


static VALUE x_QApplicatiton_init(VALUE self)
{
    int argc = 1;
    char *argv[] = {(char*)"handby"};
    QApplication *app = new QApplication(argc, argv);

    SAVE_CI2(QApplication, app);

    return self;
}

static VALUE x_QApplication_exec(VALUE self)
{
    GET_CI2(QApplication);

    int n = ci->exec();

    return n;
}

// for qApp 等全局变量
VALUE x_Qt_global_variable_get(ID id, VALUE *data, struct global_entry *entry)
{
    // qDebug()<<id<<data<<entry;
    // qDebug()<<rb_id2name(id)<<qApp;
    QString vname = rb_id2name(id);

    if (vname == "$qApp") {
        void *v = qApp;
        if (v == NULL) return Qnil;
        // TODO 提高查找效率。
        return Qom::inst()->getObject(v);
    }
    qDebug()<<"undef variable:"<<vname;
    return INT2NUM(123456);
    return Qnil;
}

void x_Qt_global_variable_set(VALUE value, ID id, VALUE *data, struct global_entry *entry)
{
    qDebug()<<TYPE(value)<<id<<data<<entry;
    return;
}

static VALUE x_Qt_Constant_missing(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<"hehhe constant missing."<<argc<<TYPE(self);

    QString vspace = "Qt";
    QString vname;
    
    // assert(argc == 1);
    // assert(TYPE(self) == T_SYMBOL)
    for (int i = 0; i < argc; i++) {
        qDebug()<<__FUNCTION__<<TYPE(argv[i]);
        switch (TYPE(argv[i])) {
        case T_STRING:
            qDebug()<<i<<"is string:"<<RSTRING_PTR(argv[i]);
            vname = RSTRING_PTR(argv[i]);
            break;
        case T_SYMBOL:
            qDebug()<<i<<"is symbol:"<<rb_id2name(SYM2ID(argv[i]));
            vname = rb_id2name(SYM2ID(argv[i]));
            break;
        case T_FIXNUM:
            qDebug()<<i<<"is num:"<<FIX2INT(argv[i]);
            break;
        default:
            qDebug()<<__FUNCTION__<<i<<",unknown arg type:"<<TYPE(argv[i]);
            break;
        }
    }

    // 可能是还没注册的类
    if (vname.at(0).toLatin1() == 'Q' && vname.at(1).isUpper()) {
        // TODO confirm it's a qt class, like by gce->hasClass();
        QtRegister::inst()->registClass(vname);
        VALUE cQtClass = QtRegister::inst()->getClass(vname);
        assert(cQtClass != Qnil);
        return cQtClass;
    }

    // vm exec
    int val = gce->vm_enum(vspace, vname);
    qDebug()<<"enum..."<<val;

    return INT2NUM(val);
    return Qnil;
}

// from qtobjectmanager.h
void ConnectProxy::proxycall()
{
    qDebug()<<"aaaaaaaa"<<sender()<<senderSignalIndex();
    qDebug()<<this->osender<<this->osignal<<this->oreceiver<<this->oslot;
    qDebug()<<this->argc<<this->argv<<this->self;
    // ID slot = rb_intern("slot_ruby_space");
    if (this->argc == 4) {
        ID slot = rb_intern(this->oslot.toLatin1().data());
        rb_funcall(rb_cObject, slot, 0);
    }
    else if (this->argc == 5) {
        qDebug()<<"called....";
        ID slot = rb_intern(this->oslot.toLatin1().data());
        rb_funcall(this->oreceiver, slot, 0);
    }
    else {
        qDebug()<<"unimpled.....";
    }
}

void ConnectProxy::proxycall(int arg0)
{
    qDebug()<<"aaaaaaaa"<<sender();
    ID slot = rb_intern("slot_ruby_space");
    rb_funcall(rb_cObject, slot, 0);
}

void ConnectProxy::proxycall(char arg0)
{
    qDebug()<<"aaaaaaaa"<<sender();
    ID slot = rb_intern("slot_ruby_space");
    rb_funcall(rb_cObject, slot, 0);
}

void ConnectProxy::proxycall(void * arg0)
{
    qDebug()<<"aaaaaaaa"<<sender();
    ID slot = rb_intern("slot_ruby_space");
    rb_funcall(rb_cObject, slot, 0);
}

static ConnectProxy gcp;
// for qt signal ==> rb slot
static VALUE x_Qt_connectrb(int argc, VALUE* argv, VALUE self)
{
    if (argc == 4) {
        QVariant vqobj = MarshallRuby::VALUE2Variant(argv[1]);
        QVariant vsignal = MarshallRuby::VALUE2Variant(argv[2]);
        QString signal = vsignal.toString();
        QString rsignal = QString("2%1").arg(signal); // real signal
        auto qobj = (QObject*)(vqobj.value<void*>());
        auto spec_qobj = (QTimer*)(vqobj.value<void*>()); // 物化对象
    
        // QObject::connect(qobj, SIGNAL(timeout()), qobj, SLOT(stop())); // ok
        // QObject::connect(qobj, SIGNAL(timeout()), [](){}); // error
        // auto conn = QObject::connect(qobj, &QTimer::timeout, // vsignal.toString().toLatin1().data(),
        //                              [=]() {
        //                                  qDebug()<<argc<<argv<<self;
        //                                  ID slot = rb_intern("slot_ruby_space");
        //                                  rb_funcall(rb_cObject, slot, 0);
        //                              });

        // TODO 什么时候free掉这个对象？？？
        ConnectProxy *connpxy = new ConnectProxy();
        connpxy->argc = argc;
        connpxy->argv = (RB_VALUE*)argv;
        connpxy->self = self;

        connpxy->osender = qobj;
        connpxy->osignal = rsignal;
        connpxy->oreceiver = (RB_VALUE)NULL;
        connpxy->oslot = rb_id2name(SYM2ID(argv[3]));

        QString slot = QString("1proxycall%1")
            .arg(signal.right(signal.length() - signal.indexOf('(')));
        qDebug()<<"connecting singal/slot........"<<signal.indexOf('(')<<slot;
        auto conn = QObject::connect(qobj, rsignal.toLatin1().data(), connpxy, slot.toLatin1().data()); // ok
        auto cid = connpxy->addConnection(conn);
    }
    else if (argc == 5) {
        QVariant vqobj = MarshallRuby::VALUE2Variant(argv[1]);
        QVariant vsignal = MarshallRuby::VALUE2Variant(argv[2]);
        QString signal = vsignal.toString();
        QString rsignal = QString("2%1").arg(signal); // real signal
        auto qobj = (QObject*)(vqobj.value<void*>());
        auto spec_qobj = (QTimer*)(vqobj.value<void*>()); // 物化对象
    
        // QObject::connect(qobj, SIGNAL(timeout()), qobj, SLOT(stop())); // ok
        // QObject::connect(qobj, SIGNAL(timeout()), [](){}); // error
        // auto conn = QObject::connect(qobj, &QTimer::timeout, // vsignal.toString().toLatin1().data(),
        //                              [=]() {
        //                                  qDebug()<<argc<<argv<<self;
        //                                  ID slot = rb_intern("slot_ruby_space");
        //                                  rb_funcall(rb_cObject, slot, 0);
        //                              });

        // TODO 什么时候free掉这个对象？？？
        ConnectProxy *connpxy = new ConnectProxy();
        connpxy->argc = argc;
        connpxy->argv = (RB_VALUE*)argv;
        connpxy->self = self;

        connpxy->osender = qobj;
        connpxy->osignal = rsignal;
        connpxy->oreceiver = argv[3];
        connpxy->oslot = rb_id2name(SYM2ID(argv[4]));

        QString slot = QString("1proxycall%1")
            .arg(signal.right(signal.length() - signal.indexOf('(')));
        qDebug()<<"connecting singal/slot........"<<signal.indexOf('(')<<slot;
        auto conn = QObject::connect(qobj, rsignal.toLatin1().data(), connpxy, slot.toLatin1().data()); // ok
        auto cid = connpxy->addConnection(conn);
    } else {
        qDebug()<<"unimpled...!!!";
    }
    
    return Qnil;
}

// for rb signal ==> rb slot
// signature1 Qt5::rbconnectrb(sobj, signal, robj, slot);
// signature2 Qt5::rbconnectrb(sobj, signal, Proc|Block|Lambda)
// TODO signature3 robj.rbconnectrb(sobj, signal, slot)
static VALUE x_Qt_rbconnectrb(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<argc;
    QString klass_name = get_rb_class(argv[1]);
    QString method_signature = RSTRING_PTR(argv[2]);
    QString signal_name = method_signature.split('(').at(0);

    if (argc < 3 || argc > 5) {
        qDebug()<<rb_block_given_p();
        qDebug()<<"parameter not match. need 3 to 5, given:"<<argc;
        return Qfalse;
    }

    auto add_connection =
        [](QString klass_name, QString signal_name,
           VALUE s, VALUE signal, VALUE r, VALUE slot, ID sid) -> bool {
        Qom::RubySlot *conn = new Qom::RubySlot();
        conn->sender = s;
        conn->vsignal = signal;
        conn->receiver = r;
        conn->vslot = slot;
        conn->slot = sid;

        Qom::inst()->addConnection(klass_name, signal_name, conn);
        Qom::inst()->addConnection(s, signal_name, conn);
        return true;
    };

    // signature 2, block
    if (argc == 3 && rb_block_given_p()) {
        VALUE pblk = rb_block_proc(); // ok 
        VALUE lblk = rb_block_lambda(); // ok

        ID cid = rb_intern("call");
        VALUE csym = ID2SYM(cid);
        VALUE mth = rb_obj_method(lblk, csym);

        add_connection(klass_name, signal_name, argv[1], argv[2], lblk, mth, cid);
        /*
        Qom::RubySlot *conn = new Qom::RubySlot();
        conn->sender = argv[1];
        conn->vsignal = argv[2];
        conn->receiver = lblk;
        conn->vslot = mth;
        conn->slot = cid;

        Qom::inst()->addConnection(klass_name, signal_name, conn);
        Qom::inst()->addConnection(argv[1], signal_name, conn);
        */
        return Qtrue;
    } else if (argc == 3 && !rb_block_given_p()) {
        qDebug()<<"need a block here.";
        return Qfalse;
    }
    
    // signature 2, proc,lambda
    // qDebug()<<argc<<TYPE(argv[3])<<(TYPE(argv[3]) == T_DATA);
    // qDebug()<<RTYPEDDATA_P(argv[3])<<(RBASIC_CLASS(argv[3]) == rb_cProc);
    else if (argc == 4 && RBASIC_CLASS(argv[3]) == rb_cProc) {
        ID cid = rb_intern("call");
        VALUE csym = ID2SYM(cid);
        VALUE mth = rb_obj_method(argv[3], csym);

        add_connection(klass_name, signal_name, argv[1], argv[2], argv[3], mth, cid);
        /*
        Qom::RubySlot *conn = new Qom::RubySlot();
        conn->sender = argv[1];
        conn->vsignal = argv[2];
        conn->receiver = argv[3];
        conn->vslot = mth;
        conn->slot = cid;

        Qom::inst()->addConnection(klass_name, signal_name, conn);
        Qom::inst()->addConnection(argv[1], signal_name, conn);
        */
        return Qtrue;
    } else if (argc == 4 && RBASIC_CLASS(argv[3]) != rb_cProc) {
        qDebug()<<"need a proc or lambda here.";
        return Qfalse;
    }

    // else, standard signature 1
    QString slot_signature = RSTRING_PTR(argv[4]);
    QString slot_name = slot_signature.split('(').at(0);
    qDebug()<<klass_name<<method_signature<<signal_name;

    // ID mid = rb_intern("onchange");
    ID mid = rb_intern(slot_name.toLatin1().data());
    VALUE msym = ID2SYM(mid);

    // it's Module.method_definied?
    auto rb_mod_method_defined = [](VALUE mod, VALUE mid) -> VALUE {
        ID id = rb_check_id(&mid);
        if (!id || !rb_method_boundp(mod, id, 1)) {
            return Qfalse;
        }
        return Qtrue;
    };

    VALUE ok = rb_mod_method_defined(RBASIC_CLASS(argv[1]), msym);
    // qDebug()<<ok<<(Qtrue == ok);
    if (ok == Qfalse) return Qfalse;

    add_connection(klass_name, signal_name, argv[1], argv[2], argv[3], argv[4], mid);
    /*
    Qom::RubySlot *conn = new Qom::RubySlot();
    conn->sender = argv[1];
    conn->vsignal = argv[2];
    conn->receiver = argv[3];
    conn->vslot = argv[4];
    conn->slot = mid;

    Qom::inst()->addConnection(klass_name, signal_name, conn);
    Qom::inst()->addConnection(argv[1], signal_name, conn);
    */
    
    return Qtrue;
}

// for rb signal ==> rb slot
// Qt5::rbdisconnectrb(sobj, signal, robj, slot);
// Qt5::rbdisconnectrb(sobj, signal, Proc|Block|Lambda);
// TODO sobj.rbdisconnectrb(signal, robj, slot);
// TODO sobj.rbdisconnectrb(robj, slot);
static VALUE x_Qt_rbdisconnectrb(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<argc;
    QString klass_name = get_rb_class(argv[1]);
    QString method_signature = RSTRING_PTR(argv[2]);
    QString signal_name = method_signature.split('(').at(0);
    QString slot_signature = RSTRING_PTR(argv[4]);
    QString slot_name = slot_signature.split('(').at(0);
    qDebug()<<klass_name<<method_signature<<signal_name;

    // ID mid = rb_intern("onchange");
    ID mid = rb_intern(slot_name.toLatin1().data());
    VALUE msym = ID2SYM(mid);

    // it's Module.method_definied?
    auto rb_mod_method_defined = [](VALUE mod, VALUE mid) -> VALUE {
        ID id = rb_check_id(&mid);
        if (!id || !rb_method_boundp(mod, id, 1)) {
            return Qfalse;
        }
        return Qtrue;
    };

    VALUE ok = rb_mod_method_defined(RBASIC_CLASS(argv[1]), msym);
    qDebug()<<ok<<(Qtrue == ok);

    if (ok == Qfalse) return Qfalse;

    Qom::RubySlot *conn = new Qom::RubySlot();
    conn->sender = argv[1];
    conn->vsignal = argv[2];
    conn->receiver = argv[3];
    conn->vslot = argv[4];
    conn->slot = mid;

    bool bret = Qom::inst()->removeConnection(argv[1], signal_name, conn);
    delete conn;
    return bret ? Qtrue : Qfalse;
    return Qnil;
}

// for rb signal ==> qt slot
static VALUE x_Qt_rbconnect(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<argc;
    return Qnil;
}

// for qt signal ==> qt slot
static VALUE x_Qt_connect(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<argc;
    return Qnil;
}

static VALUE x_Qt_Method_missing(int argc, VALUE* argv, VALUE self)
{
    qDebug()<<"hehhe method missing."<< argc << argv << self;
    qDebug()<<argc<<TYPE(self); // == 3(T_MODULE)
    // RSTRING_PTR(argv[0]);
    for (int i = 0; i < argc; i ++) {
        qDebug()<<i<<TYPE(argv[i])<<MarshallRuby::VALUE2Variant(argv[i]);
    }
    // argv[0] is method symbol
    // argv[1] is method arg0
    // argv[2] is method arg1
    // ...
    QString method_name = rb_id2name(SYM2ID(argv[0]));
    qDebug()<<method_name;
    
    // Qt5::connectrb    
    if (method_name == "connectrb") {
        qDebug()<<"got ittttttt."<<method_name;
        return x_Qt_connectrb(argc, argv, self);
    } 
    // Qt5::rbconnectrb
    else if (method_name == "rbconnectrb") {
        qDebug()<<"got ittttttt."<<method_name;
        return x_Qt_rbconnectrb(argc, argv, self);
    }
    // Qt5::rbdisconnectrb
    else if (method_name == "rbdisconnectrb") {
        qDebug()<<"got ittttttt."<<method_name;
        return x_Qt_rbdisconnectrb(argc, argv, self);
    }
    // Qt5::rbconnect    
    else if (method_name == "rbconnect") {
        qDebug()<<"got ittttttt."<<method_name;
        return x_Qt_rbconnect(argc, argv, self);
    }
    // Qt5::connect
    else if (method_name == "connect") {
        qDebug()<<"got ittttttt."<<method_name;
        return x_Qt_connect(argc, argv, self);        
    }

    qDebug()<<"unimpled!!!";
    
    return Qnil;
}


extern "C" {
    // VALUE cQString;
    // VALUE cQApplication;
    VALUE cModuleQt;
    
    void Init_handby000()
    {
        qInstallMessageHandler(myMessageOutput);

        ///// test code
        // test_one();
        // exit(0);

        cModuleQt = rb_define_module("Qt5");
        QtRegister::inst()->setModuleQt(cModuleQt);

        // 对所有的Qt5::someconst常量的调用注册
        rb_define_module_function(cModuleQt, "const_missing", FUNVAL x_Qt_Constant_missing, -1);
        // 对所有的Qt5::somefunc()函数的调用注册
        rb_define_module_function(cModuleQt, "method_missing", FUNVAL x_Qt_Method_missing, -1);

        // 所有Qt类的initialize, method_missing, const_missing函数注册
        register_qtruby_classes(cModuleQt);
    }

};// end extern "C"



