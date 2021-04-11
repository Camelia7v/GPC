// GPC-Tema4.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <vector>
#include <GL/glut.h>


#define dim 300 // dimensiunea ferestrei in pixeli
#define MAX_L 26
#define MAX_C 26


unsigned char prevKey;
int nivel = 0;


class CPunct
{
public:
    CPunct(double x, double y)
    {
        this->x = x;
        this->y = y;
    }

    double get_x() { return x; }
    void set_x(double x) { this->x = x; }

    double get_y() { return y; }
    void set_y(double y) { this->y = y; }

public:
    double x, y;
};


class GrilaCarteziana {
public:
    // numarul de linii/coloane ale grilei
    int linii;
    int coloane;
    // distantele dintre 2 linii/coloane consecutive
    double d_l;
    double d_c;
private:
    double cx;
    double cy;
    double epsilon = 0.1;
public:
    GrilaCarteziana() {
        linii = MAX_L;
        coloane = MAX_C;

        d_l = (2.0 - 2.0 * epsilon) / ((double)linii);
        d_c = (2.0 - 2.0 * epsilon) / ((double)coloane);
        //printf("Value of d_l = %f\n", d_l);
        //printf("Value of d_c = %f\n", d_c);
    }

    void set_linii(int l) {
        if (l <= MAX_L)
            this->linii = l;
        updateDs();
    }
    void set_coloane(int c) {
        if (c <= MAX_C)
            this->coloane = c;
        updateDs();
    }

    void updateDs() {
        d_l = (2.0 - 2.0 * epsilon) / ((double)linii);
        d_c = (2.0 - 2.0 * epsilon) / ((double)coloane);
    }
    int get_linii() { return linii; }
    int get_coloane() { return coloane; }
    ~GrilaCarteziana() {}

    void afisareGrila() {
        glColor3f(0.0, 0.0, 0.0); // negru\

        double xmax = (double)dim / (double)dim + epsilon;
        double ymax = (double)dim / (double)dim + epsilon;
        for (double i = 0; i < coloane + 1; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f(-1.0 / xmax, (2.0 / (double)coloane * i - 1) / ymax);
            glVertex2f(1.0 / xmax, (2.0 / (double)coloane * i - 1) / ymax);
            glEnd();
        }

        for (double i = 0; i < linii + 1; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f((2.0 / (double)linii * i - 1) / xmax, -1.0 / ymax);
            glVertex2f((2.0 / (double)linii * i - 1) / xmax, 1.0 / ymax);
            glEnd();
        }
    }

    void drawCircle(float x1, float y1, float r) {
        //glColor3f(0.0f, 0.5f, 1.0f); // baby blue
        glColor3f(0.0, 0.0, 0.0); //negru
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(x1, y1);

        for (float angle = 1.0f; angle < 361.0f; angle += r)
        {
            float x2 = x1 + sin(angle) * r;
            float y2 = y1 + cos(angle) * r;
            glVertex2f(x2, y2);
        }

        glEnd();
    }

    /* fisierul cap2e, pag. 21-22
       functia de desenare efectiva a punctelor */
    void writePixel(double i, double j) {
        // verificare daca i si j se afla in parametri grilei
        if (i > linii) {
            printf("i not valid\n");
            return;
        }
        if (j > coloane) {
            printf("j not valid\n");
            return;
        }

        cx = -1 + epsilon + 0.0 * d_c;
        cy = -1 + epsilon + 0.0 * d_l;

        double x_GL = cx + i * d_c;
        double y_GL = cy + j * d_l;
        float raza = 0.03;

        drawCircle((float)x_GL, (float)y_GL, raza);
    }

