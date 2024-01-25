#include "seam_carver.hpp"

#include <iostream>

#include "pixel.hpp"

using namespace std;

// implement the rest of SeamCarver's functions here

const ImagePPM& SeamCarver::GetImage() const {
  return image_;

}

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel left(0, 0, 0);
  Pixel across(0, 0, 0);
  Pixel up(0, 0, 0);
  Pixel down(0, 0, 0);
  if((col == 0 && row == 0) || (col == 0 && row == height_ - 1) || (col == width_ - 1 && row == 0) || (col == width_ - 1 && row == height_ - 1)) {
    EnergyCorner(row, col, left, across, up, down);
  }
  else if (col == (width_ - 1) || col == 0) { //START LEANER && END LEANER

    EnergyColumnEnds(row, col, left, across, up, down);
  }
  else if((row == 0 || row == height_ - 1)) {//TOP LEANER && BOTTOM LEANER
    EnergyRowEnds(row, col, left, across, up, down);
  } else { //MIDDLE GUYS

    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, col + 1);

    up = image_.GetPixel(row - 1, col); 
    down = image_.GetPixel(row + 1, col);
  }
  int left_and_right = ((left.GetRed() - across.GetRed()) * (left.GetRed() - across.GetRed())) + 
                        ((left.GetGreen() - across.GetGreen()) * (left.GetGreen() - across.GetGreen())) +
                        ((left.GetBlue() - across.GetBlue()) * (left.GetBlue() - across.GetBlue()));

int up_and_down = ((up.GetRed() - down.GetRed()) * (up.GetRed() - down.GetRed())) + 
                        ((up.GetGreen() - down.GetGreen()) * (up.GetGreen() - down.GetGreen())) +
                        ((up.GetBlue() - down.GetBlue()) * (up.GetBlue() - down.GetBlue()));
int energy = left_and_right + up_and_down;
return energy;
}
void SeamCarver::EnergyColumnEnds(int row, int col, Pixel& left, Pixel& across, Pixel& up, Pixel& down) const {
  if(col == 0) {
    left = image_.GetPixel(row, width_ - 1); //gets changed
    across = image_.GetPixel(row, col + 1);
    up = image_.GetPixel(row - 1, col); 
    down = image_.GetPixel(row + 1, col);
  } else {
    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, 0); //gets changed
    up = image_.GetPixel(row - 1, col); 
    down = image_.GetPixel(row + 1, col);
  }
}

void SeamCarver::EnergyRowEnds( int row, int col, Pixel& left, Pixel& across, Pixel& up, Pixel& down) const {

  if(row == 0) {
    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, col + 1);

    up = image_.GetPixel(height_ - 1, col); //gets changed
    down = image_.GetPixel(row + 1, col);

  } else if(row == height_ - 1) {
    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, col + 1);

    up = image_.GetPixel(row - 1, col); 
    down = image_.GetPixel(0, col); //gets changed
  }
}

void SeamCarver::EnergyCorner( int row, int col, Pixel& left, Pixel& across, Pixel& up, Pixel& down) const {
  if((col == 0 && row == 0)) {//TOP across

    left = image_.GetPixel(0, width_ - 1);
    across = image_.GetPixel(0, 1);
    up = image_.GetPixel(height_ - 1, 0);
    down = image_.GetPixel(row + 1, col);
    

  } else if(col == 0 && row == height_ - 1) { //Bottom LEFT
    left = image_.GetPixel(row, width_ - 1);
    across = image_.GetPixel(row, col + 1);
    up = image_.GetPixel(row - 1, 0);
    down = image_.GetPixel(0, 0);

  } else if (col == width_ - 1 && row == 0) { //Top across

    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, 0);
    up = image_.GetPixel(height_ - 1, col);
    down = image_.GetPixel(row + 1, col);


  } else { //col == width_ - 1 && height_ - 1 - Bottom across
    left = image_.GetPixel(row, col - 1);
    across = image_.GetPixel(row, 0);
    up = image_.GetPixel(row - 1, col);
    down = image_.GetPixel(0, col);

  }
}








int SeamCarver::GetHeight() const {

  return height_;

 }

  // returns the final_image's width
int SeamCarver::GetWidth() const {
  return width_;
 }

void SeamCarver::PopulateHorizontalSeam(int**& array) const {
  for (int row = 0; row < height_; row++) {
    array[row] = new int[width_]; //Intializing the 2D array
    array[row][width_ - 1] = GetEnergy(row, width_ - 1); //Intiailizing the starting COL(LAST COL)
  }
  for(int col = width_ - 2; col >= 0; col--) {
    for(int row = 0; row < height_; row++) {
      if(row == 0) {
        int across = array[row][col + 1];
        int across_down = array[row + 1][col + 1];
        if(across <= across_down) {
          array[row][col] = GetEnergy(row, col) + across;
        } else {
          array[row][col] = GetEnergy(row, col) + across_down;
        }
      } else if(row==height_-1) {
        int across_up=array[row-1][col+1];
        int across=array[row][col+1];
        if(across<=across_up) {
          array[row][col]=GetEnergy(row,col)+across;
        } else {
          array[row][col]=GetEnergy(row,col)+across_up;
        }
      } else {
        int across_up = array[row - 1][col + 1];
        int across = array[row][col + 1];
        int across_down = array[row + 1][col + 1];
        if(across<=across_down && across<=across_up) {
          array[row][col]=GetEnergy(row,col)+across;
        } else if(across_up<=across_down && across_up<across) {
          array[row][col]=GetEnergy(row,col)+across_up;
        } else if(across_down<across_up && across_down<across) {
          array[row][col]=GetEnergy(row,col)+across_down;
        }
      }
    }
  }
}
 

