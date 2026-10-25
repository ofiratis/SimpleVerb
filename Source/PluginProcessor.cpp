#include "PluginProcessor.h"
#include "PluginEditor.h"

SimpleVerbProcessor::SimpleVerbProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)),
      apvts (*this, nullptr, "PARAMS", createLayout())
{
}

juce::AudioProcessorValueTreeState::ParameterLayout SimpleVerbProcessor::createLayout()
{
    using P = juce::AudioParameterFloat;
    using Range = juce::NormalisableRange<float>;

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::make_unique<P> (juce::ParameterID { "size", 1 },    "Size",    Range (0.0f, 1.0f, 0.001f), 0.5f));
    layout.add (std::make_unique<P> (juce::ParameterID { "damping", 1 }, "Damping", Range (0.0f, 1.0f, 0.001f), 0.5f));
    layout.add (std::make_unique<P> (juce::ParameterID { "width", 1 },   "Width",   Range (0.0f, 1.0f, 0.001f), 1.0f));
    layout.add (std::make_unique<P> (juce::ParameterID { "mix", 1 },     "Mix",     Range (0.0f, 1.0f, 0.001f), 0.3f));
    return layout;
}

void SimpleVerbProcessor::prepareToPlay (double sampleRate, int)
{
    reverb.setSampleRate (sampleRate);
    reverb.reset();
}

bool SimpleVerbProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto out = layouts.getMainOutputChannelSet();
    if (out != juce::AudioChannelSet::mono() && out != juce::AudioChannelSet::stereo())
        return false;
    return out == layouts.getMainInputChannelSet();
}

void SimpleVerbProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer&)
{
    juce::ScopedNoDenormals noDenormals;

    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    const float mix = apvts.getRawParameterValue ("mix")->load();

    juce::Reverb::Parameters p;
    p.roomSize   = apvts.getRawParameterValue ("size")->load();
    p.damping    = apvts.getRawParameterValue ("damping")->load();
    p.width      = apvts.getRawParameterValue ("width")->load();
    p.wetLevel   = mix;
    p.dryLevel   = 1.0f - mix;
    p.freezeMode = 0.0f;
    reverb.setParameters (p);

    if (buffer.getNumChannels() >= 2)
        reverb.processStereo (buffer.getWritePointer (0), buffer.getWritePointer (1), buffer.getNumSamples());
    else if (buffer.getNumChannels() == 1)
        reverb.processMono (buffer.getWritePointer (0), buffer.getNumSamples());
}

juce::AudioProcessorEditor* SimpleVerbProcessor::createEditor()
{
    return new SimpleVerbEditor (*this);
}

void SimpleVerbProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    if (auto xml = apvts.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void SimpleVerbProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xml));
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SimpleVerbProcessor();
}
