#include "PluginEditor.h"

SimpleVerbEditor::SimpleVerbEditor (SimpleVerbProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    setupKnob (size,    "size",    "Size");
    setupKnob (damping, "damping", "Damping");
    setupKnob (width,   "width",   "Width");
    setupKnob (mix,     "mix",     "Mix");
    setSize (480, 220);
}

void SimpleVerbEditor::setupKnob (Knob& k, const juce::String& paramId, const juce::String& text)
{
    k.slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
    addAndMakeVisible (k.slider);

    k.label.setText (text, juce::dontSendNotification);
    k.label.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (k.label);

    k.attachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment> (
        processor.apvts, paramId, k.slider);
}

void SimpleVerbEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff1e1f26));
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (18.0f, juce::Font::bold));
    g.drawText ("SimpleVerb", 0, 8, getWidth(), 24, juce::Justification::centred);
}

void SimpleVerbEditor::resized()
{
    auto area = getLocalBounds().withTrimmedTop (40).reduced (10);
    const int w = area.getWidth() / 4;

    for (auto* k : { &size, &damping, &width, &mix })
    {
        auto col = area.removeFromLeft (w);
        k->label.setBounds (col.removeFromTop (20));
        k->slider.setBounds (col);
    }
}
