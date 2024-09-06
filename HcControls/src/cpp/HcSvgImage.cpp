#include <qdebug.h>
#include <QTimer>
#include <QRegion>
#include <QPointF>
#include <QDir>
#include <QQmlContext>

#include "HcSvgImage.h"

HcSvgImage::HcSvgImage(QQuickItem *parent) :QQuickPaintedItem(parent)
{
    m_pSvgRenderer = new QSvgRenderer(this);
}

//void HcSvgImage::registQML()
//{
//    qmlRegisterType<HcSvgImage>("HcSvgImage", 1, 0, "HcSvgImage");
//}

void HcSvgImage::setSource(const QUrl& source) {
    // 资源文件路径
    if (source.isRelative())
    {
        const QQmlContext *context = qmlContext(this);

        if (context != nullptr)
        {
            m_source = context->resolvedUrl(source);
        }
        else
        {
            qDebug() << "context is null";
            return;
        }
    }
    else
    {
        m_source = source;
    }

    m_pSvgRenderer->load(m_source.toString().replace("qrc", ""));
//    qDebug() << "source =" << source << ", m_source =" << m_source << ", ret = " << ret;
    emit sourceChanged();
    update();
}

void HcSvgImage::paint(QPainter *painter)
{
    QRectF rect = QRectF(QPointF(x(),y()), QSizeF(width(), height()));
    //qDebug() << "width" << width();
    //qDebug() << "height" << height();
    m_pSvgRenderer->render(painter, rect);
}
