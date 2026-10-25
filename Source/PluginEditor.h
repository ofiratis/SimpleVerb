#pragma once
#include "PluginProcessor.h"

class SimpleVerbEditor : public juce::AudioProcessorEditor
{
public:
    explicit SimpleVerbEditor (SimpleVerbProcessor&);
    ~SimpleVerbEditor() override = default;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    struct Knob
    {
        juce::Slider slider { juce::Slider::RotaryHorizontalVerticalDrag, juce::Slider::TextBoxBelow };
        juce::Label label;
        std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attachment;
    };

    void setupKnob (Knob& k, const juce::String& paramId, const juce::String& text);

    SimpleVerbProcessor& processor;
    Knob size, damping, width, mix;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleVerbEditor)
};
