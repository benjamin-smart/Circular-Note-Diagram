#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

/*
 todo:
 synth voice for each node
 
 MIDI:
 - array of notes in scale to iterate through patterns
 - duplicate up octaves
 SYNTH:
 - plays currently selected sequence of notes
 
 make nodes light up when they are playing
 sync with external source / incoming midi - play sequences into your sessions & songs
 
 
 done:
 path generator
 7 diatonic chord buttons in flexbox at bottom that generate paths
    - combobox selection menu to choose parent scale
 array of paths to represent different chords
 way of representing intervallic structure of scales
 */

class NoteNodeComponent : public Component
{
public:
    NoteNodeComponent()
    {
        addAndMakeVisible(noteNameLabel);
        noteNameLabel.setColour(0x1000281, Colours::black);
    }
    
    void paint (Graphics& g) override
    {
        g.setColour(Colours::wheat);
        Rectangle<float> nodeRect = getLocalBounds().toFloat().reduced(5);
        g.fillEllipse(nodeRect);
    }
    
    void resized () override
    {
        noteNameLabel.setBounds(10, 5, getWidth()-5, getHeight()-5);
        noteNodeCentre.setXY((float)((getWidth()-1) /2), (float)((getHeight()-1)/2));
    }
    
    void setText(const String& newText)
    {
        noteNameLabel.setText(newText, dontSendNotification);
    }
    
private:
    Label noteNameLabel;
    String noteNameString;
    Point<float> noteNodeCentre;
};


class CircleDiagramComponent : public Component
{
  
public:

    CircleDiagramComponent ()
    {
        noteNode1. setText(" C");
        noteNode2. setText("C#");
        noteNode3. setText(" D");
        noteNode4. setText("D#");
        noteNode5. setText(" E");
        noteNode6. setText(" F");
        noteNode7. setText("F#");
        noteNode8. setText(" G");
        noteNode9. setText("G#");
        noteNode10.setText(" A");
        noteNode11.setText("A#");
        noteNode12.setText(" B");
        
        addAndMakeVisible(noteNode1);
        addAndMakeVisible(noteNode2);
        addAndMakeVisible(noteNode3);
        addAndMakeVisible(noteNode4);
        addAndMakeVisible(noteNode5);
        addAndMakeVisible(noteNode6);
        addAndMakeVisible(noteNode7);
        addAndMakeVisible(noteNode8);
        addAndMakeVisible(noteNode9);
        addAndMakeVisible(noteNode10);
        addAndMakeVisible(noteNode11);
        addAndMakeVisible(noteNode12);
        addAndMakeVisible(centreNode);
        setNotePath(1);
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::black);
        g.setColour(Colours::white);
        g.drawEllipse((getLocalBounds().toFloat()).reduced(50), 2);
        g.drawEllipse((float)(centrePoint.getX()-5), (float)(centrePoint.getY()-5), 9, 9, 1);
        
        Point<float> rootPoint  = nodePoints[nodePathIndices[0]];
        Point<float> thirdPoint = nodePoints[nodePathIndices[1]];
        Point<float> fifthPoint = nodePoints[nodePathIndices[2]];
        Path notePath;
        notePath.addTriangle(rootPoint, thirdPoint, fifthPoint);
        g.strokePath(notePath, PathStrokeType(1.5f));
        g.setOpacity(0.5f);
        g.fillPath(notePath);
        
        g.setColour(Colours::red);
        g.drawEllipse(rootPoint.getX()-25, rootPoint.getY()-25, 50.0f, 50.0f, 4.0f);
        g.setColour(Colours::maroon);
        g.drawEllipse(thirdPoint.getX()-25, thirdPoint.getY()-25, 50.0f, 50.0f, 4.0f);
        g.setColour(Colours::purple);
        g.drawEllipse(fifthPoint.getX()-25, fifthPoint.getY()-25, 50.0f, 50.0f, 4.0f);
    }
    
    void resized() override
    {
        radius = (float)(getWidth()/2);
        centrePoint.setX((getWidth()  -1 ) / 2);
        centrePoint.setY((getHeight() -1 ) / 2);
        initializeNodes(radius);
    }
    
    //==============================================================================
