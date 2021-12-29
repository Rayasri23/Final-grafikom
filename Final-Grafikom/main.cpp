#include <cstdlib>
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

 //memberikan fileheader yang dibutuhkan
GLuint texture[35];

GLint slices = 35;
GLint stacks = 35;

using namespace std;
static float ypos = 0, zpos = 0, xpos = 0,a = -9, b = -5,c =-30, lKanan=0, lKiri=0;
int z=0;

struct Image {
        unsigned long sizeX;
        unsigned long sizeY;
        char *data;
};
typedef struct Image Image; //struktur data untuk
//ukuran image #bisa di set sesuai kebutuhan
#define checkImageWidth 164
#define checkImageHeight 164

GLubyte checkImage[checkImageWidth][checkImageHeight][3];
//mengambil gambar BMP
int ImageLoad(char *filename, Image *image) {
        FILE *file;
        unsigned long size; // ukuran image dalam bytes
        unsigned long i; // standard counter.
        unsigned short int plane; // number of planes in image

        unsigned short int bpp; // jumlah bits per pixel
        char temp; // temporary color storage for var warna sementara untuk memastikan filenya ada


        if ((file = fopen(filename, "rb")) == NULL) {
                printf("File Not Found : %s\n", filename);
                return 0;
        }
        // mencari file header bmp
        fseek(file, 18, SEEK_CUR);
        // read the width
        if ((i = fread(&image->sizeX, 4, 1, file)) != 1) {
                printf("Error reading width from %s.\n", filename);
                return 0;
        }
        //printf("Width of %s: %lu\n", filename, image->sizeX);
        // membaca nilai height
        if ((i = fread(&image->sizeY, 4, 1, file)) != 1) {
                printf("Error reading height from %s.\n", filename);
                return 0;
        }
        //printf("Height of %s: %lu\n", filename, image->sizeY);
        //menghitung ukuran image(asumsi 24 bits or 3 bytes per pixel).

        size = image->sizeX * image->sizeY * 3;
        // read the planes
        if ((fread(&plane, 2, 1, file)) != 1) {
                printf("Error reading planes from %s.\n", filename);
                return 0;
        }
        if (plane != 1) {
                printf("Planes from %s is not 1: %u\n", filename, plane);
                return 0;
        }
        // read the bitsperpixel
        if ((i = fread(&bpp, 2, 1, file)) != 1) {
                printf("Error reading bpp from %s.\n", filename);

                return 0;
        }
        if (bpp != 24) {
                printf("Bpp from %s is not 24: %u\n", filename, bpp);
                return 0;
        }
        // seek past the rest of the bitmap header.
        fseek(file, 24, SEEK_CUR);
        // read the data.
        image->data = (char *) malloc(size);
        if (image->data == NULL) {
                printf("Error allocating memory for color-corrected image data");
                return 0;
        }
        if ((i = fread(image->data, size, 1, file)) != 1) {
                printf("Error reading image data from %s.\n", filename);
                return 0;
        }
        for (i = 0; i < size; i += 3) { // membalikan semuan nilai warna (gbr - > rgb)
                temp = image->data[i];
                image->data[i] = image->data[i + 2];
                image->data[i + 2] = temp;
        }
        // we're done.
        return 1;
}
//mengambil tekstur
void init(void)
{
    glClearColor(0.5, 0.5, 0.5, 0.0);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        // Generate texture/ membuat texture background
}

//fungsi kotak kosong, belum diisi koordinat. diisi di display()
void kotak(float x1,float y1,float z1,float x2,float y2,float z2)
{
        //depan
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x1,y1,z1);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x2,y1,z1);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x2,y2,z1);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x1,y2,z1);

        //atas
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x1,y2,z1);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x2,y2,z1);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x2,y2,z2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x1,y2,z2);
        //belakang
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x1,y2,z2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x2,y2,z2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x2,y1,z2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x1,y1,z2);

        //bawah
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x1,y1,z2);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x2,y1,z2);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x2,y1,z1);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x1,y1,z1);

        //samping kiri
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x1,y1,z1);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x1,y2,z1);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x1,y2,z2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x1,y1,z2);

        //samping kanan
        glTexCoord2f(0.0, 0.0);
        glVertex3f(x2,y1,z1);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(x2,y2,z1);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(x2,y2,z2);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(x2,y1,z2);
}


