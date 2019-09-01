#include "gcodecompletionassist.h"

#include "gcodeconstants.h"

#include <texteditor/codeassist/assistinterface.h>

#include <QtDebug>

namespace Gcode {

GcodeCompletionAssistProcessor::GcodeCompletionAssistProcessor()
    : TextEditor::KeywordsCompletionAssistProcessor(TextEditor::Keywords())
{
    setSnippetGroup(Constants::GCODE_SNIPPET_GROUP);
}

TextEditor::IAssistProposal *GcodeCompletionAssistProcessor::perform(const TextEditor::AssistInterface *assist)
{
    qDebug() << "perform" << assist->reason() << assist->fileName() << assist->position() << assist->textAt(assist->position(), 1);

//    IdleEditor,
//    ActivationCharacter,
//    ExplicitlyInvoked


    return TextEditor::KeywordsCompletionAssistProcessor::perform(assist);
}

TextEditor::IAssistProcessor *GcodeCompletionAssistProvider::createProcessor() const
{
    return new GcodeCompletionAssistProcessor;
}

} // Gcode
