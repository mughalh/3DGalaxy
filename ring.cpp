#include <GL/freeglut.h>

// Particle structure
typedef struct {
    float x, y, z;
    float speedX, speedY, speedZ;
    float radius;
} particle;

// Array of particles
particle particles[1000];

void init() {
    // Initialize particle array with random positions and speeds
    for (int i = 0; i < 1000; i++) {
        particles[i].x = rand() % 800 + 400;
        particles[i].y = rand() % 600 + 300;
        particles[i].z = rand() % 600 + 300;
        particles[i].speedX = (float)rand() / RAND_MAX * 2 - 1;
        particles[i].speedY = (float)rand() / RAND_MAX * 2 - 1;
        particles[i].speedZ = (float)rand() / RAND_MAX * 2 - 1;
        particles[i].radius = 0.01 + (float)rand() / RAND_MAX * 0.5;
    }
}

void display() {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set up the camera and modelview matrices
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)800 / 600, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw a blue, transparent circle ring expanding outward and fading as it goes out
    glBegin(GL_LINE_LOOP);
        glColor4f(0.0, 0.0, 1.0, 0.5); // Blue color with alpha value for transparency
        for (int i = 0; i <= 100; i++) {
            float angle = i * PI / 50;
            glVertex3f(cos(angle) * 200 + 400, sin(angle) * 200 + 300, 0);
            if (i < 99) {
                glColor4f(0.0, 0.0, 1.0, particles[i].radius); // Use particle radius for fading effect
            }
        }
    glEnd();

    // Draw a few particles to create the illusion of movement
    glBegin(GL_POINTS);
        for (int i = 0; i < 100; i++) {
            glColor4f(1.0, 1.0, 1.0, 0.5); // White color with alpha value for transparency
            glVertex3f(particles[i].x + particles[i].speedX * 50, particles[i].y + particles[i].speedY
* 50, particles[i].z + particles[i].speedZ * 50);
        }
    glEnd();

    glutSwapBuffers();
}

void update() {
    // Update the position of each particle
    for (int i = 0; i < 1000; i++) {
        particles[i].x += particles[i].speedX;
        particles[i].y += particles[i].speedY;
        particles[i].z += particles[i].speedZ;

        if (particles[i].x > 800 || particles[i].x < 0) {
            particles[i].speedX *= -1;
        }
        if (particles[i].y > 600 || particles[i].y < 0) {
            particles[i].speedY *= -1;
        }
    }

    // Update the display
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    initGLUT();

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Glowing Circular Ring with Particles");

    glClearColor(0.2, 0.3, 0.5, 1.0); // Clear color for background

    glutDisplayFunc(display);
    glutIdleFunc(update);

    glutMainLoop();

    return 0;
}
