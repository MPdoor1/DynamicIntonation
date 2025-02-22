/*
  ==============================================================================

   This file is part of the JUCE framework examples.
   Copyright (c) Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the Projucer and used to generate a JUCE project.

 BEGIN_JUCE_PIP_METADATA

 name:             AudioSynthesiserDemo
 version:          1.0.0
 vendor:           JUCE
 website:          http://juce.com
 description:      Simple synthesiser application.

 dependencies:     juce_audio_basics, juce_audio_devices, juce_audio_formats,
                   juce_audio_processors, juce_audio_utils, juce_core,
                   juce_data_structures, juce_events, juce_graphics,
                   juce_gui_basics, juce_gui_extra
 exporters:        xcode_mac, vs2022, linux_make, androidstudio, xcode_iphone

 moduleFlags:      JUCE_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        AudioSynthesiserDemo

 useLocalCopy:     1

 END_JUCE_PIP_METADATA

*******************************************************************************/

//#pragma once
//
//#include "DemoUtilities.h"
//#include "AudioLiveScrollingDisplay.h"
//
////==============================================================================
///** Our demo synth sound is just a basic sine wave.. */
//struct SineWaveSound final : public SynthesiserSound
//{
//    bool appliesToNote (int /*midiNoteNumber*/) override    { return true; }
//    bool appliesToChannel (int /*midiChannel*/) override    { return true; }
//};
//
////==============================================================================
///** Our demo synth voice just plays a sine wave.. */
//struct SineWaveVoice final : public SynthesiserVoice
//{
//    bool canPlaySound (SynthesiserSound* sound) override
//    {
//        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
//    }
//
//    void startNote (int midiNoteNumber, float velocity,
//                    SynthesiserSound*, int /*currentPitchWheelPosition*/) override
//    {
//        currentAngle = 0.0;
//        level = velocity * 0.15;
//        tailOff = 0.0;
//
//        auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz (midiNoteNumber);
//        auto cyclesPerSample = cyclesPerSecond / getSampleRate();
//
//        angleDelta = cyclesPerSample * MathConstants<double>::twoPi;
//    }
//
//    void stopNote (float /*velocity*/, bool allowTailOff) override
//    {
//        if (allowTailOff)
//        {
//            // start a tail-off by setting this flag. The render callback will pick up on
//            // this and do a fade out, calling clearCurrentNote() when it's finished.
//
//            if (approximatelyEqual (tailOff, 0.0)) // we only need to begin a tail-off if it's not already doing so - the
//                tailOff = 1.0;                     // stopNote method could be called more than once.
//        }
//        else
//        {
//            // we're being told to stop playing immediately, so reset everything..
//            clearCurrentNote();
//            angleDelta = 0.0;
//        }
//    }
//
//    void pitchWheelMoved (int /*newValue*/) override                              {}
//    void controllerMoved (int /*controllerNumber*/, int /*newValue*/) override    {}
//
//    void renderNextBlock (AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
//    {
//        if (! approximatelyEqual (angleDelta, 0.0))
//        {
//            if (tailOff > 0.0)
//            {
//                while (--numSamples >= 0)
//                {
//                    auto currentSample = (float) (std::sin (currentAngle) * level * tailOff);
//
//                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
//                        outputBuffer.addSample (i, startSample, currentSample);
//
//                    currentAngle += angleDelta;
//                    ++startSample;
//
//                    tailOff *= 0.99;
//
//                    if (tailOff <= 0.005)
//                    {
//                        clearCurrentNote();
//
//                        angleDelta = 0.0;
//                        break;
//                    }
//                }
//            }
//            else
//            {
//                while (--numSamples >= 0)
//                {
//                    auto currentSample = (float) (std::sin (currentAngle) * level);
//
//                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
//                        outputBuffer.addSample (i, startSample, currentSample);
//
//                    currentAngle += angleDelta;
//                    ++startSample;
//                }
//            }
//        }
//    }
//
//    using SynthesiserVoice::renderNextBlock;
//
//private:
//    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0, tailOff = 0.0;
//};
//
////==============================================================================
//// This is an audio source that streams the output of our demo synth.
//struct SynthAudioSource final : public AudioSource
//{
//    SynthAudioSource (MidiKeyboardState& keyState)  : keyboardState (keyState)
//    {
//        // Add some voices to our synth, to play the sounds..
//        for (auto i = 0; i < 4; ++i)
//        {
//            synth.addVoice (new SineWaveVoice());   // These voices will play our custom sine-wave sounds..
//            synth.addVoice (new SamplerVoice());    // and these ones play the sampled sounds
//        }
//
//        // ..and add a sound for them to play...
//        setUsingSineWaveSound();
//    }
//
//    void setUsingSineWaveSound()
//    {
//        synth.clearSounds();
//        synth.addSound (new SineWaveSound());
//    }
//
//    void setUsingSampledSound()
//    {
//        WavAudioFormat wavFormat;
//
//        std::unique_ptr<AudioFormatReader> audioReader (wavFormat.createReaderFor (createAssetInputStream ("reverb_ir.wav").release(), true));
//
//        BigInteger allNotes;
//        allNotes.setRange (0, 128, true);
//
//        synth.clearSounds();
//        synth.addSound (new SamplerSound ("demo sound",
//                                          *audioReader,
//                                          allNotes,
//                                          75,   // root midi note
//                                          1,  // attack time
//                                          5,  // release time
//                                          10.0  // maximum sample length
//                                          ));
// 
//    }
//
//    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
//    {
//        midiCollector.reset (sampleRate);
//
//        synth.setCurrentPlaybackSampleRate (sampleRate);
//    }
//
//    void releaseResources() override {}
//
//    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
//    {
//        // the synth always adds its output to the audio buffer, so we have to clear it
//        // first..
//        bufferToFill.clearActiveBufferRegion();
//
//        // fill a midi buffer with incoming messages from the midi input.
//        MidiBuffer incomingMidi;
//        midiCollector.removeNextBlockOfMessages (incomingMidi, bufferToFill.numSamples);
//
//        // pass these messages to the keyboard state so that it can update the component
//        // to show on-screen which keys are being pressed on the physical midi keyboard.
//        // This call will also add midi messages to the buffer which were generated by
//        // the mouse-clicking on the on-screen keyboard.
//        keyboardState.processNextMidiBuffer (incomingMidi, 0, bufferToFill.numSamples, true);
//
//        // and now get the synth to process the midi events and generate its output.
//        synth.renderNextBlock (*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
//    }
//
//    //==============================================================================
//    // this collects real-time midi messages from the midi input device, and
//    // turns them into blocks that we can process in our audio callback
//    MidiMessageCollector midiCollector;
//
//    // this represents the state of which keys on our on-screen keyboard are held
//    // down. When the mouse is clicked on the keyboard component, this object also
//    // generates midi messages for this, which we can pass on to our synth.
//    MidiKeyboardState& keyboardState;
//
//    // the synth itself!
//    Synthesiser synth;
//};
//
////==============================================================================
//class Callback final : public AudioIODeviceCallback
//{
//public:
//    Callback (AudioSourcePlayer& playerIn, LiveScrollingAudioDisplay& displayIn)
//        : player (playerIn), display (displayIn) {}
//
//    void audioDeviceIOCallbackWithContext (const float* const* inputChannelData,
//                                           int numInputChannels,
//                                           float* const* outputChannelData,
//                                           int numOutputChannels,
//                                           int numSamples,
//                                           const AudioIODeviceCallbackContext& context) override
//    {
//        player.audioDeviceIOCallbackWithContext (inputChannelData,
//                                                 numInputChannels,
//                                                 outputChannelData,
//                                                 numOutputChannels,
//                                                 numSamples,
//                                                 context);
//        display.audioDeviceIOCallbackWithContext (outputChannelData,
//                                                  numOutputChannels,
//                                                  nullptr,
//                                                  0,
//                                                  numSamples,
//                                                  context);
//    }
//
//    void audioDeviceAboutToStart (AudioIODevice* device) override
//    {
//        player.audioDeviceAboutToStart (device);
//        display.audioDeviceAboutToStart (device);
//    }
//
//    void audioDeviceStopped() override
//    {
//        player.audioDeviceStopped();
//        display.audioDeviceStopped();
//    }
//
//private:
//    AudioSourcePlayer& player;
//    LiveScrollingAudioDisplay& display;
//};
//
////==============================================================================
//class AudioSynthesiserDemo final : public Component
//{
//public:
//    AudioSynthesiserDemo()
//    {
//        addAndMakeVisible (keyboardComponent);
//
//        addAndMakeVisible (sineButton);
//        sineButton.setRadioGroupId (321);
//        sineButton.setToggleState (true, dontSendNotification);
//        sineButton.onClick = [this] { synthAudioSource.setUsingSineWaveSound(); };
//
//        addAndMakeVisible (sampledButton);
//        sampledButton.setRadioGroupId (321);
//        sampledButton.onClick = [this] { synthAudioSource.setUsingSampledSound(); };
//
//        addAndMakeVisible (liveAudioDisplayComp);
//        audioSourcePlayer.setSource (&synthAudioSource);
//
//       #ifndef JUCE_DEMO_RUNNER
//        audioDeviceManager.initialise (0, 2, nullptr, true, {}, nullptr);
//       #endif
//
//        audioDeviceManager.addAudioCallback (&callback);
//        audioDeviceManager.addMidiInputDeviceCallback ({}, &(synthAudioSource.midiCollector));
//
//        setOpaque (true);
//        setSize (640, 480);
//    }
//
//    ~AudioSynthesiserDemo() override
//    {
//        audioSourcePlayer.setSource (nullptr);
//        audioDeviceManager.removeMidiInputDeviceCallback ({}, &(synthAudioSource.midiCollector));
//        audioDeviceManager.removeAudioCallback (&callback);
//    }
//
//    //==============================================================================
//    void paint (Graphics& g) override
//    {
//        g.fillAll (getUIColourIfAvailable (LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
//    }
//
//    void resized() override
//    {
//        keyboardComponent   .setBounds (8, 96, getWidth() - 16, 64);
//        sineButton          .setBounds (16, 176, 150, 24);
//        sampledButton       .setBounds (16, 200, 150, 24);
//        liveAudioDisplayComp.setBounds (8, 8, getWidth() - 16, 64);
//    }
//
//private:
//    // if this PIP is running inside the demo runner, we'll use the shared device manager instead
//   #ifndef JUCE_DEMO_RUNNER
//    AudioDeviceManager audioDeviceManager;
//   #else
//    AudioDeviceManager& audioDeviceManager { getSharedAudioDeviceManager (0, 2) };
//   #endif
//
//    MidiKeyboardState keyboardState;
//    AudioSourcePlayer audioSourcePlayer;
//    SynthAudioSource synthAudioSource        { keyboardState };
//    MidiKeyboardComponent keyboardComponent  { keyboardState, MidiKeyboardComponent::horizontalKeyboard};
//
//    ToggleButton sineButton     { "Use sine wave" };
//    ToggleButton sampledButton  { "Use sampled sound" };
//
//    LiveScrollingAudioDisplay liveAudioDisplayComp;
//
//    Callback callback { audioSourcePlayer, liveAudioDisplayComp };
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioSynthesiserDemo)
//};


