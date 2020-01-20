#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <array>

// - changed all variable names "note" that should have been "node" (and noteNode -> node etc).  Node is the spot of the circle.  Note is the musical note.

class NodeComponent : public Component
{
public:
    NodeComponent()
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
        nodeCentre.setXY((float)((getWidth()-1) /2), (float)((getHeight()-1)/2));
    }
    
    void setText(const String& newText)
    {
        noteNameLabel.setText(newText, dontSendNotification);
    }
    
private:
    Label noteNameLabel;
    String noteNameString;
    Point<float> nodeCentre;
};


class CircleDiagramComponent : public Component
{
  
public:

    CircleDiagramComponent ()
    {
        node1. setText(" C");
        node2. setText("C#");
        node3. setText(" D");
        node4. setText("D#");
        node5. setText(" E");
        node6. setText(" F");
        node7. setText("F#");
        node8. setText(" G");
        node9. setText("G#");
        node10.setText(" A");
        node11.setText("A#");
        node12.setText(" B");
        
        addAndMakeVisible(node1);
        addAndMakeVisible(node2);
        addAndMakeVisible(node3);
        addAndMakeVisible(node4);
        addAndMakeVisible(node5);
        addAndMakeVisible(node6);
        addAndMakeVisible(node7);
        addAndMakeVisible(node8);
        addAndMakeVisible(node9);
        addAndMakeVisible(node10);
        addAndMakeVisible(node11);
        addAndMakeVisible(node12);
        addAndMakeVisible(centreNode);
        setNodePath(1);
        
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
        Path nodePath;
        nodePath.addTriangle(rootPoint, thirdPoint, fifthPoint);
        g.strokePath(nodePath, PathStrokeType(1.5f));
        g.setOpacity(0.5f);
        g.fillPath(nodePath);
        
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
 // i forget what I meant by this but I think its important...
 if (nodePathIndices[0] == 0 || nodePathIndices[1] == 0 || nodePathIndices[2] == 0)
    // play C
 if (nodePathIndices[0] == 1 || nodePathIndices[1] == 1 || nodePathIndices[2] == 1)
    // play C#
 if (nodePathIndices[0] == 1 || nodePathIndices[1] == 1 || nodePathIndices[2] == 1)
 
 */
    
    
    void setNodePath(int newPath)
    {
        switch (newPath) // chord numbers 1-7 become node indices 0-11 here
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
        nodePlacementAngleDelta = MathConstants<float>::twoPi / (float) numberOfNodes;
//        float currentPlacementAngle = MathConstants<float>::halfPi + MathConstants<float>::pi; // (moved to member variable for rotate slider)
        
        String initializeNodesText = String("currentPlacementAngle:") + String(currentPlacementAngle) + newLine +
                                            "nodePlacementAngleDelta:" + String(nodePlacementAngleDelta) + newLine;;
        DBG(initializeNodesText);

        
        // find node locations...
        for (int i = 0; i < numberOfNodes; i++)
        {
            float cosineOfAngle = std::cos(currentPlacementAngle);
            float sineOfAngle = std::sin(currentPlacementAngle);
            float currentX = (cosineOfAngle * radius * distanceFromCentre) + radius;
            float currentY = (sineOfAngle   * radius * distanceFromCentre) + radius;
            nodeXValues[i] = currentX;
            nodeYValues[i] = currentY;

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
            
            currentPlacementAngle += nodePlacementAngleDelta;
        }
    
        node1 .setBounds(nodeXValues[0]  - 25, nodeYValues[0]  - 25, 50, 50);
        node2 .setBounds(nodeXValues[1]  - 25, nodeYValues[1]  - 25, 50, 50);
        node3 .setBounds(nodeXValues[2]  - 25, nodeYValues[2]  - 25, 50, 50);
        node4 .setBounds(nodeXValues[3]  - 25, nodeYValues[3]  - 25, 50, 50);
        node5 .setBounds(nodeXValues[4]  - 25, nodeYValues[4]  - 25, 50, 50);
        node6 .setBounds(nodeXValues[5]  - 25, nodeYValues[5]  - 25, 50, 50);
        node7 .setBounds(nodeXValues[6]  - 25, nodeYValues[6]  - 25, 50, 50);
        node8 .setBounds(nodeXValues[7]  - 25, nodeYValues[7]  - 25, 50, 50);
        node9 .setBounds(nodeXValues[8]  - 25, nodeYValues[8]  - 25, 50, 50);
        node10.setBounds(nodeXValues[9]  - 25, nodeYValues[9]  - 25, 50, 50);
        node11.setBounds(nodeXValues[10] - 25, nodeYValues[10] - 25, 50, 50);
        node12.setBounds(nodeXValues[11] - 25, nodeYValues[11] - 25, 50, 50);
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
    
    double nodePlacementAngleDelta; // in radians, used to find placement of nodes around circle
    
    std::array<float, 12> nodeXValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<float, 12> nodeYValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<Point<float>, 12> nodePoints; // point objects used to draw paths
    std::array<int, 3> nodePathIndices    = {0, 4, 7}; // index 0 to 11 of which nodes to draw lines between
//    std::array<Path, 7>   scalePaths      = { } // preallocated array of paths?
    
    
    
    
//    ==============================================================================
    NodeComponent centreNode;
    NodeComponent node1;
    NodeComponent node2;
    NodeComponent node3;
    NodeComponent node4;
    NodeComponent node5;
    NodeComponent node6;
    NodeComponent node7;
    NodeComponent node8;
    NodeComponent node9;
    NodeComponent node10;
    NodeComponent node11;
    NodeComponent node12;
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
                                        circleDiagram.setNodePath(1); };
        chordButton2.onClick = [this] { chordButton2.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(2); };
        chordButton3.onClick = [this] { chordButton3.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(3); };
        chordButton4.onClick = [this] { chordButton4.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(4); };
        chordButton5.onClick = [this] { chordButton5.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(5); };
        chordButton6.onClick = [this] { chordButton6.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(6); };
        chordButton7.onClick = [this] { chordButton7.setToggleState(true, dontSendNotification);
                                        circleDiagram.setNodePath(7); };
        

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
