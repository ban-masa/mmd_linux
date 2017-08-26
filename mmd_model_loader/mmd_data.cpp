#include <iostream>
#include <fstream>
#include "mmd_data.hpp"
#include "conv_utf16.hpp"
#include <GL/glut.h>
#include <cv.h>
#include <highgui.h>
#include <cmath>

void read_based_on_bytesize(std::ifstream &ifs, int &output, int type)
{
  if (type == 1) {
    int8_t temp;
    ifs.read((char*)&temp, sizeof(temp));
    output = (int)temp;
  } else if (type == 2) {
    int16_t temp;
    ifs.read((char*)&temp, sizeof(temp));
    output = (int)temp;
  } else if (type == 4) {
    int32_t temp;
    ifs.read((char*)&temp, sizeof(temp));
    output = (int)temp;
  }
}

void Vertex::read_data(std::ifstream &ifs, unsigned char (&info)[8], int num)
{
    this->id = num;
    for (int i = 0; i < 4; i++) this->bone_index[i] = -1;
    for (int i = 0; i < 3; i++) {
        ifs.read((char*)(&(this->locate[i])), sizeof(this->locate[i]));
    }
    this->locate[2] = -this->locate[2];
    for (int i = 0; i < 3; i++) {
        ifs.read((char*)(&(this->normal[i])), sizeof(this->normal[i]));
    }
    this->normal[2] = -this->normal[2];
    for (int i = 0; i < 2; i++) {
        ifs.read((char*)(&(this->uv[i])), sizeof(this->uv[i]));
    }
    this->ex_uv = new float* [(int)info[1]];
    this->ex_uv_num = (int)info[1];
    for (int i = 0; i < (int)info[1]; i++) {
        this->ex_uv[i] = new float[4];
        for (int j = 0; j < 4; j++) {
            ifs.read((char*)(&(this->ex_uv[i][j])), sizeof(this->ex_uv[i][j]));
        }
    }
    ifs.read((char*)(&(this->weight_type)), sizeof(this->weight_type));

    if (this->weight_type == 0) {
        if (info[5] == 1) {
            int8_t temp;
            ifs.read((char*)&temp, sizeof(int8_t));
            this->bone_index[0] = (int)temp;
        } else if (info[5] == 2) {
            int16_t temp;
            ifs.read((char*)&temp, sizeof(int16_t));
            this->bone_index[0] = (int)temp;
        } else if (info[5] == 4) {
            int32_t temp;
            ifs.read((char*)&temp, sizeof(int32_t));
            this->bone_index[0] = (int)temp;
        }
        this->bone_weight[0] = 1.0;
        for (int i = 1; i < 4; i++) {
            this->bone_index[i] = -1;
            this->bone_weight[i] = 0.0;
        }
    } else if (this->weight_type == 1) {
        if (info[5] == 1) {
            int8_t temp;
            ifs.read((char*)&temp, sizeof(int8_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int8_t));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 2) {
            int16_t temp;
            ifs.read((char*)&temp, sizeof(int16_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int16_t));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 4) {
            int32_t temp;
            ifs.read((char*)&temp, sizeof(int32_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int32_t));
            this->bone_index[1] = (int)temp;
        }
        float w;
        ifs.read((char*)&w, sizeof(float));
        this->bone_weight[0] = w;
        this->bone_weight[1] = 1.0 - w;
        for (int i = 2; i < 4; i++) {
            this->bone_index[i] = -1;
            this->bone_weight[i] = 0.0;
        }
    } else if (this->weight_type == 2) {
        if (info[5] == 1) {
            int8_t temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(int8_t));
                this->bone_index[i] = (int)temp;
            }
            float w;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&w, sizeof(float));
                this->bone_weight[i] = w;
            }
        } else if (info[5] == 2) {
            int16_t temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(int16_t));
                this->bone_index[i] = (int)temp;
            }
            float w;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&w, sizeof(float));
                this->bone_weight[i] = w;
            }
        } else if (info[5] == 4) {
            int32_t temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(int32_t));
                this->bone_index[i] = (int)temp;
            }
            float w;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&w, sizeof(float));
                this->bone_weight[i] = w;
            }
        }
    } else if (this->weight_type == 3) {
        if (info[5] == 1) {
            int8_t temp;
            ifs.read((char*)&temp, sizeof(int8_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int8_t));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 2) {
            int16_t temp;
            ifs.read((char*)&temp, sizeof(int16_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int16_t));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 4) {
            int32_t temp;
            ifs.read((char*)&temp, sizeof(int32_t));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int32_t));
            this->bone_index[1] = (int)temp;
        }
        float w;
        ifs.read((char*)&w, sizeof(float));
        this->bone_weight[0] = w;
        this->bone_weight[1] = 1.0 - w;
        for (int i = 2; i < 4; i++) {
            this->bone_index[i] = -1;
            this->bone_weight[i] = 0.0;
        }
        ifs.read((char*)(this->sdef_c), sizeof(float) * 3);
        ifs.read((char*)(this->sdef_r0), sizeof(float) * 3);
        ifs.read((char*)(this->sdef_r1), sizeof(float) * 3);
    }
    ifs.read((char*)(&(this->edge_mag)), sizeof(float));
}

