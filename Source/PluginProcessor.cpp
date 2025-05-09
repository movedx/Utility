/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
UtilityAudioProcessor::UtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    gainParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Gain"));
    jassert(gainParam);

	balanceParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Balance"));
	jassert(balanceParam);

	stereoWidthParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("Width"));
	jassert(stereoWidthParam);

	muteParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("Mute"));
	jassert(muteParam);

	dcParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("DC"));
	jassert(dcParam);

	monoParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("Mono"));
	jassert(monoParam);

	bassMonoParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("BassMono"));
	jassert(bassMonoParam);

	bassMonoCrossoverParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("BassMonoCrossover"));
	jassert(bassMonoCrossoverParam);

	bassMonoPreviewParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("BassMonoPreview"));
	jassert(bassMonoPreviewParam);

	invertPhaseLeftParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("InvertPhaseLeft"));
	jassert(invertPhaseLeftParam);

	invertPhaseRightParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("InvertPhaseRight"));
	jassert(invertPhaseRightParam);

	modeParam = dynamic_cast<juce::AudioParameterChoice*>(apvts.getParameter("Mode"));
	jassert(modeParam);

    midSideModeParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("MidSideMode"));
    jassert(midSideModeParam);

    midSideParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("MidSide"));
    jassert(midSideParam);

    panner.setRule(juce::dsp::PannerRule::balanced);
}

UtilityAudioProcessor::~UtilityAudioProcessor()
{
}

//==============================================================================
const juce::String UtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UtilityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UtilityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UtilityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UtilityAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String UtilityAudioProcessor::getProgramName (int index)
{
    return {};
}

void UtilityAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void UtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;

    gain.prepare(spec);
    gain.setRampDurationSeconds(0.03f);
    gain.setGainDecibels(gainParam->get());

    panner.prepare(spec);
    panner.setPan(0.f);

    dcHighPassFilter.state = juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, 10);
    dcHighPassFilter.prepare(spec);

    HP.prepare(spec);
    LP.prepare(spec);

    HP.setType(juce::dsp::LinkwitzRileyFilterType::highpass);
    HP.setCutoffFrequency(bassMonoCrossoverParam->get());
    LP.setType(juce::dsp::LinkwitzRileyFilterType::lowpass);
    LP.setCutoffFrequency(bassMonoCrossoverParam->get());
}

void UtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UtilityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void makeMono(juce::AudioBuffer<float>& buffer, int numChannels)
{
    if (numChannels >= 2)
    {
        auto* leftChannnel = buffer.getWritePointer(0);
        auto* rightChannel = buffer.getWritePointer(1);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            float monoSample = (leftChannnel[sample] + rightChannel[sample]) * 0.5f;
            leftChannnel[sample] = monoSample;
            rightChannel[sample] = monoSample;
        }
    }
}

void muteChannel(juce::AudioBuffer<float>& buffer, int channel)
{
    jassert(channel == 0 || channel == 1);
    auto* chPtr = buffer.getWritePointer(channel);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        chPtr[sample] = 0.f;
    }
}

void swapChannels(juce::AudioBuffer<float>& buffer)
{
    jassert(buffer.getNumChannels() == 2);
    auto* left = buffer.getWritePointer(0);
    auto* right = buffer.getWritePointer(1);
    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        float tmp = left[sample];
        left[sample] = right[sample];
        right[sample] = tmp;
    }
}

void UtilityAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}

    // inv L inv R

    if (invertPhaseLeftParam->get())
    {
        auto* leftChannel = buffer.getWritePointer(0);

        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            leftChannel[sample] = leftChannel[sample] * -1.f;
        }
    }

    if (invertPhaseRightParam->get())
    {
        auto* rightChannel = buffer.getWritePointer(1);
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            rightChannel[sample] = rightChannel[sample] * -1.f;
        }
    }

    // Mode

    switch (modeParam->getIndex())
    {
    case 0: // stereo
        break;
    case 1: // left
        muteChannel(buffer, 1);
        break;
    case 2: // right
        muteChannel(buffer, 0);
        break;
    case 3: // swap
        swapChannels(buffer);
        break;
    }

    // Stereo Width / MidSide balance


    if (totalNumInputChannels == 2)
    {
        auto* leftChannel = buffer.getWritePointer(0);
        auto* rightChannel = buffer.getWritePointer(1);

        if (midSideModeParam->get())
        {
            float midSideBalance = midSideParam->get() * 0.01f;

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float mid = (leftChannel[sample] + rightChannel[sample]) * 0.5f;
                float side = (leftChannel[sample] - rightChannel[sample]) * 0.5f;

                mid *= (1.0f - midSideBalance);
                side *= (1.0f + midSideBalance);

                leftChannel[sample] = mid + side;
                rightChannel[sample] = mid - side;
            }
        }
        else
        {
            float width = stereoWidthParam->get() * 0.01f;

            for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
            {
                float mid = (leftChannel[sample] + rightChannel[sample]) * 0.5f;
                float side = (leftChannel[sample] - rightChannel[sample]) * 0.5f;

                side *= width;

                leftChannel[sample] = mid + side;
                rightChannel[sample] = mid - side;
            }
        }
    }

    // Mono

    if (monoParam->get())
    {
        makeMono(buffer, totalNumInputChannels);
    }

    // Bass Mono Crossover

    if (bassMonoParam->get())
    {
        HP.setCutoffFrequency(bassMonoCrossoverParam->get());
        LP.setCutoffFrequency(bassMonoCrossoverParam->get());
        hpBuffer.makeCopyOf(buffer);

        makeMono(buffer, totalNumInputChannels);

        lpBuffer.makeCopyOf(buffer);
        juce::dsp::AudioBlock<float> hpBlock = juce::dsp::AudioBlock<float>(hpBuffer);
        juce::dsp::AudioBlock<float> lpBlock = juce::dsp::AudioBlock<float>(lpBuffer);
        auto hpCtx = juce::dsp::ProcessContextReplacing<float>(hpBlock);
        auto lpCtx = juce::dsp::ProcessContextReplacing<float>(lpBlock);

        HP.process(hpCtx);
        LP.process(lpCtx);

        for (auto i = 0; i < buffer.getNumChannels(); i++)
        {
            buffer.clear(i, 0, buffer.getNumSamples());
            if (!bassMonoPreviewParam->get())
            {
                buffer.addFrom(i, 0, hpBuffer, i, 0, buffer.getNumSamples());
            }
            buffer.addFrom(i, 0, lpBuffer, i, 0, buffer.getNumSamples());
        }   
    }

    //if (bassMonoPreviewParam->get())
    //{
    //    hpBuffer.clear();
    //}

    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto ctx = juce::dsp::ProcessContextReplacing<float>(block);

    // Gain
    gain.setGainDecibels(gainParam->get());
    gain.process(ctx);

    // Balance
    panner.setPan(juce::jmap(balanceParam->get(), balanceMinRange, balanceMaxRange, -1.f, 1.f));
    panner.process(ctx);

    // Mute
    if (muteParam->get())
    {
        buffer.clear();
        return;
    }

    // Remove DC
    if (dcParam->get())
    {
        dcHighPassFilter.process(ctx);
    }
}

