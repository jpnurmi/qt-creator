#include "gcodeplugin.h"

#include "gcodecompletionassist.h"
#include "gcodeconstants.h"
#include "gcodeeditor.h"
#include "gcodeoutline.h"

#include <texteditor/snippets/snippetprovider.h>

namespace Gcode {

class GcodePluginPrivate
{
public:
    GcodeCompletionAssistProvider m_completionAssistProvider;
    GcodeEditorFactory m_editorFactory;
    GcodeOutlineWidgetFactory m_outlineWidgetFactory;
};

static GcodePluginPrivate *dd = nullptr;

GcodePlugin::~GcodePlugin()
{
    delete dd;
}

bool GcodePlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    dd = new GcodePluginPrivate;

    TextEditor::SnippetProvider::registerGroup(Constants::GCODE_SNIPPET_GROUP, tr("G-code", "SnippetProvider"));

    return true;
}

void GcodePlugin::extensionsInitialized()
{
}

} // Gcode