void Face::read_data(std::ifstream &ifs, unsigned char (&info)[8])
{
    if (info[2] == 1) {
        for (int i = 0; i < 3; i++) {
            unsigned char temp;
            ifs.read((char*)&temp, sizeof(temp));
            this->vertex_index[i] = (unsigned int)temp;
        }
    } else if (info[2] == 2) {
        for (int i = 0; i < 3; i++) {
            unsigned short temp;
            ifs.read((char*)&temp, sizeof(temp));
            this->vertex_index[i] = (unsigned int)temp;
        }
    } else if (info[2] == 4) {
        for (int i = 0; i < 3; i++) {
            unsigned int temp;
            ifs.read((char*)&temp, sizeof(temp));
            this->vertex_index[i] = (unsigned int)temp;
        }
    }
}

void Texture::read_data(std::ifstream &ifs, unsigned char (&info)[8])
{
    ifs.read((char*)(&(this->num)), sizeof(int));
    if (info[0] == 0) {
        this->path = new char[1024];
        char* temp = new char[this->num];
        if (this->num > 1024) {
            std::cout << "Buffer Error" << std::endl;
            return;
        }
        ifs.read((char*)temp, sizeof(char) * this->num);
        conv_utf16(temp, this->path, this->num);
    } else if (info[0] == 1) {
        this->path = new char[this->num];
        ifs.read((char*)(this->path), sizeof(char) * this->num);
    }
}

void Texture::read_image(void)
{
    std::string fname = this->path;
    std::string::size_type index(fname.find("\\"));
    while (index != std::string::npos) {
      fname[index] = '/';
      index = fname.find("\\", index);
    }

    index = fname.rfind("tga");
    if (index != std::string::npos) {
      fname = fname.replace(index, 3, "png");
    }
    strcpy(this->path, fname.c_str());

    if ((this->image = cvLoadImage(this->path, CV_LOAD_IMAGE_COLOR)) == 0) {
        std::cerr << this->path << std::endl;
        std::cerr << "Load Error" << std::endl;
    //    exit(1);
    } else {
      cvCvtColor(this->image, this->image, CV_BGR2RGB);
    }
}

