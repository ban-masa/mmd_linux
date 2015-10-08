#include <string>
#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <GL/glut.h>

class Vertex {
public:
    
    float locate[3];
    float normal[3];
    float uv[2];
    float** ex_uv;
    unsigned char weight_type;
    int bone_index[4];
    float bone_weight[4];
    float sdef_c[3];
    float sdef_r0[3];
    float sdef_r1[3];
    float edge_mag;
    int ex_uv_num;
    int id;
    Vertex()
    {
        for (int i = 0; i < 3; i++) {
            this->locate[i] = 0.0;
            this->normal[i] = 0.0;
        }
    }

    ~Vertex()
    {
        for (int i = 0; i < this->ex_uv_num; i++) {
            delete [] this->ex_uv[i];
        }
        delete [] this->ex_uv;
    }

    void read_data(std::ifstream &ifs, unsigned char (&info)[8], int num);
};

class Face {
public:
    unsigned int vertex_index[3];
    void read_data(std::ifstream &ifs, unsigned char (&info)[8]);
};

class Texture {
public:
    GLuint id;
    int num;
    char* path;
    IplImage* image;
    void read_data(std::ifstream &ifs, unsigned char (&info)[8]);
    void read_image(void);
};

class Material {
public:
    char* material_name[2];
    float diffuse[4];
    float specular[4];
    float co_specular;
    float ambient[4];
    unsigned char bit_flag;
    float edge_color[4];
    float edge_size;
    int normal_texture_index;
    int sphere_texture_index;
    unsigned char sphere_mode;
    unsigned char common_toon_flag;
    int toon_texture_index;
    unsigned char common_toon_texture;
    char* memo;
    int vertex_count;
    void read_data(std::ifstream &ifs, unsigned char (&info)[8]);
};

class IK_link {
public:
    int link_bone_index;
    unsigned char angle_limit_flag;
    float upper_limit[3];
    float lower_limit[3];
};

class Bone {
public:
    char* bone_name[2];
    float pos[3];
    int parent_index;
    int transform_hierarchy;
    unsigned short bit_flag;
    float pos_offset[3];
    int connect_bone_index;
    int grant_bone_index;
    float grant_rate;
    float axis_vect[3];
    float x_axis_vect[3];
    float z_axis_vect[3];
    int key_val;
    int IK_target_bone_index;
    int IK_loop_num;
    float IK_limit_angle;
    int IK_link_num;
    IK_link* IK_link_data;
    void read_data(std::ifstream &ifs, unsigned char (&info)[8]);
};

class MMD_model {
public:
    unsigned char magic1[4]; //マジックナンバー用
    float version;
    unsigned char info_byte;
    unsigned char info[8]; //各種情報（詳細は仕様書）
    /*
     バイト列 - byte
  [0] - エンコード方式  | 0:UTF16 1:UTF8
  [1] - 追加UV数 	| 0〜4 詳細は頂点参照

  [2] - 頂点Indexサイズ | 1,2,4 のいずれか
  [3] - テクスチャIndexサイズ | 1,2,4 のいずれか
  [4] - 材質Indexサイズ | 1,2,4 のいずれか
  [5] - ボーンIndexサイズ | 1,2,4 のいずれか
  [6] - モーフIndexサイズ | 1,2,4 のいずれか
  [7] - 剛体Indexサイズ | 1,2,4 のいずれか

     */
    char *text_buf[4];
    int vertex_num;
    int face_num;
    int texture_num;
    int material_num;
    int bone_num;
    int distance;
    int rotate_angle_x;
    int rotate_angle_y;
    float* vertex_index;
    float* norm_index;
    float* uv_index;
    unsigned int* face_vertex_index;

    Vertex* vertex_data;
    Face* face_data;
    Texture* texture_data;
    Material* material_data;
    Bone* bone_data;

    MMD_model()
    {
        distance = 0;
        rotate_angle_x = 0;
        rotate_angle_y = 0;
    }

    ~MMD_model()
    {
        for (int i = 0; i < 4; i++) {
            delete [] this->text_buf[i];
        }
        delete [] this->vertex_data;
        delete [] this->face_data;
        delete [] this->texture_data;
    }

    void read_model(char* filename);
    void display(void);
    void texture_config(void);
};
