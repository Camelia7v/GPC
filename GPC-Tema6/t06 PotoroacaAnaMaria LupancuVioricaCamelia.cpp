#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 300

unsigned char prevKey;

// coordonate triunghi
// P1
double p1_x = 1.0;
double p1_y = 1.0;
double p1_z = 1.0;
// P2 
double p2_x = 0.0;
double p2_y = 7.0;
double p2_z = 0.0;
// P3 
double p3_x = 0.0;
double p3_y = 7.0;
double p3_z = 7.0;


// pt. exercitiul 2, case 1-5, din functia Display


// calculare distante dintre puncte
double D1 = sqrt((p2_x - p1_x) * (p2_x - p1_x) + (p2_z - p1_z) * (p2_z - p1_z));
double D2 = sqrt((p2_y - p1_y) * (p2_y - p1_y) + D1 * D1);
double D3 = sqrt(p3_x * p3_x + p3_y * p3_y);

// calculare valori unghiuri pentru rotatii cub
double theta = 180 - acos((p2_z - p1_z) / D1);
double alfa = 180 - acos(p3_y / D3);
double phi = 180 - acos(D1 / D2);

enum EObiect { cubw, cubs, sferaw, sferas, trianglew, triangles } ob = cubw;

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


void DisplayMainDiagonal() {
    glLineWidth(4);
    glColor3f(0.0f, 0.1f, 0.1f); // Dark blue
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.95f, 0.95f, 0.0f);
    glVertex3f(0.0f, 0.0f, 0.95f);
    glEnd();

    glLineWidth(1);
}

void DisplayWireCube(double size = 1) {
    glColor3f(1, 0, 0);
    glTranslatef(0.3f, 0.3f, 0.0f);
    glutWireCube(size);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  // this tells it to only render lines

    //glBegin(GL_LINES);
    //glVertex3f(-0.5, -0.5, 0.5);
    //glVertex3f(-0.5, 0.5, 0.5);
    //glVertex3f(-0.5, 0.5, -0.5);
    //glVertex3f(-0.5, -0.5, -0.5);
    //glEnd();
}

// cub wireframe
void Display1(double size = 1) {
    glColor3f(1, 0, 0);
    glutWireCube(size);
}

// cub solid
void Display2(double size = 1) {
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
void Display5(double p1_x, double p1_y, double p1_z, double p2_x, double p2_y, double p2_z, double p3_x, double p3_y, double p3_z) {
    //glColor3f(1, 0, 0);
    //glutWireTetrahedron();

    glColor3f(1.0f, 0.5f, 0.0f);//Orange
    glBegin(GL_LINE_LOOP);
    glVertex3f(p1_x, p1_y, p1_z);
    glVertex3f(p2_x, p2_y, p2_z);
    glVertex3f(p3_x, p3_y, p3_z);
    glEnd();
}

// triangle solid
void Display6(double p1_x, double p1_y, double p1_z, double p2_x, double p2_y, double p2_z, double p3_x, double p3_y, double p3_z) {
    /*glColor3f(1, 0, 0);
    glutSolidTetrahedron();*/
    glLineWidth(3);
    glColor3f(1.0f, 0.5f, 0.0f);//Orange
    glBegin(GL_TRIANGLES);
    glVertex3f(p1_x, p1_y, p1_z);
    glVertex3f(p2_x, p2_y, p2_z);
    glVertex3f(p3_x, p3_y, p3_z);
    glEnd();
    glLineWidth(1);
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
        Display5(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        break;
    case triangles:
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
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
    //glTranslatef(0.2, 0.2, 0.2);
    //glTranslatef(0.0,0.0,0.0);
    glTranslatef(-1.0, -1.0, 0.0); //merge inainte pe Oz
    //glTranslatef(1.0, 1.0, 0.0); //trece pe Ox
    //glTranslatef(0.0, 1.0, -1.0); //trece pe Oy
}

// Scalarea cu 1.2, 1.2, 1.2
void DisplayS() {
    glMatrixMode(GL_MODELVIEW);
    glScalef(1.3, 1.4, 1.4);
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
        //display axe + wire triunghi
        //CASE 0
    case 'q':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        Display5(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = trianglew;
        break;
        /*========================================*/
        /*==============EXERCITIUL 2==============*/
        //display axe + triunghi
        //CASE 1
    case 'w':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = triangles;
        break;
        //display axe + triunghi
        //CASE 2 = TRANSLATE
    case 'e':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        glTranslated(-p1_x, -p1_y, -p1_z);
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = triangles;
        break;
        //display axe + triunghi
        //CASE 3 = ROTATE + TRANSLATE
    case 'r':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        printf("theta = %f\n", theta);
        glRotated(theta - 90, 0, 1, 0);
        glTranslated(-p1_x, -p1_y, -p1_z);
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = triangles;
        break;
        //display axe + triunghi
        //CASE 4 = ROTATED + ROTATED + TRANSLATE
    case 'c':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        printf("alfa = %f\n", alfa);
        glRotated(alfa, 0, 0, 1);
        glRotated(theta - 90, 0, 1, 0);
        glTranslated(-p1_x, -p1_y, -p1_z);
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = triangles;
        break;
        //display axe + triunghi
        //CASE 5 = ROTATED + ROTATED + ROTATED + TRANSLATE
    case 'v':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        printf("phi = %f\n", phi);
        glRotated(phi, 0, 0, 1);
        glRotated(alfa, 1, 0, 0);
        glRotated(theta - 90, 0, 1, 0);
        glTranslated(-p1_x, -p1_y, -p1_z);
        Display6(p1_x, p1_y, p1_z, p2_x, p2_y, p2_z, p3_x, p3_y, p3_z);
        ob = triangles;
        break;
        /*========================================*/
        /*==============EXERCITIUL 3==============*/
    case 'b':
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotated(20, 1, 0, 0);
        glRotated(-20, 0, 1, 0);
        DisplayAxe();
        DisplayMainDiagonal();
        //Display1();
        DisplayWireCube();
        ob = cubw;
        break;
        /*=======================================*/
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