/*
 if (notePathIndices[0] == 0 || notePathIndices[1] == 0 || notePathIndices[2] == 0)
    // play C
 if (notePathIndices[0] == 1 || notePathIndices[1] == 1 || notePathIndices[2] == 1)
    // play C#
 if (notePathIndices[0] == 1 || notePathIndices[1] == 1 || notePathIndices[2] == 1)
 
 */
    
    
    void setNotePath(int newPath)
    {
        switch (newPath) // chord numbers 1-7 become note indices 0-11 here
        {
            case 1:
                nodePathIndices[0] = 0;
                nodePathIndices[1] = 4;
                nodePathIndices[2] = 7;
                break;
            case 2:
                nodePathIndices[0] = 2;
                nodePathIndices[1] = 5;
                nodePathIndices[2] = 9;
                break;
            case 3:
                nodePathIndices[0] = 4;
                nodePathIndices[1] = 7;
                nodePathIndices[2] = 11;
                break;
            case 4:
                nodePathIndices[0] = 5;
                nodePathIndices[1] = 9;
                nodePathIndices[2] = 0;
                break;
            case 5:
                nodePathIndices[0] = 7;
                nodePathIndices[1] = 11;
                nodePathIndices[2] = 2;
                break;
            case 6:
                nodePathIndices[0] = 9;
                nodePathIndices[1] = 0;
                nodePathIndices[2] = 4;
                break;
            case 7:
                nodePathIndices[0] = 11;
                nodePathIndices[1] = 2;
                nodePathIndices[2] = 5;
                break;
        }

        repaint();
    }

    void initializeNodes(float radius)
    {
        notePlacementAngleDelta = MathConstants<float>::twoPi / (float) numberOfNodes;
//        float currentPlacementAngle = MathConstants<float>::halfPi + MathConstants<float>::pi;
        
        String initializeNodesText = String("currentPlacementAngle:") + String(currentPlacementAngle) + newLine +
                                            "notePlacementAngleDelta:" + String(notePlacementAngleDelta) + newLine;;
        DBG(initializeNodesText);

        
        // find node locations...
        for (int i = 0; i < numberOfNodes; i++)
        {
            float cosineOfAngle = std::cos(currentPlacementAngle);
            float sineOfAngle = std::sin(currentPlacementAngle);
            float currentX = (cosineOfAngle * radius * distanceFromCentre) + radius;
            float currentY = (sineOfAngle   * radius * distanceFromCentre) + radius;
            noteNodeXValues[i] = currentX;
            noteNodeYValues[i] = currentY;

            Point<float> currentPoint (currentX, currentY);
            nodePoints[i] = currentPoint;
            
            String findNodesText =
                        "currentPlacementAngle:" + String(currentPlacementAngle) + newLine +
                        "Cosine:"                + String(cosineOfAngle) + newLine +
                        "Cosine:"                + String(cosineOfAngle) + newLine +
                        "Sine:"                  + String(sineOfAngle)   + newLine +
                        "currentX:"              + String((int)currentX) + newLine +
                        "currentY:"              + String((int)currentY) + newLine;
            DBG(findNodesText);
            
            currentPlacementAngle += notePlacementAngleDelta;
        }
    
        noteNode1 .setBounds(noteNodeXValues[0]  - 25, noteNodeYValues[0]  - 25, 50, 50);
        noteNode2 .setBounds(noteNodeXValues[1]  - 25, noteNodeYValues[1]  - 25, 50, 50);
        noteNode3 .setBounds(noteNodeXValues[2]  - 25, noteNodeYValues[2]  - 25, 50, 50);
        noteNode4 .setBounds(noteNodeXValues[3]  - 25, noteNodeYValues[3]  - 25, 50, 50);
        noteNode5 .setBounds(noteNodeXValues[4]  - 25, noteNodeYValues[4]  - 25, 50, 50);
        noteNode6 .setBounds(noteNodeXValues[5]  - 25, noteNodeYValues[5]  - 25, 50, 50);
        noteNode7 .setBounds(noteNodeXValues[6]  - 25, noteNodeYValues[6]  - 25, 50, 50);
        noteNode8 .setBounds(noteNodeXValues[7]  - 25, noteNodeYValues[7]  - 25, 50, 50);
        noteNode9 .setBounds(noteNodeXValues[8]  - 25, noteNodeYValues[8]  - 25, 50, 50);
        noteNode10.setBounds(noteNodeXValues[9]  - 25, noteNodeYValues[9]  - 25, 50, 50);
        noteNode11.setBounds(noteNodeXValues[10] - 25, noteNodeYValues[10] - 25, 50, 50);
        noteNode12.setBounds(noteNodeXValues[11] - 25, noteNodeYValues[11] - 25, 50, 50);
        centreNode.setBounds(radius - 5, radius - 5, 10, 10);
        repaint();
    }
    
    void rotateNodes(float newPlacementAngle)
    {
        currentPlacementAngle = newPlacementAngle;
        initializeNodes(radius);
    }
    