//GROK 3 generated code v1.0

//#pragma once
//
//#include "DemoUtilities.h"
//#include "AudioLiveScrollingDisplay.h"
//#include <array>
//
////==============================================================================
//struct SineWaveSound final : public SynthesiserSound
//{
//    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
//    bool appliesToChannel(int /*midiChannel*/) override { return true; }
//};
//
////==============================================================================
//struct SineWaveVoice final : public SynthesiserVoice
//{
//    bool canPlaySound(SynthesiserSound* sound) override
//    {
//        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
//    }
//
//    void startNote(int midiNoteNumber, float velocity,
//        SynthesiserSound*, int /*currentPitchWheelPosition*/) override
//    {
//        currentAngle.fill(0.0);
//        level = velocity * 0.15;
//        tailOff = 0.0;
//
//        // Calculate chord notes (e.g., major triad: root, +4 semitones, +7 semitones)
//        std::array<int, 3> chordNotes = { midiNoteNumber, midiNoteNumber + 4, midiNoteNumber + 7 }; // Major triad
//
//        for (size_t i = 0; i < chordNotes.size(); ++i)
//        {
//            auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(chordNotes[i]);
//            auto cyclesPerSample = cyclesPerSecond / getSampleRate();
//            angleDelta[i] = cyclesPerSample * MathConstants<double>::twoPi;
//        }
//    }
//
//    void stopNote(float /*velocity*/, bool allowTailOff) override
//    {
//        if (allowTailOff)
//        {
//            if (approximatelyEqual(tailOff, 0.0))
//                tailOff = 1.0;
//        }
//        else
//        {
//            clearCurrentNote();
//            angleDelta.fill(0.0);
//        }
//    }
//
//    void pitchWheelMoved(int /*newValue*/) override {}
//    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override {}
//
//    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
//    {
//        if (!std::all_of(angleDelta.begin(), angleDelta.end(), [](double d) { return approximatelyEqual(d, 0.0); }))
//        {
//            if (tailOff > 0.0)
//            {
//                while (--numSamples >= 0)
//                {
//                    float currentSample = 0.0f;
//                    for (size_t i = 0; i < angleDelta.size(); ++i)
//                    {
//                        currentSample += (float)(std::sin(currentAngle[i]) * level * tailOff / angleDelta.size());
//                        currentAngle[i] += angleDelta[i];
//                    }
//
//                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
//                        outputBuffer.addSample(i, startSample, currentSample);
//
//                    ++startSample;
//                    tailOff *= 0.99;
//
//                    if (tailOff <= 0.005)
//                    {
//                        clearCurrentNote();
//                        angleDelta.fill(0.0);
//                        break;
//                    }
//                }
//            }
//            else
//            {
//                while (--numSamples >= 0)
//                {
//                    float currentSample = 0.0f;
//                    for (size_t i = 0; i < angleDelta.size(); ++i)
//                    {
//                        currentSample += (float)(std::sin(currentAngle[i]) * level / angleDelta.size());
//                        currentAngle[i] += angleDelta[i];
//                    }
//
//                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
//                        outputBuffer.addSample(i, startSample, currentSample);
//
//                    ++startSample;
//                }
//            }
//        }
//    }
//
//    using SynthesiserVoice::renderNextBlock;
//
//private:
//    std::array<double, 3> currentAngle = { 0.0, 0.0, 0.0 }; // For root, third, fifth
//    std::array<double, 3> angleDelta = { 0.0, 0.0, 0.0 };   // Angular frequency for each note
//    double level = 0.0, tailOff = 0.0;
//};
//
////==============================================================================
//struct SynthAudioSource final : public AudioSource
//{
//    SynthAudioSource(MidiKeyboardState& keyState) : keyboardState(keyState)
//    {
//        for (auto i = 0; i < 4; ++i)
//        {
//            synth.addVoice(new SineWaveVoice());
//            synth.addVoice(new SamplerVoice());
//        }
//        setUsingSineWaveSound();
//    }
//
//    void setUsingSineWaveSound()
//    {
//        synth.clearSounds();
//        synth.addSound(new SineWaveSound());
//    }
//
//    void setUsingSampledSound()
//    {
//        WavAudioFormat wavFormat;
//        std::unique_ptr<AudioFormatReader> audioReader(wavFormat.createReaderFor(createAssetInputStream("cello.wav").release(), true));
//
//        BigInteger allNotes;
//        allNotes.setRange(0, 128, true);
//
//        synth.clearSounds();
//        synth.addSound(new SamplerSound("demo sound",
//            *audioReader,
//            allNotes,
//            75,   // root midi note
//            1,    // attack time
//            5,    // release time
//            10.0  // maximum sample length
//        ));
//    }
//
//    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
//    {
//        midiCollector.reset(sampleRate);
//        synth.setCurrentPlaybackSampleRate(sampleRate);
//    }
//
//    void releaseResources() override {}
//
//    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
//    {
//        bufferToFill.clearActiveBufferRegion();
//        MidiBuffer incomingMidi;
//        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
//        keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);
//        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
//    }
//
//    MidiMessageCollector midiCollector;
//    MidiKeyboardState& keyboardState;
//    Synthesiser synth;
//};
//
////==============================================================================
//class Callback final : public AudioIODeviceCallback
//{
//public:
//    Callback(AudioSourcePlayer& playerIn, LiveScrollingAudioDisplay& displayIn)
//        : player(playerIn), display(displayIn) {
//    }
//
//    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
//        int numInputChannels,
//        float* const* outputChannelData,
//        int numOutputChannels,
//        int numSamples,
//        const AudioIODeviceCallbackContext& context) override
//    {
//        player.audioDeviceIOCallbackWithContext(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples, context);
//        display.audioDeviceIOCallbackWithContext(outputChannelData, numOutputChannels, nullptr, 0, numSamples, context);
//    }
//
//    void audioDeviceAboutToStart(AudioIODevice* device) override
//    {
//        player.audioDeviceAboutToStart(device);
//        display.audioDeviceAboutToStart(device);
//    }
//
//    void audioDeviceStopped() override
//    {
//        player.audioDeviceStopped();
//        display.audioDeviceStopped();
//    }
//
//private:
//    AudioSourcePlayer& player;
//    LiveScrollingAudioDisplay& display;
//};
//
////==============================================================================
//class AudioSynthesiserDemo final : public Component
//{
//public:
//    AudioSynthesiserDemo()
//    {
//        addAndMakeVisible(keyboardComponent);
//        addAndMakeVisible(sineButton);
//        sineButton.setRadioGroupId(321);
//        sineButton.setToggleState(true, dontSendNotification);
//        sineButton.onClick = [this] { synthAudioSource.setUsingSineWaveSound(); };
//
//        addAndMakeVisible(sampledButton);
//        sampledButton.setRadioGroupId(321);
//        sampledButton.onClick = [this] { synthAudioSource.setUsingSampledSound(); };
//
//        addAndMakeVisible(liveAudioDisplayComp);
//        audioSourcePlayer.setSource(&synthAudioSource);
//
//#ifndef JUCE_DEMO_RUNNER
//        audioDeviceManager.initialise(0, 2, nullptr, true, {}, nullptr);
//#endif
//
//        audioDeviceManager.addAudioCallback(&callback);
//        audioDeviceManager.addMidiInputDeviceCallback({}, &(synthAudioSource.midiCollector));
//
//        setOpaque(true);
//        setSize(640, 480);
//    }
//
//    ~AudioSynthesiserDemo() override
//    {
//        audioSourcePlayer.setSource(nullptr);
//        audioDeviceManager.removeMidiInputDeviceCallback({}, &(synthAudioSource.midiCollector));
//        audioDeviceManager.removeAudioCallback(&callback);
//    }
//
//    void paint(Graphics& g) override
//    {
//        g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
//    }
//
//    void resized() override
//    {
//        keyboardComponent.setBounds(8, 96, getWidth() - 16, 64);
//        sineButton.setBounds(16, 176, 150, 24);
//        sampledButton.setBounds(16, 200, 150, 24);
//        liveAudioDisplayComp.setBounds(8, 8, getWidth() - 16, 64);
//    }
//
//private:
//#ifndef JUCE_DEMO_RUNNER
//    AudioDeviceManager audioDeviceManager;
//#else
//    AudioDeviceManager& audioDeviceManager{ getSharedAudioDeviceManager(0, 2) };
//#endif
//
//    MidiKeyboardState keyboardState;
//    AudioSourcePlayer audioSourcePlayer;
//    SynthAudioSource synthAudioSource{ keyboardState };
//    MidiKeyboardComponent keyboardComponent{ keyboardState, MidiKeyboardComponent::horizontalKeyboard };
//
//    ToggleButton sineButton{ "Use sine wave" };
//    ToggleButton sampledButton{ "Use sampled sound" };
//
//    LiveScrollingAudioDisplay liveAudioDisplayComp;
//    Callback callback{ audioSourcePlayer, liveAudioDisplayComp };
//
//    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSynthesiserDemo)
//};