void Material::read_data(std::ifstream &ifs, unsigned char (&info)[8])
{
    for (int i = 0; i < 2; i++) {
        int temp;
        ifs.read((char*)&temp, sizeof(int));
        this->material_name[i] = new char[temp];
        ifs.read((char*)(this->material_name[i]), sizeof(char) * temp);
    }

    ifs.read((char*)(this->diffuse), sizeof(float) * 4);
    ifs.read((char*)(this->specular), sizeof(float) * 3);
    this->specular[3] = 1.0;
    ifs.read((char*)(&(this->co_specular)), sizeof(float));
    ifs.read((char*)(this->ambient), sizeof(float) * 3);
    this->ambient[3] = 1.0;
    ifs.read((char*)(&(this->bit_flag)), sizeof(unsigned char));
    ifs.read((char*)(this->edge_color), sizeof(float) * 4);
    ifs.read((char*)(&(this->edge_size)), sizeof(float));
//info[3]
    ifs.read((char*)(&(this->normal_texture_index)), sizeof(char) * info[3]);
    ifs.read((char*)(&(this->sphere_texture_index)), sizeof(char) * info[3]);
    ifs.read((char*)(&(this->sphere_mode)), sizeof(unsigned char));
    ifs.read((char*)(&(this->common_toon_flag)), sizeof(unsigned char));
    if (this->common_toon_flag == 0) {
        ifs.read((char*)(&(this->toon_texture_index)), sizeof(unsigned char) * info[3]);
    } else if (this->common_toon_flag == 1) {
        ifs.read((char*)(&(this->common_toon_texture)), sizeof(this->common_toon_flag));
    }
    int temp;
    ifs.read((char*)&temp, sizeof(temp));
    this->memo = new char[temp];
    ifs.read((char*)(this->memo), sizeof(char) * temp);
    ifs.read((char*)(&(this->vertex_count)), sizeof(int));
}

void Bone::read_data(std::ifstream &ifs, unsigned char (&info)[8])
{
    int temp;

    if (info[0] == 0) {
        this->bone_name[0] = new char[1024];
        this->bone_name[1] = new char[1024];
        ifs.read((char*)&temp, sizeof(int));
        char* buff1 = new char[temp];
        ifs.read((char*)buff1, sizeof(char) * temp);
        conv_utf16(buff1, this->bone_name[0], temp);
        ifs.read((char*)&temp, sizeof(int));
        char* buff2 = new char[temp];
        ifs.read((char*)buff2, sizeof(char) * temp);
        conv_utf16(buff2, this->bone_name[1], temp);
    } else if (info[0] == 1) {
        ifs.read((char*)&temp, sizeof(int));
        this->bone_name[0] = new char[temp];
        ifs.read((char*)(&(this->bone_name[0])), sizeof(char) * temp);
        ifs.read((char*)&temp, sizeof(int));
        this->bone_name[1] = new char[temp];
        ifs.read((char*)(&(this->bone_name[1])), sizeof(char) * temp);
    }
    for (int i = 0; i < 2; i++) this->bone_name_string[i] = this->bone_name[i];
    ifs.read((char*)(&(this->pos)), sizeof(float) * 3);
    this->pos[2] = -this->pos[2];

    read_based_on_bytesize(ifs, this->parent_index, info[5]);
    //ifs.read((char*)(&(this->parent_index)), sizeof(unsigned char) * info[5]);
    ifs.read((char*)(&(this->transform_hierarchy)), sizeof(int));
    ifs.read((char*)(&(this->bit_flag)), sizeof(unsigned short));
    if (this->bit_flag & 0x0001) {
        read_based_on_bytesize(ifs, this->connect_bone_index, info[5]);
        //ifs.read((char*)(&(this->connect_bone_index)), sizeof(unsigned char) * info[5]);
    } else {
        ifs.read((char*)(this->pos_offset), sizeof(float) * 3);
        this->pos_offset[2] = -this->pos_offset[2];
    }
    if (this->bit_flag & (0x0100 | 0x0200)) {
        read_based_on_bytesize(ifs, this->grant_bone_index, info[5]);
        //ifs.read((char*)(&(this->grant_bone_index)), sizeof(unsigned char) * info[5]);
        ifs.read((char*)(&(this->grant_rate)), sizeof(float));
    }
    if (this->bit_flag & 0x0400) {
        ifs.read((char*)(this->axis_vect), sizeof(float) * 3);
        this->axis_vect[2] = -this->axis_vect[2];
    }
    if (this->bit_flag & 0x0800) {
        ifs.read((char*)(this->x_axis_vect), sizeof(float) * 3);
        ifs.read((char*)(this->z_axis_vect), sizeof(float) * 3);
        this->x_axis_vect[2] = -this->x_axis_vect[2];
        this->z_axis_vect[2] = -this->z_axis_vect[2];

        for (int i = 0; i < 3; i++) this->z_axis_vect[i] = -this->z_axis_vect[i];
    }
    if (this->bit_flag & 0x2000) {
        ifs.read((char*)(&(this->key_val)), sizeof(int));
    }
    if (this->bit_flag & 0x0020) {
        read_based_on_bytesize(ifs, this->IK_target_bone_index, info[5]);
        //ifs.read((char*)(&(this->IK_target_bone_index)), sizeof(unsigned char) * info[5]);
        ifs.read((char*)(&(this->IK_loop_num)), sizeof(int));
        ifs.read((char*)(&(this->IK_limit_angle)), sizeof(float));
        ifs.read((char*)(&(this->IK_link_num)), sizeof(int));
        this->IK_link_data = new IK_link[this->IK_link_num];
        for (int i = 0; i < this->IK_link_num; i++) {
            read_based_on_bytesize(ifs, this->IK_link_data[i].link_bone_index, info[5]);
            //ifs.read((char*)(&(this->IK_link_data[i].link_bone_index)), sizeof(unsigned char) * info[5]);
            ifs.read((char*)(&(this->IK_link_data[i].angle_limit_flag)), sizeof(unsigned char));
            if (this->IK_link_data[i].angle_limit_flag == 1) {
                ifs.read((char*)(this->IK_link_data[i].upper_limit), sizeof(float) * 3);
                ifs.read((char*)(this->IK_link_data[i].lower_limit), sizeof(float) * 3);
            }
        }
    }
    q = Eigen::Quaternionf::Identity();
    R = Eigen::Matrix3f::Identity();
    pdiff = Eigen::Vector3f::Zero();
    rel_pos = Eigen::Vector3f::Zero();
}

