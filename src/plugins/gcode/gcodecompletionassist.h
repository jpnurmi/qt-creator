#pragma once

#include <texteditor/codeassist/completionassistprovider.h>
#include <texteditor/codeassist/keywordscompletionassist.h>

namespace Gcode {

class GcodeCompletionAssistProcessor : public TextEditor::KeywordsCompletionAssistProcessor
{
public:
    GcodeCompletionAssistProcessor();

    TextEditor::IAssistProposal *perform(const TextEditor::AssistInterface *interface) override;
};

class GcodeCompletionAssistProvider : public TextEditor::CompletionAssistProvider
{
    Q_OBJECT

public:
    TextEditor::IAssistProcessor *createProcessor() const override;
};

} // Gcode