void keyboard(unsigned char key, int x, int y)
{
        switch (key)
        {
                //putar arah jarum jam (menambah derajat putaran)
                case 'z':
                ypos=ypos+5;
                if (ypos>360) ypos=0;
                glutPostRedisplay();
                break;
                //putar berlawanan arah jarum jam (mengurangi derajat putaran)
                case 'x':
                ypos=ypos-5;
                if (ypos>360) ypos=0;
                glutPostRedisplay();
                break;
                //vertikal bawah (menambah koordinat y)
                case 's':
                b = b + 1;
                glutPostRedisplay();
                break;
                //vertikal atas (mengurangi koordinat y)
                case 'w':
                b = b - 1;
                glutPostRedisplay();
                break;
                //horisontal kiri (mengurangi koordinat x)
                case 'a':
                a = a + 1;
                glutPostRedisplay();
                break;
                //horisontal kanan (menambah koordinat x)
                case 'd':
                a = a - 1;
                glutPostRedisplay();
                break;
                //memperbesar objek (menambah koordinat z)
                case 'q':
                c = c + 1;
                glutPostRedisplay();
                break;
                //memperkecil abjek(mengurangi koordinat z)
                case 'e':
                c = c - 1;
                glutPostRedisplay();
                break;


                //putar arah jarum jam (menambah derajat putaran)
                case '[':
                xpos=xpos+5;
                if (xpos>360) xpos=0;
                glutPostRedisplay();
                break;
                //putar berlawanan arah jarum jam (mengurangi derajat putaran)
                case ']':
                xpos=xpos-5;
                if (xpos>360) xpos=0;
                glutPostRedisplay();

        }
}
void display()
{
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity ();
        glEnable(GL_DEPTH_TEST);
        //perpindahan
        glTranslatef(a,b,c);
        //putaran
        glRotatef(xpos,1,0,0);
        glRotatef(ypos,0,1,0);
        glRotatef(zpos,0,0,1);

        //lemari pakaian
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[2]);
                glBegin(GL_QUADS);
                    glColor3f(1.0f,0.0f,0.0f);
                        kotak(2,0.6,3.5,9.8,8.5,0.5);
                        //kanan
                        kotak(9.8,0.5,3.7,10,8.7,0.5);
                        //kiri
                        kotak(1.8,0.5,3.7,2,8.7,0.5);
                        //gagang pintu kiri
                        glColor3f(1.1f,1.0f,1.0f);
                        kotak(7.3,4.5,3.9,7.5,5.7,3.7);
                        //gagang pintu kanan
                        kotak(4.3,4.5,3.9,4.5,5.7,3.7);
                glEnd();
        glPopMatrix();
    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, texture[3]);
                glBegin(GL_QUADS);
                glColor3f(0.2f,0.1f,0.1f);
                        //pintu kiri
                        kotak(6.2,0.9,3.7,9.7,8.3,3.5);
                        //pintu kanan
                        kotak(2.1,0.9,3.7,5.6,8.3,3.5);
                glEnd();
        glPopMatrix();


        glFlush();
        glutSwapBuffers();
}

void reshape (int w, int h)
{

    glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60.0, 1.0 * (GLfloat) w / (GLfloat) h, 1.0, 40.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0, 0.0, 0.0);
        gluLookAt (0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize (1024, 600);
    glutInitWindowPosition (20, 75);
    glutCreateWindow ("Lemari");

    init ();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
return 0;
}