void Bone::print_data(void)
{
    std::cout << "Bone Id: " << this->id << std::endl;
    std::cout << "Bone Name: " << this->bone_name[0] << std::endl;
    std::cout << "Position: " << this->pos[0] << ", " << this->pos[1] << ", " << this->pos[2] << std::endl;
    std::cout << "Parent Index: " << this->parent_index << std::endl;;
    std::cout << "Hierarchy: " << this->transform_hierarchy << std::endl;
    if (this->bit_flag & 0x0001) {
      std::cout << "Connect Bone Index: " << this->connect_bone_index << std::endl;
    } else {
      std::cout << "Pos Offset: " << this->pos_offset[0] << ", " << this->pos_offset[1] << ", " << this->pos_offset[2] << std::endl;
    }
    if (this->bit_flag & (0x0100 | 0x0200)) {
      std::cout << "Grant Bone Index: " << this->grant_bone_index << std::endl;
      std::cout << "Grant Rate: " << this->grant_rate << std::endl;
    }
    if (this->bit_flag & 0x0400) {
      std::cout << "Axis Vect: " << this->axis_vect[0] << ", " << this->axis_vect[1] << ", " << this->axis_vect[2] << std::endl;
    }
    if (this->bit_flag & 0x0800) {
      std::cout << "X Axis Vect: " << this->x_axis_vect[0] << ", " << this->x_axis_vect[1] << ", " << this->x_axis_vect[2] << std::endl;
      std::cout << "Z Axis Vect: " << this->z_axis_vect[0] << ", " << this->z_axis_vect[1] << ", " << this->z_axis_vect[2] << std::endl;
    }
    if (this->bit_flag & 0x2000) {
      std::cout << "Key Value: " << this->key_val << std::endl;
    }
    if (this->bit_flag & 0x0020) {
      std::cout << "IK Target Bone Index: " << this->IK_target_bone_index << std::endl;
      std::cout << "IK Loop Num: " << this->IK_loop_num << std::endl;
      std::cout << "IK Limit Angle: " << this->IK_limit_angle << std::endl;
      std::cout << "IK Link Num: " << this->IK_link_num << std::endl;
      for (int i = 0; i < this->IK_link_num; i++) {
        std::cout << "    Link Bone Index: " << this->IK_link_data[i].link_bone_index << std::endl;
        if (this->IK_link_data[i].angle_limit_flag == 1) {
          std::cout << "    Upper Limit: " << this->IK_link_data[i].upper_limit[0] << ", " << this->IK_link_data[i].upper_limit[1] << ", " << this->IK_link_data[i].upper_limit[2] << std::endl;
          std::cout << "    Lower Limit: " << this->IK_link_data[i].lower_limit[0] << ", " << this->IK_link_data[i].lower_limit[1] << ", " << this->IK_link_data[i].lower_limit[2] << std::endl;
        }
      }
    }
    std::cout << std::endl;
}

