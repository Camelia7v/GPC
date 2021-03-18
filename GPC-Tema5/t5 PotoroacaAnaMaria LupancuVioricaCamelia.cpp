#define _CRT_SECURE_NO_WARNINGS

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <assert.h>
#include <float.h>
#include <GL/glut.h>


// dimensiunea ferestrei in pixeli
#define dim 300

// numarul maxim de iteratii pentru testarea apartenentei la multimea Julia-Fatou
#define iteratii_MJF 500
// modulul maxim pentru testarea apartenentei la multimea Julia-Fatou
#define apartenenta_MJF 100
// ratiile pentru multimea Julia-Fatou
#define RATIE_X_MJF 0.028
#define RATIE_Y_MJF 0.028

// numarul maxim de iteratii pentru testarea apartenentei la multimea Mandelbrot
#define iteratii_M 500
// modulul maxim pentru testarea apartenentei la multimea Mandelbrot
#define apartenenta_M 2
// ratiile pentru multimea Mandelbrot
#define RATIE_X_M 0.01
#define RATIE_Y_M 0.01


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

class CCurbaKoch
{
public:
    void segmentKoch(double lungime, int nivel, CPunct& p, CVector v)
    {
        CPunct p1;
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
        }
        else
        {
            //    v.print(stderr);
            //    fprintf(stderr, "\n");
            segmentKoch(lungime / 3.0, nivel - 1, p, v);
            p1 = v.getDest(p, lungime / 3.0);
            v.rotatie(60);
            //    v.print(stderr);
            //    fprintf(stderr, "\n");
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
            p1 = v.getDest(p1, lungime / 3.0);
            v.rotatie(-120);
            //    v.print(stderr);
            //    fprintf(stderr, "\n");
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
            p1 = v.getDest(p1, lungime / 3.0);
            v.rotatie(60);
            //    v.print(stderr);
            //    fprintf(stderr, "\n");
            segmentKoch(lungime / 3.0, nivel - 1, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v1(sqrt(3.0) / 2.0, 0.5);
        CPunct p1(-1.0, 0.0);

        CVector v2(0.0, -1.0);
        CPunct p2(0.5, sqrt(3.0) / 2.0);

        CVector v3(-sqrt(3.0) / 2.0, 0.5);
        CPunct p3(0.5, -sqrt(3.0) / 2.0);

        segmentKoch(lungime, nivel, p1, v1);
        segmentKoch(lungime, nivel, p2, v2);
        segmentKoch(lungime, nivel, p3, v3);
    }
};

class CArboreBinar
{
public:
    void arboreBinar(double lungime, int nivel, CPunct& p, CVector v)
    {
        CPunct p1;
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
        }
        else
        {
            arboreBinar(lungime, nivel - 1, p, v);
            p1 = v.getDest(p, lungime);

            v.rotatie(-45);
            arboreBinar(lungime / 2.0, nivel - 1, p1, v);

            v.rotatie(90);
            arboreBinar(lungime / 2.0, nivel - 1, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 1.0);

        arboreBinar(lungime, nivel, p, v);
    }
};

class CArborePerron
{
public:
    void arborePerron(double lungime,
        int nivel,
        double factordiviziune,
        CPunct p,
        CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;
        if (nivel == 0)
        {
        }
        else
        {
            v.rotatie(30);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            v.rotatie(-90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            v.rotatie(-30);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(90);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            p2 = p1;

            v.rotatie(30);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-90);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            arborePerron(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(0.0, -1.0);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        arborePerron(lungime, nivel, 0.4, p, v);
    }
};

class CCurbaHilbert
{
public:
    void curbaHilbert(double lungime, int nivel, CPunct& p, CVector& v, int d)
    {
        if (nivel == 0)
        {
        }
        else
        {
            v.rotatie(d * 90);
            curbaHilbert(lungime, nivel - 1, p, v, -d);

            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            v.rotatie(-d * 90);
            curbaHilbert(lungime, nivel - 1, p, v, d);

            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            curbaHilbert(lungime, nivel - 1, p, v, d);

            v.rotatie(-d * 90);
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);

            curbaHilbert(lungime, nivel - 1, p, v, -d);

            v.rotatie(d * 90);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(0.0, 0.0);

        curbaHilbert(lungime, nivel, p, v, 1);
    }
};


//clasa pentru lucrul cu numere complexe
class CComplex {
private:
    double re, im;
public:
    CComplex() : re(0.0), im(0.0) {}
    CComplex(double re1, double im1) : re(re1 * 1.0), im(im1 * 1.0) {}
    CComplex(const CComplex& c) : re(c.re), im(c.im) {}
    ~CComplex() {}

    CComplex& operator=(const CComplex& c)
    {
        re = c.re;
        im = c.im;
        return *this;
    }

    double getRe() { return re; }
    void setRe(double re1) { re = re1; }

    double getIm() { return im; }
    void setIm(double im1) { im = im1; }

    double getModul() { return sqrt(re * re + im * im); }

    int operator==(CComplex& c1)
    {
        return ((re == c1.re) && (im == c1.im));
    }

    CComplex powComplex()
    {
        CComplex rez;
        rez.re = powl(re * 1.0, 2) - powl(im * 1.0, 2);
        rez.im = 2.0 * re * im;
        return rez;
    }

    void print(FILE* f)
    {
        fprintf(f, "%.20f%+.20f i", re, im);
    }

    friend CComplex operator+(const CComplex& c1, const CComplex& c2)
    {
        CComplex rez(c1.re + c2.re, c1.im + c2.im);
        return rez;
    }

    friend CComplex operator*(CComplex& c1, CComplex& c2)
    {
        CComplex rez(c1.re * c2.re - c1.im * c2.im,
            c1.re * c2.im + c1.im * c2.re);
        return rez;
    }
};


// afisare curba lui Koch "fulg de zapada"
void Display1() {
    CCurbaKoch cck;
    cck.afisare(sqrt(3.0), nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'h');

    nivel++;
}

// afisare arbore binar
void Display2() {
    CArboreBinar cab;
    cab.afisare(1, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');

    nivel++;
}

// afisare arborele lui Perron
void Display3() {
    CArborePerron cap;

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, -0.5, 0.0);
    cap.afisare(1, nivel);
    glPopMatrix();
    nivel++;
}

// afisare curba lui Hilbert
void Display4() {
    CCurbaHilbert cch;
    cch.afisare(0.05, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'c');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');

    nivel++;
}


class MJuliaFatou {
private:
    struct SDate {
        CComplex c;
        int nr_iteratii;
        double modmax; // modulul maxim
    } m;

public:
    MJuliaFatou()
    {
        // m.c = 0+0i by default
        m.nr_iteratii = iteratii_MJF;
        m.modmax = apartenenta_MJF;
    }

    MJuliaFatou(CComplex& c)
    {
        m.c = c;
        m.nr_iteratii = iteratii_MJF;
        m.modmax = apartenenta_MJF;
    }

    ~MJuliaFatou() {}

    void set_modmax(double mod) { if (mod <= apartenenta_MJF); m.modmax = mod; }
    double get_modmax() { return m.modmax; }

    void set_nr_iteratii(int iteratii) { if (iteratii <= iteratii_MJF); m.nr_iteratii = iteratii; }
    int get_nr_iteratii() { return m.nr_iteratii; }

    // testeaza daca un punct apartine multimii Julia-Fatou
    // si returneaza 0 daca acesta apartine, -1 daca converge finit, 1 daca converge infinit
    int esteInMultimeaJuliaFatou(CComplex& x)
    {
        int flag = 0;
        CComplex z0, zn_1;
        z0 = x;

        for (int i = 1; i < m.nr_iteratii; i++)
        {
            zn_1 = z0 * z0 + m.c;

            // punctul nu apartine multimii Julia-Fatou
            if (zn_1 == z0)
            {
                flag = -1;
                break;
            }
            else if (zn_1.getModul() > m.modmax)
            {
                flag = 1;
                break;
            }

            z0 = zn_1;
        }
        return flag;
    }

    void display(double xmin, double ymin, double xmax, double ymax)
    {
        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += RATIE_X_MJF) {
            for (double y = ymin; y <= ymax; y += RATIE_Y_MJF)
            {
                CComplex punct_curent(x, y);
                int flag = esteInMultimeaJuliaFatou(punct_curent);

                // punctul apartine multimii Julia-Fatou
                if (flag == 0) {
                    glColor3f(1.0, 0.1, 0.1); //rosu
                    glVertex3d(x, y, 0);
                }
            }
        }
        glEnd();
    }
};

// multimea Julia-Fatou 1
void Display5() {
    // numarul c de la care se pleaca
    CComplex c(-0.12375, 0.056805);
    MJuliaFatou mjf(c);

    glColor3f(1.0, 0.1, 0.1); // rosu
    mjf.set_nr_iteratii(30);
    // se apeleaza display cu valorile lui c1 si c2, date
    mjf.display(-0.8, -0.4, 0.8, 0.4);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'J');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '-');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
}

