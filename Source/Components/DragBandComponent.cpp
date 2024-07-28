/*
  ==============================================================================

    DragBandComponent.cpp
    Created: 9 Apr 2024 1:08:02pm
    Author:  Alex Huber

  ==============================================================================
*/

#include "DragBandComponent.h"

DragBandComponent::Thumb::Thumb(juce::Colour color)
: color(color)
{
    constrainer.setMinimumOnscreenAmounts(thumbSize, thumbSize, thumbSize, thumbSize);
}

void DragBandComponent::Thumb::paint(juce::Graphics &g)
{
    g.setColour(color);
    g.fillEllipse(getLocalBounds().toFloat());
}

void DragBandComponent::Thumb::mouseDown(const juce::MouseEvent &event)
{
    dragger.startDraggingComponent(this, event);
}

void DragBandComponent::Thumb::mouseDrag(const juce::MouseEvent &event)
{
    dragger.dragComponent(this, event, &constrainer);
    if (moveCallback)
        moveCallback(getPosition().toDouble());
}

void DragBandComponent::Thumb::mouseWheelMove(const juce::MouseEvent &event, const juce::MouseWheelDetails &wheel)
{
    if (wheelCallback)
        wheelCallback(wheel);
}

DragBandComponent::DragBandComponent(PaperEQAudioProcessor& p, int band, juce::Colour color)
: audioProcessor(p),
color(color),
band(band),
thumb(color)
{
    addAndMakeVisible(thumb);
    setInterceptsMouseClicks(false, true);
    thumb.moveCallback = [&](juce::Point<double> position)
    {
        const auto bounds = getLocalBounds().toDouble();
        const auto width = static_cast<double>(thumbSize);
        for (auto* slider : xSliders)
        {
            auto linearPosition = static_cast<double>(juce::jmap(position.getX(),
                                           0.0,
                                           bounds.getWidth() - width,
                                           slider->getMinimum(),
                                           slider->getMaximum()));
            
            double minLog = std::log(slider->getMinimum());
            double maxLog = std::log(slider->getMaximum());
            double logPosition = std::exp(minLog + (maxLog - minLog) * (linearPosition - slider->getMinimum()) / (slider->getMaximum() - slider->getMinimum()));
            
            slider->setValue(logPosition, juce::NotificationType::sendNotification);
        }
        for (auto* slider : ySliders)
        {
            slider->setValue(static_cast<double>(juce::jmap(position.getY(),
                                                            0.0,
                                                            bounds.getHeight() - width,
                                                            slider->getMaximum(),
                                                            slider->getMinimum())),
                             juce::NotificationType::sendNotification);
        }
        repaint();
    };
    thumb.wheelCallback = [&](juce::MouseWheelDetails wheel)
    {
        for (auto* slider : zSliders)
        {
            slider->setValue(slider->getValue() + wheel.deltaY * 5);
        }
        repaint();
    };
}

void DragBandComponent::registerSlider(juce::Slider* slider, Axis axis)
{
    slider->addListener(this);
    if (axis == Axis::X)
        xSliders.push_back(slider);
    if (axis == Axis::Y)
        ySliders.push_back(slider);
    if (axis == Axis::Z)
        zSliders.push_back(slider);
}

void DragBandComponent::deregisterSlider(juce::Slider *slider)
{
    slider->removeListener(this);
    xSliders.erase(std::remove(xSliders.begin(), xSliders.end(), slider), xSliders.end());
    ySliders.erase(std::remove(ySliders.begin(), ySliders.end(), slider), ySliders.end());
    zSliders.erase(std::remove(zSliders.begin(), zSliders.end(), slider), zSliders.end());
}

void DragBandComponent::sliderValueChanged(juce::Slider* slider)
{
    if (thumb.isMouseOverOrDragging(false))
        return;
    
    const auto isXAxisSlider = std::find(xSliders.begin(), xSliders.end(), slider) != xSliders.end();
    const auto isYAxisSlider = std::find(ySliders.begin(), ySliders.end(), slider) != ySliders.end();
    const auto bounds = getLocalBounds().toDouble();
    const auto width = static_cast<double>(thumbSize);
    if (isXAxisSlider)
    {
        thumb.setTopLeftPosition(juce::jmap(
                                            juce::mapFromLog10(slider->getValue(), static_cast<double>(20.f), static_cast<double>(20000.f)),
                                            juce::mapFromLog10(slider->getMinimum(), static_cast<double>(20.f), static_cast<double>(20000.f)),
                                            juce::mapFromLog10(slider->getMaximum(), static_cast<double>(20.f), static_cast<double>(20000.f)),
                                            0.0,
                                            bounds.getWidth()),
                                 thumb.getY());
    }
    else if (isYAxisSlider)
    {
        thumb.setTopLeftPosition(thumb.getX(), juce::jmap(
                                                          slider->getValue(),
                                                          slider->getMaximum(),
                                                          slider->getMinimum(),
                                                          0.0,
                                                          bounds.getHeight()));
    }
    repaint();
}

void DragBandComponent::paint(juce::Graphics &g)
{
    const auto coefficients = updateCoefficients(band);
    
    const auto sampleRate = audioProcessor.getSampleRate();
    
    const auto bounds = getLocalBounds().toFloat();
    const auto left = bounds.getX();
    const auto width = bounds.getWidth();
    const auto top = bounds.getY();
    const auto bottom = bounds.getHeight();
    
    std::vector<float> lineMagnitudes;
    lineMagnitudes.resize(width);
    
    for (int i = 0; i < width; ++i)
    {
        float lineMagnitude = 1.f;
        
        auto freq = juce::mapToLog10(static_cast<float>(i) / width, 20.f, 20000.f);
        
        for (auto coefficient : coefficients)
        {
            lineMagnitude *= coefficient->getMagnitudeForFrequency(freq, sampleRate);
        }
        
        lineMagnitudes[i] = juce::Decibels::gainToDecibels(lineMagnitude);
    }
    
    juce::Path responseCurve;
    
    auto map = [top, bottom] (float magnitude)
    {
        return juce::jmap(magnitude, 18.f, -18.f, top, bottom);
    };
    
    responseCurve.startNewSubPath(left, map(lineMagnitudes.front()));
    
    for (size_t i = 1; i < lineMagnitudes.size(); ++i)
    {
        responseCurve.lineTo(left + i, map(lineMagnitudes[i]));
    }
    
    g.setColour(color);
    g.strokePath(responseCurve, juce::PathStrokeType(1.f));
}

void DragBandComponent::resized()
{
    thumb.setBounds(getLocalBounds().withSizeKeepingCentre(thumbSize, thumbSize));
    if (!xSliders.empty())
        sliderValueChanged(xSliders[0]);
    if (!ySliders.empty())
        sliderValueChanged(ySliders[0]);
}

juce::Array<juce::dsp::IIR::Filter<float>::CoefficientsPtr> DragBandComponent::updateCoefficients(int band)
{
    switch (band)
    {
        case 0:
            return audioProcessor.getLowCutCoefficients();
            break;
        case 1:
            return audioProcessor.getLowShelfCoefficients();
            break;
        case 2:
            return audioProcessor.getPeakCoefficients();
            break;
        case 3:
            return audioProcessor.getHighShelfCoefficients();
        default:
            return audioProcessor.getHighCutCoefficients();
            break;
    }
}
