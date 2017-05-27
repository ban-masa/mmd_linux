#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <iconv.h>
#include <cmath>
#include <eigen3/Eigen/Core>
#include <eigen3/Eigen/Geometry>

void conv_encode(char* input, char* output, int num, const char* encfrom, const char* encto);

class MotionData {
public:

  void read_data(std::ifstream &ifs);

  void print_bone_name(void);

  char bone_name[15];
  int frame_number;
  float p[3];
  float r[4];
  char interpolation[64];
};

class VMDData {
public:

  void read_motion_data(char* filename);

  void print_data(void);

  char motion_name[30];
  char model_name[20];
  int motion_count;
  MotionData* motion_data;
  MotionData* motion_data_sorted;
  int max_frame_number;
};

