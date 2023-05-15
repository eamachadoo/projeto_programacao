#include "Image.hpp"

namespace prog
{
  Image::Image(int w, int h, const Color &fill) : w_(w), h_(h) {
    p = new Color[w*h]; // defenir tamanho
    Color* pointer = p; // pointer aponta para o primeiro elemento de p
    for (int i = 0; i < w*h;++i){
      *pointer = fill;
      ++pointer;
    } // iterar o pointer em si, os elementos do pointer
  }

  Image::~Image()
  {
    delete[] p;
  }

  int Image::width() const
  {
    return w_;
  }
  int Image::height() const
  {
    return h_;
  }

  Color& Image::at(int x, int y)
  {
    return p[x*h_ + y];
  }

  const Color& Image::at(int x, int y) const
  {
    return p[x*h_ + y];
  }
}