//Grok v1.1 added :

// 1 - Chord Type Selection (major, minor, diminished)
// 2 - Increase Number of Voices for polyphony
// 3 - Amplitude Adjustment for chord voicings
#pragma once

#include "DemoUtilities.h"
#include "AudioLiveScrollingDisplay.h"
#include <array>

//==============================================================================
struct SineWaveSound final : public SynthesiserSound
{
    bool appliesToNote(int /*midiNoteNumber*/) override { return true; }
    bool appliesToChannel(int /*midiChannel*/) override { return true; }
};

//==============================================================================
struct SineWaveVoice final : public SynthesiserVoice
{
    enum class ChordType { Major, Minor, Diminished };

    SineWaveVoice() : chordType(ChordType::Major), voicingAmplitude{ 1.0, 0.8, 0.6 } {}

    bool canPlaySound(SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void setChordType(ChordType type) { chordType = type; }
    void setVoicingAmplitude(float root, float third, float fifth)
    {
        voicingAmplitude = { root, third, fifth };
    }

    void startNote(int midiNoteNumber, float velocity,
        SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle.fill(0.0);
        level = velocity * 0.15;
        tailOff = 0.0;

        // Define chord intervals based on chord type
        std::array<int, 3> intervals;
        switch (chordType)
        {
        case ChordType::Major:      intervals = { 0, 4, 7 }; break; // Root, major third, perfect fifth
        case ChordType::Minor:      intervals = { 0, 3, 7 }; break; // Root, minor third, perfect fifth
        case ChordType::Diminished: intervals = { 0, 3, 6 }; break; // Root, minor third, diminished fifth
        default:                    intervals = { 0, 4, 7 }; break;
        }

        std::array<int, 3> chordNotes;
        for (size_t i = 0; i < chordNotes.size(); ++i)
            chordNotes[i] = midiNoteNumber + intervals[i];

        for (size_t i = 0; i < chordNotes.size(); ++i)
        {
            auto cyclesPerSecond = MidiMessage::getMidiNoteInHertz(chordNotes[i]);
            auto cyclesPerSample = cyclesPerSecond / getSampleRate();
            angleDelta[i] = cyclesPerSample * MathConstants<double>::twoPi;
        }
    }

    void stopNote(float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            if (approximatelyEqual(tailOff, 0.0))
                tailOff = 1.0;
        }
        else
        {
            clearCurrentNote();
            angleDelta.fill(0.0);
        }
    }

