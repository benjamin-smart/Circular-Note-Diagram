/*
  ==============================================================================

    SynthSectionComponent.h
    Created: 20 Jan 2020 3:38:04pm
    Author:  Benjamin Smart

  ==============================================================================
*/
// key value pair - mode to all the mode data


#pragma once
#include "../JuceLibraryCode/JuceHeader.h"
#include "SynthSectionComponent.h"
#include "ScaleData.h"


//==============================================================================
class WavetableOscillator
{
public:
    WavetableOscillator(const AudioSampleBuffer& wavetableToUse)
        : wavetable (wavetableToUse)
    {
        jassert (wavetable.getNumChannels() == 1);
    }

    void setFrequency (float frequency, float sampleRate)
    {
        auto tableSizeOverSampleRate = (float)wavetable.getNumSamples() / sampleRate;
        tableDelta = frequency * tableSizeOverSampleRate;
    }
    
    float getOscillatorGain()
    {
        return oscillatorGain;
    }

    void setOscillatorGain(float gain)
    {
        oscillatorGain = gain;
    }

    forcedinline float getNextSample() noexcept
    {
        auto tableSize = wavetable.getNumSamples();
        
        auto index0 = (unsigned int) currentIndex;
        auto index1 = index0 == (tableSize - 1) ? (unsigned int) 0 : index0 + 1; // if (index0 == 127) set to 0, else set to index0+1
        
        auto frac = currentIndex - (float) index0;
        
        auto* table = wavetable.getReadPointer(0);
        auto value0 = table[index0];
        auto value1 = table[index1];
        
        auto currentSample = value0 + frac * (value1 - value0); // interpolate between the values...
        
        if ((currentIndex += tableDelta) > tableSize)
            currentIndex -= tableSize;
        
        return currentSample;
    }

private:
    const AudioSampleBuffer& wavetable;
    float currentIndex = 0.0f, tableDelta = 0.0f, oscillatorGain = 0.0f;
};

//==============================================================================
class SynthAudioSource : public AudioSource
{
public:
    SynthAudioSource()
    {
        createWavetable();
        updateSynthMidiMode(currentMode);
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        synthSampleRate = sampleRate;
        
        auto* fundamentalOscillator = new WavetableOscillator(sineTable);
        auto* firstHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* secondHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* thirdHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* fourthHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* fifthHarmonicOscillator = new WavetableOscillator(sineTable);
        
        oscillators.add(fundamentalOscillator);
        oscillators.add(firstHarmonicOscillator);
        oscillators.add(secondHarmonicOscillator);
        oscillators.add(thirdHarmonicOscillator);
        oscillators.add(fourthHarmonicOscillator);
        oscillators.add(fifthHarmonicOscillator);
        
        auto* thirdChordToneOscillator = new WavetableOscillator(sineTable);
        auto* fifthChordToneOscillator = new WavetableOscillator(sineTable);
        oscillatorsThird.add(thirdChordToneOscillator);
        oscillatorsFifth.add(fifthChordToneOscillator);

        level = 0.25f / (oscillators.size() + oscillatorsThird.size() + oscillatorsFifth.size());
        setSynthFrequency(midiNote, midiNoteThird, midiNoteFifth);
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
        auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);
        
        for (auto oscillatorIndex = 0; oscillatorIndex < oscillators.size(); ++oscillatorIndex)
        {
            auto* oscillator = oscillators.getUnchecked(oscillatorIndex);
            auto oscillatorGain = oscillator->getOscillatorGain();
            
            for (auto sampleIndex = 0; sampleIndex < bufferToFill.numSamples; ++sampleIndex)
            {
                auto levelSample = oscillator->getNextSample() * level;
                
                leftBuffer[sampleIndex] += (levelSample * oscillatorGain);
                rightBuffer[sampleIndex] += (levelSample * oscillatorGain);
            }
        }
        
