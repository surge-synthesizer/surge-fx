#include "../JuceLibraryCode/JuceHeader.h"

class SurgeLookAndFeel : public LookAndFeel_V4
{
public:

    Colour surgeGrayBg;
    Colour surgeOrange;
    Colour surgeBlue;

    SurgeLookAndFeel() {
        surgeGrayBg = Colour(205,206,212);
        surgeOrange = Colour(255,144,0);
        surgeBlue = Colour(18,52,99);

        setColour(Label::ColourIds::textColourId, surgeBlue);
    }
};