    void pitchWheelMoved(int /*newValue*/) override {}
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override {}

    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        if (!std::all_of(angleDelta.begin(), angleDelta.end(), [](double d) { return approximatelyEqual(d, 0.0); }))
        {
            if (tailOff > 0.0)
            {
                while (--numSamples >= 0)
                {
                    float currentSample = 0.0f;
                    for (size_t i = 0; i < angleDelta.size(); ++i)
                    {
                        currentSample += (float)(std::sin(currentAngle[i]) * level * tailOff * voicingAmplitude[i]);
                        currentAngle[i] += angleDelta[i];
                    }
                    currentSample /= voicingAmplitude[0]; // Normalize by root amplitude to avoid clipping

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    ++startSample;
                    tailOff *= 0.99;

                    if (tailOff <= 0.005)
                    {
                        clearCurrentNote();
                        angleDelta.fill(0.0);
                        break;
                    }
                }
            }
            else
            {
                while (--numSamples >= 0)
                {
                    float currentSample = 0.0f;
                    for (size_t i = 0; i < angleDelta.size(); ++i)
                    {
                        currentSample += (float)(std::sin(currentAngle[i]) * level * voicingAmplitude[i]);
                        currentAngle[i] += angleDelta[i];
                    }
                    currentSample /= voicingAmplitude[0]; // Normalize by root amplitude

                    for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                        outputBuffer.addSample(i, startSample, currentSample);

                    ++startSample;
                }
            }
        }
    }

    using SynthesiserVoice::renderNextBlock;

