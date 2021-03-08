#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GL/glut.h>

// dimensiunea ferestrei in pixeli
#define dim 300
double t = 0.00;
unsigned char prevKey;

// concoida lui Nicomede (concoida dreptei)
// $x = a + b \cdot cos(t), y = a \cdot tg(t) + b \cdot sin(t)$. sau
// $x = a - b \cdot cos(t), y = a \cdot tg(t) - b \cdot sin(t)$. unde
// $t \in (-\pi / 2, \pi / 2)$
void Display1() {
    double xmax, ymax, xmin, ymin;
    double a = 1, b = 2;
    double pi = 4 * atan(1);
    double ratia = 0.05;

    // calculul valorilor maxime/minime ptr. x si y
    // aceste valori vor fi folosite ulterior la scalare
    xmax = a - b - 1;
    xmin = a + b + 1;
    ymax = ymin = 0;
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = a + b * cos(t);
        xmax = (xmax < x1) ? x1 : xmax;
        xmin = (xmin > x1) ? x1 : xmin;

        x2 = a - b * cos(t);
        xmax = (xmax < x2) ? x2 : xmax;
        xmin = (xmin > x2) ? x2 : xmin;

        y1 = a * tan(t) + b * sin(t);
        ymax = (ymax < y1) ? y1 : ymax;
        ymin = (ymin > y1) ? y1 : ymin;

        y2 = a * tan(t) - b * sin(t);
        ymax = (ymax < y2) ? y2 : ymax;
        ymin = (ymin > y2) ? y2 : ymin;
    }

    xmax = (fabs(xmax) > fabs(xmin)) ? fabs(xmax) : fabs(xmin);
    ymax = (fabs(ymax) > fabs(ymin)) ? fabs(ymax) : fabs(ymin);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    double x1, y1, x2, y2;
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();

    glBegin(GL_LINE_STRIP);
    for (t = -pi / 2 + ratia; t < pi / 2; t += ratia) {
        double x1, y1, x2, y2;
        x1 = (a + b * cos(t)) / xmax;
        x2 = (a - b * cos(t)) / xmax;
        y1 = (a * tan(t) + b * sin(t)) / ymax;
        y2 = (a * tan(t) - b * sin(t)) / ymax;

        glVertex2f(x2, y2);
    }
    glEnd();
}

// graficul functiei 
// $f(x) = \bar sin(x) \bar \cdot e^{-sin(x)}, x \in \langle 0, 8 \cdot \pi \rangle$, 
void Display2() {
    double pi = 4 * atan(1);
    double xmax = 8 * pi;
    double ymax = exp(1.1);
    double ratia = 0.05;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        double x1, y1;
        x1 = x / xmax;
        y1 = (fabs(sin(x)) * exp(-sin(x))) / ymax;

        glVertex2f(x1, y1);
    }
    glEnd();
}


double d(double x) {
    double higher_value = ceil(x) - x;
    double lower_value = x - floor(x);
    if (lower_value < higher_value)
        return lower_value;
    else
        return higher_value;
}
// graficul functiei cu 2 cazuri
// 1 daca x=0;
// d(x)/x daca 0<x<=100  , unde d(x)=distanta de la x cel mai apropiat intreg
void Display3() {
    double ratia = 0.05;
    double xmax = 25.0;
    double y = 1.0;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double x = 0; x < xmax; x += ratia) {
        if (x > 0)
        {
            y = d(x) / x;
        }
        double x1, y1;
        x1 = x / xmax;
        y1 = y - ratia / 2;

        glVertex2f(x1, y1);
    }

    glEnd();
}

//Display4-Display8
//curbe date prin ecuatii parametrice


//melcul lui Pascal
void Display4() {
    double ratia = 0.05;
    double a = 0.3;
    double b = 0.2;
    double pi = 4 * atan(1);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (t = -pi + ratia; t < pi; t += ratia) {
        double x1, y1;
        x1 = 2 * (a * cos(t) + b) * cos(t);
        y1 = 2 * (a * cos(t) + b) * sin(t);

        glVertex2f(x1, y1);
    }
    glEnd();
}


