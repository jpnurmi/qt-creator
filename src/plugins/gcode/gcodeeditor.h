#pragma once

#include <texteditor/textdocument.h>
#include <texteditor/texteditor.h>

namespace Gcode {

class GcodeEditorFactory : public TextEditor::TextEditorFactory
{
    Q_OBJECT

public:
    GcodeEditorFactory();
};

} // namespace Gcode
