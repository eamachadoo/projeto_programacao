#include <iostream>
#include <fstream>
#include "Script.hpp"
#include "PNG.hpp"
#include "XPM2.hpp"

using namespace std;

namespace prog {
    // Use to read color values from a script file.
    istream& operator>>(istream& input, Color& c) {
        int r, g, b;
        input >> r >> g >> b;
        c.red() = r;
        c.green() = g;
        c.blue() = b;
        return input;
    }

    Script::Script(const string& filename) :
            image(nullptr), input(filename) {

    }
    void Script::clear_image_if_any() {
        if (image != nullptr) {
            delete image;
            image = nullptr;
        }
    }
    Script::~Script() {
        clear_image_if_any();
    }

    void Script::run() {
        string command;
        while (input >> command) {
            cout << "Executing command '" << command << "' ..." << endl;
            if (command == "open") {
                open();
                continue;
            }
            if (command == "blank") {
                blank();
                continue;
            }
            // Other commands require an image to be previously loaded.
            if (command == "save") {
                save();
                continue;
            } 
            if (command == "invert"){
                invert();
                continue;
            }
            if(command == "to_gray_scale"){
                to_gray_scale();
                continue;
            }
            if(command == "replace"){
                int r1,g1,b1,r2,g2,b2;
                input >> r1 >>g1 >>b1 >> r2 >> g2>> b2;
                replace(r1,g1,b1,r2,g2,b2);
                continue;
            }
            // TODO ...

        }
    }
    void Script::open() {
        // Replace current image (if any) with image read from PNG file.
        clear_image_if_any();
        string filename;
        input >> filename;
        image = loadFromPNG(filename);
    }
    void Script::blank() {
        // Replace current image (if any) with blank image.
        clear_image_if_any();
        int w, h;
        Color fill;
        input >> w >> h >> fill;
        image = new Image(w, h, fill);
    }
    void Script::save() {
        // Save current image to PNG file.
        string filename;
        input >> filename;
        saveToPNG(filename, image);
    }
    void Script::invert(){
        // Invert color
        for(int h =0; h < image->height(); h++){
            for(int w=0;w < image->width();w++){
                Color& color = image->at(w,h);
                color.red() = 255 - color.red();
                color.green() = 255 - color.green();
                color.blue() = 255 - color.blue(); // defenir as 3 colores no ponto para onde image at está a apontar.
            }
        }
    }
    void Script::to_gray_scale(){
        for(int h =0; h < image->height(); h++){
            for(int w=0;w < image->width();w++){
                Color& color = image->at(w,h);
                int v = (color.red() + color.green() + color.blue()) / 3 ;
                color.red() = v;
                color.green() = v;
                color.blue() = v;
            }
        }
    }
    void Script::replace(int r1,int g1,int b1,int r2,int g2,int b2){
       for(int h =0; h < image->height(); h++){
            for(int w=0;w < image->width();w++){
                Color& color = image->at(w,h);
                if(color.red() == r1 && color.green() == g1 && color.blue() == b1){
                    color.red() = r2;
                    color.green() = g2;
                    color.blue() = b2;
                }
            }
        }
    }
    void Script::fill(int x, int y, int w, int h, int r, int g, int b){
        for(int i = x; x < x+w; i++){
            for(int j = y; y < y+w;j++){
                Color& color = image->at(i,j);
                color.red() = r;
                color.green() = g;
                color.blue() = b;
            }
        }
    }
    void Script::h_mirror(){
        for(int i = 0; i < image->width(); i++){
            for(int j = 0; j < image->height(); j++){
                Color& color1 = image->at(i,j);
                Color& color2 = image->at(image->width() - 1 - i, j);
                swap(color1,color2);
            }
        }
    }
    void Script::v_mirror(){
        for(int i = 0; i < image->height(); i++){
            for(int j = 0; j < image->width(); j++){
                Color& color1 = image->at(i,j);
                Color& color2 = image->at(i, image->height() - 1 - j);
                swap(color1,color2);
            }
        }
    }
    void Script::add(string filename, int r,int g,int b,int x,int y){
        
    }
}
