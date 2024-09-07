#ifndef REGISTERTOQML_H
#define REGISTERTOQML_H

#include <QObject>

class RegisterToQml : public QObject
{
    Q_OBJECT
public:
    explicit RegisterToQml(QObject *parent = nullptr);

    static RegisterToQml *getInstance();

signals:

private:
    static RegisterToQml *m_registerToQml;
};

#endif // REGISTERTOQML_H