        auto* oscillatorThird = oscillatorsThird.getUnchecked(0);
//        auto oscillatorThirdGain = oscillatorThird->getOscillatorGain();
        auto* oscillatorFifth = oscillatorsFifth.getUnchecked(0);
//        auto oscillatorFifthGain = oscillatorFifth->getOscillatorGain();

        for (auto sampleIndex = 0; sampleIndex < bufferToFill.numSamples; ++sampleIndex)
        {
            auto levelSampleThird = oscillatorThird->getNextSample() * level;
            auto levelSampleFifth = oscillatorFifth->getNextSample() * level;

            leftBuffer[sampleIndex] += (levelSampleThird * levelThird);
            rightBuffer[sampleIndex] += (levelSampleThird * levelThird);
            
            leftBuffer[sampleIndex] += (levelSampleFifth * levelFifth);
            rightBuffer[sampleIndex] += (levelSampleFifth * levelFifth);
        }
    }
    
    void releaseResources() override {}
    
//==============================================================================
    void createWavetable()
    {
        sineTable.setSize(1, tableSize);
        auto* samples = sineTable.getWritePointer(0);
        auto angleDelta = MathConstants<double>::twoPi / (double) (tableSize - 1);
        auto currentAngle = 0.0;
        
        for (auto i = 0; i < tableSize; ++i)
        {
            auto sample = std::sin(currentAngle);
            samples[i] = (float) sample;
            currentAngle += angleDelta;
        }
    }
    
    void setSynthFrequency(int requestedMidiNote, int requestedMidiThird, int requestedMidiFifth)
    {
        auto requestedFrequencyRoot  = 440.0 * pow(2.0, (requestedMidiNote - 69) / 12.0);
        auto requestedFrequencyThird = 440.0 * pow(2.0, (requestedMidiThird - 69) / 12.0);
        auto requestedFrequencyFifth = 440.0 * pow(2.0, (requestedMidiFifth - 69) / 12.0);
        
        for (int i = 0; i < oscillators.size(); ++i)
        {
            auto* oscillatorRoot = oscillators.getUnchecked(i);
            oscillatorRoot->setFrequency(requestedFrequencyRoot * (i + 1), synthSampleRate);
        }
        
        for (int i = 0; i < oscillatorsThird.size(); ++i)
        {
            auto* oscillatorThird = oscillatorsThird.getUnchecked(i);
            oscillatorThird->setFrequency(requestedFrequencyThird * (i + 1), synthSampleRate);
        }
        
        for (int i = 0; i < oscillatorsFifth.size(); ++i)
        {
            auto* oscillatorFifth = oscillatorsFifth.getUnchecked(i);
            oscillatorFifth->setFrequency(requestedFrequencyFifth * (i + 1), synthSampleRate);
        }
    }
    
    void setOscillatorGainByIndex(int uncheckedOscillatorIndex, float gainToSet)
    {
//        if (uncheckedOscillatorIndex < (oscillators.size() - 1) )
        {
            auto* oscillator = oscillators.getUnchecked(uncheckedOscillatorIndex);
            oscillator->setOscillatorGain(gainToSet);
        }
    }

    void setThirdLevel(float levelToSet)
    {
        levelThird = levelToSet;
    }
    void setFifthLevel(float levelToSet)
    {
        levelFifth = levelToSet;
    }

    void updateSynthMidiNoteState(MelodicMajorModes currentMode, int currentChord)
    {
        switch (currentMode)
        {
            case (MelodicMajorModes::ionian):
                midiNote = ionianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::dorian):
                midiNote = dorianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::phrygian):
                midiNote = phrygianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::lydian):
                midiNote = lydianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::mixolydian):
                midiNote = mixolydianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::aeolian):
                midiNote = aeolianMidiNotes[currentChord-1];
                break;
            case (MelodicMajorModes::locrian):
                midiNote = locrianMidiNotes[currentChord-1];
                break;
        }
        setSynthFrequency(midiNote, midiNoteThird, midiNoteFifth);
    }

    void updateSynthMidiMode(MelodicMajorModes newMode)
    {
        switch (newMode)
        {
            case (MelodicMajorModes::ionian):
                currentMidiNotes = ionianMidiNotes;
                break;
            case (MelodicMajorModes::dorian):
                currentMidiNotes = dorianMidiNotes;
                break;
            case (MelodicMajorModes::phrygian):
                currentMidiNotes = phrygianMidiNotes;
                break;
            case (MelodicMajorModes::lydian):
                currentMidiNotes = lydianMidiNotes;
                break;
            case (MelodicMajorModes::mixolydian):
                currentMidiNotes = mixolydianMidiNotes;
                break;
            case (MelodicMajorModes::aeolian):
                currentMidiNotes = aeolianMidiNotes;
                break;
            case (MelodicMajorModes::locrian):
                currentMidiNotes = locrianMidiNotes;
                break;
        }
        updateSynthMidiChord(currentChord);
    }
    
    void updateSynthMidiChord(int newChord)
    {
//        switch (newChord)   //this is compressed within one octave - revised switch statement below gives us root position triads up through the second octave.
//        {
//            case (1):
//                midiNote      = currentMidiNotes[0];
//                midiNoteThird = currentMidiNotes[2];
//                midiNoteFifth = currentMidiNotes[4];
//                break;
//            case (2):
//                midiNote      = currentMidiNotes[1];
//                midiNoteThird = currentMidiNotes[3];
//                midiNoteFifth = currentMidiNotes[5];
//                break;
//            case (3):
//                midiNote      = currentMidiNotes[2];
//                midiNoteThird = currentMidiNotes[4];
//                midiNoteFifth = currentMidiNotes[6];
//                break;
//            case (4):
//                midiNote      = currentMidiNotes[3];
//                midiNoteThird = currentMidiNotes[5];
//                midiNoteFifth = currentMidiNotes[0];
//                break;
//            case (5):
//                midiNote      = currentMidiNotes[4];
//                midiNoteThird = currentMidiNotes[6];
//                midiNoteFifth = currentMidiNotes[1];
//                break;
//            case (6):
//                midiNote      = currentMidiNotes[5];
//                midiNoteThird = currentMidiNotes[0];
//                midiNoteFifth = currentMidiNotes[2];
//                break;
//            case (7):
//                midiNote      = currentMidiNotes[6];
//                midiNoteThird = currentMidiNotes[1];
//                midiNoteFifth = currentMidiNotes[3];
//                break;
//        }
        
        switch (newChord)
        {
            case (1):
                midiNote      = currentMidiNotes[0];
                midiNoteThird = currentMidiNotes[2];
                midiNoteFifth = currentMidiNotes[4];
                break;
            case (2):
                midiNote      = currentMidiNotes[1];
                midiNoteThird = currentMidiNotes[3];
                midiNoteFifth = currentMidiNotes[5];
                break;
            case (3):
                midiNote      = currentMidiNotes[2];
                midiNoteThird = currentMidiNotes[4];
                midiNoteFifth = currentMidiNotes[6];
                break;
            case (4):
                midiNote      = currentMidiNotes[3];
                midiNoteThird = currentMidiNotes[5];
                midiNoteFifth = currentMidiNotes[7];
                break;
            case (5):
                midiNote      = currentMidiNotes[4];
                midiNoteThird = currentMidiNotes[6];
                midiNoteFifth = currentMidiNotes[8];
                break;
            case (6):
                midiNote      = currentMidiNotes[5];
                midiNoteThird = currentMidiNotes[7];
                midiNoteFifth = currentMidiNotes[9];
                break;
            case (7):
                midiNote      = currentMidiNotes[6];
                midiNoteThird = currentMidiNotes[8];
                midiNoteFifth = currentMidiNotes[10];
                break;
        }
        currentChord = newChord;
        setSynthFrequency(midiNote, midiNoteThird, midiNoteFifth);
    }
    
