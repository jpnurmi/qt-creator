#include "gcodeparser.h"
#include <QtCore/qdebug.h>

namespace Gcode {

GcodeParser::GcodeParser(const QByteArray &data)
{
    m_reader.setData(data);
}

Gcode GcodeParser::gcode() const
{
    return m_data;
}

bool GcodeParser::parse(const QByteArray &data)
{
    m_mutex.lock();
    m_cancel = false;
    m_mutex.unlock();

    if (!data.isEmpty())
        m_reader.setData(data);

    while (m_reader.next() && !isCanceled()) {
        Gcode::Line line { -1, m_data.layerCount() - 1, m_tool, m_reader.command() };

        switch (m_reader.type()) {
        case GcodeReader::G:
            parseG(line);
            break;
        case GcodeReader::M:
            parseM(line);
            break;
        case GcodeReader::T:
            parseT(line);
            break;
        default:
            break;
        }

        m_data.addLine(line);

        emit progress(qRound(100.0 * m_reader.pos() / m_reader.size()));
    }

#if 0
    qDebug() << "GcodeParser::run():" << m_data.lineCount() << "lines:";
    for (int i = 0; i < m_data.lineCount(); ++i)
        qDebug() << "\tline" << i << "tool:" << m_data.lineAt(i).tool << "vertex:" << m_data.lineAt(i).vertex << "layer:" << m_data.lineAt(i).layer;

    qDebug() << "GcodeParser::run():" << m_data.vertexCount() << "vertices:";
    for (int i = 0; i < m_data.vertexCount(); ++i)
        qDebug() << "\tvertex" << i << "from:" << m_data.vertexAt(i).from << "to:" << m_data.vertexAt(i).to;

    qDebug() << "GcodeParser::run():" << m_data.layerCount() << "layers:";
    for (int i = 0; i < m_data.layerCount(); ++i)
        qDebug() << "\tlayer" << i << "from:" << m_data.layerAt(i).from << "count:" << m_data.layerAt(i).count;
#endif

    return true; // ### TODO
}

bool GcodeParser::isCanceled() const
{
    QMutexLocker locker(&m_mutex);
    return m_cancel;
}

void GcodeParser::cancel()
{
    QMutexLocker locker(&m_mutex);
    m_cancel = true;
}

void GcodeParser::parseG(Gcode::Line &line)
{
    QVector3D pos;

    switch (m_reader.code()) {
    case 0:
    case 1:
        if (m_absolute) {
            pos.setX(m_reader.value('X', m_pos.x()).toFloat());
            pos.setY(m_reader.value('Y', m_pos.y()).toFloat());
            pos.setZ(m_reader.value('Z', m_pos.z()).toFloat());
            break;
        }
        Q_FALLTHROUGH();
    case 7: // relative
        pos.setX(m_pos.x() + m_reader.value('X').toFloat());
        pos.setY(m_pos.y() + m_reader.value('Y').toFloat());
        pos.setZ(m_pos.z() + m_reader.value('Z').toFloat());
        break;
    case 90:
        m_absolute = true;
        return;
    case 91:
        m_absolute = false;
        return;
    case 92:
        if (!m_reader.hasValue('X') && !m_reader.hasValue('Y') && !m_reader.hasValue('Z')) {
            m_offset = toAbsolutePosition(m_pos);
            m_pos = QVector3D(0, 0, 0);
        } else {
            const QVector3D abs = toAbsolutePosition(m_pos);
            if (m_reader.hasValue('X')) {
                m_pos.setX(m_reader.value('X').toFloat());
                m_offset.setX(abs.x() - m_pos.x());
            }
            if (m_reader.hasValue('Y')) {
                m_pos.setY(m_reader.value('Y').toFloat());
                m_offset.setY(abs.y() - m_pos.y());
            }
            if (m_reader.hasValue('Z')) {
                m_pos.setZ(m_reader.value('Z').toFloat());
                m_offset.setZ(abs.z() - m_pos.y());
            }
        }
        return;
    default:
        return;
    }

    if (m_reader.hasValue('Z'))
        m_zline = m_reader.num() - 1;

    if (m_reader.value('E').toReal() > 0) {
        Gcode::Vertex vertex { toAbsolutePosition(m_pos), toAbsolutePosition(pos) };
        m_data.addVertex(vertex);

        if (!m_data.hasLayers() || !qFuzzyCompare(m_zval, pos.z())) {
            Gcode::Layer layer { m_data.vertexCount() - 1, 0 };
            m_data.addLayer(layer);
            m_zval = pos.z();

            for (int i = std::max(0, m_zline); i < m_data.lineCount(); ++i)
                m_data.getLine(i).layer = m_data.layerCount() - 1;
        }

        if (m_data.hasLayers())
            ++m_data.lastLayer().count;

        line.layer = m_data.layerCount() - 1;
        line.vertex = m_data.vertexCount() - 1;
    }

    m_pos = pos;
}

void GcodeParser::parseM(Gcode::Line &line)
{
    Q_UNUSED(line)
}

void GcodeParser::parseT(Gcode::Line &line)
{
    m_tool = m_reader.code();
    if (!m_data.hasTool(m_tool))
        m_data.addTool(m_tool);
    line.tool = m_tool;
}

QVector3D GcodeParser::toLogicalPosition(const QVector3D &pos) const
{
    return pos - m_offset;
}

QVector3D GcodeParser::toAbsolutePosition(const QVector3D &pos) const
{
    return m_offset + pos;
}

} // namespace Gcode
