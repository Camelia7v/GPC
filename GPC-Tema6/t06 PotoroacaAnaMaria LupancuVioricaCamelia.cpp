#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

enum EObiect { cubw, cubs, sferaw, sferas, trianglew, triangles} ob = cubw;

void DisplayAxe() {
    int X, Y, Z;
    X = Y = 200;
    Z = 200;

    glLineWidth(2);

    // axa Ox - verde
    glColor3f(0, 1, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(X, 0, 0);
    glEnd();

    // axa Oy - albastru
    glColor3f(0, 0, 1);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, Y, 0);
    glEnd();

    // axa Oz - rosu
    glColor3f(1, 0, 0);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, Z);
    glEnd();

    glLineWidth(1);
}

// cub wireframe
void Display1() {
    glColor3f(1, 0, 0);
    glutWireCube(1);
}

// cub solid
void Display2() {
    glColor3f(1, 0, 0);
    glutSolidCube(1);
}

// sfera wireframe
void Display3() {
    glColor3f(0, 0, 1);
    glutWireSphere(1, 10, 10);
}

// sfera solida
void Display4() {
    glColor3f(0, 0, 1);
    glutSolidSphere(1, 10, 10);
}

// triangle wireframe
void Display5() {
    //glColor3f(1, 0, 0);
    //glutWireTetrahedron();
    glPushMatrix();
        glColor3f(1.0f, 0.5f, 0.0f);//Orange
        glBegin(GL_LINE_LOOP);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.5, 1.0);
        glEnd();
    glPopMatrix();
}

// triangle solid
void Display6() {
    /*glColor3f(1, 0, 0);
    glutSolidTetrahedron();*/

    glPushMatrix();
        glColor3f(1.0f, 0.5f, 0.0f);//Orange
        glBegin(GL_TRIANGLES);
        glVertex3f(0.0, 0.0, 0.0);
        glVertex3f(0.0, 1.0, 0.0);
        glVertex3f(0.0, 0.5, 1.0);
        glEnd();
    glPopMatrix();
}

void DisplayObiect()
{
    switch (ob)
    {
    case cubw:
        Display1();
        break;
    case cubs:
        Display2();
        break;
    case sferaw:
        Display3();
        break;
    case sferas:
        Display4();
        break;
    case trianglew:
        Display5();
        break;
    case triangles:
        Display6();
        break;
    default:
        break;
    }
}

// rotatia cu un unghi de 20 grade in raport cu axa x
void DisplayX() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(20, 1, 0, 0);
}

// rotatia cu un unghi de 20 grade in raport cu axa y
void DisplayY() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(20, 0, 1, 0);
}

// rotatia cu un unghi de 20 grade in raport cu axa z
void DisplayZ() {
    glMatrixMode(GL_MODELVIEW);
    glRotated(20, 0, 0, 1);
}

// Translatia cu 0.2, 0.2, 0.2
void DisplayT() {
    glMatrixMode(GL_MODELVIEW);
    glTranslatef(0.2, 0.2, 0.2);
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayS() {
    glMatrixMode(GL_MODELVIEW);
    glScalef(4, 4, 4);
}

void Init(void) {
    glClearColor(1, 1, 1, 1);
    glLineWidth(2);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-10, 10, -10, 10, 30, -30);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotated(20, 1, 0, 0);
    glRotated(-20, 0, 1, 0);
}

void Display(void) {
    switch (prevKey)
    {
    case 'a':
        DisplayAxe();
        break;
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        break;
    case '1':
        Display1();
        ob = cubw;
        break;
    case '2':
        Display2();
        ob = cubs;
        break;
    case '3':
        Display3();
        ob = sferaw;
        break;
    case '4':
        Display4();
        ob = sferas;
        break;
    case '5':
        Display5();
        ob = trianglew;
        break;
    case '6':
        Display6();
        ob = triangles;
        break;
    case 'x':
        glClear(GL_COLOR_BUFFER_BIT);
        //axe variante
        /*DisplayX();
        DisplayAxe();
        DisplayObiect();*/

        //axe invariante
        DisplayAxe();
        glPushMatrix();
            DisplayX();
            DisplayObiect();
        glPopMatrix();
        break;
    case 'y':
        glClear(GL_COLOR_BUFFER_BIT);
        //axe variante
        //DisplayY();
        //DisplayAxe();
        //DisplayObiect();

        //axe invariante
        DisplayAxe();
        glPushMatrix();
            DisplayY();
            DisplayObiect();
        glPopMatrix();
        break;
    case 'z':
        glClear(GL_COLOR_BUFFER_BIT);
        //axe variante
        //DisplayZ();
        //DisplayAxe();
        //DisplayObiect();
        
        //axe invariante
        DisplayAxe();
        glPushMatrix();
            DisplayZ();
            DisplayObiect();
        glPopMatrix();
        break;
    case 't':
        glClear(GL_COLOR_BUFFER_BIT);
        //axe variante
        //DisplayT();
        //DisplayAxe();
        //DisplayObiect();

        //axe invariante
        DisplayAxe();
        glPushMatrix();
            DisplayT();
            DisplayObiect();
        glPopMatrix();
        break;
    case 's':
        glClear(GL_COLOR_BUFFER_BIT);
        //axe variante
        //DisplayS();
        //DisplayAxe();
        //DisplayObiect();

        //axe invariante
        DisplayAxe();
        glPushMatrix();
            DisplayS();
            DisplayObiect();
        glPopMatrix();
        break;
    default:
        break;
    }
    glutSwapBuffers();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}