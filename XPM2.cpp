#include "XPM2.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

namespace prog {

    Color hex_to_rgb(const string& r_hex, const string& g_hex, const string& b_hex) {
        map<char, int> hex_int =
        {
            {'0', 0}, {'1', 1}, {'2', 2}, {'3', 3}, {'4', 4}, {'5', 5}, {'6', 6}, {'7', 7},
            {'8', 8}, {'9', 9}, {'a', 10}, {'b', 11}, {'c', 12}, {'d', 13}, {'e', 14}, {'f', 15}
        };

        int red = hex_int[r_hex[0]] * 16 + hex_int[r_hex[1]];
        int green = hex_int[g_hex[0]] * 16 + hex_int[g_hex[1]];
        int blue = hex_int[b_hex[0]] * 16 + hex_int[b_hex[1]];
        Color result((rgb_value)red, (rgb_value)green, (rgb_value)blue);
        return result;
    }

    Image* loadFromXPM2(const std::string& file) {
        cout << "isto é um teste" << endl;
        ifstream input_file;
        input_file.open(file); 
        string line; 
        getline(input_file, line);  
        getline(input_file, line); 
        istringstream in(line);
        int large;
        int alt;
        int n_cores; 
        int char_por_pixel; // constante 1.
        in >> large >> alt >> n_cores >> char_por_pixel;

        Image* resposta_imagem = new Image(large, alt); 

        char caractere_cor;
        string hex_val_cor; 
        map<char, Color> char_to_color_map; 
        string _; 
        
        for (int i = 0; i < n_cores; i++) {
            getline(input_file, line);
            istringstream line_in(line);
            line_in >> caractere_cor >> _ >> hex_val_cor;
            hex_val_cor = hex_val_cor.substr(1, 6); 
            for (char& current_char: hex_val_cor) {
                current_char = tolower(current_char);
            }
            char_to_color_map[caractere_cor] = hex_to_rgb(hex_val_cor.substr(0, 2), hex_val_cor.substr(2, 2), hex_val_cor.substr(4, 2));
        }

        //imagem resultado.
        for (int num_line = 0; num_line < resposta_imagem->height(); num_line++) {
            getline(input_file, line);
            for (int num_char = 0; num_char < resposta_imagem->width(); num_char++) {
                resposta_imagem->at(num_char, num_line) = char_to_color_map[line[num_char]]; 
            }       
        }
        input_file.close();
        return resposta_imagem;
    }

    bool check_color_in_vector(const vector<pair<Color, char>> vector, const Color color) {
        for (pair<Color, char> cor_para_caractere : vector) {
            if (cor_para_caractere.first == color) {
                return true;
            }
        }
        return false;
    }

    string convert_from_rgb_value_to_hex(rgb_value value) {
        map<int, char> int_hex =
        {
            {0, '0'}, {1, '1'}, {2, '2'}, {3, '3'}, {4, '4'}, {5, '5'}, {6, '6'}, {7, '7'},
            {8, '8'}, {9, '9'}, {10, 'a'}, {11, 'b'}, {12, 'c'}, {13, 'd'}, {14, 'e'}, {15, 'f'}
        };
        string result = "";
        result += int_hex[value / 16];
        result += int_hex[value % 16];
        return result;
    }

    void saveToXPM2(const std::string& file, const Image* image) {
        ofstream output_file;
        output_file.open(file);
        vector<pair<Color, char>> color_to_char_vector;
        size_t num_colors = 0;
        for (int y = 0; y < image->height(); y++) {
            for (int x = 0; x < image->width(); x++) {
                if (!check_color_in_vector(color_to_char_vector, image->at(x, y))){
                    color_to_char_vector.emplace_back(image->at(x, y), 'a' + num_colors);
                    num_colors++;
                }
            }
        }
        output_file << "! XPM2" << endl;
        output_file << image->width() << " " << image->height() << " " << num_colors << " " << "1" << endl;
        for (const auto& pair : color_to_char_vector) {
            const Color& current_color = pair.first;
            char char_of_current_color = pair.second;
            output_file << char_of_current_color << " c " << "#" << convert_from_rgb_value_to_hex(current_color.red()) << convert_from_rgb_value_to_hex(current_color.green()) << convert_from_rgb_value_to_hex(current_color.blue()) << endl;
        }
        
        for (int y = 0; y < image->height(); y++) {
            for (int x = 0; x < image->width(); x++) {
                const Color& pixel_color = image->at(x, y);
                // Encontra a cor do pixel atual que está no vetor de cores para caracteres.
                auto iterator_pair = find_if(color_to_char_vector.begin(), color_to_char_vector.end(), [&](pair<Color, char> element_pair) { return element_pair.first == pixel_color; });
                if (iterator_pair != color_to_char_vector.end()) {
                    output_file << iterator_pair->second;
                }
            }
            output_file << endl;
        }
        output_file.close();
    }
}