bool Bone::ikbone(void) {
  if (bit_flag & 0x0020) return true;
  return false;
}

bool Bone::localaxis(void)
{
  if (this->bit_flag & 0x0800) return true;
  return false;
}

bool Bone::fixedaxis(void)
{
  if (this->bit_flag & 0x0400) return true;
  return false;
}

void Bone::set_parent_bone(Bone* bone_list)
{
  if (parent_index < 0) {
    rel_pos = Eigen::Vector3f::Zero();
    return;
  }
  parent_bone = &(bone_list[parent_index]);
  rel_pos = Eigen::Vector3f(pos[0] - parent_bone->pos[0], pos[1] - parent_bone->pos[1], pos[2] - parent_bone->pos[2]);
  //std::cout << rel_pos(0) << " " << rel_pos(1) << " " << rel_pos(2) << std::endl;
}

void Bone::calc_rotation_matrix(void)
{
  R = q.matrix();
}

void Bone::calc_ht_matrix(void)
{
  calc_rotation_matrix();
  Eigen::Matrix4f Hthis = Eigen::Matrix4f::Identity();
  Hthis.block(0, 0, 3, 3) = R;
  for (int i = 0; i < 3; i++) {
    Hthis(i, 3) = rel_pos(i) + pdiff(i);
  }
  if (parent_index < 0) {
    H = Hthis;
  } else {
    H = parent_bone->H * Hthis;
  }
}

void Bone::calc_pos(void)
{
  if (parent_index < 0) {
    Eigen::Vector3f pos_v = rel_pos + pdiff;
    pos[0] = pos_v.x();
    pos[1] = pos_v.y();
    pos[2] = pos_v.z();
  } else {
    Eigen::Vector4f x;
    for (int i = 0; i < 3; i++) x(i) = rel_pos(i) + pdiff(i);
    x(3) = 1.0;
    Eigen::Vector4f pos_v = parent_bone->H * x;
    for (int i = 0; i < 3; i++) pos[i] = pos_v(i);
  }
}

void Bone::set_pos_qua(Eigen::Vector3f gp, Eigen::Quaternionf gq)
{
  this->pdiff = gp;
  this->q = gq;
}

void Bone::set_qua(Eigen::Quaternionf gq)
{
  this->q = gq;
}

bool Bone::grantbone(void) {
  if (bit_flag & 0x0100) return true;
  if (bit_flag & 0x0200) return true;
  return false;
}

void Bone::set_ik_bone_list(Bone* bonelist)
{
  if (!ikbone()) return;
  ik_bone_list = new Bone*[this->IK_link_num];
  for (int i = 0; i < IK_link_num; i++) {
    ik_bone_list[i] = &(bonelist[IK_link_data[i].link_bone_index]);
  }
  target_bone = &(bonelist[IK_target_bone_index]);
}

/*void Bone::inverse_kinematics(void)
{
  for (int i = 0; i < this->IK_loop_num; i++) {
    for (int j = 0; j < this->IK_link_num; i++) {
      Vector3f ik_goal_pos;
      ik_goal_pos << this->pos[0], this->pos[1], this->pos[2];
      Vector3f target_pos;
      target_pos << target_bone->pos[0], target_bone->pos[1], target_bone->pos[2];
      Bone& bn = *(ik_bone_list[j]);
      Vector3f link_pos;
      link_pos << bn.pos[0], bn.pos[1], bn.pos[2];
      Vector3f v_target = target_pos - link_pos;
      Vector3f v_ik = ik_goal_pos - link_pos;
      if (this->localaxis()) {

      }
    }
  }
}*/

