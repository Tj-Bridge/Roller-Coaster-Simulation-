#pragma once
/* Generates a color using HSV rather than RGB
   See: 
      https://www.rapidtables.com/convert/color/hsv-to-rgb.html
      https://en.wikipedia.org/wiki/HSL_and_HSV#From_HSV 
*/
namespace ColorNG {
    System::Drawing::Color colorFromHSV(float hue, float saturation, float value);

    void colorToHSV(System::Drawing::Color aColor, float& hue, float& saturation, float& value);
}