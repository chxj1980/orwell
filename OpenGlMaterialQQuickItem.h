class OpenGlMaterialQQuickItem: public QQuickItem
{
    //Q_OBJECT

    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
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
        /*
        void setColor(const QColor &color) {
            if (m_color != color) {
                m_color = color;
                emit colorChanged();
                update();
            }
        }

        QColor color() const {
            return m_color;
        }
        */

    signals:
        //void colorChanged();

    private:
        //QColor m_color;
        
};