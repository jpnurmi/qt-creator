#pragma once

#include <QtCore/qglobal.h>

namespace Gcode {
namespace Constants {

const char GCODE_MIME_TYPE[]  = "text/x.gcode";
const char GCODE_EDITOR_ID[] = "GcodeEditor";
const char GCODE_EDITOR_DISPLAY_NAME[] = QT_TRANSLATE_NOOP("Gcode::GcodeEditorFactory", "G-code Editor");
const char GCODE_SNIPPET_GROUP[] = "G-code";
const char GCODE_COMMENT  = ';';

} // namespace Constants
} // namespace Gcode
