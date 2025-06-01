#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int NUM_STARS = 500;  // Increased number of stars
float starPositions[NUM_STARS][3];

float angleX = 0.0f;
float angleY = 0.0f;
int lastMouseX, lastMouseY;
bool isDragging = false;
bool autoRotate = false;  // Added control for auto-rotation

void initStars() {
    srand(time(0));
    for (int i = 0; i < NUM_STARS; ++i) {
        // Generate random direction (unit sphere)
        float theta = ((float)rand() / RAND_MAX) * 2.0f * M_PI;
        float phi = acosf(2.0f * ((float)rand() / RAND_MAX) - 1.0f);
        float radius = ((float)rand() / RAND_MAX) * 50.0f + 30.0f;

        // Convert spherical to Cartesian
        starPositions[i][0] = radius * sinf(phi) * cosf(theta);
        starPositions[i][1] = radius * sinf(phi) * sinf(theta);
        starPositions[i][2] = radius * cosf(phi);
    }
}

void drawRing(float radius, int segments) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float theta = 2.0f * M_PI * i / segments;
        glVertex3f(radius * cosf(theta), radius * sinf(theta), 0.0f);
    }
    glEnd();
}

void drawStars() {
    glPushMatrix();
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    // Draw stars with varying sizes
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < NUM_STARS; ++i) {
        float speed = 0.5f + (starPositions[i][2] + 50.0f) / 100.0f;
        glVertex3f(starPositions[i][0] * speed,
                   starPositions[i][1] * speed,
                   starPositions[i][2]);

        // Make some stars brighter
        if (i % 20 == 0) {
            glColor3f(1.0f, 1.0f, 0.8f);
            glPointSize(2.5f);
            glVertex3f(starPositions[i][0] * speed * 1.1,
                       starPositions[i][1] * speed * 1.1,
                       starPositions[i][2]);
            glColor3f(1.0f, 1.0f, 1.0f);
            glPointSize(2.0f);
        }
    }
    glEnd();

    // Draw rings
    glColor3f(0.6f, 0.6f, 0.8f);
    glLineWidth(1.5f);
    for (int i = 0; i < 3; ++i) {
        drawRing(40.0f + i * 10.0f, 50 + i * 10);
    }
    glPopMatrix();
}

void drawPlanet() {
    // Draw main planet with improved lighting
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_position[] = { 5.0, 5.0, 5.0, 1.0 };
    GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glColor3f(0.7f, 0.5f, 0.3f);
    glutSolidSphere(3.0f, 32, 32);

    glDisable(GL_LIGHTING);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera position
    gluLookAt(0.0, 0.0, 15.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    // Draw background stars
    drawStars();

    // Apply rotation to the planet
    glPushMatrix();
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);
    drawPlanet();
    glPopMatrix();

    glutSwapBuffers();
}

void update(int value) {
    if (autoRotate) {
        angleY += 0.5f;
        if (angleY > 360) angleY -= 360;
    }
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Pure black background

    // Enable point smoothing for better looking stars
    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    // Set up perspective
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45.0, 800.0 / 600.0, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouseButton(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        isDragging = (state == GLUT_DOWN);
        lastMouseX = x;
        lastMouseY = y;
    }
}

void mouseMotion(int x, int y) {
    if (isDragging) {
        angleX += (y - lastMouseY) * 0.5f;
        angleY += (x - lastMouseX) * 0.5f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (key == ' ') {
        autoRotate = !autoRotate;  // Toggle auto-rotation with spacebar
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Space Scene");

    initOpenGL();
    initStars();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);  // Added keyboard input

    glutMainLoop();
    return 0;
}
