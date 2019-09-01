#include "gcodeoutline.h"

#include "gcodeconstants.h"

#include <coreplugin/editormanager/ieditor.h>
#include <coreplugin/idocument.h>

#include <QtCore/qdebug.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qlabel.h>

namespace Gcode {

GcodeOutlineWidget::GcodeOutlineWidget(QWidget *parent)
    : TextEditor::IOutlineWidget(parent)
{
    QLabel *label = new QLabel("G-code", this);
    label->setAlignment(Qt::AlignCenter);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(label);
}

QList<QAction *> GcodeOutlineWidget::filterMenuActions() const
{
    return QList<QAction *>();
}

void GcodeOutlineWidget::setCursorSynchronization(bool syncWithCursor)
{
    Q_UNUSED(syncWithCursor);
}

bool GcodeOutlineWidgetFactory::supportsEditor(Core::IEditor *editor) const
{
    Core::IDocument *doc = editor->document();
    return doc->mimeType() == QLatin1String(Constants::GCODE_MIME_TYPE);
}

TextEditor::IOutlineWidget *GcodeOutlineWidgetFactory::createWidget(Core::IEditor *editor)
{
    Q_UNUSED(editor);

    return new GcodeOutlineWidget(editor->widget());
}

} // namespace Gcode
