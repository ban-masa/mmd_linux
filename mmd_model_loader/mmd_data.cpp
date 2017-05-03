#include <iostream>
#include <fstream>
#include "mmd_data.hpp"
#include "conv_utf16.hpp"
#include <GL/glut.h>
#include <cv.h>
#include <highgui.h>
#include <string.h>

void Vertex::read_data(std::ifstream &ifs, unsigned char (&info)[8], int num)
{
    this->id = num;
    for (int i = 0; i < 3; i++) {
        ifs.read((char*)(&(this->locate[i])), sizeof(this->locate[i]));
    }
    for (int i = 0; i < 3; i++) {
        ifs.read((char*)(&(this->normal[i])), sizeof(this->normal[i]));
    }
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
    std::string::size_type index(fname.rfind("tga"));
    if (index != std::string::npos) {
      strcpy(this->path, fname.replace(index, 3, "png").c_str());
    }

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
        std::cout << this->bone_name[0] << std::endl;
    } else if (info[0] == 1) {
        ifs.read((char*)&temp, sizeof(int));
        this->bone_name[0] = new char[temp];
        ifs.read((char*)(&(this->bone_name[0])), sizeof(char) * temp);
        ifs.read((char*)&temp, sizeof(int));
        this->bone_name[1] = new char[temp];
        ifs.read((char*)(&(this->bone_name[1])), sizeof(char) * temp);
    }
    ifs.read((char*)(&(this->pos)), sizeof(float) * 3);
    ifs.read((char*)(&(this->parent_index)), sizeof(unsigned char) * info[5]);
    ifs.read((char*)(&(this->transform_hierarchy)), sizeof(int));
    ifs.read((char*)(&(this->bit_flag)), sizeof(unsigned short));
    if (this->bit_flag & 0x0001) {
        ifs.read((char*)(&(this->connect_bone_index)), sizeof(unsigned char) * info[5]);
    } else {
        ifs.read((char*)(this->pos_offset), sizeof(float) * 3);
    }
    if (this->bit_flag & (0x0100 | 0x0200)) {
        ifs.read((char*)(&(this->grant_bone_index)), sizeof(unsigned char) * info[5]);
        ifs.read((char*)(&(this->grant_rate)), sizeof(float));
    }
    if (this->bit_flag & 0x0400) {
        ifs.read((char*)(this->axis_vect), sizeof(float) * 3);
    }
    if (this->bit_flag & 0x0800) {
        ifs.read((char*)(this->x_axis_vect), sizeof(float) * 3);
        ifs.read((char*)(this->z_axis_vect), sizeof(float) * 3);
    }
    if (this->bit_flag & 0x2000) {
        ifs.read((char*)(&(this->key_val)), sizeof(int));
    }
    if (this->bit_flag & 0x0020) {
        ifs.read((char*)(&(this->IK_target_bone_index)), sizeof(unsigned char) * info[5]);
        ifs.read((char*)(&(this->IK_loop_num)), sizeof(int));
        ifs.read((char*)(&(this->IK_limit_angle)), sizeof(float));
        ifs.read((char*)(&(this->IK_link_num)), sizeof(int));
        this->IK_link_data = new IK_link[this->IK_link_num];
        for (int i = 0; i < this->IK_link_num; i++) {
            ifs.read((char*)(&(this->IK_link_data[i].link_bone_index)), sizeof(unsigned char) * info[5]);
            ifs.read((char*)(&(this->IK_link_data[i].angle_limit_flag)), sizeof(unsigned char));
            if (this->IK_link_data[i].angle_limit_flag == 1) {
                ifs.read((char*)(this->IK_link_data[i].upper_limit), sizeof(float) * 3);
                ifs.read((char*)(this->IK_link_data[i].lower_limit), sizeof(float) * 3);
            }
        }
    }
}

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
        this->bone_data[i].read_data(ifs, this->info);
    }
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

void MMD_model::display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    {
        glTranslated(0.0, 0.0, (double)(this->distance));
        glRotated((double)(this->rotate_angle_y), 0.0, 1.0, 0.0);
        glRotated((double)(this->rotate_angle_x), 1.0, 0.0, 0.0);
  
/*        int start_face = 0;
        for (int i = 0; i < this->material_num; i++) {
            glMaterialfv(GL_FRONT, GL_AMBIENT, this->material_data[i].ambient);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, this->material_data[i].diffuse);
            glMaterialfv(GL_FRONT, GL_SPECULAR, this->material_data[i].specular);
            glMaterialfv(GL_FRONT, GL_SHININESS, &(this->material_data[i].co_specular));

            for (int j = 0; j < this->material_data[i].vertex_count/3; j++) {
                glBegin(GL_TRIANGLES);
                for (int k = 0; k < 3; k++) {
                    glNormal3fv((this->vertex_data[this->face_vertex_index[start_face + j * 3 + k]]).normal);
                    glVertex3fv((this->vertex_data[this->face_vertex_index[start_face + j * 3 + k]]).locate);
                }
                glEnd();
            }
            start_face += this->material_data[i].vertex_count;
        }*/
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

/*        for (int i = 0; i < this->face_num; i++) {
            glColor3d(0.2, 0.2, 0.2);
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < 3; j++) {
                glNormal3fv((this->vertex_data[this->face_data[i].vertex_index[j]]).normal);
                glVertex3fv((this->vertex_data[this->face_data[i].vertex_index[j]]).locate);
            }
            glEnd();
        }*/
    }
    glPopMatrix();
    glutSwapBuffers();
    glFlush();
}
