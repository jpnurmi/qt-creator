#pragma once

#include <QtCore/qbuffer.h>
#include <QtCore/qbytearray.h>
#include <QtCore/qhash.h>
#include <QtCore/qvariant.h>

namespace Gcode {

class GcodeReader
{
public:
    GcodeReader(const QByteArray &data = QByteArray());

    QByteArray data() const;
    void setData(const QByteArray &data);

    int num() const;
    qint64 pos() const;
    qint64 size() const;

    bool next();

    bool isEmpty() const;
    bool isG() const;
    bool isM() const;
    bool isT() const;

    enum Type { Empty, G, M, T, Unknown };
    Type type() const;

    QByteArray line() const;
    QByteArray command() const;
    int code() const;

    bool hasValue(char key) const;
    QHash<char, QVariant> values() const;
    QVariant value(char key, const QVariant &defaultValue = QVariant()) const;

private:
    int m_num = 0;
    QBuffer m_buffer;
    QByteArray m_line;
    QByteArray m_cmd;
    QHash<char, QVariant> m_values;
};

} // namespace Gcode
