
#include <windows.h>  
#include <GL/glut.h>  
#include<math.h>
#define REPEAT 4
#define PI 3.14159265

double t = 0.0;
double angle = 0;
double posx = 0; double posz = 0;
int flag = 0;

static int submenu_id1;
static int submenu_id2;
static int submenu_id3;
static int menu_id;

GLfloat tetrahedron_vertex[][3] = {
     0.0f,		 0.0f,		 1.0f,
     0.0f,		 0.942809f, -0.333333f,
    -0.816497f, -0.471405f, -0.333333f,
     0.816497f, -0.471405f, -0.333333f
};

void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

}

void timer(int) {
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);

    t += 0.9;
    if (t > 180.0)
        t = t - 180.0;
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 'q' || key == 'Q') {
        angle -= 1.5;
    }
    else if (key == 'e' || key == 'E') {
        angle += 1.5;
    }

    if (key == 'w' || key == 'W') {
        posz += 2 * cos((angle) * (PI / 180));
        posx += 2 * sin((angle) * (PI / 180));
    }
    else if (key == 's' || key == 'S') {
        posz -= 2 * cos((angle) * (PI / 180));
        posx -= 2 * sin((angle) * (PI / 180));
    }

    if (key == 'd' || key == 'D') {
        posz -= 2 * sin((angle) * (PI / 180));
        posx += 2 * cos((angle) * (PI / 180));
    }
    else if (key == 'a' || key == 'A') {
        posz += 2 * sin((angle) * (PI / 180));
        posx -= 2 * cos((angle) * (PI / 180));
    }

    if (angle < 0) {
        angle = 360;
    }
    else if (angle > 360) {
        angle = 0;
    }
}

float intensity() {
    float x;
    x = sin((t) * (PI / 180));
    if (x <= 0.3) {
        return x = 0.3;
    }

    return x;
}

void normalize(GLfloat* v)
{
    GLfloat d = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] /= d;
    v[1] /= d; 
    v[2] /= d;
}