private:
    ChordType chordType;
    std::array<float, 3> voicingAmplitude; // Amplitude for root, third, fifth
    std::array<double, 3> currentAngle = { 0.0, 0.0, 0.0 };
    std::array<double, 3> angleDelta = { 0.0, 0.0, 0.0 };
    double level = 0.0, tailOff = 0.0;
};

//==============================================================================
struct SynthAudioSource final : public AudioSource
{
    SynthAudioSource(MidiKeyboardState& keyState) : keyboardState(keyState)
    {
        for (auto i = 0; i < 8; ++i) // Increased to 8 voices for better polyphony
        {
            auto* voice = new SineWaveVoice();
            synth.addVoice(voice);
            sineVoices.push_back(voice); // Store pointers for chord type control
            synth.addVoice(new SamplerVoice());
        }
        setUsingSineWaveSound();
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
        synth.addSound(new SineWaveSound());
    }

    void setUsingSampledSound()
    {
        WavAudioFormat wavFormat;
        std::unique_ptr<AudioFormatReader> audioReader(wavFormat.createReaderFor(createAssetInputStream("reverb_ir.wav").release(), true));

        BigInteger allNotes;
        allNotes.setRange(0, 128, true);

        synth.clearSounds();
        synth.addSound(new SamplerSound("demo sound", *audioReader, allNotes, 75, 1, 5, 10.0));
    }

