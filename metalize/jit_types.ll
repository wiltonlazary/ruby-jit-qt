; ModuleID = 'metalize/jit_types.cpp'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

%class.YaQAccessible = type { i8 }
%class.QAccessible = type { i8 }
%class.YaQAction = type { %class.QAction }
%class.QAction = type { %class.QObject }
%class.QObject = type { i32 (...)**, %class.QScopedPointer }
%class.QScopedPointer = type { %class.QObjectData* }
%class.QObjectData = type { i32 (...)**, %class.QObject*, %class.QObject*, %class.QList, i32, i32, %struct.QDynamicMetaObjectData* }
%class.QList = type { %union.anon }
%union.anon = type { %struct.QListData }
%struct.QListData = type { %"struct.QListData::Data"* }
%"struct.QListData::Data" = type { %"class.QtPrivate::RefCount", i32, i32, i32, [1 x i8*] }
%"class.QtPrivate::RefCount" = type { %class.QBasicAtomicInteger }
%class.QBasicAtomicInteger = type { i32 }
%struct.QDynamicMetaObjectData = type opaque
%class.YaQActionGroup = type { %class.QActionGroup }
%class.QActionGroup = type { %class.QObject }
%class.YaQObject = type { %class.QObject }
%class.YaQString = type { %class.QString }
%class.QString = type { %struct.QTypedArrayData* }
%struct.QTypedArrayData = type { %struct.QArrayData }
%struct.QArrayData = type { %"class.QtPrivate::RefCount", i32, i32, i32 }
%class.YaQBitArray = type { %class.QBitArray }
%class.QBitArray = type { %class.QByteArray }
%class.QByteArray = type { %struct.QTypedArrayData.0* }
%struct.QTypedArrayData.0 = type { %struct.QArrayData }
%class.YaQBitmap = type { %class.QBitmap }
%class.QBitmap = type { %class.QPixmap }
%class.QPixmap = type { %class.QPaintDevice, %class.QExplicitlySharedDataPointer }
%class.QPaintDevice = type { i32 (...)**, i16, %class.QPaintDevicePrivate* }
%class.QPaintDevicePrivate = type opaque
%class.QExplicitlySharedDataPointer = type { %class.QPlatformPixmap* }
%class.QPlatformPixmap = type opaque
%class.YaQBuffer = type { %class.QBuffer }
%class.QBuffer = type { %class.QIODevice }
%class.QIODevice = type { %class.QObject }
%class.YaQByteArray = type { %class.QByteArray }
%class.YaQThread = type { %class.QThread }
%class.QThread = type { %class.QObject }
%class.YaQCoreApplication = type { %class.QCoreApplication }
%class.QCoreApplication = type { %class.QObject }
%class.YaQChar = type { %class.QChar }
%class.QChar = type { i16 }
%class.YaQStringList = type { %class.QStringList }
%class.QStringList = type { %class.QList.1 }
%class.QList.1 = type { %union.anon.2 }
%union.anon.2 = type { %struct.QListData }
%class.YaQSize = type { %class.QSize }
%class.QSize = type { i32, i32 }
%class.YaQTimer = type { %class.QTimer.base, [3 x i8] }
%class.QTimer.base = type <{ %class.QObject, i32, i32, i32, i8 }>
%class.QTimer = type { %class.QObject, i32, i32, i32, i8 }
%class.YaQDateTime = type { %class.QDateTime }
%class.QDateTime = type { %class.QExplicitlySharedDataPointer.3 }
%class.QExplicitlySharedDataPointer.3 = type { %class.QDateTimePrivate* }
%class.QDateTimePrivate = type opaque
%class.YaQUrl = type { %class.QUrl }
%class.QUrl = type { %class.QUrlPrivate* }
%class.QUrlPrivate = type opaque
%class.YaQFile = type { %class.QFile }
%class.QFile = type { %class.QFileDevice }
%class.QFileDevice = type { %class.QIODevice }
%class.YaQRegExp = type { %class.QRegExp }
%class.QRegExp = type { %struct.QRegExpPrivate* }
%struct.QRegExpPrivate = type opaque
%class.YaQHostAddress = type { %class.QHostAddress }
%class.QHostAddress = type { %class.QScopedPointer.4 }
%class.QScopedPointer.4 = type { %class.QHostAddressPrivate* }
%class.QHostAddressPrivate = type opaque
%class.YaQAbstractSocket = type { %class.QAbstractSocket }
%class.QAbstractSocket = type { %class.QIODevice }
%class.YaQTcpSocket = type { %class.QTcpSocket }
%class.QTcpSocket = type { %class.QAbstractSocket }
%class.YaQTcpServer = type { %class.QTcpServer }
%class.QTcpServer = type { %class.QObject }
%class.YaQSslSocket = type { %class.QSslSocket }
%class.QSslSocket = type { %class.QTcpSocket }
%class.YaQSslCipher = type { %class.QSslCipher }
%class.QSslCipher = type { %class.QScopedPointer.5 }
%class.QScopedPointer.5 = type { %class.QSslCipherPrivate* }
%class.QSslCipherPrivate = type opaque
%class.YaQSslConfiguration = type { %class.QSslConfiguration }
%class.QSslConfiguration = type { %class.QSharedDataPointer }
%class.QSharedDataPointer = type { %class.QSslConfigurationPrivate* }
%class.QSslConfigurationPrivate = type opaque
%class.YaQSslError = type { %class.QSslError }
%class.QSslError = type { %class.QScopedPointer.6 }
%class.QScopedPointer.6 = type { %class.QSslErrorPrivate* }
%class.QSslErrorPrivate = type opaque
%class.YaQSslKey = type { %class.QSslKey }
%class.QSslKey = type { %class.QExplicitlySharedDataPointer.7 }
%class.QExplicitlySharedDataPointer.7 = type { %class.QSslKeyPrivate* }
%class.QSslKeyPrivate = type opaque
%class.YaQNetworkCookie = type { %class.QNetworkCookie }
%class.QNetworkCookie = type { %class.QSharedDataPointer.8 }
%class.QSharedDataPointer.8 = type { %class.QNetworkCookiePrivate* }
%class.QNetworkCookiePrivate = type opaque
%class.YaQNetworkCookieJar = type { %class.QNetworkCookieJar }
%class.QNetworkCookieJar = type { %class.QObject }
%class.YaQNetworkAccessManager = type { %class.QNetworkAccessManager }
%class.QNetworkAccessManager = type { %class.QObject }
%class.YaQNetworkRequest = type { %class.QNetworkRequest }
%class.QNetworkRequest = type { %class.QSharedDataPointer.9 }
%class.QSharedDataPointer.9 = type { %class.QNetworkRequestPrivate* }
%class.QNetworkRequestPrivate = type opaque
%class.YaQNetworkReply = type { %class.QNetworkReply }
%class.QNetworkReply = type { %class.QIODevice }
%class.YaQGuiApplication = type { %class.QGuiApplication }
%class.QGuiApplication = type { %class.QCoreApplication }
%class.YaQIcon = type { %class.QIcon }
%class.QIcon = type { %class.QIconPrivate* }
%class.QIconPrivate = type opaque
%class.YaQFont = type { %class.QFont }
%class.QFont = type { %class.QExplicitlySharedDataPointer.10, i32 }
%class.QExplicitlySharedDataPointer.10 = type { %class.QFontPrivate* }
%class.QFontPrivate = type opaque
%class.YaQPaintDevice = type { %class.QPaintDevice }
%class.YaQWidget = type { %class.QWidget }
%class.QWidget = type { %class.QObject, %class.QPaintDevice, %class.QWidgetData* }
%class.QWidgetData = type { i32, i32, %class.QFlags, i32, %class.QRect, %class.QPalette, %class.QFont, %class.QRect }
%class.QFlags = type { i32 }
%class.QPalette = type { %class.QPalettePrivate*, %union.anon.11 }
%class.QPalettePrivate = type opaque
%union.anon.11 = type { %"struct.QPalette::Data" }
%"struct.QPalette::Data" = type { i32 }
%class.QRect = type { i32, i32, i32, i32 }
%class.YaQMainWindow = type { %class.QMainWindow }
%class.QMainWindow = type { %class.QWidget }
%class.YaQPushButton = type { %class.QPushButton }
%class.QPushButton = type { %class.QAbstractButton }
%class.QAbstractButton = type { %class.QWidget }
%class.YaQLCDNumber = type { %class.QLCDNumber }
%class.QLCDNumber = type { %class.QFrame }
%class.QFrame = type { %class.QWidget }
%class.YaQSlider = type { %class.QSlider }
%class.QSlider = type { %class.QAbstractSlider }
%class.QAbstractSlider = type { %class.QWidget }
%class.YaQLayout = type { %class.QLayout }
%class.QLayout = type { %class.QObject, %class.QLayoutItem }
%class.QLayoutItem = type { i32 (...)**, %class.QFlags.12 }
%class.QFlags.12 = type { i32 }
%class.YaQBoxLayout = type { %class.QBoxLayout }
%class.QBoxLayout = type { %class.QLayout }
%class.QHBoxLayout = type { %class.QBoxLayout }
%class.QVBoxLayout = type { %class.QBoxLayout }
%class.YaQGridLayout = type { %class.QGridLayout }
%class.QGridLayout = type { %class.QLayout }
%class.YaQApplication = type { %class.QApplication }
%class.QApplication = type { %class.QGuiApplication }

