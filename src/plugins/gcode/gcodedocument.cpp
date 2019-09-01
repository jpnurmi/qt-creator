#include "gcodedocument.h"

#include "gcodeconstants.h"

#include <QtDebug>

namespace Gcode {

GcodeDocument::GcodeDocument() : TextEditor::TextDocument(Constants::GCODE_EDITOR_ID)
{
    setMimeType(QLatin1String(Constants::GCODE_MIME_TYPE));

    connect(this, &TextEditor::TextDocument::contentsChanged, []() { qDebug() << "contentsChanged"; });
}

bool GcodeDocument::setContents(const QByteArray &contents)
{
    qDebug() << contents;
    return TextEditor::TextDocument::setContents(contents);
}

} // namespace Gcode