    void setChordType(SineWaveVoice::ChordType type)
    {
        for (auto* voice : sineVoices)
            voice->setChordType(type);
    }

    void setVoicingAmplitude(float root, float third, float fifth)
    {
        for (auto* voice : sineVoices)
            voice->setVoicingAmplitude(root, third, fifth);
    }

    void prepareToPlay(int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        midiCollector.reset(sampleRate);
        synth.setCurrentPlaybackSampleRate(sampleRate);
    }

    void releaseResources() override {}

    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
    {
        bufferToFill.clearActiveBufferRegion();
        MidiBuffer incomingMidi;
        midiCollector.removeNextBlockOfMessages(incomingMidi, bufferToFill.numSamples);
        keyboardState.processNextMidiBuffer(incomingMidi, 0, bufferToFill.numSamples, true);
        synth.renderNextBlock(*bufferToFill.buffer, incomingMidi, 0, bufferToFill.numSamples);
    }

    MidiMessageCollector midiCollector;
    MidiKeyboardState& keyboardState;
    Synthesiser synth;
    std::vector<SineWaveVoice*> sineVoices; // For controlling chord settings
};

//==============================================================================
class Callback final : public AudioIODeviceCallback
{
public:
    Callback(AudioSourcePlayer& playerIn, LiveScrollingAudioDisplay& displayIn)
        : player(playerIn), display(displayIn) {
    }

