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
        
        addAndMakeVisible(chordVoicingMenu);
        chordVoicingMenu.addItem("Triad Root Position", 1);
        chordVoicingMenu.addItem("Triad First Inversion", 2);
        chordVoicingMenu.addItem("Triad Second Inversion", 3);
        chordVoicingMenu.setSelectedId(1);
        chordVoicingMenu.onChange = [this] { changeChordVoicing(chordVoicingMenu.getSelectedIdAsValue().getValue() ); };
        
        addAndMakeVisible(parentScaleMenu);
        parentScaleMenu.addItem("Melodic Major",  1);
        parentScaleMenu.addItem("Harmonic Major", 2);
        parentScaleMenu.addItem("Melodic Minor",  3);
        parentScaleMenu.addItem("Harmonic Minor", 4);
        parentScaleMenu.setSelectedId(1);
        parentScaleMenu.onChange = [this] { changeParentScale(parentScaleMenu.getSelectedIdAsValue().getValue() ); };
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
        
        Rectangle<int> parentScaleMenuBounds  = Rectangle<int> ( 10, 10, getWidth()-20, 30);
        parentScaleMenu.setBounds(parentScaleMenuBounds);
        
        Rectangle<int> chordVoicingMenuBounds = Rectangle<int> ( 10, 50, getWidth()-20, 30);
        chordVoicingMenu.setBounds(chordVoicingMenuBounds);
    }
    
    void changeParentScale(int parentScaleNumber)
    {
        printToTextEditor(String("parentScaleNumber: ") + String(parentScaleNumber));
    }
    
    void changeChordVoicing(int voicing)
    {
        switch (voicing)
        {
            case 1:
                currentChordIntervals = rootPositionIntervals;
                break;
            case 2:
                currentChordIntervals = firstInversionInvervals;
                break;
            case 3:
                currentChordIntervals = secondInversionIntervals;
                break;
        }
        for (int interval : currentChordIntervals)
            printToTextEditor(String("currentChordInterval: ") + String(interval));
    }
    
    void printToTextEditor (const String& textToPrint)
    {
        textEditor.insertTextAtCaret(textToPrint);
        textEditor.moveCaretToEnd();
        textEditor.insertTextAtCaret(newLine);
    }

private:
    std::array<int, 2> currentChordIntervals =    { 0, 0 };
    std::array<int, 2> rootPositionIntervals =    { 2, 2 };
    std::array<int, 2> firstInversionInvervals =  { 2, 3 };
    std::array<int, 2> secondInversionIntervals = { 3, 2 };
    
    TextEditor textEditor;
    ComboBox chordVoicingMenu;
    ComboBox parentScaleMenu;
    int rectangleWidth;
    
//    Label PlaybackLabel;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaybackSectionComponent)
};
