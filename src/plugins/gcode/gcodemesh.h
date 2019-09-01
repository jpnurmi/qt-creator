#pragma once

#include <QtCore/qurl.h>
#include <QtCore/qvector.h>
#include <QtGui/qcolor.h>
#include <QtGui/qvector3d.h>
#include <Qt3DRender/qattribute.h>
#include <Qt3DRender/qbuffer.h>
#include <Qt3DRender/qgeometry.h>
#include <Qt3DRender/qgeometryrenderer.h>
#include "gcode.h"

namespace Gcode {

class GcodeMesh : public Qt3DRender::QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(int layerCount READ layerCount NOTIFY layerCountChanged)
    Q_PROPERTY(int minimumLayer READ minimumLayer WRITE setMinimumLayer NOTIFY minimumLayerChanged)
    Q_PROPERTY(int maximumLayer READ maximumLayer WRITE setMaximumLayer NOTIFY maximumLayerChanged)
    Q_PROPERTY(int highlightLayer READ highlightLayer WRITE setHighlightLayer NOTIFY highlightLayerChanged)
    Q_PROPERTY(QColor layerColor READ layerColor WRITE setLayerColor NOTIFY layerColorChanged)
    Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)
    Q_PROPERTY(QVector<QColor> toolColors READ toolColors WRITE setToolColors NOTIFY toolColorsChanged)

public:
    explicit GcodeMesh(Qt3DCore::QNode *parent = nullptr);

    Gcode gcode() const;

    int layerCount() const;

    int minimumLayer() const;
    void setMinimumLayer(int layer);

    int maximumLayer() const;
    void setMaximumLayer(int layer);

    int highlightLayer() const;
    void setHighlightLayer(int layer);

    QColor layerColor() const;
    void setLayerColor(const QColor &color);

    QColor highlightColor() const;
    void setHighlightColor(const QColor &color);

    QVector<QColor> toolColors() const;
    void setToolColors(const QVector<QColor> &colors);

    Q_INVOKABLE QColor toolColor(int pos) const;

public slots:
    void highlightLine(int line);
    void load(const Gcode &gcode);

signals:
    void layerCountChanged(int count);
    void minimumLayerChanged(int layer);
    void maximumLayerChanged(int layer);
    void highlightLayerChanged(int layer);
    void layerColorChanged();
    void highlightColorChanged();
    void toolColorsChanged();

private slots:
    void rebuild();

private:
    Qt3DRender::QBuffer *createColorBuffer(int offset, const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent = nullptr) const;
    Qt3DRender::QBuffer *createPositionBuffer(const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent = nullptr) const;
    Qt3DRender::QAttribute *createAttribute(const QString &name, const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent = nullptr) const;
    Qt3DRender::QGeometry *createGeometry(Qt3DCore::QNode *parent = nullptr) const;

    Gcode m_gcode;
    Gcode::Line m_highlight;
    int m_minimumLayer = 0;
    int m_maximumLayer = -1;
    QColor m_layerColor;
    QColor m_highlightColor;
    QVector<QColor> m_toolColors = {Qt::blue, Qt::red, Qt::green};
};

} // namespace Gcode