; Function Attrs: nounwind
define void @_Z16__keep_jit_typesv() #0 {
  %v0 = alloca i8*, align 4
  store i8* null, i8** %v0, align 4
  %1 = load i8** %v0, align 4
  %2 = bitcast i8* %1 to %class.YaQAccessible*
  %3 = load i8** %v0, align 4
  %4 = bitcast i8* %3 to %class.QAccessible*
  %5 = load i8** %v0, align 4
  %6 = bitcast i8* %5 to %class.YaQAction*
  %7 = load i8** %v0, align 4
  %8 = bitcast i8* %7 to %class.QAction*
  %9 = load i8** %v0, align 4
  %10 = bitcast i8* %9 to %class.YaQActionGroup*
  %11 = load i8** %v0, align 4
  %12 = bitcast i8* %11 to %class.QActionGroup*
  %13 = load i8** %v0, align 4
  %14 = bitcast i8* %13 to %class.YaQObject*
  %15 = load i8** %v0, align 4
  %16 = bitcast i8* %15 to %class.QObject*
  %17 = load i8** %v0, align 4
  %18 = bitcast i8* %17 to %class.YaQString*
  %19 = load i8** %v0, align 4
  %20 = bitcast i8* %19 to %class.QString*
  %21 = load i8** %v0, align 4
  %22 = bitcast i8* %21 to %class.YaQBitArray*
  %23 = load i8** %v0, align 4
  %24 = bitcast i8* %23 to %class.QBitArray*
  %25 = load i8** %v0, align 4
  %26 = bitcast i8* %25 to %class.YaQBitmap*
  %27 = load i8** %v0, align 4
  %28 = bitcast i8* %27 to %class.QBitmap*
  %29 = load i8** %v0, align 4
  %30 = bitcast i8* %29 to %class.YaQBuffer*
  %31 = load i8** %v0, align 4
  %32 = bitcast i8* %31 to %class.QBuffer*
  %33 = load i8** %v0, align 4
  %34 = bitcast i8* %33 to %class.YaQByteArray*
  %35 = load i8** %v0, align 4
  %36 = bitcast i8* %35 to %class.QByteArray*
  %37 = load i8** %v0, align 4
  %38 = bitcast i8* %37 to %class.YaQThread*
  %39 = load i8** %v0, align 4
  %40 = bitcast i8* %39 to %class.QThread*
  %41 = load i8** %v0, align 4
  %42 = bitcast i8* %41 to %class.YaQCoreApplication*
  %43 = load i8** %v0, align 4
  %44 = bitcast i8* %43 to %class.QCoreApplication*
  %45 = load i8** %v0, align 4
  %46 = bitcast i8* %45 to %class.YaQChar*
  %47 = load i8** %v0, align 4
  %48 = bitcast i8* %47 to %class.QChar*
  %49 = load i8** %v0, align 4
  %50 = bitcast i8* %49 to %class.YaQStringList*
  %51 = load i8** %v0, align 4
  %52 = bitcast i8* %51 to %class.QStringList*
  %53 = load i8** %v0, align 4
  %54 = bitcast i8* %53 to %class.YaQSize*
  %55 = load i8** %v0, align 4
  %56 = bitcast i8* %55 to %class.QSize*
  %57 = load i8** %v0, align 4
  %58 = bitcast i8* %57 to %class.YaQTimer*
  %59 = load i8** %v0, align 4
  %60 = bitcast i8* %59 to %class.QTimer*
  %61 = load i8** %v0, align 4
  %62 = bitcast i8* %61 to %class.YaQDateTime*
  %63 = load i8** %v0, align 4
  %64 = bitcast i8* %63 to %class.QDateTime*
  %65 = load i8** %v0, align 4
  %66 = bitcast i8* %65 to %class.YaQUrl*
  %67 = load i8** %v0, align 4
  %68 = bitcast i8* %67 to %class.QUrl*
  %69 = load i8** %v0, align 4
  %70 = bitcast i8* %69 to %class.YaQFile*
  %71 = load i8** %v0, align 4
  %72 = bitcast i8* %71 to %class.QFile*
  %73 = load i8** %v0, align 4
  %74 = bitcast i8* %73 to %class.YaQRegExp*
  %75 = load i8** %v0, align 4
  %76 = bitcast i8* %75 to %class.QRegExp*
  %77 = load i8** %v0, align 4
  %78 = bitcast i8* %77 to %class.YaQHostAddress*
  %79 = load i8** %v0, align 4
  %80 = bitcast i8* %79 to %class.QHostAddress*
  %81 = load i8** %v0, align 4
  %82 = bitcast i8* %81 to %class.YaQAbstractSocket*
  %83 = load i8** %v0, align 4
  %84 = bitcast i8* %83 to %class.QAbstractSocket*
  %85 = load i8** %v0, align 4
  %86 = bitcast i8* %85 to %class.YaQTcpSocket*
  %87 = load i8** %v0, align 4
  %88 = bitcast i8* %87 to %class.QTcpSocket*
  %89 = load i8** %v0, align 4
  %90 = bitcast i8* %89 to %class.YaQTcpServer*
  %91 = load i8** %v0, align 4
  %92 = bitcast i8* %91 to %class.QTcpServer*
  %93 = load i8** %v0, align 4
  %94 = bitcast i8* %93 to %class.YaQSslSocket*
  %95 = load i8** %v0, align 4
  %96 = bitcast i8* %95 to %class.QSslSocket*
  %97 = load i8** %v0, align 4
  %98 = bitcast i8* %97 to %class.YaQSslCipher*
  %99 = load i8** %v0, align 4
  %100 = bitcast i8* %99 to %class.QSslCipher*
  %101 = load i8** %v0, align 4
  %102 = bitcast i8* %101 to %class.YaQSslConfiguration*
  %103 = load i8** %v0, align 4
  %104 = bitcast i8* %103 to %class.QSslConfiguration*
  %105 = load i8** %v0, align 4
  %106 = bitcast i8* %105 to %class.YaQSslError*
  %107 = load i8** %v0, align 4
  %108 = bitcast i8* %107 to %class.QSslError*
  %109 = load i8** %v0, align 4
  %110 = bitcast i8* %109 to %class.YaQSslKey*
  %111 = load i8** %v0, align 4
  %112 = bitcast i8* %111 to %class.QSslKey*
  %113 = load i8** %v0, align 4
  %114 = bitcast i8* %113 to %class.YaQNetworkCookie*
  %115 = load i8** %v0, align 4
  %116 = bitcast i8* %115 to %class.QNetworkCookie*
  %117 = load i8** %v0, align 4
  %118 = bitcast i8* %117 to %class.YaQNetworkCookieJar*
  %119 = load i8** %v0, align 4
  %120 = bitcast i8* %119 to %class.QNetworkCookieJar*
  %121 = load i8** %v0, align 4
  %122 = bitcast i8* %121 to %class.YaQNetworkAccessManager*
  %123 = load i8** %v0, align 4
  %124 = bitcast i8* %123 to %class.QNetworkAccessManager*
  %125 = load i8** %v0, align 4
  %126 = bitcast i8* %125 to %class.YaQNetworkRequest*
  %127 = load i8** %v0, align 4
  %128 = bitcast i8* %127 to %class.QNetworkRequest*
  %129 = load i8** %v0, align 4
  %130 = bitcast i8* %129 to %class.YaQNetworkReply*
  %131 = load i8** %v0, align 4
  %132 = bitcast i8* %131 to %class.QNetworkReply*
  %133 = load i8** %v0, align 4
  %134 = bitcast i8* %133 to %class.YaQGuiApplication*
  %135 = load i8** %v0, align 4
  %136 = bitcast i8* %135 to %class.QGuiApplication*
  %137 = load i8** %v0, align 4
  %138 = bitcast i8* %137 to %class.YaQIcon*
  %139 = load i8** %v0, align 4
  %140 = bitcast i8* %139 to %class.QIcon*
  %141 = load i8** %v0, align 4
  %142 = bitcast i8* %141 to %class.YaQFont*
  %143 = load i8** %v0, align 4
  %144 = bitcast i8* %143 to %class.QFont*
  %145 = load i8** %v0, align 4
  %146 = bitcast i8* %145 to %class.YaQPaintDevice*
  %147 = load i8** %v0, align 4
  %148 = bitcast i8* %147 to %class.QPaintDevice*
  %149 = load i8** %v0, align 4
  %150 = bitcast i8* %149 to %class.YaQWidget*
  %151 = load i8** %v0, align 4
  %152 = bitcast i8* %151 to %class.QWidget*
  %153 = load i8** %v0, align 4
  %154 = bitcast i8* %153 to %class.YaQMainWindow*
  %155 = load i8** %v0, align 4
  %156 = bitcast i8* %155 to %class.QMainWindow*
  %157 = load i8** %v0, align 4
  %158 = bitcast i8* %157 to %class.YaQPushButton*
  %159 = load i8** %v0, align 4
  %160 = bitcast i8* %159 to %class.QPushButton*
  %161 = load i8** %v0, align 4
  %162 = bitcast i8* %161 to %class.YaQLCDNumber*
  %163 = load i8** %v0, align 4
  %164 = bitcast i8* %163 to %class.QLCDNumber*
  %165 = load i8** %v0, align 4
  %166 = bitcast i8* %165 to %class.YaQSlider*
  %167 = load i8** %v0, align 4
  %168 = bitcast i8* %167 to %class.QSlider*
  %169 = load i8** %v0, align 4
  %170 = bitcast i8* %169 to %class.YaQLayout*
  %171 = load i8** %v0, align 4
  %172 = bitcast i8* %171 to %class.QLayout*
  %173 = load i8** %v0, align 4
  %174 = bitcast i8* %173 to %class.YaQBoxLayout*
  %175 = load i8** %v0, align 4
  %176 = bitcast i8* %175 to %class.QBoxLayout*
  %177 = load i8** %v0, align 4
  %178 = bitcast i8* %177 to %class.QHBoxLayout*
  %179 = load i8** %v0, align 4
  %180 = bitcast i8* %179 to %class.QVBoxLayout*
  %181 = load i8** %v0, align 4
  %182 = bitcast i8* %181 to %class.YaQGridLayout*
  %183 = load i8** %v0, align 4
  %184 = bitcast i8* %183 to %class.QGridLayout*
  %185 = load i8** %v0, align 4
  %186 = bitcast i8* %185 to %class.YaQApplication*
  %187 = load i8** %v0, align 4
  %188 = bitcast i8* %187 to %class.QApplication*
  ret void
}

; Function Attrs: nounwind
define void @_Z31__keep_jit_cannot_gen_functionsv() #0 {
  ret void
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.5.0 (tags/RELEASE_350/final)"}
