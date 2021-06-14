#ifndef MYCLASS_H
#define MYCLASS_H
#include <QObject>
#include <mylib_global.h>
class MYLIB_EXPORT MyClass : public QObject {
    Q_OBJECT
public:
    explicit MyClass(QObject* parent = nullptr);
};
#endif