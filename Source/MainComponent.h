#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "CircleDiagramComponent.h"

//==============================================================================

class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent()
    {
        setSize (1200, 1200);
        circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::ionian);
        
        addAndMakeVisible(circleDiagram);
        addAndMakeVisible(distanceFromCentreSlider);
        addAndMakeVisible(rotateNodesSlider);
        
        addAndMakeVisible(chordButton1);
        addAndMakeVisible(chordButton2);
        addAndMakeVisible(chordButton3);
        addAndMakeVisible(chordButton4);
        addAndMakeVisible(chordButton5);
        addAndMakeVisible(chordButton6);
        addAndMakeVisible(chordButton7);
        
        addAndMakeVisible(modeButton1);
        addAndMakeVisible(modeButton2);
        addAndMakeVisible(modeButton3);
        addAndMakeVisible(modeButton4);
        addAndMakeVisible(modeButton5);
        addAndMakeVisible(modeButton6);
        addAndMakeVisible(modeButton7);
        
        distanceFromCentreSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        distanceFromCentreSlider.setRange(0.4f, 0.9f);
        distanceFromCentreSlider.setValue(0.9f);
        distanceFromCentreSlider.onValueChange = [this]{ circleDiagram.setDistanceFromCentre((float)distanceFromCentreSlider.getValue()); };
        distanceFromCentreSlider.setNumDecimalPlacesToDisplay(2);
        
        rotateNodesSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        rotateNodesSlider.setRange(0.0f, MathConstants<float>::twoPi);
        rotateNodesSlider.setValue(MathConstants<float>::halfPi + MathConstants<float>::pi);
        rotateNodesSlider.onValueChange = [this]{ circleDiagram.rotateNodes(rotateNodesSlider.getValue()); };
        rotateNodesSlider.setNumDecimalPlacesToDisplay(2);
        
        chordButton1.setRadioGroupId(1);
        chordButton2.setRadioGroupId(1);
        chordButton3.setRadioGroupId(1);
        chordButton4.setRadioGroupId(1);
        chordButton5.setRadioGroupId(1);
        chordButton6.setRadioGroupId(1);
        chordButton7.setRadioGroupId(1);
        chordButton1.setToggleState(true,  dontSendNotification);
        chordButton2.setToggleState(false, dontSendNotification);
        chordButton3.setToggleState(false, dontSendNotification);
        chordButton4.setToggleState(false, dontSendNotification);
        chordButton5.setToggleState(false, dontSendNotification);
        chordButton6.setToggleState(false, dontSendNotification);
        chordButton7.setToggleState(false, dontSendNotification);
        chordButton1.onClick = [this] { chordButton1.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(1); };
        chordButton2.onClick = [this] { chordButton2.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(2); };
        chordButton3.onClick = [this] { chordButton3.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(3); };
        chordButton4.onClick = [this] { chordButton4.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(4); };
        chordButton5.onClick = [this] { chordButton5.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(5); };
        chordButton6.onClick = [this] { chordButton6.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(6); };
        chordButton7.onClick = [this] { chordButton7.setToggleState(true, dontSendNotification); circleDiagram.setNodePath(7); };
        
        modeButton1.setRadioGroupId(2);
        modeButton2.setRadioGroupId(2);
        modeButton3.setRadioGroupId(2);
        modeButton4.setRadioGroupId(2);
        modeButton5.setRadioGroupId(2);
        modeButton6.setRadioGroupId(2);
        modeButton7.setRadioGroupId(2);
        modeButton1.setToggleState(true,  dontSendNotification);
        modeButton2.setToggleState(false, dontSendNotification);
        modeButton3.setToggleState(false, dontSendNotification);
        modeButton4.setToggleState(false, dontSendNotification);
        modeButton5.setToggleState(false, dontSendNotification);
        modeButton6.setToggleState(false, dontSendNotification);
        modeButton7.setToggleState(false, dontSendNotification);
        modeButton1.onClick = [this] { modeButton1.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::ionian); };
        modeButton2.onClick = [this] { modeButton2.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::dorian); };
        modeButton3.onClick = [this] { modeButton3.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::phrygian); };
        modeButton4.onClick = [this] { modeButton4.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::lydian); };
        modeButton5.onClick = [this] { modeButton5.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::mixolydian); };
        modeButton6.onClick = [this] { modeButton6.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::aeolian); };
        modeButton7.onClick = [this] { modeButton7.setToggleState(true, dontSendNotification); circleDiagram.setNodesDiatonicToMode(MelodicMajorModes::locrian); };
    }

    ~MainComponent() { shutdownAudio(); }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override {}
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override { bufferToFill.clearActiveBufferRegion(); }
    void releaseResources()                                             override {}

//==============================================================================
    void paint (Graphics& g) override {}

    void resized() override
    {
        circleDiagram.setBounds           (100,  100, 1000, 1000);
        distanceFromCentreSlider.setBounds(10,   10,  80,   1180);
        rotateNodesSlider.setBounds       (1110, 10,  80,   1180);
        
        Rectangle<int> chordButtonsBounds = Rectangle<int>(110, 1110, 980, 80);
        FlexBox chordButtonsFlexBox;
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton1));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton2));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton3));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton4));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton5));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton6));
        chordButtonsFlexBox.items.add(FlexItem(80, 80, chordButton7));
        chordButtonsFlexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
        chordButtonsFlexBox.performLayout(chordButtonsBounds);
        
        Rectangle<int> modeButtonsBounds = Rectangle<int>(110, 10, 980, 80);
        FlexBox modeButtonsFlexBox;
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton1));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton2));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton3));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton4));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton5));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton6));
        modeButtonsFlexBox.items.add(FlexItem(80, 80, modeButton7));
        modeButtonsFlexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
        modeButtonsFlexBox.performLayout(modeButtonsBounds);
    }
//==============================================================================

private:
    void setCircleDiagramDistanceFromCentre(CircleDiagramComponent& circleDiagramToSet, float valueToSet)
    {
        circleDiagramToSet.setDistanceFromCentre(valueToSet);
    }
    
    //==============================================================================

    CircleDiagramComponent circleDiagram;
    
    Slider distanceFromCentreSlider;
    Slider rotateNodesSlider;
    
    TextButton chordButton1 {"I"};
    TextButton chordButton2 {"ii"};
    TextButton chordButton3 {"iii"};
    TextButton chordButton4 {"IV"};
    TextButton chordButton5 {"V"};
    TextButton chordButton6 {"vi"};
    TextButton chordButton7 {"vii"};
    
    
    TextButton modeButton1 {"ionian"};
    TextButton modeButton2 {"dorian"};
    TextButton modeButton3 {"phrygian"};
    TextButton modeButton4 {"lydian"};
    TextButton modeButton5 {"mixolydian"};
    TextButton modeButton6 {"aeolian"};
    TextButton modeButton7 {"locrian"};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
