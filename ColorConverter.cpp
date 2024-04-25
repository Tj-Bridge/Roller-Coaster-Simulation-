
#include <math.h>
#include <algorithm>
#include "ColorConverter.h"

System::Drawing::Color ColorNG::colorFromHSV(float hue, float saturation, float value)
{
	// using formula from:
	// https://www.rapidtables.com/convert/color/hsv-to-rgb.html

	// adjust H to be 0 <= H <= 360
	hue = fabs(fmod(hue, 360.));  // note that H=360 is the same as H=0

	double C = value * saturation;
	double X = C * (1 - fabs(fmod(hue / 60., 2) - 1));
	double m = value - C;
	double Rprime, Gprime, Bprime;

	if (hue < 60.) {
		Rprime = C;
		Gprime = X;
		Bprime = 0.;
	}
	else if (hue < 120.) {
		Rprime = X;
		Gprime = C;
		Bprime = 0.;
	}
	else if (hue < 180.) {
		Rprime = 0.;
		Gprime = C;
		Bprime = X;
	}
	else if (hue < 240.) {
		Rprime = 0.;
		Gprime = X;
		Bprime = C;
	}
	else if (hue < 300.) {
		Rprime = X;
		Gprime = 0.;
		Bprime = C;
	}
	else if (hue < 360.) {
		Rprime = C;
		Gprime = 0.;
		Bprime = X;
	}

	return System::Drawing::Color::FromArgb((Rprime + m)*255, 
		(Gprime + m) * 255, (Bprime + m) * 255);

}

void ColorNG::colorToHSV(System::Drawing::Color aColor, float& hue, float& saturation, float& value)
{
	using namespace std;

	// using approach from https://www.geeksforgeeks.org/program-change-rgb-color-model-hsv-color-model/
	double r = aColor.R / 255.;
	double g = aColor.G / 255.;
	double b = aColor.B / 255.;
	double tolerance = 1e-8;

	// h, s, v = hue, saturation, value 

	double cmax = max(r, max(g, b)); // maximum of r, g, b 
	double cmin = min(r, min(g, b)); // minimum of r, g, b 
	double diff = cmax - cmin; // diff of cmax and cmin. 
	hue = -1;
	saturation = -1;

	// if cmax and cmin are equal then hue = 0 
	if (fabs(cmax - cmin) < tolerance)
		hue = 0.f;

	// if cmax equals r then compute hue 
	else if (fabs(cmax - r)  < tolerance)
		hue = fmod(60. * ((g - b) / diff) + 360., 360.);

	// if cmax equal g then compute hue 
	else if (fabs(cmax - g) < tolerance)
		hue = fmod(60 * ((b - r) / diff) + 120., 360.);

	// if cmax equal b then compute hue 
	else if (fabs(cmax - b) < tolerance)
		hue = fmod(60 * ((r - g) / diff) + 240., 360.);

	// if cmax equal zero 
	if (fabs(cmax) < tolerance)
		saturation = 0.;
	else
		saturation = (diff / cmax);

	// compute value
	value = cmax;
	//std::cout << "(" << hue << " " << saturation << " " << v << ")" << endl;
}


