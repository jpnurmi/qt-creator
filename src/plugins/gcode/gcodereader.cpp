#include "gcodereader.h"
#include <QtCore/qbuffer.h>

namespace Gcode {

GcodeReader::GcodeReader(const QByteArray &data)
{
    m_buffer.setData(data);
    m_buffer.open(QIODevice::ReadWrite);
}

QByteArray GcodeReader::data() const
{
    return m_buffer.data();
}

void GcodeReader::setData(const QByteArray &data)
{
    m_buffer.close();
    m_buffer.setData(data);
    m_buffer.open(QIODevice::ReadWrite);
}

int GcodeReader::num() const
{
    return m_num;
}

qint64 GcodeReader::pos() const
{
    return m_buffer.pos();
}

qint64 GcodeReader::size() const
{
    return m_buffer.size();
}

static QByteArray cleanupLine(const QByteArray &line)
{
    int index = line.indexOf(';');
    if (index == -1)
        return line.trimmed();

    QByteArray copy = line;
    copy.truncate(index);
    return copy.trimmed();
}

static QVariant parseValue(const QByteArray &value)
{
    bool ok = false;
    int i = value.toInt(&ok);
    if (ok)
        return i;

    double d = value.toDouble(&ok);
    if (ok)
        return d;

    return value;
}

static bool parseLine(const QByteArray &line, QByteArray &cmd, QHash<char, QVariant> &args)
{
    cmd.clear();
    args.clear();

    QByteArrayList commands = line.split(' ');
    cmd = commands.takeFirst().toUpper();

    for (const QByteArray &command : qAsConst(commands)) {
        if (command.isEmpty())
            continue;

        const QChar key = QChar(command.front()).toUpper();
        args.insert(key.toLatin1(), parseValue(command.mid(1)));
    }

    return true;
}

bool GcodeReader::next()
{
    if (m_buffer.atEnd())
        return false;

    m_line = cleanupLine(m_buffer.readLine());
    if (!parseLine(m_line, m_cmd, m_values))
        return false;

    ++m_num;
    return true;
}

bool GcodeReader::isEmpty() const
{
    return m_line.isEmpty();
}

bool GcodeReader::isG() const
{
    return m_cmd.startsWith('G');
}

bool GcodeReader::isM() const
{
    return m_cmd.startsWith('M');
}

bool GcodeReader::isT() const
{
    return m_cmd.startsWith('T');
}

GcodeReader::Type GcodeReader::type() const
{
    if (m_cmd.isEmpty())
        return Empty;

    switch (m_cmd.front()) {
    case 'G':
        return G;
    case 'M':
        return M;
    case 'T':
        return T;
    default:
        return Unknown;
    }
}

QByteArray GcodeReader::line() const
{
    return m_line;
}

QByteArray GcodeReader::command() const
{
    return m_cmd;
}

int GcodeReader::code() const
{
    return m_cmd.mid(1).toInt();
}

bool GcodeReader::hasValue(char key) const
{
    return m_values.contains(key);
}

QHash<char, QVariant> GcodeReader::values() const
{
    return m_values;
}

QVariant GcodeReader::value(char key, const QVariant &defaultValue) const
{
    return m_values.value(key, defaultValue);
}

} // namespace Gcode