    void audioDeviceIOCallbackWithContext(const float* const* inputChannelData,
        int numInputChannels,
        float* const* outputChannelData,
        int numOutputChannels,
        int numSamples,
        const AudioIODeviceCallbackContext& context) override
    {
        player.audioDeviceIOCallbackWithContext(inputChannelData, numInputChannels, outputChannelData, numOutputChannels, numSamples, context);
        display.audioDeviceIOCallbackWithContext(outputChannelData, numOutputChannels, nullptr, 0, numSamples, context);
    }

    void audioDeviceAboutToStart(AudioIODevice* device) override
    {
        player.audioDeviceAboutToStart(device);
        display.audioDeviceAboutToStart(device);
    }

    void audioDeviceStopped() override
    {
        player.audioDeviceStopped();
        display.audioDeviceStopped();
    }

private:
    AudioSourcePlayer& player;
    LiveScrollingAudioDisplay& display;
};

//==============================================================================
class AudioSynthesiserDemo final : public Component
{
public:
    AudioSynthesiserDemo()
    {
        addAndMakeVisible(keyboardComponent);

        addAndMakeVisible(sineButton);
        sineButton.setRadioGroupId(321);
        sineButton.setToggleState(true, dontSendNotification);
        sineButton.onClick = [this] { synthAudioSource.setUsingSineWaveSound(); };

        addAndMakeVisible(sampledButton);
        sampledButton.setRadioGroupId(321);
        sampledButton.onClick = [this] { synthAudioSource.setUsingSampledSound(); };

        // Chord type buttons
        addAndMakeVisible(majorButton);
        majorButton.setRadioGroupId(322);
        majorButton.setToggleState(true, dontSendNotification);
        majorButton.onClick = [this] { synthAudioSource.setChordType(SineWaveVoice::ChordType::Major); };

        addAndMakeVisible(minorButton);
        minorButton.setRadioGroupId(322);
        minorButton.onClick = [this] { synthAudioSource.setChordType(SineWaveVoice::ChordType::Minor); };

        addAndMakeVisible(dimButton);
        dimButton.setRadioGroupId(322);
        dimButton.onClick = [this] { synthAudioSource.setChordType(SineWaveVoice::ChordType::Diminished); };

        // Voicing sliders
        addAndMakeVisible(rootAmpSlider);
        rootAmpSlider.setRange(0.1, 1.0, 0.01);
        rootAmpSlider.setValue(1.0);
        rootAmpSlider.onValueChange = [this] { updateVoicing(); };

        addAndMakeVisible(thirdAmpSlider);
        thirdAmpSlider.setRange(0.1, 1.0, 0.01);
        thirdAmpSlider.setValue(0.8);
        thirdAmpSlider.onValueChange = [this] { updateVoicing(); };

        addAndMakeVisible(fifthAmpSlider);
        fifthAmpSlider.setRange(0.1, 1.0, 0.01);
        fifthAmpSlider.setValue(0.6);
        fifthAmpSlider.onValueChange = [this] { updateVoicing(); };

        addAndMakeVisible(liveAudioDisplayComp);
        audioSourcePlayer.setSource(&synthAudioSource);

#ifndef JUCE_DEMO_RUNNER
        audioDeviceManager.initialise(0, 2, nullptr, true, {}, nullptr);
#endif

        audioDeviceManager.addAudioCallback(&callback);
        audioDeviceManager.addMidiInputDeviceCallback({}, &(synthAudioSource.midiCollector));

        setOpaque(true);
        setSize(640, 480);

        updateVoicing(); // Set initial voicing
    }

