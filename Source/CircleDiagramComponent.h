#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "CircleDiagramComponent.h"
#include <array>

// why is the root note selection circle slightly misaligned on the right side of the diagram?

class NodeComponent : public Component
{
public:
    NodeComponent()
    {
        addAndMakeVisible(noteNameLabel);
//        addAndMakeVisible(nodeButton);
        noteNameLabel.setColour(0x1000281, Colours::black);
//        noteNameLabel.setFont(Font::)
    }
    
    void paint (Graphics& g) override
    {
//        g.setColour(Colours::wheat);
        g.setColour(isDiatonic ? Colours::wheat : Colours::darkgrey);
        Rectangle<float> nodeRect = getLocalBounds().toFloat().reduced(5.0f);
        g.fillEllipse(nodeRect);
        
        if (isDiatonic)
        {
            Rectangle<float> diatonicIndicatorArea = getLocalBounds().toFloat().expanded(5.0f, 5.0f);
            g.setColour(Colours::lightblue);
            g.drawEllipse(diatonicIndicatorArea, 1.5f);
        }
    }
    
    void resized () override
    {
        noteNameLabel.setBounds(10, 5, getWidth()-5, getHeight()-5);
//        nodeCentre.setXY((float)((getWidth()-1) /2), (float)((getHeight()-1)/2));
    }
    
    void setText(const String& newText)
    {
        noteNameLabel.setText(newText, dontSendNotification);
    }
    
    
    bool isDiatonic = false;

private:
    Label noteNameLabel;
    String noteNameString;
//    Point<float> nodeCentre;
//    ShapeButton nodeButton;
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
        