// multimea Julia-Fatou 2
void Display6() {
    //numarul c de la care se pleaca
    CComplex c(-0.012, 0.74);
    MJuliaFatou mjf(c);

    glColor3f(1.0, 0.1, 0.1); // rosu
    mjf.set_nr_iteratii(30);
    mjf.set_modmax(2);
    // se apeleaza display cu valorile lui c, date
    mjf.display(-1, -1, 1, 1);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'J');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '-');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
}


class MMandelbrot {
private:
    struct SDate {
        CComplex c;
        int nr_iteratii;
        double modmax; // modulul maxim
    } m;

public:
    MMandelbrot()
    {
        // m.c = 0+0i by default
        m.nr_iteratii = iteratii_M;
        m.modmax = apartenenta_M;
    }

    MMandelbrot(CComplex& c)
    {
        m.c = c;
        m.nr_iteratii = iteratii_M;
        m.modmax = apartenenta_M;
    }

    ~MMandelbrot() {}

    void set_modmax(double mod) { if (mod <= apartenenta_M); m.modmax = mod; }
    double get_modmax() { return m.modmax; }

    void set_nr_iteratii(int iteratii) { if (iteratii <= iteratii_M); m.nr_iteratii = iteratii; }
    int get_nr_iteratii() { return m.nr_iteratii; }