public:
    float radius;
    float distanceFromCentre = 0.9f;;
    float currentPlacementAngle = MathConstants<float>::halfPi + MathConstants<float>::pi;
    static constexpr int numberOfNodes = 12;
    
    Point<int> centrePoint;
    
private:
    
    double notePlacementAngleDelta; // in radians, used to find placement of nodes around circle
    
    std::array<float, 12> noteNodeXValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<float, 12> noteNodeYValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<Point<float>, 12> nodePoints; // point objects used to draw paths
    std::array<int, 3> nodePathIndices    = {0, 4, 7}; // index 0 to 11 of which nodes to draw lines between
//    std::array<Path, 7>   scalePaths      = { } // preallocated array of paths?
    
    
    
    
//    ==============================================================================
    NoteNodeComponent centreNode;
    NoteNodeComponent noteNode1;
    NoteNodeComponent noteNode2;
    NoteNodeComponent noteNode3;
    NoteNodeComponent noteNode4;
    NoteNodeComponent noteNode5;
    NoteNodeComponent noteNode6;
    NoteNodeComponent noteNode7;
    NoteNodeComponent noteNode8;
    NoteNodeComponent noteNode9;
    NoteNodeComponent noteNode10;
    NoteNodeComponent noteNode11;
    NoteNodeComponent noteNode12;
};

//==============================================================================

class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent()
    {
        setSize (1200, 1200);
        setDistanceFromCentre();
        
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
        
        chordButton1.setRadioGroupId(1);
        chordButton2.setRadioGroupId(1);
        chordButton3.setRadioGroupId(1);
        chordButton4.setRadioGroupId(1);
        chordButton5.setRadioGroupId(1);
        chordButton6.setRadioGroupId(1);
        chordButton7.setRadioGroupId(1);

        chordButton1.setToggleState(true, dontSendNotification);
        chordButton2.setToggleState(false, dontSendNotification);
        chordButton3.setToggleState(false, dontSendNotification);
        chordButton4.setToggleState(false, dontSendNotification);
        chordButton5.setToggleState(false, dontSendNotification);
        chordButton6.setToggleState(false, dontSendNotification);
        chordButton7.setToggleState(false, dontSendNotification);
        
        
        chordButton1.onClick = [this] { chordButton1.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(1); };
        chordButton2.onClick = [this] { chordButton2.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(2); };
        chordButton3.onClick = [this] { chordButton3.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(3); };
        chordButton4.onClick = [this] { chordButton4.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(4); };
        chordButton5.onClick = [this] { chordButton5.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(5); };
        chordButton6.onClick = [this] { chordButton6.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(6); };
        chordButton7.onClick = [this] { chordButton7.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNotePath(7); };
        

        distanceFromCentreSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        distanceFromCentreSlider.setRange(0.4f, 0.9f);
        distanceFromCentreSlider.setValue(0.9f);
        distanceFromCentreSlider.onValueChange = [this]{ setDistanceFromCentre(); };
        distanceFromCentreSlider.setNumDecimalPlacesToDisplay(2);
        
        rotateNodesSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        rotateNodesSlider.setRange(0.0f, MathConstants<float>::twoPi);
        rotateNodesSlider.setValue(MathConstants<float>::halfPi + MathConstants<float>::pi);
        rotateNodesSlider.onValueChange = [this]{ circleDiagram.rotateNodes(rotateNodesSlider.getValue()); };
        rotateNodesSlider.setNumDecimalPlacesToDisplay(2);
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
//        chordButtonsFlexBox.alignContent   = FlexBox::AlignContent::spaceAround;
        
        chordButtonsFlexBox.performLayout(chordButtonsBounds);
    }
    
private:
    void setDistanceFromCentre()
    {
        circleDiagram.distanceFromCentre = (float)distanceFromCentreSlider.getValue();
        circleDiagram.initializeNodes(circleDiagram.radius);
        circleDiagram.resized();
    }
    
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
