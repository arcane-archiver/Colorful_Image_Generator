/**
 * @file HSLAPixel.cpp
 * Implementation of the HSLAPixel class for use in with the PNG library.
 *
 * @author CS 225: Data Structures
 * @version 2018r1-lab1
 */

#include "HSLAPixel.h"
#include <cmath>
#include <iostream>
using namespace std;

#include <stdexcept> // Adrian included

namespace cs225 {
    HSLAPixel::HSLAPixel(): h(0), s(1), l(1), a(1.0) {}

    HSLAPixel::HSLAPixel(double hue, double saturation, double luminance):
    h(hue), s(saturation), l(luminance), a(1/*opaque*/) {
        if (hue < 0 || 360 <= hue) {
            throw std::runtime_error("hue must be in [0, 360) but is " + to_string(hue));
        } else if (saturation < 0 || 1 < saturation) {
            throw std::runtime_error("saturation must be in [0, 1] but is " + to_string(saturation));
        } else if (luminance < 0 || 1 < luminance) {
            throw std::runtime_error("luminance must be in [0, 1] but is " + to_string(luminance));
        }
    }

    HSLAPixel::HSLAPixel(double hue, double saturation, double luminance, double alpha):
    h(hue), s(saturation), l(luminance), a(alpha) {
        if (hue < 0 || 360 <= hue) {
            throw std::runtime_error("hue must be in [0, 360) but is " + to_string(hue));
        } else if (saturation < 0 || 1 < saturation) {
            throw std::runtime_error("saturation must be in [0, 1] but is " + to_string(saturation));
        } else if (luminance < 0 || 1 < luminance) {
            throw std::runtime_error("luminance must be in [0, 1] but is " + to_string(luminance));
        } else if (alpha < 0 || 1 < alpha) {
            throw std::runtime_error("alpha must be in [0, 1] but is " + to_string(alpha));
        }
    }
}