    // testeaza daca un punct apartine multimii Mandelbrot
    // si returneaza 0 daca acesta apartine, i daca acesta nu apartine
    int esteInMultimeaMandelbrot(CComplex& c) {
        int flag = 0;
        CComplex z0(0, 0);
        CComplex zn_1;

        for (int i = 1; i < m.nr_iteratii; i++)
        {
            zn_1 = z0 * z0 + c;

            // punctul nu apartine multimii Mandelbrot
            if (zn_1.getModul() > m.modmax)
            {
                flag = i;
                break;
            }

            z0 = zn_1;
        }

        return flag;

    }

    void display(double xmin, double ymin, double xmax, double ymax)
    {
        glBegin(GL_POINTS);
        for (double x = xmin; x <= xmax; x += RATIE_X_M) {
            for (double y = ymin; y <= ymax; y += RATIE_Y_M)
            {
                CComplex punct_curent(x, y);
                int flag = esteInMultimeaMandelbrot(punct_curent);

                // punctul apartine multimii Mandelbrot
                if (flag == 0) { 
                    glColor3f(1.0, 0.1, 0.1); //rosu
                    glVertex3d(x/2, y/2, 0);
                }
                else { // punctul nu apartine multimii Mandelbrot
                    if (flag < 5) {
                        glColor3f(0.5f, 0.5f, 0.5f); // violet
                        glVertex3d(x / 2, y / 2, 0);
                    }
                    if (flag >= 5 && flag < 10) {
                        glColor3f(2.0f, 0.5f, 1.0f); // liliac
                        glVertex3d(x / 2, y / 2, 0);
                    }
                    if (flag >= 10 && flag < 15) {
                        glColor3f(0.0f, 0.5f, 1.0f); // gri
                        glVertex3d(x / 2, y / 2, 0);
                    }
                    if (flag >= 15) {
                        glColor4f(0.0f, 1.0f, 1.0f, 1.0f); // albastru
                        glVertex3d(x / 2, y / 2, 0);
                    }
                }

            }
        }
        glEnd();
    }
};

