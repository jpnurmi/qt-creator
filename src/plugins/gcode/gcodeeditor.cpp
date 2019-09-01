#include "gcodeeditor.h"

#include "gcodeautocompleter.h"
#include "gcodecompletionassist.h"
#include "gcodeconstants.h"
#include "gcodedocument.h"

#include <utils/uncommentselection.h>

namespace Gcode {

GcodeEditorFactory::GcodeEditorFactory()
{
    setId(Constants::GCODE_EDITOR_ID);
    setDisplayName(tr(Constants::GCODE_EDITOR_DISPLAY_NAME));
    addMimeType(Constants::GCODE_MIME_TYPE);

//    setEditorCreator([]() { return new GcodeEditor; });
//    setEditorWidgetCreator([]() { return new GcodeEditorWidget; });
    setDocumentCreator([]() { return new GcodeDocument; });
    setUseGenericHighlighter(true);
    setCommentDefinition(Utils::CommentDefinition(QString(Constants::GCODE_COMMENT)));
    setCodeFoldingSupported(true);

    setCompletionAssistProvider(new GcodeCompletionAssistProvider);
    setAutoCompleterCreator([]() { return new GcodeAutoCompleter; });
}

} // namespace Gcode