    ~AudioSynthesiserDemo() override
    {
        audioSourcePlayer.setSource(nullptr);
        audioDeviceManager.removeMidiInputDeviceCallback({}, &(synthAudioSource.midiCollector));
        audioDeviceManager.removeAudioCallback(&callback);
    }

    void paint(Graphics& g) override
    {
        g.fillAll(getUIColourIfAvailable(LookAndFeel_V4::ColourScheme::UIColour::windowBackground));
    }

    void resized() override
    {
        keyboardComponent.setBounds(8, 96, getWidth() - 16, 64);
        sineButton.setBounds(16, 176, 150, 24);
        sampledButton.setBounds(16, 200, 150, 24);

        majorButton.setBounds(180, 176, 100, 24);
        minorButton.setBounds(180, 200, 100, 24);
        dimButton.setBounds(180, 224, 100, 24);

        rootAmpSlider.setBounds(300, 176, 100, 24);
        thirdAmpSlider.setBounds(300, 200, 100, 24);
        fifthAmpSlider.setBounds(300, 224, 100, 24);

        liveAudioDisplayComp.setBounds(8, 8, getWidth() - 16, 64);
    }

private:
    void updateVoicing()
    {
        synthAudioSource.setVoicingAmplitude(
            (float)rootAmpSlider.getValue(),
            (float)thirdAmpSlider.getValue(),
            (float)fifthAmpSlider.getValue());
    }

#ifndef JUCE_DEMO_RUNNER
    AudioDeviceManager audioDeviceManager;
#else
    AudioDeviceManager& audioDeviceManager{ getSharedAudioDeviceManager(0, 2) };
#endif

    MidiKeyboardState keyboardState;
    AudioSourcePlayer audioSourcePlayer;
    SynthAudioSource synthAudioSource{ keyboardState };
    MidiKeyboardComponent keyboardComponent{ keyboardState, MidiKeyboardComponent::horizontalKeyboard };

    ToggleButton sineButton{ "Use sine wave" };
    ToggleButton sampledButton{ "Use sampled sound" };
    ToggleButton majorButton{ "Major" };
    ToggleButton minorButton{ "Minor" };
    ToggleButton dimButton{ "Diminished" };
    Slider rootAmpSlider{ "Root Amp" };
    Slider thirdAmpSlider{ "Third Amp" };
    Slider fifthAmpSlider{ "Fifth Amp" };

    LiveScrollingAudioDisplay liveAudioDisplayComp;
    Callback callback{ audioSourcePlayer, liveAudioDisplayComp };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioSynthesiserDemo)
};
