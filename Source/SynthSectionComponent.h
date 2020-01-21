/*
  ==============================================================================

    SynthSectionComponent.h
    Created: 20 Jan 2020 3:38:04pm
    Author:  Benjamin Smart

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

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
    }
    
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
    {
        synthSampleRate = sampleRate;
        
        auto* rootOscillator = new WavetableOscillator(sineTable);
        auto* firstHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* secondHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* thirdHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* fourthHarmonicOscillator = new WavetableOscillator(sineTable);
        auto* fifthHarmonicOscillator = new WavetableOscillator(sineTable);
        
        oscillators.add(rootOscillator);
        oscillators.add(firstHarmonicOscillator);
        oscillators.add(secondHarmonicOscillator);
        oscillators.add(thirdHarmonicOscillator);
        oscillators.add(fourthHarmonicOscillator);
        oscillators.add(fifthHarmonicOscillator);

        level = 0.25f / oscillators.size();
        setSynthFrequency(midiNote);
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
    void updateLevel(float newLevel)
    {
        level = newLevel;
    }
    
    void setSynthFrequency(int requestedMidiNote)
    {
        auto requestedFrequency = 440.0 * pow(2.0, (requestedMidiNote - 69) / 12.0);
        for (int i = 0; i < oscillators.size(); ++i)
        {
            auto* oscillator = oscillators.getUnchecked(i);
            oscillator->setFrequency(requestedFrequency * (i + 1), synthSampleRate);
        }
    }
    
    void setOscillatorGainByIndex(int uncheckedOscillatorIndex, float gainToSet)
    {
        if (uncheckedOscillatorIndex < (oscillators.size() - 1) )
        {
            auto* oscillator = oscillators.getUnchecked(uncheckedOscillatorIndex);
            oscillator->setOscillatorGain(gainToSet);
        }
    }
    
    void updateSynthMidiNoteState(MelodicMajorModes currentMode, int currentChord)
    {
        switch (currentMode)
        {
            case (MelodicMajorModes::ionian): // can i wrap this current chord variable around 7 and do math on it to get the sequences?
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
        setSynthFrequency(midiNote);
    }
    
//==============================================================================
private:
    const unsigned int tableSize = 1 << 7; // 128
    AudioSampleBuffer sineTable;
    OwnedArray<WavetableOscillator> oscillators;
    
    double synthSampleRate;
    float level;
    
    int midiNote = 48;

    const std::array<int, 7> ionianMidiNotes      = { 48, 50, 52, 53, 55, 57, 59 };
    const std::array<int, 7> dorianMidiNotes      = { 48, 50, 51, 53, 55, 57, 58 };
    const std::array<int, 7> phrygianMidiNotes    = { 48, 49, 51, 53, 55, 56, 58 };
    const std::array<int, 7> lydianMidiNotes      = { 48, 50, 52, 53, 55, 57, 59 };
    const std::array<int, 7> mixolydianMidiNotes  = { 48, 50, 52, 53, 55, 57, 58 };
    const std::array<int, 7> aeolianMidiNotes     = { 48, 50, 51, 53, 55, 56, 58 };
    const std::array<int, 7> locrianMidiNotes     = { 48, 49, 51, 53, 54, 56, 58 };
};
//==============================================================================

 /* Audio happens above this line */

//==============================================================================
class SynthSectionComponent    : public Component
{
public:
    SynthSectionComponent()
    {
        addAndMakeVisible(rootOscillatorGainSlider);
        addAndMakeVisible(firstHarmonicGainSlider);
        addAndMakeVisible(secondHarmonicGainSlider);
        addAndMakeVisible(thirdHarmonicGainSlider);
        addAndMakeVisible(fourthHarmonicGainSlider);
        addAndMakeVisible(fifthHarmonicGainSlider);
        
        rootOscillatorGainSlider.setRange(0.0f, 1.0f);
        firstHarmonicGainSlider .setRange(0.0f, 0.8f);
        secondHarmonicGainSlider.setRange(0.0f, 0.8f);
        thirdHarmonicGainSlider .setRange(0.0f, 0.8f);
        fourthHarmonicGainSlider.setRange(0.0f, 0.7f);
        fifthHarmonicGainSlider .setRange(0.0f, 0.6f);
        
        rootOscillatorGainSlider.setValue(1.0f);
        firstHarmonicGainSlider .setValue(0.0f);
        secondHarmonicGainSlider.setValue(0.0f);
        thirdHarmonicGainSlider .setValue(0.0f);
        fourthHarmonicGainSlider.setValue(0.0f);
        fifthHarmonicGainSlider .setValue(0.0f);
        
        rootOscillatorGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        firstHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        secondHarmonicGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        thirdHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        fourthHarmonicGainSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
        fifthHarmonicGainSlider .setSliderStyle(Slider::SliderStyle::LinearVertical);
        
        rootOscillatorGainSlider.setNumDecimalPlacesToDisplay(2);
        firstHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        secondHarmonicGainSlider.setNumDecimalPlacesToDisplay(2);
        thirdHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        fourthHarmonicGainSlider.setNumDecimalPlacesToDisplay(2);
        fifthHarmonicGainSlider .setNumDecimalPlacesToDisplay(2);
        
        rootOscillatorGainSlider.onValueChange = [this] { synthAudioSource.setOscillatorGainByIndex(0, (float)rootOscillatorGainSlider.getValue()); };
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
        g.drawText ("synth section", getLocalBounds().removeFromBottom(getHeight()/2), Justification::centred, true);
    }

    void resized() override
    {
        Rectangle<int> harmonicSlidersBounds = Rectangle<int>(10,10, getWidth() - 20, getHeight()/2 - 20);
        FlexBox synthSectionFlexBox;
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), rootOscillatorGainSlider));
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), firstHarmonicGainSlider));
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), secondHarmonicGainSlider));
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), thirdHarmonicGainSlider));
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), fourthHarmonicGainSlider));
        synthSectionFlexBox.items.add(FlexItem(60, harmonicSlidersBounds.getHeight(), fifthHarmonicGainSlider));
        synthSectionFlexBox.justifyContent = FlexBox::JustifyContent::spaceBetween;
        synthSectionFlexBox.performLayout(harmonicSlidersBounds);
    }
//==============================================================================
public:
    SynthAudioSource synthAudioSource;
private:
    Slider rootOscillatorGainSlider;
    Slider firstHarmonicGainSlider;
    Slider secondHarmonicGainSlider;
    Slider thirdHarmonicGainSlider;
    Slider fourthHarmonicGainSlider;
    Slider fifthHarmonicGainSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthSectionComponent)
};
