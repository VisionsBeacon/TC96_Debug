#ifndef SVG_IMAGE_H
#define SVG_IMAGE_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QQuickView>
#include <QtSvg/QSvgRenderer>

class HcSvgImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ getSource WRITE setSource NOTIFY sourceChanged)

    QML_NAMED_ELEMENT(HcSvgImage)

public:
    explicit HcSvgImage(QQuickItem *parent = nullptr);

//    static void registQML();
    QUrl getSource() const { return m_source; }
    void setSource(const QUrl& source);

signals:
    void sourceChanged();

protected:
    void paint(QPainter *painter) override;

private:
    QSvgRenderer *m_pSvgRenderer;
    QUrl m_source;
};

#endif // SVG_IMAGE_H