        setNodesDiatonicToMode();
        
    }
    
    void paint(Graphics& g) override
    {
        g.fillAll(Colours::black);

        // draw circle and centre
        g.setColour(Colours::white);
        g.drawEllipse((getLocalBounds().toFloat()).reduced(50), 2);
        g.drawEllipse((float)(centrePoint.getX()-5), (float)(centrePoint.getY()-5), 9, 9, 1);
        
        
        // make and fill path
        g.setColour(Colours::darkgrey);
        Point<float> rootPoint  = nodePoints[nodePathIndices[0]];
        Point<float> thirdPoint = nodePoints[nodePathIndices[1]];
        Point<float> fifthPoint = nodePoints[nodePathIndices[2]];
        Path nodePath;
        nodePath.addTriangle(rootPoint, thirdPoint, fifthPoint);
        g.strokePath(nodePath, PathStrokeType(1.5f));
        g.setOpacity(0.5f);
        g.fillPath(nodePath);
        
        // draw circles around path points
        g.setColour(Colours::lightgrey);
        g.drawEllipse(rootPoint.getX()-25, rootPoint.getY()-25, 50.0f, 50.0f, 4.0f);
//        g.setColour(Colours::maroon);
        g.drawEllipse(thirdPoint.getX()-25, thirdPoint.getY()-25, 50.0f, 50.0f, 1.0f);
//        g.setColour(Colours::purple);
        g.drawEllipse(fifthPoint.getX()-25, fifthPoint.getY()-25, 50.0f, 50.0f, 1.0f);
    }
    
    void makeDiatonicMajor ()
    {
        
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
    //==============================================================================

    
    void setNodePath(int newPath)  // this is just hard-coded to find paths for diatonic major triads - i need to change this to use isDiatonic to generate a path based on what scale we're in.   to get a root position triad, we need every other node that "isDiatonic" - only the first three, then we shove that in here.  ternary operators?
    {
//        unsigned int diatonicIndices[7];  // maybe we can make an array of the valid indices for our current mode, then put those into hardcoded note sequences in the switch statement below
//        for (int i = 0; i < 7; ++i)
//        {
//            if (currentMode == MelodicMajorModes::ionian)
//            {
//
//            }
//        }
        
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

    
    //==============================================================================

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
    
    
    void setDistanceFromCentre(float valueToSet)
    {
        distanceFromCentre = valueToSet;
//        initializeNodes(radius); // is called in resized
        resized();
    }
    
    void rotateNodes(float newPlacementAngle)
    {
        currentPlacementAngle = newPlacementAngle;
        initializeNodes(radius);
    }
    
    void setNodesDiatonicToMode() // let me know if you have a better way to do this...
    {
        if (currentMode == MelodicMajorModes::ionian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = false;
            node3. isDiatonic = true;
            node4. isDiatonic = false;
            node5. isDiatonic = true;
            node6. isDiatonic = true;
            node7. isDiatonic = false;
            node8. isDiatonic = true;
            node9. isDiatonic = false;
            node10.isDiatonic = true;
            node11.isDiatonic = false;
            node12.isDiatonic = true;
        }
        else if (currentMode == MelodicMajorModes::dorian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = false;
            node3. isDiatonic = true;
            node4. isDiatonic = true;
            node5. isDiatonic = false;
            node6. isDiatonic = true;
            node7. isDiatonic = false;
            node8. isDiatonic = true;
            node9. isDiatonic = false;
            node10.isDiatonic = true;
            node11.isDiatonic = true;
            node12.isDiatonic = false;
        }
        else if (currentMode == MelodicMajorModes::phrygian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = true;
            node3. isDiatonic = false;
            node4. isDiatonic = true;
            node5. isDiatonic = false;
            node6. isDiatonic = true;
            node7. isDiatonic = false;
            node8. isDiatonic = true;
            node9. isDiatonic = true;
            node10.isDiatonic = false;
            node11.isDiatonic = true;
            node12.isDiatonic = false;
        }
        else if (currentMode == MelodicMajorModes::lydian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = false;
            node3. isDiatonic = true;
            node4. isDiatonic = false;
            node5. isDiatonic = true;
            node6. isDiatonic = false;
            node7. isDiatonic = true;
            node8. isDiatonic = true;
            node9. isDiatonic = false;
            node10.isDiatonic = true;
            node11.isDiatonic = false;
            node12.isDiatonic = true;
        }
        else if (currentMode == MelodicMajorModes::mixolydian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = false;
            node3. isDiatonic = true;
            node4. isDiatonic = false;
            node5. isDiatonic = true;
            node6. isDiatonic = true;
            node7. isDiatonic = false;
            node8. isDiatonic = true;
            node9. isDiatonic = false;
            node10.isDiatonic = true;
            node11.isDiatonic = true;
            node12.isDiatonic = false;
        }
        else if (currentMode == MelodicMajorModes::aeolian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = false;
            node3. isDiatonic = true;
            node4. isDiatonic = true;
            node5. isDiatonic = false;
            node6. isDiatonic = true;
            node7. isDiatonic = false;
            node8. isDiatonic = true;
            node9. isDiatonic = true;
            node10.isDiatonic = false;
            node11.isDiatonic = true;
            node12.isDiatonic = false;
        }
        else if (currentMode == MelodicMajorModes::locrian)
        {
            node1. isDiatonic = true;
            node2. isDiatonic = true;
            node3. isDiatonic = false;
            node4. isDiatonic = true;
            node5. isDiatonic = false;
            node6. isDiatonic = true;
            node7. isDiatonic = true;
            node8. isDiatonic = false;
            node9. isDiatonic = true;
            node10.isDiatonic = false;
            node11.isDiatonic = true;
            node12.isDiatonic = false;
        }
    }
    
    
public:
    enum class MelodicMajorModes
    {
        ionian,
        dorian,
        phrygian,
        lydian,
        mixolydian,
        aeolian,
        locrian
    };
    
    /*
     
     enum class ParentScales2K
     {
        melodicMajor,
        melodicMinor,
        harmonicMajor,
        harmonicMinor,
        hungarianMajor,
        melodicFlatFive,
     };
     enum class ParentScales3K
     {
         hungarianMinor,
         poorvi,
         todi,
         maarva,
         bluessy,
         neapolitan minor,
         malini,
     };
     enum class ParentScales4K
     {
        enigmaticMinor,
        enigmaticDescendant,
        enigmaticAscendant,
        lydianSharp3Flat6
     };
     */
    
    float radius;
    float distanceFromCentre = 0.9f;;
    float currentPlacementAngle = MathConstants<float>::halfPi + MathConstants<float>::pi;
    static constexpr int numberOfNodes = 12;
    
    
private:
    
    double nodePlacementAngleDelta; // in radians, used to find placement of nodes around circle
    
    std::array<float, 12> nodeXValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<float, 12> nodeYValues = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f}; // absolute location of nodes
    std::array<Point<float>, 12> nodePoints; // point objects used to draw paths
    std::array<int, 3> nodePathIndices    = {0, 4, 7}; // index 0 to 11 of which nodes to draw lines between
    Point<int> centrePoint;

    //    std::array<Path, 7>   scalePaths      = { } // preallocated array of paths?
    
    
    MelodicMajorModes currentMode { MelodicMajorModes::ionian };
    
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
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CircleDiagramComponent)
};
