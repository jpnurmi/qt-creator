#pragma once

#include <texteditor/ioutlinewidget.h>

namespace Gcode {

class GcodeOutlineWidget : public TextEditor::IOutlineWidget
{
    Q_OBJECT

public:
    GcodeOutlineWidget(QWidget *parent = nullptr);

    QList<QAction*> filterMenuActions() const override;
    void setCursorSynchronization(bool syncWithCursor) override;

//    void restoreSettings(const QVariantMap &map) override;
//    QVariantMap settings() const override;
};

class GcodeOutlineWidgetFactory : public TextEditor::IOutlineWidgetFactory
{
    Q_OBJECT

public:
    bool supportsEditor(Core::IEditor *editor) const override;
    TextEditor::IOutlineWidget *createWidget(Core::IEditor *editor) override;
};

} // namespace Gcode
