#ifndef _NULLSPACE_MACRO_1588900396612_H
#define _NULLSPACE_MACRO_1588900396612_H
#include <QtQml/qqml.h>

#define QML_INTERFACE(x, scope, major, minor) qmlRegisterType<x>(scope, major, minor, #x)
#define QML_INTERFACE_U(x, scope, major, minor, reason)  qmlRegisterUncreatableType<x>(scope, major, minor, #x, reason);

#define QML_AUTO_TYPE_2(x, y, scope, major, minor) namespace qml_type {\
						class x##y \
						{                    \
						public:              \
						    x##y()    \
						    {                 \
						        QML_INTERFACE(x, scope, major, minor);   \
						    };                 \
						    ~x##y() {};  \
						}; }                        \
						qml_type::x##y x##init;  
#define QML_AUTO_TYPE(x, scope, major, minor) QML_AUTO_TYPE_2(x, Initialize, scope, major, minor)
#endif // _NULLSPACE_MACRO_1588900396612_H
