#ifndef GLUE_H
#define GLUE_H
#include <iostream>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QVariant>
#include <QByteArray>
#include <memory>

//https://stackoverflow.com/questions/1008019/c-singleton-design-pattern/40337728#40337728
class Glue
{
public:
    Glue(Glue const&) = delete;
    Glue& operator=(Glue const&) = delete;

    static QVariantMap streamList;

    static std::shared_ptr<Glue> instance()
    {
        static std::shared_ptr<Glue> s{new Glue};
        return s;
    }


private:
    Glue() {}
};


/*
class Glue : public QObject {
    Q_OBJECT
    //Q_PROPERTY(QVariantMap _list READ list) //WRITE setList)// NOTIFY initialPropsChanged)
    public:
        Glue(QObject* parent = 0) : QObject(parent) {
        
        }
        Glue();

        QVariantMap* list() {
            return this->_list;
        }
    private:
        QVariantMap* _list = new QVariantMap();
};
*/

#endif // GLUE_H
