/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class UtilityAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    UtilityAudioProcessor();
    ~UtilityAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
private:
    float balanceMinRange = -50.f;
    float balanceMaxRange = 50.f;
public:
    juce::AudioProcessorValueTreeState apvts{ *this, nullptr, "Parameters", createParameterLayout() };

private:
    juce::dsp::Gain<float> gain;
    juce::dsp::Panner<float> panner;
    juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> dcHighPassFilter;
    juce::dsp::LinkwitzRileyFilter<float> HP, LP;

    juce::AudioBuffer<float> hpBuffer;
    juce::AudioBuffer<float> lpBuffer;

    juce::AudioParameterFloat* gainParam{ nullptr };
    juce::AudioParameterFloat* balanceParam{ nullptr };
    juce::AudioParameterFloat* stereoWidthParam{ nullptr };
    juce::AudioParameterBool* muteParam{ nullptr };
    juce::AudioParameterBool* dcParam{ nullptr };
    juce::AudioParameterBool* monoParam{ nullptr };
    juce::AudioParameterBool* bassMonoParam{ nullptr };
    juce::AudioParameterFloat* bassMonoCrossoverParam{ nullptr };
    juce::AudioParameterBool* bassMonoPreviewParam{ nullptr };
    juce::AudioParameterBool* invertPhaseLeftParam{ nullptr };
    juce::AudioParameterBool* invertPhaseRightParam{ nullptr };
    juce::AudioParameterChoice* modeParam{ nullptr };
    juce::AudioParameterBool* midSideModeParam{ nullptr };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessor)
};