int* SeamCarver::GetHorizontalSeam() const {
  int* horizontal_seam = new int[width_]; int** array = new int*[height_]; PopulateHorizontalSeam(array);
  int first_row_smallest = array[0][0]; int first_col_smallest_index = 0;
  for(int row = 0; row < height_; row++) {
    if(array[row][0] < first_row_smallest) {
      first_row_smallest = array[row][0]; first_col_smallest_index = row; 
    }
  }
  horizontal_seam[0] = first_col_smallest_index;
  for(int col = 0; col < width_; col++) {
    int row = first_col_smallest_index;
    if(row == 0) {
      int across = array[row][col + 1]; int across_down = array[row + 1][col + 1];
      if(across <= across_down) { //row doesnt change for the guy just the col does
        horizontal_seam[col + 1] = first_col_smallest_index;
      } else {
        first_col_smallest_index = row + 1; horizontal_seam[col + 1] = first_col_smallest_index;
      }
    } else if (row == height_ -1) {
      int across_up = array[row - 1][col + 1]; int across = array[row][col + 1];
      if(across_up <= across) {
        first_col_smallest_index = row - 1; horizontal_seam[col + 1] = col + 1;
      } else {
        horizontal_seam[col + 1] = first_col_smallest_index;
      }
    } else {
      int across_up = array[row - 1][col + 1]; int across = array[row][col + 1]; int across_down = array[row + 1][col + 1];
      if(across_up <= across && across_up <= across_down) {
        first_col_smallest_index = row - 1; horizontal_seam[col + 1] = first_col_smallest_index;
      } else if (across < across_up && across <= across_down) { //fix
        horizontal_seam[col + 1] = first_col_smallest_index;
      } else {
        first_col_smallest_index = row + 1; horizontal_seam[col + 1] = first_col_smallest_index;
      }
    }
  }
  for (int i = 0; i < height_; i++) {
    delete[] array[i];
  }
  delete[] array; array = nullptr; return horizontal_seam;
}



void SeamCarver::PopulateVerticalSeam(int**& array) const {
  for (int row = 0; row < height_; row++) {
    array[row] = new int[width_];
  }
  for(int col = 0; col < width_; col++) {
    array[height_ - 1][col] = GetEnergy(height_ - 1, col);
  }
  for(int row = height_ - 2; row >= 0; row--) {
    for(int col = 0; col < width_; col++) {
      if(col == 0) {
        int down_bad = array[row + 1][col]; int down_right = array[row + 1][col + 1];
        if(down_bad <= down_right) {
          array[row][col] = GetEnergy(row, col) + down_bad;
        } else {
          array[row][col] = GetEnergy(row, col) + down_right;
        }
      } else if (col == width_ - 1) {
        int down_bad = array[row + 1][col]; int down_left = array[row + 1][col - 1];
        if(down_left <= down_bad) {
          array[row][col] = GetEnergy(row, col) + down_left;
        } else {
          array[row][col] = GetEnergy(row, col) + down_bad;
        }
      } else {
        int down_left = array[row + 1][col - 1]; int down_bad = array[row + 1][col]; int down_right = array[row + 1][col + 1];
        if(down_left <= down_bad && down_left <= down_right) {
          array[row][col] = GetEnergy(row, col) + down_left;
        } else if (down_bad <= down_right && down_bad < down_left) {
          array[row][col] = GetEnergy(row, col) + down_bad; 
        } else {
          array[row][col] = GetEnergy(row, col) + down_right;
        }
      }
    }
  }
}



int* SeamCarver::GetVerticalSeam() const {
  int* vertical_seam = new int[height_]; int** array = new int*[height_];
  PopulateVerticalSeam(array);
  int first_col_smallest = array[0][0]; int first_col_smallest_index = 0;
  for(int col = 0; col < width_; col++) {
    if(array[0][col] < first_col_smallest) {
      first_col_smallest_index = col; first_col_smallest = array[0][col];
    }
  }
  int across_down = 0; int across_up = 0; int across_middle = 0;
  vertical_seam[0] = first_col_smallest_index;
  for(int row = 1; row<height_; row++) {
    if(first_col_smallest_index == 0) {
      across_down = array[row][first_col_smallest_index+1]; across_middle = array[row][first_col_smallest_index];
      if(across_down<across_middle) {
        first_col_smallest_index++;
      } 
    } else if(first_col_smallest_index == width_-1) {
      across_up = array[row][first_col_smallest_index-1]; across_middle = array[row][first_col_smallest_index];
      if(across_up<across_middle) {
        first_col_smallest_index--;
      }
    } else {
      across_up = array[row][first_col_smallest_index-1]; across_middle = array[row][first_col_smallest_index]; across_down = array[row][first_col_smallest_index+1];
      if(across_up<=across_down && across_up<across_middle) {
        first_col_smallest_index--;
      } else if(across_down<across_up && across_down<across_middle) {
        first_col_smallest_index++;
      }
    }
    vertical_seam[row]=first_col_smallest_index;
  }
  for (int i = 0; i < height_; i++) {
    delete[] array[i];
  }
  delete[] array;
  array = nullptr;
  return vertical_seam;
}


void SeamCarver::RemoveVerticalSeam() {
  auto *final_seam = GetVerticalSeam();
  image_.VerticalCarve(final_seam);
  width_ -= 1;
  height_ -= 0;
  delete[] final_seam;
}





void SeamCarver::RemoveHorizontalSeam() {
  auto *final_seam = GetHorizontalSeam();
  image_.HorizontalCarve(final_seam);
  height_ -= 1;
  width_ -= 0;
  delete[] final_seam;
}










// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& final_image): image_(final_image) {
  height_ = final_image.GetHeight();
  width_ = final_image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& final_image) {
  image_ = final_image;
  width_ = final_image.GetWidth();
  height_ = final_image.GetHeight();
}
