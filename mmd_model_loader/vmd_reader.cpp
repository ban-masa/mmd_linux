#include "vmd_reader.hpp"

void conv_encode(char* input, char* output, int num, const char* encfrom, const char* encto)
{
  char *ptr_in = input;
  char *ptr_out = output;
  size_t mybufsz = (size_t)num;
  iconv_t ic;

  ic = iconv_open(encto, encfrom);
  iconv(ic, &ptr_in, &mybufsz, &ptr_out, &mybufsz);
  iconv_close(ic);
}

void MotionData::read_data(std::ifstream &ifs)
{
  char bn[15];
  ifs.read((char*)bn, sizeof(bn));
  conv_encode(bn, bone_name, 15, "Shift_JIS", "UTF8");

  ifs.read((char*)(&frame_number), sizeof(frame_number));
  ifs.read((char*)p, sizeof(p));
  p[2] = -p[2];
  ifs.read((char*)r, sizeof(r));
  ifs.read((char*)interpolation, sizeof(interpolation));

}

void MotionData::print_bone_name(void)
{
  std::cout << frame_number << ": " << bone_name << std::endl;
}

void VMDData::read_motion_data(char* filename)
{
  std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
  if (!ifs) {
    std::cerr << "ファイルオープンに失敗" << std::endl;
    std::exit(1);
  }

  char tmp_motion_name[30];
  char tmp_model_name[20];

  ifs.read((char*)tmp_motion_name, sizeof(tmp_motion_name));
  ifs.read((char*)tmp_model_name, sizeof(tmp_model_name));
  ifs.read((char*)(&motion_count), sizeof(motion_count));

  conv_encode(tmp_motion_name, motion_name, 30, "Shift_JIS", "UTF8");
  conv_encode(tmp_model_name, model_name, 20, "Shift_JIS", "UTF8");

  motion_data = new MotionData[motion_count];
  motion_data_sorted = new MotionData[motion_count];

  std::cout << "motion_name: " << motion_name << std::endl;
  std::cout << "model_name: " << model_name << std::endl;
  std::cout << "motion_count: " << motion_count << std::endl;

  max_frame_number = -1;
  for (int i = 0; i < motion_count; i++) {
    motion_data[i].read_data(ifs);
    if (motion_data[i].frame_number > max_frame_number) {
      max_frame_number = motion_data[i].frame_number;
    }
  }
  int idx = 0;
  for (int j = 0; j <= max_frame_number; j++) {
    for (int i = 0; i < motion_count; i++) {
      if (motion_data[i].frame_number == j) {
        motion_data_sorted[idx] = motion_data[i];
        idx++;
      }
    }
  }
  if (idx != motion_count) std::cerr << "VMD Import may be failed" << std::endl;
}

void VMDData::print_data(void)
{
  for (int i = 0; i < motion_count; i++) {
    MotionData& md = motion_data_sorted[i];
    std::cout << md.frame_number << ": " << md.bone_name << ", " << md.p[0] << ", " << md.p[1] << ", " << md.p[2] << "  ";
    double th = std::acos(md.r[3]) * 2.0;
    double x, y, z;
    if (std::sin(th / 2.0) == 0.0) {
      x = 0.0;
      y = 0.0;
      z = 0.0;
    } else {
      x = md.r[0] / std::sin(th / 2.0);
      y = md.r[1] / std::sin(th / 2.0);
      z = md.r[2] / std::sin(th / 2.0);
    }
    //std::cout << motion_data[i].r[3] << ", " << motion_data[i].r[0] << ", " << motion_data[i].r[1] << ", " << motion_data[i].r[2] << std::endl;
    std::cout << md.r[0] << ", " << md.r[1] << ", " << md.r[2] << ", " << md.r[3] << std::endl;
    std::cout << th * 180.0 / M_PI << ", " << x << ", " << y << ", " << z << std::endl;
  }
  std::cout << "Max frame number: " << max_frame_number << std::endl;
}
