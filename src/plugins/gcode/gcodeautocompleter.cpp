#include "gcodeautocompleter.h"

#include "gcodeconstants.h"

#include <QtGui/qtextcursor.h>

namespace Gcode {

bool GcodeAutoCompleter::isInComment(const QTextCursor &cursor) const
{
    QTextCursor c = cursor;
    c.movePosition(QTextCursor::StartOfLine, QTextCursor::KeepAnchor);
    return c.selectedText().contains(QLatin1Char(Constants::GCODE_COMMENT));
}

} // namespace Gcode
