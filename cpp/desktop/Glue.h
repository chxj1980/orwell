#ifndef GLUE_H
#define GLUE_H
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QByteArray>

class Glue : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantMap _list READ list) //WRITE setList)// NOTIFY initialPropsChanged)
    public:
        Glue(QObject* parent = 0) : QObject(parent) {
        
        }
        Glue();

        QVariantMap list() {
            return this->_list;
        }
    private:
        QVariantMap _list;
};
#endif // GLUE_H
