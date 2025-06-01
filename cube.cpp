#include <GL/glut.h>
#include <cmath>
#include <cstdlib>
#include <ctime>

const int NUM_STARS = 200;
float starPositions[NUM_STARS][3];

float angleX = 0.0f;
float angleY = 0.0f;
int lastMouseX, lastMouseY;
bool isDragging = false;

void initStars() {
    srand(time(0));
    for (int i = 0; i < NUM_STARS; ++i) {
        // Generate random direction (unit sphere)
        float theta = ((float)rand() / RAND_MAX) * 2.0f * M_PI;     // [0, 2π]
        float phi = ((float)rand() / RAND_MAX) * M_PI;              // [0, π]
        float radius = ((float)rand() / RAND_MAX) * 50.0f + 30.0f;  // Distance from center [30, 80]

        // Convert spherical coordinates to Cartesian
        float x = radius * sinf(phi) * cosf(theta);
        float y = radius * sinf(phi) * sinf(theta);
        float z = radius * cosf(phi);

        starPositions[i][0] = x;
        starPositions[i][1] = y;
        starPositions[i][2] = z;
    }
}

// Function to draw a ring
void drawRing(float radius, int segments, float angleStep) {
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < segments; ++i) {
        float theta = i * angleStep;
        glVertex3f(radius * cosf(theta), radius * sinf(theta), 0.0f);
    }
    glEnd();
}

void drawStars() {
    glPushMatrix();

    // Apply the camera rotation to the stars based on the current angles
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    glColor3f(1.0f, 1.0f, 1.0f); // White stars
    glPointSize(2.0f);
    glBegin(GL_POINTS);

    // Draw stars in 3D space
    for (int i = 0; i < NUM_STARS; ++i) {
        float speed = (starPositions[i][2] + 10.0f) / 100.0f;  // Speed based on z-depth
        glVertex3f(starPositions[i][0] * speed, starPositions[i][1] * speed, starPositions[i][2]);
    }

    glEnd();
    glPopMatrix();

    // Draw multiple rings around the sphere
    glColor3f(0.5f, 0.5f, 0.5f); // Grey ring
    float innerRadius = 30.0f;  // Inner radius of the largest ring
    float outerRadius = 60.0f;  // Outer radius of the smallest ring
    int numRings = 3;

    for (int i = 0; i < numRings; ++i) {
        int segments = 10;  // Number of segments in each ring
        float angleStep = 2.0f * M_PI / segments;
        if (i == numRings - 1) {  // Draw largest ring
            drawRing(innerRadius + i, segments, angleStep);
        } else {
            drawRing(outerRadius - i, segments, angleStep);
        }
    }

}

void drawCube() {
    glColor3f(0.9f, 0.6f, 0.6f); // Brown sphere
    glutSolidSphere(40.0f, 30);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set camera
    gluLookAt(0.0, 0.0, 10.0,   // Camera position
              0.0, 0.0, 0.0,    // Look-at point
              0.0, 1.0, 0.0);   // Up vector

    // Apply star movement and rotation
    drawStars();

    // Apply mouse-based rotation to the cube
    glPushMatrix();
    glRotatef(angleX, 1.0, 0.0, 0.0);
    glRotatef(angleY, 0.0, 1.0, 0.0);

    // Draw the cube with visible edges
    drawCube();

    glPopMatrix();

    glutSwapBuffers();
}

void update(int value) {
    angleY += 0.5f;
    if (angleY > 360) angleY -= 360;
    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

void initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.05, 1.0); // Dark background

    // Set up perspective projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 800.0 / 600.0, 1.0, 100.0);

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
        int dx = x - lastMouseX;
        int dy = y - lastMouseY;
        angleX += dy * 0.5f;
        angleY += dx * 0.5f;
        lastMouseX = x;
        lastMouseY = y;
        glutPostRedisplay();
    }
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Spinning Cube with Stars");

    initOpenGL();
    initStars();

    glutDisplayFunc(display);
    glutTimerFunc(0, update, 0);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);

    glutMainLoop();
    return 0;
}
