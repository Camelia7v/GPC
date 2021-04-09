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

        d_l = (2.0 * (1.0 - epsilon)) / ((double)linii - 1);
        d_c = (2.0 * (1.0 - epsilon)) / ((double)coloane - 1);
        //printf("Value of d_l = %f\n", d_l);
        //printf("Value of d_c = %f\n", d_c);
    }

    void set_linii(int l) {
        if (l <= MAX_L)
            linii = l;
    }
    void set_coloane(int c) {
        if (c <= MAX_C)
            linii = c;
    }

    int get_linii() { return linii; }
    int get_coloane() { return coloane; }
    ~GrilaCarteziana() {}

    void afisareGrila() {
        glColor3f(0.0, 0.0, 0.0); // negru
        double x_c = -1.0 + epsilon;
        double x_l = -1.0 + epsilon;

        for (double i = 0; i <= linii; i++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f(x_l, -1.0 + epsilon);
            glVertex2f(x_l, 1.0 - epsilon);
            x_l += d_l;
            glEnd();
        }
        for (double j = 0; j <= coloane; j++)
        {
            glBegin(GL_LINE_STRIP);
            glVertex2f(-1.0 + epsilon, x_c);
            glVertex2f(1.0 - epsilon, x_c);
            x_c += d_c;
            glEnd();
        }
    }

    void drawCircle(float x1, float y1, float r) {
        glColor3f(0.0f, 0.5f, 1.0f); // baby blue
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

        cx = -1 + epsilon + 7.5 * d_c;
        cy = -1 + epsilon + 7.5 * d_l;

        double x_GL = cx + i * d_c;
        double y_GL = cy + j * d_l;
        float raza = 0.04;

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
        puncte.push_back(CPunct(x - 0.5, y - 0.5));
        puncte.push_back(CPunct(x + 0.5, y + 0.5));
        puncte.push_back(CPunct(x + 0.5, y - 0.5));
        puncte.push_back(CPunct(x - 0.5, y + 0.5));
        /*puncte.push_back(CPunct(-x, -y));
        puncte.push_back(CPunct(-x, y));
        puncte.push_back(CPunct(x, -y));
        */
        if (x != y) {
            /*      puncte.push_back(CPunct(y,x));*/
            puncte.push_back(CPunct(y + 0.5, x + 0.5));
            puncte.push_back(CPunct(y - 0.5, x - 0.5));
            puncte.push_back(CPunct(y - 0.5, x + 0.5));
            puncte.push_back(CPunct(y + 0.5, x - 0.5));

            /*  puncte.push_back(CPunct(-y,-x));
              puncte.push_back(CPunct(-y,x));
              puncte.push_back(CPunct(y,-x));*/

        }
        return puncte;
    }
    std::vector<CPunct> AfisareCerc4(float R)
    {
        std::vector<CPunct>  puncte;
        int x = 0, y = R;
        int d = 1 - R;
        int dE = 3, dSE = -2 * R + 5;
        std::vector<CPunct> puncteSec = AfisarePuncteCerc3(x, y);
        puncte.insert(std::end(puncte), std::begin(puncteSec), std::end(puncteSec));
        while (y > x)
        {
            if (d < 0)
            {
                d += dE;
                dE += 2;
                dSE += 2;
            }
            else
            {
                d += dSE;
                dE += 2;
                dSE += 4;
                y--;
            }
            x++;
            std::vector<CPunct> puncteSec = AfisarePuncteCerc3(x, y);
            puncte.insert(std::end(puncte), std::begin(puncteSec), std::end(puncteSec));
        }
        return puncte;
    }

    std::vector<CPunct> UmplereElipsa(int x0, int y0, int a, int b, double val) {
        std::vector<CPunct>  puncte;
        int xi = 0, x = 0, y = b;
        double fxpyp = 0.0;
        double deltaE, deltaSE, deltaS;

        puncte.push_back(CPunct(y + y0 + 0.5, xi + x0 + 0.5));
        puncte.push_back(CPunct(y + y0 - 0.5, xi + x0 - 0.5));

        val = x + x0;
        // regiunea 1
        while (a * a * (y - 0.5) > b * b * (x + 1))
        {
            deltaE = b * b * (2 * x + 1);
            deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
            if (fxpyp + deltaE <= 0.0)
            {
                // E este in interior
                fxpyp += deltaE;
                x++;
                /*puncte.push_back(CPunct(y + y0 + 0.5, xi + x0 + 0.5));
                puncte.push_back(CPunct(y + y0 - 0.5, xi + x0 - 0.5));*/
                std::vector<CPunct> puncteSec = AfisarePuncteCerc3(y + y0, xi + x0);
                puncte.insert(std::end(puncte), std::begin(puncteSec), std::end(puncteSec));


                val = x + x0;
            }
            else if (fxpyp + deltaSE <= 0.0)
            {
                // SE este in interior
                fxpyp += deltaSE;
                x++; y--;
                //puncte.push_back(CPunct(y + y0 + 0.5, xi + x0 + 0.5));
                //puncte.push_back(CPunct(y + y0 - 0.5, xi + x0 - 0.5));
                std::vector<CPunct> puncteSec = AfisarePuncteCerc3(y + y0, xi + x0);
                puncte.insert(std::end(puncte), std::begin(puncteSec), std::end(puncteSec));

                val = x + x0;
            }
            // regiunea 2
            while (y > 0)
            {
                deltaSE = b * b * (2 * x + 1) + a * a * (-2 * y + 1);
                deltaS = a * a * (-2 * y + 1);
                if (fxpyp + deltaSE <= 0.0)
                {
                    // SE este in interior
                    fxpyp += deltaSE;
                    x++; y--;
                }
                else
                {
                    // S este in interior
                    fxpyp += deltaS;
                    y--;
                }
                /*puncte.push_back(CPunct(y + y0 + 0.5, xi + x0 + 0.5));
                puncte.push_back(CPunct(y + y0 - 0.5, xi + x0 - 0.5));*/
                std::vector<CPunct> puncteSec = AfisarePuncteCerc3(y + y0, xi + x0);
                puncte.insert(std::end(puncte), std::begin(puncteSec), std::end(puncteSec));
                val = x + x0;
            }
        }

        return puncte;
    }
    void afisareCerc(float r, int width) {
        std::vector<CPunct> pixeli = AfisareCerc4(r);
        for (int index = 0; index < pixeli.size(); index++)
        {
            //printf("Value of x = %f\n", punct.get_x());
            writePixel(pixeli[index].get_x(), pixeli[index].get_y());
        }

        glBegin(GL_POLYGON);
        double ori_x = 0.0;
        double ori_y = 0.0;
        for (int i = 0; i <= 300; i++) {
            double angle = 2 * 4 * atan(1) * i / 300;
            double x = cos(angle) * r * d_l;
            double y = sin(angle) * r * d_c;
            glVertex2d(ori_x + x, ori_y + y);
        }
        glEnd();

    }
    void afisareElipsa(int x0, int y0, int a, int b, double val) {
        std::vector<CPunct> pixeli = UmplereElipsa(x0, y0, a, b, val);
        for (int index = 0; index < pixeli.size(); index++)
        {
            //printf("Value of x = %f\n", punct.get_x());
            writePixel(pixeli[index].get_x(), pixeli[index].get_y());
        }
    }
};


void Display1() {
    GrilaCarteziana grila;
    //grila.linii = 16;
    //grila.coloane = 16;
    grila.afisareGrila();
    /* grila.afisareLinie(-7.5, 7.5, 7.5, 2.5);
     grila.afisareLinie(-7.5, -7.5, 7.5, -0.5);*/
     /* grila.afisareCerc(7, 3);*/
    grila.afisareElipsa(1, 1, 5.5, 5.5, 5);

    //coordonate extreme grila 16x16
    /*grila.writePixel(0, 0);
    grila.writePixel(-7.5, -7.5);
    grila.writePixel(-7.5, 7.5);
    grila.writePixel(7.5, -7.5);
    grila.writePixel(7.5,7.5);*/

    //coordonate extreme grila 26x26

    //stg jos
    grila.writePixel(-7.5, -7.5);
    //drp sus
    grila.writePixel(17.5, 17.5);
    //centru
    grila.writePixel(5.5, 5.5);
    //stg sus
    grila.writePixel(-7.5, 17.5);
    //drp jos
    grila.writePixel(17.5, -7.5);



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
    Display1();
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


