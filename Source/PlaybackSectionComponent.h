/*
  ==============================================================================

    PlaybackSectionComponent.h
    Created: 25 Jan 2020 3:37:47pm
    Author:  Benjamin Smart

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class PlaybackSectionComponent    : public Component
{
public:
    PlaybackSectionComponent()
    {
        addAndMakeVisible(textEditor);
        textEditor.setReadOnly(true);
        textEditor.setMultiLine(true);
        textEditor.setCaretVisible(true);
        textEditor.setJustification(Justification::left);
        
        
    }

    ~PlaybackSectionComponent()
    {
    }

    void paint (Graphics& g) override
    {

        g.fillAll (Colours::darkgrey);   // clear the background

        g.setColour (Colours::darkgrey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::lightgrey);
        g.setFont (14.0f);
        g.drawText ("PlaybackSectionComponent", getLocalBounds(),
                    Justification::centred, true);
    }

    void resized() override
    {
        Rectangle<int> textEditorBounds = Rectangle<int> ( 10, getHeight()/2 + 10, getWidth()-20, getHeight()/2 - 20);
        textEditor.setBounds(textEditorBounds);

    }

private:
    TextEditor textEditor;
//    Label PlaybackLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackSectionComponent)
};
