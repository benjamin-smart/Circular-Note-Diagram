
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class CircularNoteDiagram    : public Component
{
public:
    CircularNoteDiagram();
    ~CircularNoteDiagram();

    void paint (Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircularNoteDiagram)
};
