#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <GL/glut.h>
#include <vector>


// dimensiunea ferestrei in pixeli
#define dim 300
#define MAX_L 15
#define MAX_C 15


unsigned char prevKey;
int nivel = 0;


class C2coord
{
public:
    C2coord()
    {
        m.x = m.y = 0;
    }

    C2coord(double x, double y)
    {
        m.x = x;
        m.y = y;
    }

    C2coord(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
    }

    C2coord& operator=(C2coord& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(C2coord& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

protected:
    struct SDate
    {
        double x, y;
    } m;
};

class CPunct : public C2coord
{
public:
    CPunct() : C2coord(0.0, 0.0)
    {}

    CPunct(double x, double y) : C2coord(x, y)
    {}


    CPunct& operator=(const CPunct& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    void getxy(double& x, double& y)
    {
        x = m.x;
        y = m.y;
    }

    int operator==(CPunct& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    void marcheaza()
    {
        glBegin(GL_POINTS);
        glVertex2d(m.x, m.y);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "(%+f,%+f)", m.x, m.y);
    }

};

class CVector : public C2coord
{
public:
    CVector() : C2coord(0.0, 0.0)
    {
        normalizare();
    }

    CVector(double x, double y) : C2coord(x, y)
    {
        normalizare();
    }

    CVector& operator=(CVector& p)
    {
        m.x = p.m.x;
        m.y = p.m.y;
        return *this;
    }

    int operator==(CVector& p)
    {
        return ((m.x == p.m.x) && (m.y == p.m.y));
    }

    CPunct getDest(CPunct& orig, double lungime)
    {
        double x, y;
        orig.getxy(x, y);
        CPunct p(x + m.x * lungime, y + m.y * lungime);
        return p;
    }

    void rotatie(double grade)
    {
        double x = m.x;
        double y = m.y;
        double t = 2 * (4.0 * atan(1)) * grade / 360.0;
        m.x = x * cos(t) - y * sin(t);
        m.y = x * sin(t) + y * cos(t);
        normalizare();
    }

    void deseneaza(CPunct p, double lungime)
    {
        double x, y;
        p.getxy(x, y);
        glColor3f(1.0, 0.1, 0.1);
        glBegin(GL_LINE_STRIP);
        glVertex2d(x, y);
        glVertex2d(x + m.x * lungime, y + m.y * lungime);
        glEnd();
    }

    void print(FILE* fis)
    {
        fprintf(fis, "%+fi %+fj", C2coord::m.x, C2coord::m.y);
    }

private:
    void normalizare()
    {
        double d = sqrt(C2coord::m.x * C2coord::m.x + C2coord::m.y * C2coord::m.y);
        if (d != 0.0)
        {
            C2coord::m.x = C2coord::m.x * 1.0 / d;
            C2coord::m.y = C2coord::m.y * 1.0 / d;
        }
    }
};


class GrilaCarteziana {
private:
    int linii;
    int coloane;
    double epsilon = 0.1;
    double cx;
    double cy;
public:
    GrilaCarteziana() {
        linii = MAX_L;
        coloane = MAX_C;
    }
    void set_linii(int l){
        if (l <= MAX_L) {
            linii = l;
        }
    }
    
    void set_coloane(int c) {
        if (c <= MAX_C) {
            linii = c;
        }
    }
    int get_linii() { return linii; }
    int get_coloane() { return coloane; }
    ~GrilaCarteziana() {}

    void afisareGrila() {
        glColor3f(0.0, 0.0, 0.0);
        double x_c = -1.0 + epsilon;
        double x_l = -1.0 + epsilon;
        double d_l = (2.0 * (1.0 - epsilon)) / ((double)coloane - 1);
        double d_c = (2.0 * (1.0 - epsilon)) / ((double)linii - 1);
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

    //functia de desenare efectiva a punctelor
    void writePixel(int i, int j) {
        //verificare daca i si j se afla in parametrii grilei
        if (i > linii || i < 0) {
            printf("i not valid\n");
            return;
        }
        if (j > coloane || j < 0) {
            printf("j not valid\n");
            return;
        }
        double d_l = (2.0 * (1.0 - epsilon)) / ((double)coloane - 1);
        double d_c = (2.0 * (1.0 - epsilon)) / ((double)linii - 1);

        cx = -1 + epsilon + 3 * d_c;
        cy = -1 + epsilon + 3 * d_l;
        double x = -1.0+epsilon;
        double y = -1.0+epsilon;
        double x_GL = cx + x * d_c;
        double y_GL = cy + y * d_l;
        for (int i = 0; i <= linii; i++) {

            glColor4f(1.0f, 0.0f, 0.0f, 0.0f);//red
            glBegin(GL_POINTS);
            glVertex2d(x_GL/i+epsilon, y_GL/i+epsilon);
            x_GL *= x;
            y_GL *= y;
            glEnd();
        }
    }

    //implementarea algoritmului AfisareSegmentDreapta3
    std::vector<CPunct> AfisareSegmentDreapta3(double xmin, double ymin, double xmax, double ymax){}

    void afisareLinie(double xmin, double ymin, double xmax, double ymax, int size = 1)
    {
        //se apeleaza AfisareSegmentDreapta3 pt parametrii afisareLinie
        //pe baza punctelor date de ea -> obiecte ale clasei CPunct
        //se apeleaza writePixel()

    }

};

//
void Display1() {
    GrilaCarteziana grila;
    grila.afisareGrila();
    //grila.afisareLinie();
    //grila.afisareLinie();
    

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
