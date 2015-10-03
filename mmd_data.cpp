#include <iostream>
#include <fstream>
#include "mmd_data.hpp"
#include <GL/glut.h>

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
            char temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[0] = (int)temp;
        } else if (info[5] == 2) {
            short temp;
            ifs.read((char*)&temp, sizeof(short));
            this->bone_index[0] = (int)temp;
        } else if (info[5] == 4) {
            int temp;
            ifs.read((char*)&temp, sizeof(int));
            this->bone_index[0] = (int)temp;
        }
        this->bone_weight[0] = 1.0;
        for (int i = 1; i < 4; i++) {
            this->bone_index[i] = -1;
            this->bone_weight[i] = 0.0;
        }
    } else if (this->weight_type == 1) {
        if (info[5] == 1) {
            char temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 2) {
            short temp;
            ifs.read((char*)&temp, sizeof(short));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(short));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 4) {
            int temp;
            ifs.read((char*)&temp, sizeof(int));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(int));
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
            char temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(char));
                this->bone_index[i] = (int)temp;
            }
            float w;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&w, sizeof(float));
                this->bone_weight[i] = w;
            }
        } else if (info[5] == 2) {
            short temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(short));
                this->bone_index[i] = (int)temp;
            }
            float w;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&w, sizeof(float));
                this->bone_weight[i] = w;
            }
        } else if (info[5] == 4) {
            int temp;
            for (int i = 0; i < 4; i++) {
                ifs.read((char*)&temp, sizeof(int));
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
            char temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 2) {
            short temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[1] = (int)temp;
        } else if (info[5] == 4) {
            int temp;
            ifs.read((char*)&temp, sizeof(char));
            this->bone_index[0] = (int)temp;
            ifs.read((char*)&temp, sizeof(char));
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
            this->vertex_index[i] = (int64_t)temp;
        }
    } else if (info[2] == 2) {
        for (int i = 0; i < 3; i++) {
            unsigned short temp;
            ifs.read((char*)&temp, sizeof(temp));
            this->vertex_index[i] = (int64_t)temp;
        }
    } else if (info[2] == 4) {
        for (int i = 0; i < 3; i++) {
            unsigned int temp;
            ifs.read((char*)&temp, sizeof(temp));
            this->vertex_index[i] = (int64_t)temp;
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
    for (int i = 0; i < this->vertex_num; i++) {
        this->vertex_data[i].read_data(ifs, this->info, i);
    }
    ifs.read((char*)(&(this->face_num)), sizeof(int));
    this->face_num /= 3;
    this->face_data = new Face[this->face_num];
    for (int i = 0; i < this->face_num; i++) {
        this->face_data[i].read_data(ifs, this->info);
    }
}

void MMD_model::display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    {
        glTranslated(0.0, 0.0, (double)(this->distance));
        glRotated((double)(this->rotate_angle_y), 0.0, 1.0, 0.0);
        glRotated((double)(this->rotate_angle_x), 1.0, 0.0, 0.0);
        for (int i = 0; i < this->face_num; i++) {
            glColor3d(0.2, 0.2, 0.2);
            glBegin(GL_TRIANGLES);
            for (int j = 0; j < 3; j++) {
                if (this->face_data[i].vertex_index[j] == -1) {
                    std::cout << "???" << std::endl;
                    continue;
                }
//                glNormal3fv((this->vertex_data[this->face_data[i].vertex_index[j]]).normal);
                glVertex3fv((this->vertex_data[this->face_data[i].vertex_index[j]]).locate);
            }
            glEnd();
        }
    }
    glPopMatrix();
    glFlush();
}
