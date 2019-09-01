#pragma once

#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/qvector.h>
#include <QtGui/qvector3d.h>

namespace Gcode {

class Gcode
{
public:
    struct Line
    {
        int vertex = -1;
        int layer = -1;
        int tool = 0;
        QByteArray command;
    };

    struct Vertex
    {
        QVector3D from;
        QVector3D to;
    };

    struct Layer
    {
        int vertex = -1;
        int count = -1;
    };

    bool isEmpty() const;

    bool isPostProcessed() const;
    void setPostProcessed(bool postProcessed);

    bool hasLines() const;
    int lineCount() const;
    Line lineAt(int i) const;
    QVector<Line> lines() const;
    void addLine(const Line &line);
    Line &getLine(int i);
    Line &lastLine();

    bool hasVertices() const;
    int vertexCount() const;
    Vertex vertexAt(int i) const;
    QVector<Vertex> vertices() const;
    void addVertex(const Vertex &vertex);
    Vertex &getVertex(int i);
    Vertex &lastVertex();

    bool hasLayers() const;
    int layerCount() const;
    Layer layerAt(int i) const;
    QVector<Layer> layers() const;
    void addLayer(const Layer &layer);
    Layer &getLayer(int i);
    Layer &lastLayer();

    int toolCount() const;
    int toolAt(int i) const;
    QVector<int> tools() const;
    bool hasTool(int tool) const;
    void addTool(int tool);

private:
    bool m_postProcessed = false;
    QVector<Line> m_lines;
    QVector<Vertex> m_vertices;
    QVector<Layer> m_layers;
    QVector<int> m_tools;
};

} // namespace Gcode

Q_DECLARE_METATYPE(Gcode::Gcode)