void divide_triangle(GLfloat* a, GLfloat* b, GLfloat* c, int repeat)
{
    if (repeat > 0) {
        GLfloat ab[3], ac[3], bc[3];
        for (unsigned int i = 0; i < 3; i++)
            ab[i] = a[i] + b[i];
        normalize(ab);
        for (unsigned int i = 0; i < 3; i++)
            ac[i] = a[i] + c[i];
        normalize(ac);
        for (unsigned int i = 0; i < 3; i++)
            bc[i] = b[i] + c[i];
        normalize(bc);
        divide_triangle(a, ab, ac, repeat - 1);
        divide_triangle(b, bc, ab, repeat - 1);
        divide_triangle(c, ac, bc, repeat - 1);
        divide_triangle(ab, bc, ac, repeat - 1);
    }
    else {
        glBegin(GL_POLYGON);
            GLfloat mat_emission[] = { 1, intensity() - 0.2, 0.0, 1.0f };
            GLfloat shininess2[] = { 0 };
            GLfloat no_specular[] = {0,0,0};
            glMaterialfv(GL_FRONT, GL_EMISSION, mat_emission);
            glMaterialfv(GL_FRONT, GL_SHININESS, shininess2);
            glMaterialfv(GL_FRONT, GL_SPECULAR, no_specular);
            glColorMaterial(GL_FRONT, GL_EMISSION);
            glVertex3fv(a);
            glVertex3fv(b);
            glVertex3fv(c);
        glEnd();
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    
    glLoadIdentity();

    glOrtho(-60.0f, 60.0f, -60.0f, 60.0f, 350.0f, -150.0f);

    GLfloat cam[] = { sin((angle) * (PI / 180)) * (70) , 40, cos((angle) * (PI / 180)) * (70)  };
    gluLookAt(cam[0] + posx, -cam[1], cam[2] + posz, posx, 0, posz, 0, 1, 0);

    //house
    GLfloat no_emission[] = { 0.0, 0.0, 0.0, 0.0 };
    GLfloat ambienth[] = { 0.796f, 0.255f, 0.329f, 0.0 };
    GLfloat diffuseh[] = { 0.796f, 0.255f, 0.329f, 0.0 };
    GLfloat mat_specularh[] = { 0.796f, 0.255f, 0.329f, 0.0 };
    GLfloat mat_shininess[] = { 60.0 };
    
    //roof
    GLfloat ambientr[] = { 0.9f, 0.9f, 0.9f, 1.0f };
    GLfloat diffuser[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat specularr[] = { 0.8, 0.8, 0.8, 1.0 };
    GLfloat shininessr[] = { 100.0 };

    //grass
    GLfloat ambientg[] = { 0.3f, 0.8f, 0.3f, 1.0f };
    GLfloat diffuseg[] = { 0.3f, 0.8f, 0.3f, 1.0f };
    GLfloat mat_specularg[] = { 0.3, 0.8, 0.3, 1.0 };

    glMatrixMode(GL_MODELVIEW);

    //-------------LIGHT-------------

    //sun
    glPushMatrix();

        glRotatef(t, 0.0, 0.0, -1.0);
        glTranslatef(-50, 0, 0);

        glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.2);
        glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.03);
        glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0000001);

        GLfloat ambient0[] = { intensity(), intensity(), intensity(), 1.0f };
        GLfloat diffuse0[] = { intensity() * 0.8, intensity() * 0.8, intensity() * 0.8, 1.0f };
        GLfloat specular0[] = { intensity() * 0.5, intensity() * 0.5, intensity() * 0.5, 1.0f };
        GLfloat lightpos0[] = { -50 , 0.0 , 0.0 , 1.0 };
        
        glLightfv(GL_LIGHT0, GL_POSITION, lightpos0);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse0);
        glLightfv(GL_LIGHT0, GL_AMBIENT, ambient0);
        glLightfv(GL_LIGHT0, GL_SPECULAR, specular0);

        divide_triangle(tetrahedron_vertex[0], tetrahedron_vertex[2], tetrahedron_vertex[1], REPEAT);
        divide_triangle(tetrahedron_vertex[0], tetrahedron_vertex[3], tetrahedron_vertex[2], REPEAT);
        divide_triangle(tetrahedron_vertex[0], tetrahedron_vertex[1], tetrahedron_vertex[3], REPEAT);
        divide_triangle(tetrahedron_vertex[1], tetrahedron_vertex[2], tetrahedron_vertex[3], REPEAT);

    glPopMatrix();

    //spotlight
    glPushMatrix();

        glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30);

        GLfloat ambient1[] = { 0.8f, 0.8f, 0.8f, 1.0f };
        GLfloat diffuse1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat specular1[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        GLfloat lightpos1[] = { 0.0, -15.0, -20.0, 1.0 };
        GLfloat direction1[] = { 0.0f, 3.0f, -1.0f, 0.0f };
        
        glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse1);
        glLightfv(GL_LIGHT1, GL_AMBIENT, ambient1);
        glLightfv(GL_LIGHT1, GL_SPECULAR, specular1);

        glLightfv(GL_LIGHT1, GL_POSITION, lightpos1);
        glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direction1);

    glPopMatrix();

    //-------------LISTS-------------

    //square(house)
    glNewList(1, GL_COMPILE);
        glBegin(GL_QUADS);
            glNormal3d(0, 0, -1);
            glVertex3f(5.0f, 10.0f, -10.0f);
            glVertex3f(-5.0f, 10.0f, -10.0f);
            glVertex3f(-5.0f, 0.0f, -10.0f);
            glVertex3f(5.0f, 0.0f, -10.0f);
        glEnd();
    glEndList();

    //triangle(roof)
    glNewList(2, GL_COMPILE);
        glBegin(GL_TRIANGLES);
            glNormal3d(0, 0, -1);
            glVertex3f(-5.0f, 10.0f, 20.0f);
            glVertex3f(5.0f, 10.0f, 20.0f);
            glVertex3f(0.0f, 15.0f, 20.0f);
        glEnd();
    glEndList();

    //rectangle(roof)
    glNewList(3, GL_COMPILE);
        glBegin(GL_QUADS);
            glNormal3d(-1, 1, 0);
            glVertex3f(-5.0f, 10.0f, 10.0f);
            glVertex3f(-5.0f, 10.0f, -10.0f);
            glVertex3f(0.0f, 15.0f, -10.0f);
            glVertex3f(0.0f, 15.0f, 10.0f);
        glEnd();
    glEndList();

    //grass(small)
    glNewList(4, GL_COMPILE);
        glBegin(GL_QUADS);
           glNormal3d(0, 1, 0);
           glVertex3f(-40.0f, 0.0f, 40.0f);
           glVertex3f(-40.0f, 0.0f, 32.0f);
           glVertex3f(-32.0f, 0.0f, 32.0f);
           glVertex3f(-32.0f, 0.0f, 40.0f);
        glEnd();
    glEndList();


    //-------------DRAWING-------------

    //house

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambienth);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseh);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specularh);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);

    glCallList(1);
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glCallList(1);
    glPopMatrix();
    glPushMatrix();
        glRotatef(-90.0f, 0, 1, 0);
        glTranslatef(5.0f, 0.0f, 5.0f);
        glCallList(1);
        glTranslatef(-10.0f, 0.0f, 0.0f);
        glCallList(1);
    glPopMatrix();
    glPushMatrix();
        glRotatef(90.0f, 0, 1, 0);
        glTranslatef(5.0f, 0.0f, 5.0f);
        glCallList(1);
        glTranslatef(-10.0f, 0.0f, 0.0f);
        glCallList(1);
    glPopMatrix();

    //roof

    glColorMaterial(GL_FRONT_AND_BACK, GL_SPECULAR);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientr);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuser);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specularr);
    glMaterialfv(GL_FRONT, GL_SHININESS, shininessr);

    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -30.0f);
        glCallList(2);
    glPopMatrix();
    glPushMatrix();
        glRotatef(180, 0, 1, 0);
        glTranslatef(0.0f, 0.0f, -30.0f);
        glCallList(2);
        glNormal3d(0, 0, -1);
    glPopMatrix();
    glPushMatrix();
        glCallList(3);
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-10.0f, 10.0f, 0.0f);
        glRotatef(-90.0f, 0, 0, 1);
        glCallList(3);
    glPopMatrix();

    //grass

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambientg);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseg);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specularg);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    glPushMatrix();
        if (!flag) {
            for (int i = 0; i < 10; i++) {
                glPushMatrix();
                for (int j = 0; j < 10; j++) {
                    glCallList(4);
                    glTranslatef(8, 0, 0);
                }
                glPopMatrix();
                glTranslatef(0, 0, -8);
            }
        }
        else {
            glBegin(GL_QUADS);
                glNormal3d(0, 1, 0);
                glVertex3f(-40.0f, 0.0f, 40.0f);
                glVertex3f(-40.0f, 0.0f, -40.0f);
                glVertex3f(40.0f, 0.0f, -40.0f);
                glVertex3f(40.0f, 0.0f, 40.0f);
            glEnd();
        }
    glPopMatrix();

    glutSwapBuffers();
}