// multimea Mandelbrot
void Display7() {
    MMandelbrot mm;

    glColor3f(1.0, 0.1, 0.1);
    mm.set_nr_iteratii(30);
    mm.set_modmax(2);
    mm.display(-2, -2, 2, 2);

    glRasterPos2d(-0.9, 0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'u');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'd');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'b');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'r');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'o');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 't');
}


class CImaginea1 {
public:
    void imaginea1(CPunct p, double lungime, int nivel_l)
    {
        double cx, cy;
        p.getxy(cx, cy);
  
        if (nivel_l == 0)
        { 
            // patratul din mijloc

            double lm = lungime / 3;
            CVector cv(0, 1);
            CPunct cp(cx - lm / 2, cy - lm / 2);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);
        }
        else
        {
            // patratul din mijloc

            double lm = lungime / 3;
            CVector cv(0, 1);
            CPunct cp(cx - lm / 2, cy - lm / 2);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);

            cv.rotatie(-90);
            cv.deseneaza(cp, lm);
            cp = cv.getDest(cp, lm);


            // patratele mici

            CPunct stanga_sus(cx - lm, cy + lm);
            imaginea1(stanga_sus, lm, nivel_l - 1);

            CPunct centru_sus(cx, cy + lm);
            imaginea1(centru_sus, lm, nivel_l - 1);

            CPunct dreapta_sus(cx + lm, cy + lm);
            imaginea1(dreapta_sus, lm, nivel_l - 1);

            CPunct stanga(cx - lm, cy);
            imaginea1(stanga, lm, nivel_l - 1);

            CPunct dreapta(cx + lm, cy);
            imaginea1(dreapta, lm, nivel_l - 1);
            CPunct stanga_jos(cx - lm, cy - lm);
            imaginea1(stanga_jos, lm, nivel_l - 1);

            CPunct centru_jos(cx, cy - lm);
            imaginea1(centru_jos, lm, nivel_l - 1);

            CPunct dreapta_jos(cx + lm, cy - lm);
            imaginea1(dreapta_jos, lm, nivel_l - 1);
        }
    }

    void desenarePatratExterior(CPunct p, double lungime, int nivel_l) 
    {
        // patratul exterior

        double cx, cy;
        p.getxy(cx, cy);
        double lm = lungime;
        CVector cv(0, 1);
        CPunct cp(cx - lm / 2, cy - lm / 2);
        cv.deseneaza(cp, lm);
        cp = cv.getDest(cp, lm);

        cv.rotatie(-90);
        cv.deseneaza(cp, lm);
        cp = cv.getDest(cp, lm);

        cv.rotatie(-90);
        cv.deseneaza(cp, lm);
        cp = cv.getDest(cp, lm);

        cv.rotatie(-90);
        cv.deseneaza(cp, lm);
        cp = cv.getDest(cp, lm);
    }

    void afisare(double lungime, int nivel_l)
    {
        CPunct p(0.0, 0.0);
        imaginea1(p, lungime, nivel_l);
        desenarePatratExterior(p, lungime, nivel_l);
    }
};

void Display8() {
    CImaginea1 img1;
    img1.afisare(1, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '1');

    nivel++;
}


class CImagine2
{
public:

