#pragma once

#include <texteditor/autocompleter.h>

namespace Gcode {

class GcodeAutoCompleter : public TextEditor::AutoCompleter
{
public:
    bool isInComment(const QTextCursor &cursor) const override;
};

} // namespace Gcode
