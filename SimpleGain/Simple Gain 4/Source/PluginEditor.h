#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class SimpleGainAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    explicit SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor&);
    ~SimpleGainAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    SimpleGainAudioProcessor& audioProcessor;

    juce::Slider gainSlider;
    juce::ToggleButton bypassButton;

    juce::AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment bypassAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SimpleGainAudioProcessorEditor)
};
