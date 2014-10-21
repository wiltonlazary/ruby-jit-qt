#ifndef INVOKESTORAGE_H
#define INVOKESTORAGE_H

typedef struct {
    int iretval;
    bool bretval;
    QString sretval;
    QChar cretval;

    QString *sbyvalret; // by value return result

    // 最大支持10个参数，参数的临时值放在这
    int ival[10];
    bool bval[10];
    QString sval[10];
    QChar cval[10];
} InvokeStorage;
// TODO 多线程支持
// InvokeStorage gis;



#endif /* INVOKESTORAGE_H */
