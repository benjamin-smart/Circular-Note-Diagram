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
        
        distanceFromCentreSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        distanceFromCentreSlider.setRange(0.4f, 0.9f);
        distanceFromCentreSlider.setValue(0.9f);
        distanceFromCentreSlider.onValueChange = [this]{ setCircleDiagramDistanceFromCentre(circleDiagram, (float)distanceFromCentreSlider.getValue()); };
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