//MMD_model methods

void MMD_model::read_model(char* filename)
{
    std::ifstream ifs(filename, std::ios_base::in | std::ios_base::binary);
    if (!ifs) {
        std::cerr << "ファイルオープンに失敗" << std::endl;
        std::exit(1);
    }

    ifs.read((char*)(this->magic1), sizeof(unsigned char) * 4);
    ifs.read((char*)(&(this->version)), sizeof(float));
    std::cout << "Version: " << this->version << std::endl;
    ifs.read((char*)(&(this->info_byte)), sizeof(unsigned char));
    ifs.read((char*)(this->info), sizeof(unsigned char) * 8);
    
    for (int i = 0; i < 4; i++) {
        int temp;
        ifs.read((char*)&temp, sizeof(int));
        this->text_buf[i] = new char[temp];
        ifs.read((char*)(this->text_buf[i]), sizeof(char) * temp);
    }

    ifs.read((char*)(&(this->vertex_num)), sizeof(int));
    this->vertex_data = new Vertex[this->vertex_num];
    this->vertex_index = new float[this->vertex_num * 3];
    this->norm_index = new float[this->vertex_num * 3];
    this->uv_index = new float[this->vertex_num * 2];
    for (int i = 0; i < this->vertex_num; i++) {
        this->vertex_data[i].read_data(ifs, this->info, i);
        for (int j = 0; j < 3; j++) {
            this->vertex_index[3 * i + j] = this->vertex_data[i].locate[j];
            this->norm_index[3 * i + j] = this->vertex_data[i].normal[j];
        }
        this->uv_index[2 * i] = this->vertex_data[i].uv[0];
        this->uv_index[2 * i + 1] = this->vertex_data[i].uv[1];
    }
    ifs.read((char*)(&(this->face_num)), sizeof(int));
    this->face_vertex_index = new unsigned int[this->face_num];
    this->face_num /= 3;
    this->face_data = new Face[this->face_num];
    for (int i = 0; i < this->face_num; i++) {
        this->face_data[i].read_data(ifs, this->info);
        for (int j = 0; j < 3; j++) {
            this->face_vertex_index[3 * i + j] = this->face_data[i].vertex_index[j];
        }
    }
    ifs.read((char*)(&(this->texture_num)), sizeof(int));
    this->texture_data = new Texture[this->texture_num];
    for (int i = 0; i < this->texture_num; i++) {
        this->texture_data[i].read_data(ifs, this->info);
    }
    ifs.read((char*)(&(this->material_num)), sizeof(int));
    this->material_data = new Material[this->material_num];
    for (int i = 0; i < this->material_num; i++) {
        this->material_data[i].read_data(ifs, this->info);
    }
    ifs.read((char*)(&(this->bone_num)), sizeof(int));
    this->bone_data = new Bone[this->bone_num];
    for (int i = 0; i < this->bone_num; i++) {
        this->bone_data[i].id = i;
        this->bone_data[i].read_data(ifs, this->info);
        this->bone_data[i].print_data();
    }
    for (int i = 0; i < this->bone_num; i++) {
      this->bone_data[i].set_parent_bone(this->bone_data);
      this->bone_data[i].set_ik_bone_list(this->bone_data);
    }
    sort_bone_parent_child_relation();
}

void MMD_model::texture_config(void)
{
    for (int i = 0; i < this->texture_num; i++) {
        this->texture_data[i].read_image();
    }
    glEnable(GL_TEXTURE_2D);
    for (int i = 0; i < this->texture_num; i++) {
        if (this->texture_data[i].image->imageSize != 0) {
            glGenTextures(1, &(this->texture_data[i].id));
            glBindTexture(GL_TEXTURE_2D, this->texture_data[i].id);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, this->texture_data[i].image->width, this->texture_data[i].image->height, 0, GL_RGB, GL_UNSIGNED_BYTE, this->texture_data[i].image->imageData);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
    }
    glDisable(GL_TEXTURE_2D);
}