void menu(int id) {
    switch (id) {
    case 0:
        exit(0);
        break;
    case 1:
        flag = 1;
        break;
    case 2:
        flag = 0;
        break;
    case 3:
        glShadeModel(GL_SMOOTH);
        break;
    case 4:
        glShadeModel(GL_FLAT);
        break;
    case 5:
        glEnable(GL_LIGHT1);
        break;
    case 6:
        glDisable(GL_LIGHT1);
        break;
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(50, 50);
    glutCreateWindow("HOUSE");
    glutKeyboardFunc(keyboard);

    //menu start
    submenu_id3 = glutCreateMenu(menu);
    glutAddMenuEntry("On", 5);
    glutAddMenuEntry("Off", 6);
    submenu_id2 = glutCreateMenu(menu);
    glutAddMenuEntry("Smooth", 3);
    glutAddMenuEntry("Flat", 4);
    submenu_id1 = glutCreateMenu(menu);
    glutAddMenuEntry("Low Poly", 1);
    glutAddMenuEntry("High Poly", 2);
    menu_id = glutCreateMenu(menu);
    glutAddSubMenu("Grass Polygons", submenu_id1);
    glutAddSubMenu("Shading", submenu_id2);
    glutAddSubMenu("Spotlight", submenu_id3);
    glutAddMenuEntry("Quit", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    //menu end

    glutDisplayFunc(display);
    glutTimerFunc(0, timer, 0);
    initGL();
    glutMainLoop();
    return 0;
}