//==============================================================================
bool UtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* UtilityAudioProcessor::createEditor()
{
    return new UtilityAudioProcessorEditor (*this);
    //return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void UtilityAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    // This is the function the DAW calls to save the state.
    // We ask the APVTS to write its current state into an XML format,
    // then copy that XML data into the MemoryBlock provided by the host.

    // Create an XML element to hold the state
    auto state = apvts.copyState(); // Gets the APVTS state as a ValueTree
    std::unique_ptr<juce::XmlElement> xml(state.createXml()); // Converts the ValueTree to XML

    // If the XML was created successfully, copy it to the destination MemoryBlock
    if (xml.get() != nullptr)
        copyXmlToBinary(*xml, destData);

    // **Important:** If you had OTHER non-parameter state variables
    // (e.g., internal filter modes, settings not tied to an AudioParameter)
    // you would need to save them here too, typically by adding them
    // as attributes or child elements to the 'xml' object before calling copyXmlToBinary.
    // For example:
    // if (xml != nullptr) {
    //     xml->setAttribute ("myCustomSetting", myInternalVariable);
    //     copyXmlToBinary (*xml, destData);
    // }
}

void UtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    // This is the function the DAW calls to load the state.
    // We try to convert the binary data back into an XML element.
    // If successful, we tell the APVTS to replace its current state
    // with the state described in the loaded XML.

    // Try to get the XML state from the binary data
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    // If the XML was parsed successfully...
    if (xmlState.get() != nullptr)
    {
        // ...and if it represents a valid ValueTree state...
        if (xmlState->hasTagName(apvts.state.getType()))
        {
            // ...then replace the current APVTS state with the loaded one.
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));

            // **Important:** If you saved OTHER non-parameter state variables
            // in getStateInformation, you need to load them back here
            // from the 'xmlState' object *after* restoring the APVTS state.
            // For example:
            // myInternalVariable = xmlState->getIntAttribute ("myCustomSetting", defaultValue);
            // updateInternalStuffBasedOnLoadedState(); // Call any functions needed to apply changes
        }
    }
    // Make sure the editor reflects the newly loaded state if it's open
    if (auto* editor = getActiveEditor())
    {
        // You might need a function in your editor to update all controls
        // based on the current processor parameters, although APVTS attachments
        // often handle this automatically. If not, trigger an update here.
        // editor->updateControlsFromProcessor();
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout UtilityAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    layout.add(std::make_unique<juce::AudioParameterFloat>("Gain", "Gain", juce::NormalisableRange<float>(-50.f, 50.f, 1.0f, 1.f), 0.f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("Balance", "Balance", juce::NormalisableRange<float>(balanceMinRange, balanceMaxRange, 1.f, 1.f), 0.f));
	layout.add(std::make_unique<juce::AudioParameterFloat>("Width", "Width", juce::NormalisableRange<float>(0.f, 400.f, 1.f, 0.4f), 100.f));
	layout.add(std::make_unique<juce::AudioParameterBool>("Mute", "Mute", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("DC", "DC", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("Mono", "Mono", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("BassMono", "Bass Mono", false));
	layout.add(std::make_unique<juce::AudioParameterFloat>("BassMonoCrossover", "Bass Mono Crossover", juce::NormalisableRange<float>(20.f, 500.f, 1.f), 120.f));
	layout.add(std::make_unique<juce::AudioParameterBool>("BassMonoPreview", "Bass Mono Preview", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("InvertPhaseLeft", "Invert Phase Left", false));
	layout.add(std::make_unique<juce::AudioParameterBool>("InvertPhaseRight", "Invert Phase Right", false));
	layout.add(std::make_unique<juce::AudioParameterChoice>("Mode", "Mode", juce::StringArray{ "Stereo", "Left", "Right", "Swap", }, 0));
    layout.add(std::make_unique<juce::AudioParameterBool>("MidSideMode", "Mid/Side Mode", false));
    layout.add(std::make_unique<juce::AudioParameterFloat>("MidSide", "Mid/Side", juce::NormalisableRange<float>(-100.f, 100.f, 1.0f, 1.0f), 0.f));

    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UtilityAudioProcessor();
}
