#include "../JuceLibraryCode/JuceHeader.h"

class SurgeLookAndFeel : public LookAndFeel_V4
{
private:
    ScopedPointer<Drawable> surgeLogo;

public:

    enum SurgeColourIds
    {
        grayBg = 0x2700001,
        orange,
        orangeMedium,
        orangeDark, 
        blue,
        knobHandle,
    };

    SurgeLookAndFeel() {
        Colour surgeGrayBg = Colour(205,206,212);
        Colour surgeOrange = Colour(255,144,0);
        Colour surgeBlue = Colour(18,52,99);
        
        setColour(SurgeColourIds::grayBg, surgeGrayBg);
        setColour(SurgeColourIds::orange, surgeOrange);
        setColour(SurgeColourIds::orangeDark, Colour(101, 50, 3));
        setColour(SurgeColourIds::orangeMedium, Colour(227, 112, 8));
        setColour(SurgeColourIds::blue, surgeBlue);
        setColour(SurgeColourIds::knobHandle, Colour(255,255,255));
        
        setColour(Label::ColourIds::textColourId, surgeBlue);

        surgeLogo = Drawable::createFromImageData (BinaryData::SurgeLogoOnlyBlue_svg, BinaryData::SurgeLogoOnlyBlue_svgSize);
    }


    virtual void drawRotarySlider(Graphics &g,
                                  int x, int y, int width, int height,
                                  float sliderPos,
                                  float rotaryStartAngle, float rotaryEndAngle,
                                  Slider &slider) override
    {
        auto fill = findColour(SurgeColourIds::orange);
        auto edge = findColour(SurgeColourIds::blue);
        auto tick = findColour(SurgeColourIds::knobHandle);

        if( ! slider.isEnabled() )
        {
            fill = findColour(SurgeColourIds::orangeMedium);
            tick = edge;
        }
        

        auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (10);
        g.setColour(fill);
        g.fillEllipse(bounds);
        g.setColour(edge);
        g.drawEllipse(bounds, 1.0);

        
        auto radius = jmin (bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto arcRadius = radius;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto thumbWidth = 5;

        Point<float> thumbPoint (bounds.getCentreX() + arcRadius * std::cos (toAngle - MathConstants<float>::halfPi),
                                 bounds.getCentreY() + arcRadius * std::sin (toAngle - MathConstants<float>::halfPi));
        
        g.setColour (tick);
        g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint));
        g.setColour(edge);
        g.drawEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre (thumbPoint), 1.0);
        g.setColour(tick);
        g.fillEllipse (juce::Rectangle<float> (thumbWidth, thumbWidth).withCentre( bounds.getCentre() ) );

        auto l = Line<float>(thumbPoint, bounds.getCentre());
        g.drawLine(l, thumbWidth );
    }

    virtual void drawButtonBackground(Graphics &g,
                                      Button &button,
                                      const Colour &backgroundColour,
                                      bool 	shouldDrawButtonAsHighlighted,
                                      bool 	shouldDrawButtonAsDown
        ) override
    {
        auto bounds = button.getLocalBounds().toFloat().reduced (0.5f, 0.5f);
        auto col  = findColour(SurgeColourIds::orangeDark);

        if( shouldDrawButtonAsHighlighted )
            col = findColour(SurgeColourIds::blue);

        if( shouldDrawButtonAsDown )
            col = Colour(18 * 1.2, 52 * 1.2, 99 * 1.2);

        if (button.getToggleState() )
            col = findColour(SurgeColourIds::orange);
        
        g.setColour(col);
        g.fillRoundedRectangle(bounds, 3);

    }
    
    void paintComponentBackground(Graphics &g, int w, int h)
    {
        g.fillAll(findColour(SurgeColourIds::grayBg));

        int orangeHeight = 20;
        g.setColour(findColour(SurgeColourIds::orange));
        g.fillRect(0,h-orangeHeight,w,orangeHeight);
        
        juce::Rectangle<float> logoBound { w/2.f-30, h-orangeHeight + 2.f, 60, orangeHeight - 4.f };
        surgeLogo->drawWithin(g, logoBound, juce::RectanglePlacement::xMid | juce::RectanglePlacement::yMid, 1.0 );
        
        g.setColour(findColour(SurgeColourIds::blue));
        g.drawLine(0,h-orangeHeight,w,h-orangeHeight);
    }
    
};
