#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
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
            if(command == "fill"){
                int x, y, w, h, r, g, b;
                input >> x >> y >> w >> h >> r >> g >> b;
                fill(x,y,w,h,r,g,b);
                continue;
            }
            if(command == "h_mirror"){
                h_mirror();
                continue;
            }
            if(command == "v_mirror"){
                v_mirror();
                continue;
            }
            if(command == "add"){
                string filename;
                int x, y, r, g, b;
                input >> filename >> r >> g >> b >> x >> y ;
                add(filename,r,g,b,x,y);
                continue;
            }
            if(command == "crop"){
                int x,y,w,h;
                input >> x >> y >> w >> h;
                crop(x,y,w,h);
                continue;
            }
            if(command == "rotate_left"){
                rotate_left();
                continue;
            }
            if(command == "rotate_right"){
                rotate_right();
                continue;
            }
            if(command == "median_filter"){
                int ws;
                input >> ws;
                median_filter(ws);
                continue;
            }
            if(command == "xpm2_open"){
                clear_image_if_any(); //para evitar memory leak 
                string filename;
                input >> filename;
                loadFromXPM2(filename);
                continue;
            }
            if(command == "xpm2_save"){ //xpm2_save chama xpm2_open
                string filename;
                input >> filename;
                saveToXPM2(filename,image);
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
        for(int i = x; i < x+w; i++){
            for(int j = y; j < y+h;j++){
                Color& color = image->at(i,j);
                color.red() = r;
                color.green() = g;
                color.blue() = b;
            }
        }
    }
    void Script::h_mirror(){
        for(int y = 0; y < image->height(); y++){
            for(int x = 0; x < image->width()/2; x++){
                Color color = image->at(x,y);
                image->at(x,y) = image->at(image->width() - x - 1,y);
                image->at(image->width()-x-1,y) = color;
            }
        }
    }
    void Script::v_mirror(){
        for(int y = 0; y < image->height()/2; y++){
            for(int x = 0; x < image->width(); x++){
                Color color = image->at(x,y);
                image->at(x,y) = image->at(x,image->height()-y-1);
                image->at(x,image->height()-y-1) = color;
            }
        }
    }
    
    void Script::add(std::string filename, int r,int g,int b,int x,int y){
        Image* inicial_image_to_add = loadFromPNG(filename);
         for(int i = 0; i < inicial_image_to_add->height(); i++){
            for(int j = 0; j < inicial_image_to_add->width(); j++){
                Color& color = inicial_image_to_add->at(j,i);
                if(color.red() != r || color.green() != g || color.blue() != b){
                    int new_image_x = x + j;
                    int new_image_y = y + i;

                    if(new_image_x >= 0 && new_image_x < image->width() && new_image_y >= 0 && new_image_y < image->height()){
                        image->at(new_image_x,new_image_y) = color;
                    }
                }
            }
        }
        delete inicial_image_to_add;
    }
    
    void Script::crop(int x, int y, int w, int h){
       Image* cropped_image = new Image(w,h);
       for (int i = 0; i < h; i++){
        for (int j = 0; j < w; j++){
            Color pixel = image->at(x + j, y + i);
            cropped_image->at(j,i) = pixel;
        }
       }
       delete image;
       image = cropped_image;
    }
    void Script::rotate_left(){
        int width = image->width();
        int height = image->height();
        Image* rotated_image = new Image(height,width);
        for(int i = 0; i < image->height();i++){
            for(int j = 0; j < image->width();j++){
                Color pixel = image->at(j,i);
                rotated_image->at(i,width-1-j) = pixel;
            }
        }
        delete image;
        image = rotated_image;
    }
    void Script::rotate_right(){
        int width = image->width();
        int height = image->height();
        Image* rotated_image = new Image(height,width);
        for(int i = 0; i < image->height();i++){
            for(int j = 0; j < image->width();j++){
                Color pixel = image->at(j,i);
                rotated_image->at(height-1-i,j) = pixel;
            }
        }
        delete image;
        image = rotated_image; 
    }
    void Script::median_filter(int ws){ //ws = window size
        Image* modified_image = new Image(image->width(),image->height());
        for(int y = 0; y < image->height();y++){
            for(int x = 0; x < image->width();x++){
                vector<rgb_value> red;
                vector<rgb_value> green;
                vector<rgb_value> blue;
                for(int neighbor_y = y - ws / 2; neighbor_y <= y + ws / 2; neighbor_y++){
                    for(int neighbor_x = x - ws /2; neighbor_x <= x + ws / 2;neighbor_x++){
                        if(neighbor_x >= 0 && neighbor_x < image->width() && neighbor_y >= 0 && neighbor_y < image->height()){ //verificar se estamos a aceder a um pixel que existe e está contido na imagem para evitar buffer overflows
                            red.push_back(image->at(neighbor_x, neighbor_y).red());
                            green.push_back(image->at(neighbor_x, neighbor_y).green());
                            blue.push_back(image->at(neighbor_x, neighbor_y).blue());
                        }
                    }
                }
                sort(red.begin(),red.end());
                sort(green.begin(),green.end());
                sort(blue.begin(),blue.end());

                if(red.size() % 2){
                    modified_image->at(x,y).red() = red[red.size()/2];
                    modified_image->at(x,y).green() = green[red.size()/2];
                    modified_image->at(x,y).blue() = blue[red.size()/2];
                }
                else{
                    modified_image->at(x,y).red() = (red[red.size()/2] + red[red.size()/2-1])/2;
                    modified_image->at(x,y).green() = (green[green.size()/2] + green[green.size()/2-1])/2;
                    modified_image->at(x,y).blue() = (blue[blue.size()/2] + blue[blue.size()/2-1])/2;
                }
            }
        }
        delete image;
        image = modified_image;
    }
}
