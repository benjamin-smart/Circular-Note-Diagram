/*
 todo:
 
 REFACTOR:
 - make paths generate from selected scale/mode
 - restructure nodeComponent
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
 - create scale concept, 
 - refactor circle display into it's own cpp file

 path generator
 7 diatonic chord buttons in flexbox at bottom that generate paths
    - combobox selection menu to choose parent scale
 array of paths to represent different chords
 way of representing intervallic structure of scales
 */
