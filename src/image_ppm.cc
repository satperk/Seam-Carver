#include "image_ppm.hpp"

using namespace std;

// implement the rest of ImagePPM's functions here
Pixel ImagePPM::GetPixel(int row, int col)	const {
  return pixels_[row][col];
}


int ImagePPM::GetMaxColorValue() const {
  return max_color_value_; 
}


void ImagePPM::HorizontalCarve(int const* array) {
  Pixel** final_pixels = new Pixel*[height_ - 1];
  for (int row = 0; row < height_ - 1; row++) {
    final_pixels[row] = new Pixel[width_];
  }
  for (int col = 0; col < width_; col++) {
    int row_ctr = 0;
    for (int row = 0; row < height_; row++) {
      if (row != array[col]) {
        final_pixels[row_ctr][col] = pixels_[row][col];
        row_ctr += 1;
      }
    }
  }
  int h = height_ - 1;
  int w = width_;
  Clear();
  height_ = h;
  width_ = w;
  pixels_ = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    pixels_[i] = new Pixel[width_];
  }
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      pixels_[i][j] = final_pixels[i][j];
    }
  }
  for (int j = 0; j < height_; ++j) {
    delete[] final_pixels[j];
  }
  delete[] final_pixels;
}

void ImagePPM::VerticalCarve(int const* array) {
  Pixel** final_pixel = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    final_pixel[row] = new Pixel[width_ - 1];
  }
  for (int row = 0; row < height_; row++) {
    int col_ctr = 0;
    for (int col = 0; col < width_; col++) {
      if (col != array[row]) {
        final_pixel[row][col_ctr] = pixels_[row][col];
        col_ctr += 1;
      }
    }
  }
  int h = height_;
  int w = width_ - 1;
  Clear();
  height_ = h;
  width_ = w;
  pixels_ = new Pixel*[height_];
  for (int i = 0; i < height_; i++) {
    pixels_[i] = new Pixel[width_];
  }
  for (int i = 0; i < height_; i++) {
    for (int j = 0; j < width_; j++) {
      pixels_[i][j] = final_pixel[i][j];
    }
  }
  for (int j = 0; j < height_; ++j) {
    delete[] final_pixel[j];
  }
  delete[] final_pixel;
}


void ImagePPM::RemoveHorizontalSeamHelper(Pixel** pixel) {
  for(int r=0;r<height_;r++) {
    delete[] pixels_[r];
  }
  delete[] pixels_;
  pixels_ = nullptr;
  pixels_ = pixel;
  height_--;
}

void ImagePPM::RemoveVerticalSeamHelper(Pixel** p) {
  for(int r=0;r<height_;r++) {
    delete[] pixels_[r];
  }
  delete[] pixels_;
  pixels_ = nullptr;
  pixels_= p;
  width_--;
}



std::ostream& operator<<(std::ostream& os, const ImagePPM& image)	{
    os<<"P3"<< std::endl;
    os<<image.GetWidth()<< " "<<image.GetHeight()<< std::endl;
    os<<image.GetMaxColorValue()<<std::endl;
    for(int r=0; r<image.GetHeight(); r++) {
        for(int c=0; c<image.GetWidth(); c++) {
            os<<image.GetPixel(r,c).GetRed()<<std::endl;
            os<<image.GetPixel(r,c).GetGreen()<<std::endl;
            os<<image.GetPixel(r,c).GetBlue()<<std::endl;
        }
    }
    return os;
}



// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM(const std::string& path) {
  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) { *this = source; }

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }
  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }
void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;
  getline(is, line);
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }
  int blank = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, blank));
  image.height_ = std::stoi(line.substr(blank + 1));
  getline(is, line);
  image.max_color_value_ = std::stoi(line);
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }
  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);
      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }
  return is;
}