    void imagine2(double lungime,int nivel,double factordiviziune, CPunct p, CVector v)
    {
        assert(factordiviziune != 0);
        CPunct p1, p2;

        if (nivel == 0)
        {
        }
        else
        {
            v.rotatie(-50);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            v.rotatie(90);
            v.deseneaza(p, lungime);
            p1 = v.getDest(p, lungime);
            p2 = p1;

            v.rotatie(30);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            imagine2(lungime * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(-70);
            v.deseneaza(p1, lungime);
            p1 = v.getDest(p1, lungime);
            p2 = p1;

            double l = lungime / 1.3;
            v.rotatie(-90);
            v.deseneaza(p1, l);
            p1 = v.getDest(p1, l);
            imagine2(l * factordiviziune, nivel - 1, factordiviziune, p1, v);

            p1 = p2;
            v.rotatie(120);
            v.deseneaza(p1, l);
            p1 = v.getDest(p1, l);
            imagine2(l * factordiviziune, nivel - 1, factordiviziune, p1, v);
        }
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, -1.0);
        CPunct p(0.0, 2.5);

        v.deseneaza(p, 0.25);
        p = v.getDest(p, 0.25);
        imagine2(lungime, nivel, 0.4, p, v);
    }
};

void Display9() {
    CImagine2 img2;

    glPushMatrix();
    glLoadIdentity();
    glScaled(0.4, 0.4, 1);
    glTranslated(-0.5, -0.5, 0.0);
    img2.afisare(1, nivel);
    glPopMatrix();

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '2');

    nivel++;
}


class CImaginea3
{
public:
    void imagine3(double lungime, int nivel, CPunct& p, CVector& v, int d)
    {
        if (nivel == 0)
        {
            v.deseneaza(p, lungime);
            p = v.getDest(p, lungime);
            return;
        }
        imagine3(lungime / 3, nivel - 1, p, v, -d);

        v.rotatie(d * 60);
        imagine3(lungime / 3, nivel - 1, p, v, d);

        v.rotatie(d * 60);
        imagine3(lungime / 3, nivel - 1, p, v, -d);
    }

    void afisare(double lungime, int nivel)
    {
        CVector v(0.0, 1.0);
        CPunct p(0.0, -0.5);

        imagine3(lungime * pow(1.5, nivel), nivel, p, v, 1);
    }
};

void Display10() {
    CImaginea3 img3;
    img3.afisare(1, nivel);

    char c[3];
    sprintf(c, "%2d", nivel);
    glRasterPos2d(-0.98, -0.98);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'v');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'l');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '=');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[0]);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, c[1]);

    glRasterPos2d(-1.0, -0.9);
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'm');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'g');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'i');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'n');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'e');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'a');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '3');

    nivel++;
}

void Init(void) {

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glLineWidth(1);

    glPointSize(3);

    glPolygonMode(GL_FRONT, GL_LINE);
}

void Display(void)
{
    switch (prevKey)
    {
    case '0':
        glClear(GL_COLOR_BUFFER_BIT);
        nivel = 0;
        fprintf(stderr, "nivel = %d\n", nivel);
        break;
    case '1':
        glClear(GL_COLOR_BUFFER_BIT);
        Display1();
        break;
    case '2':
        glClear(GL_COLOR_BUFFER_BIT);
        Display2();
        break;
    case '3':
        glClear(GL_COLOR_BUFFER_BIT);
        Display3();
        break;
    case '4':
        glClear(GL_COLOR_BUFFER_BIT);
        Display4();
        break;
    case '5':
        glClear(GL_COLOR_BUFFER_BIT);
        Display5();
        break;
    case '6':
        glClear(GL_COLOR_BUFFER_BIT);
        Display6();
        break;
    case '7':
        glClear(GL_COLOR_BUFFER_BIT);
        Display7();
        break;
    case '8':
        glClear(GL_COLOR_BUFFER_BIT);
        Display8();
        break;
    case '9':
        glClear(GL_COLOR_BUFFER_BIT);
        Display9();
        break;
    case '-':
        glClear(GL_COLOR_BUFFER_BIT);
        Display10();
        break;
    default:
        break;
    }

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