    /* fisierul cap2e, pag. 6
       implementarea algoritmului AfisareSegmentDreapta3 */
    std::vector<CPunct> AfisareSegmentDreapta3(double xmin, double ymin, double xmax, double ymax) {
        /* valoarea initiala a variabilelor de decizie
           dx, dy sunt constante - a se vedea mai sus */

        std::vector<CPunct> puncte;
        int dx = xmax - xmin;
        int dy = ymax - ymin;
        double x = xmin;
        double y = ymin;
        int d = 2 * dy - dx;
        int dE = 2 * dy;
        int dNE = 2 * (dy - dx);

        CPunct q(x, y);
        puncte.push_back(q);

        while (y < ymax) {
            if (d <= 0) { // alegem E
                d += dE;
                x++;
            }
            else { // alegem NE
                d += dNE;
                x++;
                y++;
            }

            CPunct q(x, y);
            puncte.push_back(q);
            //printf("Value of x = %f\n", puncte[0].get_x());

            for (int i = 1; i <= 5 / 2; i++) {
                if (y + i < linii / 2)
                    puncte.push_back(CPunct(x, y + i));
            }
            for (int i = -1; i <= 5 / 2; i++) {
                if (-y + i < linii / 2)
                    if (x + i < linii / 2)
                        puncte.push_back(CPunct(x + i, -y));
            }
        }
        return puncte;
    }

    void afisareLinie(double xmin, double ymin, double xmax, double ymax)
    {
        /* se apeleaza AfisareSegmentDreapta3 pt parametrii afisareLinie
           pe baza punctelor date de ea -> obiecte ale clasei CPunct
           se apeleaza writePixel() */

        std::vector<CPunct> puncte = AfisareSegmentDreapta3(xmin, ymin, xmax, ymax);
        //printf("Value of x = %f\n", puncte[0].x);
        //printf("vector puncte %d " , puncte.size());

        for (int index = 0; index < puncte.size(); index++)
        {
            //printf("Value of x = %f\n", punct.get_x());
            writePixel(puncte[index].get_x(), puncte[index].get_y());
        }

        glColor4f(1.0f, 0.0f, 0.0f, 0.0f); // rosu
        glLineWidth(3);
        glBegin(GL_LINES);
        glVertex2d(xmin * d_l, ymin * d_c);
        glVertex2d(xmax * d_l, ymax * d_c);
        glEnd();
        glLineWidth(1);
    }
    std::vector<CPunct> AfisarePuncteCerc3(int x, int y)
    {

        std::vector<CPunct>  puncte;
        puncte.push_back(CPunct(x, y));
        for (int i = 1; i <= 3 / 2; i++)
        {
            puncte.push_back(CPunct(x - i, y));
            puncte.push_back(CPunct(x + i, y));
        }

        return puncte;
    }
    std::vector<CPunct> AfisareCerc4(float R)
    {
        std::vector<CPunct>  puncte;
        std::vector<CPunct>  puncteSecundare;
        int x = R;
        int y = 0;
        int d = 1 - R;
        int dN = 3;
        int dNW = -2 * R + 5;

        puncte.push_back(CPunct(x, y));
        puncteSecundare = AfisarePuncteCerc3(x, y);
        puncte.insert(std::end(puncte), std::begin(puncteSecundare), std::end(puncteSecundare));

        while (x > y)
        {
            if (d > 0)
            {
                // NW
                d += dNW;
                dN += 2;
                dNW += 4;
                x--;
            }
            else
            {
                // N
                d += dN;
                dN += 2;
                dNW += 2;
            }
            y++;

            puncte.push_back(CPunct(x, y));
            puncteSecundare = AfisarePuncteCerc3(x, y);
            puncte.insert(std::end(puncte), std::begin(puncteSecundare), std::end(puncteSecundare));

        }
        return puncte;
    }


    void afisareCerc(float  R, int width) {
        std::vector<CPunct> pixeli = AfisareCerc4(R);
        for (int index = 0; index < pixeli.size(); index++)
        {
            //printf("Value of x = %f\n", punct.get_x());
            writePixel(pixeli[index].get_x(), pixeli[index].get_y());
        }
        glColor3f(1.0, 0.1, 0.1); // rosu
        glLineWidth(4.f);
        double xmax = 1 + epsilon;
        double ymax = 1 + epsilon;
        float rx = (2.0 / (double)coloane * (0 + R) - 1) / xmax;
        float ry = (2.0 / (double)linii * 0 - 1) / ymax;
        glBegin(GL_LINE_LOOP);
        for (int i = 0; i < 1000; i++)
        {
            float theta = 2.0f * 3.1415926f * float(i) / float(1000);//get the current angle

            float x = (rx - cx) * cosf(theta);//calculate the x component
            float y = (rx - cx) * sinf(theta);//calculate the y component

            glVertex2f((double)x + cx, ((double)y + cy));//output vertex

        }
        glEnd();
        glLineWidth(1.f);

    }