//==============================================================================
private:
    const unsigned int tableSize = 1 << 7; // 128
    AudioSampleBuffer sineTable;
    OwnedArray<WavetableOscillator> oscillators;
    OwnedArray<WavetableOscillator> oscillatorsThird;
    OwnedArray<WavetableOscillator> oscillatorsFifth;
    
    double synthSampleRate;
    float level;
    float levelThird = 0.0f;
    float levelFifth = 0.0f;
    int currentChord = 1;
    MelodicMajorModes currentMode = MelodicMajorModes::ionian;
    
    int midiNote = 48;
    int midiNoteThird = 52;
    int midiNoteFifth = 55;
//    int midiNoteSeventh;
//    int midiNoteNinth;
    
    
    std::array<int, 15>       currentMidiNotes     = { 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0  };
    const std::array<int, 15> ionianMidiNotes      = { 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 65, 67, 69, 71, 72 };
    const std::array<int, 15> dorianMidiNotes      = { 48, 50, 51, 53, 55, 57, 58, 60, 62, 63, 65, 67, 69, 70, 72 };
    const std::array<int, 15> phrygianMidiNotes    = { 48, 49, 51, 53, 55, 56, 58, 60, 61, 63, 65, 67, 68, 70, 72 };
    const std::array<int, 15> lydianMidiNotes      = { 48, 50, 52, 53, 55, 57, 59, 60, 62, 64, 66, 67, 69, 71, 72 };
    const std::array<int, 15> mixolydianMidiNotes  = { 48, 50, 52, 53, 55, 57, 58, 60, 62, 64, 65, 67, 69, 70, 72 };
    const std::array<int, 15> aeolianMidiNotes     = { 48, 50, 51, 53, 55, 56, 58, 60, 62, 63, 65, 67, 68, 70, 72 };
    const std::array<int, 15> locrianMidiNotes     = { 48, 49, 51, 53, 54, 56, 58, 60, 61, 63, 65, 66, 68, 70, 72 };
};
//==============================================================================

 /* Audio happens above this line */

