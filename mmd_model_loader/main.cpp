#include <iostream>
#include "mmd_data.hpp"
#include <GL/glut.h>

GLfloat lightPosition[4] = {-20.0, 20.0, 10000.0, 0.0};
GLfloat white[] = {0.5, 0.5, 0.5, 1.0};
int toggle_display_func = 0;
int boneid = 0;

MMD_model sample;

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void resize(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
    glViewport(0, 0, w, h);
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 5000.0);
    gluLookAt(0.0, 10.0, 100.0, 0.0, 10.0, -10.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, white);

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    if (toggle_display_func % 3 == 0) {
      sample.display();
    } else if (toggle_display_func % 3 == 1) {
      sample.bone_display();
    } else {
      sample.bone_with_vertex_display(boneid);
    }
    glutSwapBuffers();
    glFlush();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'q':
            exit(0);
        case 'z':
            sample.distance++;
            break;
        case 'Z':
            sample.distance--;
            break;
        case 'e':
            sample.rotate_angle_x++;
            break;
        case 'E':
            sample.rotate_angle_x--;
            break;
        case 'r':
            sample.rotate_angle_y++;
            break;
        case 'R':
            sample.rotate_angle_y--;
            break;
        case 't':
            toggle_display_func++;
            break;
        case 'n':
            boneid++;
            std::cout << boneid << std::endl;
            break;
        case 'u':
            sample.update_bone();
            break;
        case 'i':
            sample.set_angle();
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "ファイルを指定してください" << std::endl;
        std::exit(1);
    }

    sample.read_model(argv[1]);
    std::cout << std::endl;
    std::cout << "Read End" << std::endl;
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    init();
    sample.texture_config();
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}//30643
