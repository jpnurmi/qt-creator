#pragma once

#include <QtCore/qiodevice.h>
#include <QtCore/qmutex.h>
#include <QtCore/qvector.h>
#include <QtGui/qvector3d.h>
#include "gcode.h"
#include "gcodereader.h"

namespace Gcode {

class GcodeParser : public QObject
{
    Q_OBJECT

public:
    GcodeParser(const QByteArray &data = QByteArray());

    Gcode gcode() const;

    bool parse(const QByteArray &data = QByteArray());

    bool isCanceled() const;
    void cancel();

signals:
    void progress(int progress);

protected:
    void parseG(Gcode::Line &line);
    void parseM(Gcode::Line &line);
    void parseT(Gcode::Line &line);

    QVector3D toLogicalPosition(const QVector3D &pos) const;
    QVector3D toAbsolutePosition(const QVector3D &pos) const;

private:
    bool m_cancel = false;
    bool m_absolute = true;
    int m_tool = -1;
    int m_zline = -1;
    float m_zval = 0;
    QVector3D m_pos;
    QVector3D m_offset;
    Gcode m_data;
    GcodeReader m_reader;
    mutable QMutex m_mutex;
};

} // namespace Gcode
