#include "gcode.h"

namespace Gcode {

bool Gcode::isEmpty() const
{
    return m_lines.isEmpty() && m_vertices.isEmpty() && m_layers.isEmpty();
}

bool Gcode::isPostProcessed() const
{
    return m_postProcessed;
}

void Gcode::setPostProcessed(bool postProcessed)
{
    m_postProcessed = postProcessed;
}

bool Gcode::hasLines() const
{
    return !m_lines.isEmpty();
}

int Gcode::lineCount() const
{
    return m_lines.count();
}

Gcode::Line Gcode::lineAt(int i) const
{
    return m_lines.value(i);
}

QVector<Gcode::Line> Gcode::lines() const
{
    return m_lines;
}

void Gcode::addLine(const Line &line)
{
    m_lines += line;
}

Gcode::Line &Gcode::getLine(int i)
{
    return m_lines[i];
}

Gcode::Line &Gcode::lastLine()
{
    return m_lines.last();
}

bool Gcode::hasVertices() const
{
    return !m_vertices.isEmpty();
}

int Gcode::vertexCount() const
{
    return m_vertices.count();
}

Gcode::Vertex Gcode::vertexAt(int i) const
{
    return m_vertices.value(i);
}

QVector<Gcode::Vertex> Gcode::vertices() const
{
    return m_vertices;
}

void Gcode::addVertex(const Vertex &vertex)
{
    m_vertices += vertex;
}

Gcode::Vertex &Gcode::getVertex(int i)
{
    return m_vertices[i];
}

Gcode::Vertex &Gcode::lastVertex()
{
    return m_vertices.last();
}

bool Gcode::hasLayers() const
{
    return !m_layers.isEmpty();
}

int Gcode::layerCount() const
{
    return m_layers.count();
}

Gcode::Layer Gcode::layerAt(int i) const
{
    return m_layers.value(i);
}

QVector<Gcode::Layer> Gcode::layers() const
{
    return m_layers;
}

void Gcode::addLayer(const Layer &layer)
{
    m_layers += layer;
}

Gcode::Layer &Gcode::getLayer(int i)
{
    return m_layers[i];
}

Gcode::Layer &Gcode::lastLayer()
{
    return m_layers.last();
}

int Gcode::toolCount() const
{
    return m_tools.count();
}

int Gcode::toolAt(int i) const
{
    return m_tools.value(i);
}

QVector<int> Gcode::tools() const
{
    return m_tools;
}

bool Gcode::hasTool(int tool) const
{
    return m_tools.contains(tool);
}

void Gcode::addTool(int tool)
{
    auto it = std::lower_bound(m_tools.begin(), m_tools.end(), tool);
    m_tools.insert(it, tool);
}

} // namespace Gcode
