#ifndef __prog_XPM2_hpp__
#define __prog_XPM2_hpp__

#include <string>
#include <utility>
#include "Image.hpp"
#include "Color.hpp"

using namespace std;

namespace prog {

    Image* loadFromXPM2(const std::string &file);
    Color hex_to_rgb(const string& r_hex, const string& g_hex, const string& b_hex);
    void saveToXPM2(const std::string& file, const Image* img);
    Color hex_to_rgb(const string& r_hex, const string& g_hex, const string& b_hex);
    bool check_color_in_vector(const vector<pair<Color, char>> vector, const Color color);
    void saveToXPM2(const std::string& file, const Image* image);
    string convert_from_rgb_value_to_hex(rgb_value value);
}
#endif
