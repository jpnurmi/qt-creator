#pragma once

#include <texteditor/textdocument.h>

#include "gcodeparser.h"

namespace Gcode {

class GcodeDocument : public TextEditor::TextDocument
{
    Q_OBJECT

public:
    GcodeDocument();

    bool setContents(const QByteArray &contents) override;

private:
    GcodeParser m_parser;
};

} // namespace Gcode