//trisectoarea lui Longchamps
//  -pi/2 -> -pi/6 -> pi/6 -> -pi/2
void Display5() {
    double ratia = 0.005;
    double a = 0.2;
    double pi = 4 * atan(1);
    double x[1000];
    double y[1000];

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(0.0, 0.0, 1.0); // albastru
    glBegin(GL_LINE_LOOP);
    glVertex2f(-0.85, 0.98);
    int i = 0;
    for (double t = -(pi / 2) + ratia; t < -(pi / 6); t += ratia) {
        double x1, y1;
        x1 = a / (4 * cos(t) * cos(t) - 3);
        y1 = (a * tan(t)) / (4 * cos(t) * cos(t) - 3);
        //se memoreaza coordonatele punctelor gasite cu formulele date
        x[i] = x1;
        y[i] = y1;
        i++;
        //pt liniile de sus
        if (x1 < -0.85 || y1 > 0.98) continue;
        glVertex2f(x1, y1);
    }
    glEnd();
 
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_TRIANGLES);
    //se creaza triunghuri pe baza punctelor consecutive memorate si a coltului din drp sus , well, aproape coltul
    for (int j = 1; j < i - 1; j = j + 3) {
        if (x[j] <= -0.83 || y[j] >= 0.97) continue;
        glVertex2f(-0.83, 0.97);
        glVertex2f(x[j], y[j]);
        glVertex2f(x[j + 1], y[j + 1]);
    }
    glEnd();
}


//cicloida
void Display6() {
    double ratia = 0.05;
    double a = 0.1;
    double b = 0.2;
    double pi = 4 * atan(1);

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = -(4 * pi) + ratia; t < 4 * pi; t += ratia) {
        double x1, y1;
        x1 = a * t - b * sin(t);
        y1 = a - b * cos(t);

        glVertex2f(x1, y1);
    }
    glEnd();
}

//epicicloida
void Display7() {
    double ratia = 0.05;
    double R = 0.1;
    double r = 0.3;
    double pi = 4 * atan(1);
    double raport_r = r / R;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R + r) * cos(raport_r * t) - r * cos(t + raport_r * t);
        y1 = (R + r) * sin(raport_r * t) - r * sin(t + raport_r * t);

        glVertex2f(x1, y1);
    }
    glEnd();
}


//hipocicloida
void Display8() {
    double ratia = 0.05;
    double R = 0.1;
    double r = 0.3;
    double pi = 4 * atan(1);
    double raport_r = r / R;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t <= 2 * pi; t += ratia) {
        double x1, y1;
        x1 = (R - r) * cos(raport_r * t) - r * cos(t - raport_r * t);
        y1 = (R - r) * sin(raport_r * t) - r * sin(t - raport_r * t);

        glVertex2f(x1, y1);
    }
    glEnd();
}


//Display9-Display10
//curbe date de ecuatii polare
// t intre a si b
// r = f(x)
// x = r * cos(t) 
// y = r * sin(t)


//lemniscata lui Bernoulli
void Display9() {
    double ratia = 0.005;
    double pi = 4 * atan(1);
    double a = 0.4;
    double r;
    double x1, y1;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = pi / 4 - ratia; t > -(pi / 4); t -= ratia) {
        r = a * sqrt(2 * cos(2 * t));
        x1 = r * cos(t);
        y1 = r * sin(t);
        glVertex2f(x1, y1);
    }

    for (double t = -(pi / 4) + ratia; t < pi / 4; t += ratia) {
        r = -(a * sqrt(2 * cos(2 * t)));
        x1 = r * cos(t);
        y1 = r * sin(t);
        glVertex2f(x1, y1);
    }
    glEnd();
}


//spirala logaritmica
void Display10() {
    double ratia = 0.05;
    double pi = 4 * atan(1);
    double a = 0.02;
    double r;
    double x1, y1;

    // afisarea punctelor propriu-zise precedata de scalare
    glColor3f(1, 0.1, 0.1); // rosu
    glBegin(GL_LINE_STRIP);
    for (double t = 0; t < 300; t += ratia) {
        r = a * pow(exp(1.0), 1 + t);
        x1 = r * cos(t);
        y1 = r * sin(t);
        glVertex2f(x1, y1);
    }
    glEnd();
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    //   glPointSize(4);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    switch (prevKey) {
    case '1':
        Display1();
        break;
    case '2':
        Display2();
        break;
    case '3':
        Display3();
        break;
    case '4':
        Display4();
        break;
    case '5':
        Display5();
        break;
    case '6':
        Display6();
        break;
    case '7':
        Display7();
        break;
    case '8':
        Display8();
        break;
    case '9':
        Display9();
        break;
    case '0':
        Display10();
        break;
    default:
        break;
    }

    glFlush();
}

void Reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y) {
    prevKey = key;
    if (key == 27) // escape
       exit(1);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y) {
}

int main(int argc, char** argv) {

    glutInit(&argc, argv);

    glutInitWindowSize(dim, dim);

    glutInitWindowPosition(100, 100);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutCreateWindow(argv[0]);

    Init();

    glutReshapeFunc(Reshape);

    glutKeyboardFunc(KeyboardFunc);

    glutMouseFunc(MouseFunc);

    glutDisplayFunc(Display);

    glutMainLoop();

    return 0;
}