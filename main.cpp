#include <iostream>
#include "mmd_data.hpp"
#include <GL/glut.h>

MMD_model sample;

void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
/*    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);*/
}

void resize(int w, int h)
{
    glViewport(0, 0, w, h);
    glLoadIdentity();
    gluPerspective(30.0, (double)w / (double)h, 1.0, 100.0);
    gluLookAt(0.0, 10.0, 10.0, 0.0, 10.0, -10.0, 0.0, 1.0, 0.0);
}

void display(void)
{
    sample.display();
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
    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow(argv[0]);
    glutDisplayFunc(display);
    glutReshapeFunc(resize);
    glutKeyboardFunc(keyboard);
    init();
    glutPostRedisplay();
    glutMainLoop();
    return 0;
}//30643
