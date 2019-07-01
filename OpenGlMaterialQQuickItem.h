#ifndef OpenGlMaterialQQuickItem_H
#define OpenGlMaterialQQuickItem_H

#include <boost/thread.hpp>
#include "MediaStream.h"
#include "reactitem.h"

//class OpenGlMaterialQQuickItem: public QQuickItem
class OpenGlMaterialQQuickItem: public ReactItem
{
    //Q_OBJECT

    Q_PROPERTY(QString uri WRITE setUri)// NOTIFY uriChanged)

    public:
        std::string uri;

        QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

        OpenGlMaterialQQuickItem()
        {
            setFlag(ItemHasContents, true);
        }

        void setUri(const QString &a) {
            uri = a.toStdString();
        }
        
};
#endif 