//==============================================================================
class SynthSectionComponent    : public Component
{
public:
    SynthSectionComponent()
    {
        addAndMakeVisible(thirdChordToneSlider);
        addAndMakeVisible(fifthChordToneSlider);
        thirdChordToneSlider.setRange(0.0f, 1.0f);
        fifthChordToneSlider.setRange(0.0f, 1.0f);
        thirdChordToneSlider.setValue(0.0f);
        fifthChordToneSlider.setValue(0.0f);
        thirdChordToneSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        fifthChordToneSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
        thirdChordToneSlider.setNumDecimalPlacesToDisplay(2);
        fifthChordToneSlider.setNumDecimalPlacesToDisplay(2);
        
        thirdChordToneSlider.onValueChange = [this] { synthAudioSource.setThirdLevel((float)thirdChordToneSlider.getValue()); };
        fifthChordToneSlider.onValueChange = [this] { synthAudioSource.setFifthLevel((float)fifthChordToneSlider.getValue()); };
        
        
        addAndMakeVisible(fundamentalOscillatorGainSlider);
        addAndMakeVisible(firstHarmonicGainSlider);
        addAndMakeVisible(secondHarmonicGainSlider);
        addAndMakeVisible(thirdHarmonicGainSlider);
        addAndMakeVisible(fourthHarmonicGainSlider);
        addAndMakeVisible(fifthHarmonicGainSlider);
        
        fundamentalOscillatorGainSlider.setRange(0.0f, 1.0f);
        firstHarmonicGainSlider .setRange(0.0f, 0.8f);
        secondHarmonicGainSlider.setRange(0.0f, 0.8f);
        thirdHarmonicGainSlider .setRange(0.0f, 0.8f);
        fourthHarmonicGainSlider.setRange(0.0f, 0.7f);
        fifthHarmonicGainSlider .setRange(0.0f, 0.6f);
        
        fundamentalOscillatorGainSlider.setValue(0.0f);
        firstHarmonicGainSlider .setValue(0.0f);
        secondHarmonicGainSlider.setValue(0.0f);
        thirdHarmonicGainSlider .setValue(0.0f);
        fourthHarmonicGainSlider.setValue(0.0f);
        fifthHarmonicGainSlider .setValue(0.0f);
        
        fundamentalOscillatorGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        firstHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        secondHarmonicGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        thirdHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        fourthHarmonicGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        fifthHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        
        fundamentalOscillatorGainSlider.setNumDecimalPlacesToDisplay(2);
        firstHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        secondHarmonicGainSlider.setNumDecimalPlacesToDisplay(2);
        thirdHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        fourthHarmonicGainSlider.setNumDecimalPlacesToDisplay(2);
        fifthHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        
        fundamentalOscillatorGainSlider.onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(0, (float)fundamentalOscillatorGainSlider.getValue()); };
        firstHarmonicGainSlider .onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(1, (float)firstHarmonicGainSlider .getValue()); };
        secondHarmonicGainSlider.onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(2, (float)secondHarmonicGainSlider.getValue()); };
        thirdHarmonicGainSlider .onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(3, (float)thirdHarmonicGainSlider .getValue()); };
        fourthHarmonicGainSlider.onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(4, (float)fourthHarmonicGainSlider.getValue()); };
        fifthHarmonicGainSlider .onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(5, (float)fifthHarmonicGainSlider .getValue()); };
    }

