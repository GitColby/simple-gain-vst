#include "PluginEditor.h"

SimpleGainAudioProcessorEditor::SimpleGainAudioProcessorEditor(SimpleGainAudioProcessor& p)
    : AudioProcessorEditor(&p),
      audioProcessor(p),
      gainAttachment(audioProcessor.parameters, "gain", gainSlider),
      bypassAttachment(audioProcessor.parameters, "bypass", bypassButton)
{
    addAndMakeVisible(gainSlider);
    addAndMakeVisible(bypassButton);

    gainSlider.setSliderStyle(juce::Slider::Rotary);
    gainSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 50, 20);

    bypassButton.setButtonText("Bypass");

    setSize(200, 150);
}

SimpleGainAudioProcessorEditor::~SimpleGainAudioProcessorEditor() = default;

void SimpleGainAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawFittedText("Simple Gain Plugin", getLocalBounds(), juce::Justification::centredTop, 1);
}

void SimpleGainAudioProcessorEditor::resized()
{
    gainSlider.setBounds(30, 40, 140, 80);
    bypassButton.setBounds(60, 120, 80, 20);
}
