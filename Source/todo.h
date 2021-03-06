/*
 todo:
 - harmonic synths for third and fifth
 - octave toggle
 - chord voicing select
 - parent scale select
 
 
 - rotate nodes slider changes master tuning from 220 hz to 880 hz
 - distance from centre slider is volume slider
 - midi / playback section
 - scales start from other notes?
     - add button to nodeComponent to change root that scale is generated from
 - parent scales enums
 MIDI:
 - array of notes in scale to iterate through patterns
 - duplicate up octaves
 - send midi to circularDisplay, to trigger highlighting of notes
 - sequences (combobox of satta33 progressions)
 -  sync with external source / incoming midi - play sequences into your sessions & songs
 - mono mode for sequences, polyphonic chord mode

 SYNTH:
 - plays currently selected sequence of notes
 - take from wavetable harmonic synth (another project)
 CIRCLEDISPLAY:
- make colors exactly like satta33
- highlight notes of the scale (not c#, d#)
- make clicking on notes move to that note of the scale
- make nodes light up when they are playing

 GUI:
 - fretboard display (more frets, full pattern completion and adjust pattern start point)
 - make clicking on notes move to that note of the scale
  
 
 done:
 - finish getting synth to work
    - hook up sliders
    - can we make SynthSectionComponent be an AudioAppComponent and include the audio processing in there?
 - get synth to play notes of chords that are displayed on screen
    - 3 separate synths needed for root third and fifth, different voices are used for harmonics
    - array of midi notes?  how do we wanna do this?  how are they being triggered?
 
- make paths generate from selected scale/mode
 - create scale concept
 - refactor circle display into it's own cpp file

 path generator
 7 diatonic chord buttons in flexbox at bottom that generate paths
    - combobox selection menu to choose parent scale
 array of paths to represent different chords
 way of representing intervallic structure of scales
 */



// why is the root note selection circle slightly misaligned on the right side of the diagram?

    //==============================================================================
/*
 // possible way of making sound for active nodes
 if (nodePathIndices[0] == 0 || nodePathIndices[1] == 0 || nodePathIndices[2] == 0)
    // play C
 if (nodePathIndices[0] == 1 || nodePathIndices[1] == 1 || nodePathIndices[2] == 1)
    // play C#
 if (nodePathIndices[0] == 1 || nodePathIndices[1] == 1 || nodePathIndices[2] == 1)
 
 */
//=============================================================================