void MMD_model::read_motion_data(char* filename)
{
  vmd_data = new VMDData;
  vmd_data->read_motion_data(filename);
}

void MMD_model::sort_bone_parent_child_relation(void)
{
  sorted_bone_index = new int[bone_num];
  bool* flag_list = new bool[bone_num];
  for (int i = 0; i < bone_num; i++) flag_list[i] = false;

  int idx = 0;
  for (int i = 0; i < bone_num; i++) {
    if (bone_data[i].parent_index < 0) {
      sorted_bone_index[idx] = i;
      idx++;
      flag_list[i] = true;
    }
  }
  while (idx < bone_num) {
    for (int i = 0; i < bone_num; i++) {
      if (flag_list[i]) continue;

      for (int j = 0; j < idx; j++) {
        if (sorted_bone_index[j] == bone_data[i].parent_index) {
          sorted_bone_index[idx] = i;
          idx++;
          flag_list[i] = true;
          break;
        }
      }
    }
  }
}

void MMD_model::update_bone(void)
{
  for (int i = 0; i < bone_num; i++) {
    Bone& bn = bone_data[sorted_bone_index[i]];
    bn.calc_pos();
    bn.calc_ht_matrix();
  }
}

void MMD_model::display(void)
{
    glPushMatrix();
    {
        glTranslated(0.0, 0.0, (double)(this->distance));
        glRotated((double)(this->rotate_angle_y), 0.0, 1.0, 0.0);
        glRotated((double)(this->rotate_angle_x), 1.0, 0.0, 0.0);
  
        glEnable(GL_TEXTURE_2D);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(3, GL_FLOAT, 0, this->vertex_index);
        glNormalPointer(GL_FLOAT, 0, this->norm_index);
        glTexCoordPointer(2, GL_FLOAT, 0, this->uv_index);

        int start_face = 0;
        for (int i = 0; i < this->material_num; i++) {
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
            glBindTexture(GL_TEXTURE_2D, this->texture_data[this->material_data[i].normal_texture_index].id);
            glMaterialfv(GL_FRONT, GL_AMBIENT, this->material_data[i].ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, this->material_data[i].diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, this->material_data[i].specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &(this->material_data[i].co_specular));
            glDrawElements(GL_TRIANGLES, this->material_data[i].vertex_count, GL_UNSIGNED_INT, &(this->face_vertex_index[start_face]));
            start_face += this->material_data[i].vertex_count;
        }    
        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisable(GL_TEXTURE_2D);

    }
    glPopMatrix();
}

void MMD_model::bone_display(void)
{
  glPushMatrix();
  {
    glTranslated(0.0, 0.0, (double)(this->distance));
    glRotated((double)(this->rotate_angle_y), 0.0, 1.0, 0.0);
    glRotated((double)(this->rotate_angle_x), 1.0, 0.0, 0.0);
    glColor3d(0.2, 0.2, 0.2);
    for (int i = 0; i < this->bone_num; i++) {
      Bone& bn = this->bone_data[i];
      glPushMatrix();

      if (bn.parent_index > 0 && !bn.grantbone()) {
        Bone& pbn = this->bone_data[bn.parent_index];
        glBegin(GL_LINES);
        glVertex3d(bn.pos[0], bn.pos[1], bn.pos[2]);
        glVertex3d(pbn.pos[0], pbn.pos[1], pbn.pos[2]);
        glEnd();
      }
      glEnable(GL_COLOR_MATERIAL);
      
      glTranslated(bn.pos[0], bn.pos[1], bn.pos[2]);
      if (bn.ikbone()) {
        glColor3d(1.0, 0.0, 0.0);
        glutSolidCube(0.5);
      } else if (bn.grantbone()) {
      } else {
        glColor3d(0.0, 1.0, 1.0);
        glutSolidCube(0.2);
      }
      glDisable(GL_COLOR_MATERIAL);
      glPopMatrix();
    }
  }
  glPopMatrix();
}

