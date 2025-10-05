#include <glut.h>
#include <stdio.h> 

// --- Definição Manual de PI ---
#ifndef M_PI
#define M_PI 3.14
#endif

// --- Protótipos de Funções Matemáticas ---
extern double cos(double x);
extern double sin(double x);

// --- Constantes e Variáveis Globais (em Português/camelCase) ---

// Configurações do Cubo (Limite da caixa)
const float TAMANHO_CUBO = 2.0f; 

// Posição, Raio e Velocidade da Bola
float bolaX = 0.0f;
float bolaY = 0.0f;
float bolaZ = 0.0f;
// Raio da Bola
const float RAIO_BOLA = 0.2f; 

// Velocidade ajustada para o novo tamanho
float velocidadeBolaX = 0.02f;
float velocidadeBolaY = 0.015f;
float velocidadeBolaZ = 0.01f;

// Cor e Material da Bola (Azul com sombreamento)
GLfloat bolaAmbiente[] = {0.0f, 0.0f, 0.2f, 1.0f};
GLfloat bolaDifusa[] = {0.0f, 0.0f, 1.0f, 1.0f}; 
GLfloat bolaEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; 
GLfloat bolaBrilho[] = {25.0f}; 

// Posição da Câmera
float distanciaCamera = 7.0f;
float anguloCameraY = 45.0f;
float anguloCameraX = 30.0f;

// --- Funções de Desenho (em Português/camelCase) ---

//Configura e desenha a bola azul com sombreamento.
void desenhaBola() {
    glPushMatrix();
    glTranslatef(bolaX, bolaY, bolaZ);

    glMaterialfv(GL_FRONT, GL_AMBIENT, bolaAmbiente);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bolaDifusa);
    glMaterialfv(GL_FRONT, GL_SPECULAR, bolaEspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bolaBrilho);

    glutSolidSphere(RAIO_BOLA, 30, 30);

    glPopMatrix();
}

/**
 * Desenha o cubo em wireframe (arestas) branco.
 */
void desenhaCuboWireframe() {
    glDisable(GL_LIGHTING); 
    glColor3f(1.0f, 1.0f, 1.0f); // Cor Branca

    // O lado do cubo é TAMANHO_CUBO * 2.0
    glutWireCube(TAMANHO_CUBO * 2.0f); 

    glEnable(GL_LIGHTING); 
}

/**
 * Função principal de desenho (callback de exibição).
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    // --- Configuração da Câmera (Visão) ---
    float camX = distanciaCamera * cos(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);
    float camY = distanciaCamera * sin(anguloCameraX * M_PI / 180.0);
    float camZ = distanciaCamera * sin(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);

    // Posiciona a câmera olhando para o centro do cubo
    gluLookAt(camX, camY, camZ, 
              0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    // --- Desenho da Cena ---
    desenhaCuboWireframe();
    desenhaBola();

    glutSwapBuffers();
}

// --- Funções de Animação e Física (em Português/camelCase) ---

/**
 * Atualiza a posição da bola e verifica colisões.
 */
void atualizaPosicaoBola(int valor) {
    // 1. Atualiza a posição da bola
    bolaX += velocidadeBolaX;
    bolaY += velocidadeBolaY;
    bolaZ += velocidadeBolaZ;

    // 2. Verifica Colisão com as Paredes do Cubo (Eixos X, Y, Z)
    float limite = TAMANHO_CUBO - RAIO_BOLA; 

    // Colisão no Eixo X
    if (bolaX > limite) {
        bolaX = limite;
        velocidadeBolaX = -velocidadeBolaX; 
    } else if (bolaX < -limite) {
        bolaX = -limite;
        velocidadeBolaX = -velocidadeBolaX;
    }

    // Colisão no Eixo Y
    if (bolaY > limite) {
        bolaY = limite;
        velocidadeBolaY = -velocidadeBolaY;
    } else if (bolaY < -limite) {
        bolaY = -limite;
        velocidadeBolaY = -velocidadeBolaY;
    }

    // Colisão no Eixo Z
    if (bolaZ > limite) {
        bolaZ = limite;
        velocidadeBolaZ = -velocidadeBolaZ;
    } else if (bolaZ < -limite) {
        bolaZ = -limite;
        velocidadeBolaZ = -velocidadeBolaZ;
    }

    // 3. Redesenha a cena
    glutPostRedisplay();

    // 4. Agenda a próxima atualização
    glutTimerFunc(16, atualizaPosicaoBola, 0); 
}

// --- Funções de Inicialização e Main (em Português/camelCase) ---

void inicializaGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 0.5f); 
    glEnable(GL_DEPTH_TEST);

    // --- Configuração de Iluminação ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Posição da luz
    GLfloat posicaoLuz[] = {TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz);

    GLfloat luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat luzDifusa[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
}

void remodela(int largura, int altura) {
    if (altura == 0) altura = 1; 
    float aspecto = (float)largura / (float)altura;

    glViewport(0, 0, largura, altura);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, aspecto, 0.1f, 100.0f); 
    glMatrixMode(GL_MODELVIEW);
}

// ----------------------------------------------------------------------
// NOTA: A função main DEVE manter o formato (int argc, char** argv)
// ----------------------------------------------------------------------
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Bola Dentro do Cubo");

    glutDisplayFunc(display);
    glutReshapeFunc(remodela);
    glutTimerFunc(16, atualizaPosicaoBola, 0); 

    inicializaGL();

    glutMainLoop();

    return 0;
}
