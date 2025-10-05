#include <glut.h>
#include <stdio.h> 

// --- Definição Manual de PI ---
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// --- Protótipos de Funções Matemáticas ---
extern double cos(double x);
extern double sin(double x);

// --- Constantes e Variáveis Globais ---

// Configurações do Cubo (Limite da caixa). Reduzido para 2.0.
const float CUBE_SIZE = 2.0f; 

// Posição, Raio e Velocidade da Bola
float ballX = 0.0f;
float ballY = 0.0f;
float ballZ = 0.0f;
// Raio da Bola. Reduzido para 0.2.
const float BALL_RADIUS = 0.2f; 

// Velocidade ajustada para o novo tamanho
float ballSpeedX = 0.02f;
float ballSpeedY = 0.015f;
float ballSpeedZ = 0.01f;

// Cor e Material da Bola (Azul com sombreamento)
GLfloat ball_ambient[] = {0.0f, 0.0f, 0.2f, 1.0f};
GLfloat ball_diffuse[] = {0.0f, 0.0f, 1.0f, 1.0f}; 
GLfloat ball_specular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat ball_shininess[] = {50.0f}; 

// Posição da Câmera. Reduzida para enquadrar melhor o cubo.
float camera_distance = 7.0f;
float camera_angle_y = 45.0f;
float camera_angle_x = 30.0f;

// --- Funções de Desenho ---

/**
 * Configura e desenha a bola azul com sombreamento.
 */
void drawBall() {
    glPushMatrix();
    glTranslatef(ballX, ballY, ballZ);

    glMaterialfv(GL_FRONT, GL_AMBIENT, ball_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, ball_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, ball_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, ball_shininess);

    glutSolidSphere(BALL_RADIUS, 30, 30);

    glPopMatrix();
}

/**
 * Desenha o cubo em wireframe (arestas) branco.
 */
void drawCubeWireframe() {
    glDisable(GL_LIGHTING); 
    glColor3f(1.0f, 1.0f, 1.0f); // Cor Branca

    // O lado do cubo é CUBE_SIZE * 2.0 (4.0)
    glutWireCube(CUBE_SIZE * 2.0f); 

    glEnable(GL_LIGHTING); 
}

/**
 * Função principal de desenho (callback de exibição).
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    // --- Configuração da Câmera (Visão) ---
    float camX = camera_distance * cos(camera_angle_y * M_PI / 180.0) * cos(camera_angle_x * M_PI / 180.0);
    float camY = camera_distance * sin(camera_angle_x * M_PI / 180.0);
    float camZ = camera_distance * sin(camera_angle_y * M_PI / 180.0) * cos(camera_angle_x * M_PI / 180.0);

    // Posiciona a câmera olhando para o centro do cubo
    gluLookAt(camX, camY, camZ, 
              0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    // --- Desenho da Cena ---
    drawCubeWireframe();
    drawBall();

    glutSwapBuffers();
}

// --- Funções de Animação e Física ---

/**
 * Atualiza a posição da bola e verifica colisões.
 */
void updateBallPosition(int value) {
    // 1. Atualiza a posição da bola
    ballX += ballSpeedX;
    ballY += ballSpeedY;
    ballZ += ballSpeedZ;

    // 2. Verifica Colisão com as Paredes do Cubo (Eixos X, Y, Z)
    float limit = CUBE_SIZE - BALL_RADIUS; 

    // Colisão no Eixo X
    if (ballX > limit) {
        ballX = limit;
        ballSpeedX = -ballSpeedX; 
    } else if (ballX < -limit) {
        ballX = -limit;
        ballSpeedX = -ballSpeedX;
    }

    // Colisão no Eixo Y
    if (ballY > limit) {
        ballY = limit;
        ballSpeedY = -ballSpeedY;
    } else if (ballY < -limit) {
        ballY = -limit;
        ballSpeedY = -ballSpeedY;
    }

    // Colisão no Eixo Z
    if (ballZ > limit) {
        ballZ = limit;
        ballSpeedZ = -ballSpeedZ;
    } else if (ballZ < -limit) {
        ballZ = -limit;
        ballSpeedZ = -ballSpeedZ;
    }

    // 3. Redesenha a cena
    glutPostRedisplay();

    // 4. Agenda a próxima atualização
    glutTimerFunc(16, updateBallPosition, 0); 
}

// --- Funções de Inicialização e Main ---

void initGL() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f); 
    glEnable(GL_DEPTH_TEST);

    // --- Configuração de Iluminação ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Posição da luz
    GLfloat light_position[] = {CUBE_SIZE * 1.5f, CUBE_SIZE * 1.5f, CUBE_SIZE * 1.5f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    GLfloat light_ambient[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat light_diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
}

void reshape(int w, int h) {
    if (h == 0) h = 1; 
    float aspect = (float)w / (float)h;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    // Perspectiva de 45 graus funciona bem para este tamanho de cubo
    gluPerspective(45.0f, aspect, 0.1f, 100.0f); 
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bola Quicando em Cubo Wireframe (C/GLUT) - Enquadrado");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(16, updateBallPosition, 0); 

    initGL();

    glutMainLoop();

    return 0;
}