void MMD_model::bone_with_vertex_display(int id)
{
  glPushMatrix();
  {
    glTranslated(0.0, 0.0, (double)(this->distance));
    glRotated((double)(this->rotate_angle_y), 0.0, 1.0, 0.0);
    glRotated((double)(this->rotate_angle_x), 1.0, 0.0, 0.0);
    glColor3d(0.2, 0.2, 0.2);
    glPointSize(0.5);
    glBegin(GL_POINTS);
    for (int i = 0; i < this->vertex_num; i++) {
      Vertex& vt = this->vertex_data[i];
      if ((vt.bone_index[0] != id) && (vt.bone_index[1] != id) && (vt.bone_index[2] != id) && (vt.bone_index[3] != id)) continue;
      glVertex3d(vt.locate[0], vt.locate[1], vt.locate[2]);
    }
    glEnd();
    Bone& bn = this->bone_data[id];
    glTranslated(bn.pos[0], bn.pos[1], bn.pos[2]);
    glutSolidCube(0.2);
  }
  glPopMatrix();
}

void MMD_model::set_angle(void)
{
  std::string bonename;
  std::cout << "Enter Axis Name: ";
  std::cin >> bonename;
  bool flag = false;
  Bone* bn;
  for (int i = 0; i < bone_num; i++) {
    if (bone_data[i].bone_name_string[0] == bonename) {
      bn = &(bone_data[i]);
      flag = true;
      break;
    }
  }
  if (!flag) {
    std::cerr << "Specified bone is unfound" << std::endl;
    return;
  }
  double angle;
  std::string axis;
  std::cout << "Enter Axis (x, y, z): ";
  std::cin >> axis;
  if (axis != "x" && axis != "y" && axis != "z") {
    std::cerr << "Unknown axis" << std::endl;
    return;
  }
  std::cout << "Enter Angle: ";
  std::cin >> angle;
  while (angle > 360 || angle < 0) {
    if (angle > 0) angle -= 360;
    else angle += 360;
  }
  if (angle > 180) angle = angle - 360;
  Eigen::Vector3f set_axis = Eigen::Vector3f::Zero();
  if (axis == "x") {
    set_axis(0) = 1.0;
  } else if (axis == "y") {
    set_axis(1) = 1.0;
  } else if (axis == "z") {
    set_axis(2) = 1.0;
  }
  Eigen::Quaternionf set_q;
  set_q = Eigen::AngleAxisf(angle * M_PI / 180.0, set_axis);
  Eigen::Vector3f gp = Eigen::Vector3f::Zero();
  bn->set_pos_qua(gp, set_q);
}

bool MMD_model::search_bone(char* bonename, Bone** bonep)
{
  std::string bn1(bonename);
  for (int i = 0; i < bone_num; i++) {
    if (bone_data[i].bone_name_string[0] == bn1) {
      *bonep = &(bone_data[i]);
      return true;
    }
  }
  return false;
}

void MMD_model::play_motion_data(int frame_number)
{
  for (int i = 0; i < vmd_data->motion_count; i++) {
    if (vmd_data->motion_data_sorted[i].frame_number == frame_number) {
      set_motion_data_to_bone(vmd_data->motion_data_sorted[i]);
    }
  }
  update_bone();
}

void MMD_model::set_motion_data_to_bone(MotionData& md)
{
  Bone* bone;
  if (!search_bone(md.bone_name, &bone)) return;
  if (bone->grantbone()) return;
  Eigen::Vector3f gp(md.p[0], md.p[1], md.p[2]);
  Eigen::Quaternionf gq(md.r[3], md.r[0], md.r[1], md.r[2]);
  bone->set_pos_qua(gp, gq);
}

void MMD_model::play_motion_sequence(void)
{
  std::cout << "start" << std::endl;
  for (int i = 0; i <= vmd_data->max_frame_number; i++) {
    play_motion_data(i);
    glutPostRedisplay();
  }
  std::cout << "finish" << std::endl;
}

void MMD_model::play_motion_frame(void)
{
  int frame;
  std::cout << "Frame: ";
  std::cin >> frame;
  play_motion_data(frame);
  glutPostRedisplay();
}
