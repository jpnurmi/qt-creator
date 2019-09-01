#include "gcodemesh.h"

namespace Gcode {

GcodeMesh::GcodeMesh(Qt3DCore::QNode *parent) : Qt3DRender::QGeometryRenderer(parent)
{
    setPrimitiveType(Qt3DRender::QGeometryRenderer::Lines);
}

Gcode GcodeMesh::gcode() const
{
    return m_gcode;
}

int GcodeMesh::layerCount() const
{
    return m_gcode.layerCount();
}

int GcodeMesh::minimumLayer() const
{
    return m_minimumLayer;
}

void GcodeMesh::setMinimumLayer(int layer)
{
    if (m_minimumLayer == layer)
        return;

    m_minimumLayer = layer;
    rebuild();
    emit minimumLayerChanged(layer);
}

int GcodeMesh::maximumLayer() const
{
    return m_maximumLayer;
}

void GcodeMesh::setMaximumLayer(int layer)
{
    if (m_maximumLayer == layer)
        return;

    m_maximumLayer = layer;
    rebuild();
    emit maximumLayerChanged(layer);
}

int GcodeMesh::highlightLayer() const
{
    return m_highlight.layer;
}

void GcodeMesh::setHighlightLayer(int layer)
{
    if (m_highlight.layer == layer)
        return;

    m_highlight.layer = layer;
    rebuild();
    emit highlightLayerChanged(layer);
}

QColor GcodeMesh::layerColor() const
{
    return m_layerColor;
}

void GcodeMesh::setLayerColor(const QColor &color)
{
    if (m_layerColor == color)
        return;

    m_layerColor = color;
    emit layerColorChanged();
}

QColor GcodeMesh::highlightColor() const
{
    return m_highlightColor;
}

void GcodeMesh::setHighlightColor(const QColor &color)
{
    if (m_highlightColor == color)
        return;

    m_highlightColor = color;
    emit highlightColorChanged();
}

QVector<QColor> GcodeMesh::toolColors() const
{
    return m_toolColors;
}

void GcodeMesh::setToolColors(const QVector<QColor> &colors)
{
    if (m_toolColors == colors)
        return;

    m_toolColors = colors;
    emit toolColorsChanged();
}

QColor GcodeMesh::toolColor(int pos) const
{
    if (m_gcode.isPostProcessed())
        --pos;

    if (pos >= 0 && pos < m_toolColors.count())
        return m_toolColors.at(pos);

    QColor base = m_toolColors.value(0).toHsl();
    return QColor::fromHsl((base.hue() + pos * 120) % 360, base.saturation(), base.lightness());
}

void GcodeMesh::highlightLine(int line)
{
    m_highlight = m_gcode.lineAt(line);
    rebuild();
}

void GcodeMesh::load(const Gcode &gcode)
{
    m_gcode = gcode;
    rebuild();
}

void GcodeMesh::rebuild()
{
    Qt3DRender::QGeometry *oldGeometry = geometry();
    if (!m_gcode.hasVertices())
        setGeometry(nullptr);
    else
        setGeometry(createGeometry(/*this*/));
    delete oldGeometry;
}

static QVector3D toVector3D(const QColor &color)
{
    return QVector3D(color.redF(), color.greenF(), color.blueF());
}

Qt3DRender::QBuffer *GcodeMesh::createColorBuffer(int offset, const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent) const
{
    Qt3DRender::QBuffer *buffer = new Qt3DRender::QBuffer(parent);

    const Gcode::Layer min = m_gcode.layerAt(m_minimumLayer);
    QVector<QVector3D> colors(2 * vertices.count(), toVector3D(m_layerColor));

    if (m_highlight.layer >= 0 && (m_minimumLayer < 0 || m_highlight.layer >= m_minimumLayer) && (m_maximumLayer < 0 || m_highlight.layer <= m_maximumLayer)) {
        const Gcode::Layer highlight = m_gcode.layerAt(m_highlight.layer);
        int from = 2 * (highlight.vertex - min.vertex);
        int to = 2 * (highlight.vertex - min.vertex + highlight.count);
        const QVector3D color = toVector3D(m_highlightColor);
        for (int i = from; i < to; ++i)
            colors[i] = color;

        int vertex = m_highlight.vertex - offset;
        if (vertex >= 0 && vertex < vertices.count()) {
            const QVector3D color = toVector3D(toolColor(m_highlight.tool));
            colors[2 * vertex] = color;
            colors[2 * vertex + 1] = color;
        }
    }

    const int size = vertices.count() * sizeof(Gcode::Vertex);
    const char *data = reinterpret_cast<const char *>(colors.data());
    buffer->setData(QByteArray(data, size));
    return buffer;
}

Qt3DRender::QBuffer *GcodeMesh::createPositionBuffer(const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent) const
{
    Qt3DRender::QBuffer *buffer = new Qt3DRender::QBuffer(parent);
    const int size = vertices.count() * sizeof(Gcode::Vertex);
    const char *data = reinterpret_cast<const char *>(vertices.data());
    buffer->setData(QByteArray(data, size));
    return buffer;
}

Qt3DRender::QAttribute *GcodeMesh::createAttribute(const QString &name, const QVector<Gcode::Vertex> &vertices, Qt3DCore::QNode *parent) const
{
    Qt3DRender::QAttribute *attribute = new Qt3DRender::QAttribute(parent);
    attribute->setName(name);
    attribute->setAttributeType(Qt3DRender::QAttribute::VertexAttribute);
    attribute->setVertexBaseType(Qt3DRender::QAttribute::Float);
    attribute->setVertexSize(3);
    attribute->setCount(3 * vertices.count());
    attribute->setByteStride(sizeof(QVector3D));
    return attribute;
}

Qt3DRender::QGeometry *GcodeMesh::createGeometry(Qt3DCore::QNode *parent) const
{
    const Gcode::Layer min = m_gcode.layerAt(m_minimumLayer);
    const Gcode::Layer max = m_maximumLayer >= 0 ? m_gcode.layerAt(m_maximumLayer) : m_gcode.layerAt(m_gcode.layerCount() - 1);
    const QVector<Gcode::Vertex> vertices = m_gcode.vertices().mid(min.vertex, max.vertex + max.count - min.vertex);
    if (vertices.isEmpty())
        return nullptr;

    Qt3DRender::QGeometry *geometry = new Qt3DRender::QGeometry(parent);

    const QString positionAttributeName = Qt3DRender::QAttribute::defaultPositionAttributeName();
    Qt3DRender::QAttribute *positionAttribute = createAttribute(positionAttributeName, vertices/*, geometry*/);
    positionAttribute->setBuffer(createPositionBuffer(vertices/*, geometry*/));
    geometry->addAttribute(positionAttribute);

    const QString colorAttributeName = Qt3DRender::QAttribute::defaultColorAttributeName();
    Qt3DRender::QAttribute *colorAttribute = createAttribute(colorAttributeName, vertices/*, geometry*/);
    colorAttribute->setBuffer(createColorBuffer(min.vertex, vertices/*, geometry*/));
    geometry->addAttribute(colorAttribute);

    return geometry;
}

} // namespace Gcode