//==============================================================================
    void paint (Graphics& g) override
    {
        g.fillAll (Colours::darkgrey);
        g.setColour (Colours::maroon);
        g.drawRect (getLocalBounds(), 1);
        g.setColour (Colours::lightgrey);
        g.setFont (28.0f);
//        g.drawText ("synth section", getLocalBounds().removeFromBottom(getHeight()/2), Justification::centred, true);
    }

    void resized() override
    {
        Rectangle<int> harmonicSlidersBounds = Rectangle<int>(10,10, getWidth() - 20, getHeight()/2 - 20);
        FlexBox harmonicSlidersFlexBox;
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), fundamentalOscillatorGainSlider));
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), firstHarmonicGainSlider));
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), secondHarmonicGainSlider));
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), thirdHarmonicGainSlider));
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), fourthHarmonicGainSlider));
        harmonicSlidersFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), fifthHarmonicGainSlider));
        harmonicSlidersFlexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
        harmonicSlidersFlexBox.performLayout(harmonicSlidersBounds);
        
        
        Rectangle<int> chordToneSlidersBounds = Rectangle<int>(10, getHeight()/2 + 10 , getWidth() - 20, getHeight()/2 - 20);
        FlexBox chordToneSlidersFlexBox;
        chordToneSlidersFlexBox.items.add(FlexItem(chordToneSlidersBounds.getWidth()/2, chordToneSlidersBounds.getHeight(), thirdChordToneSlider));
        chordToneSlidersFlexBox.items.add(FlexItem(chordToneSlidersBounds.getWidth()/2, chordToneSlidersBounds.getHeight(), fifthChordToneSlider));
        chordToneSlidersFlexBox.performLayout(chordToneSlidersBounds);
        
    }
//==============================================================================
public:
    SynthAudioSource synthAudioSource;
private:
    Slider fundamentalOscillatorGainSlider;
    Slider firstHarmonicGainSlider;
    Slider secondHarmonicGainSlider;
    Slider thirdHarmonicGainSlider;
    Slider fourthHarmonicGainSlider;
    Slider fifthHarmonicGainSlider;
    
    Slider thirdChordToneSlider;
    Slider fifthChordToneSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthSectionComponent)
};