    std::vector<CPunct> UmplereElipsa(int x0, int y0, int a, int b, double val) {
        std::vector<CPunct>  puncte;
        int x = -a;
        int y = 0;

        double d2 = b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
        double d1;

        puncte.push_back(CPunct(x + x0, y + y0));
        for (int i = x; i < 0; i++)
            puncte.push_back(CPunct(x + x0 - i, y + y0));

        while (a * a * (y - 0.5) > b * b * (x + 1))
        {
            if (d2 >= 0)
            {
                // selectam SE
                d2 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
                x++;
                y--;
            }
            else
            {
                // selectam S
                d2 += a * a * (-2 * y + 3);
                y--;
            }

            for (int i = x; i < 0; i++)
                puncte.push_back(CPunct(x + x0 - i, y + y0));
        }

        d1 = b * b * (x + 1) * (x + 1) + a * a * (y - 0.5) * (y - 0.5) - a * a * b * b;
        while (y > -b)
        {
            if (d1 >= 0)
            {
                // selectam E
                d1 += b * b * (2 * x + 3);
                x++;
            }
            else
            {
                // selectam SE
                d1 += b * b * (2 * x + 3) + a * a * (-2 * y + 2);
                x++;
                y--;
            }

            for (int i = x; i < 0; i++)
                puncte.push_back(CPunct(x + x0 - i, y + y0));
        }
        return puncte;
    }
    void afisareElipsa(int x0, int y0, int a, int b, double val) {
        std::vector<CPunct> pixeli = UmplereElipsa(x0, y0, a, b, val);
        for (int index = 0; index < pixeli.size(); index++)
        {
            //printf("Value of x = %f\n", punct.get_x());
            writePixel(pixeli[index].get_x(), pixeli[index].get_y());
        }


        float raza = 0.03;
        glColor3f(1.0, 0.1, 0.1); // rosu
        glLineWidth(4.f);
        glBegin(GL_LINE_STRIP);

        for (float angle = 1.0f; angle < 361.0f; angle += raza)
        {
            //2.3 distanta la margini
            float x2 = sin(angle) * raza * x0 * 2.3;
            float y2 = cos(angle) * raza * y0 * 2.3;
            glVertex2f(x2, y2 - .44);
        }

        glEnd();
        glLineWidth(1.f);


    }
};


void Display1() {
    GrilaCarteziana grila;
    grila.set_linii(16);
    grila.set_coloane(16);
    grila.afisareGrila();
    /* grila.afisareLinie(-7.5, 7.5, 7.5, 2.5);
     grila.afisareLinie(-7.5, -7.5, 7.5, -0.5);*/
    grila.writePixel(0.0, 0.0);

    grila.afisareCerc(14, 3);
    //grila.afisareElipsa(1, 1, 10, 10, 5);

    glFlush();
}


void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT);

    GrilaCarteziana grilaCarteziana1;
    grilaCarteziana1.set_linii(16);
    grilaCarteziana1.set_coloane(16);
    GrilaCarteziana grilaCarteziana2;

    switch (prevKey)
    {
    case '1':
        grilaCarteziana1.afisareGrila();
        grilaCarteziana1.afisareCerc(14, 3);
        //grilaCarteziana1.writePixel(0, 0);
        break;
    case '2':
        //centru elipsei 13,13
        grilaCarteziana2.afisareGrila();
        grilaCarteziana2.afisareElipsa(13, 7, 13, 7, 1);
        break;
        //case '3':
        //    grilaCarteziana1.afisareGrila();;
        //    grilaCarteziana1.deseneazaPoligon("poligon.txt");
        //    break;
    default:
        break;
    }

    glFlush();
    glFlush();
    glFlush();
}

void Reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
    prevKey = key;
    if (key == 27) // escape
        exit(0);
    glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
}

int main(int argc, char** argv)
{
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


