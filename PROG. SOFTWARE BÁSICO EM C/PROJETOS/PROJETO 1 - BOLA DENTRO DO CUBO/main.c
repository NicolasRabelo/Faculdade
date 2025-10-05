#include <glut.h>
#include <stdio.h> 

// --- Defini��o Manual de PI ---
#ifndef M_PI
#define M_PI 3.14
#endif

// --- Prot�tipos de Fun��es Matem�ticas ---
extern double cos(double x);
extern double sin(double x);

// --- Constantes e Vari�veis Globais (em Portugu�s/camelCase) ---

// Configura��es do Cubo (Limite da caixa)
const float TAMANHO_CUBO = 2.0f; 

// Posi��o, Raio e Velocidade da Bola
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

// Posi��o da C�mera
float distanciaCamera = 7.0f;
float anguloCameraY = 45.0f;
float anguloCameraX = 30.0f;

// --- Fun��es de Desenho (em Portugu�s/camelCase) ---

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

    // O lado do cubo � TAMANHO_CUBO * 2.0
    glutWireCube(TAMANHO_CUBO * 2.0f); 

    glEnable(GL_LIGHTING); 
}

/**
 * Fun��o principal de desenho (callback de exibi��o).
 */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity(); 

    // --- Configura��o da C�mera (Vis�o) ---
    float camX = distanciaCamera * cos(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);
    float camY = distanciaCamera * sin(anguloCameraX * M_PI / 180.0);
    float camZ = distanciaCamera * sin(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);

    // Posiciona a c�mera olhando para o centro do cubo
    gluLookAt(camX, camY, camZ, 
              0.0, 0.0, 0.0,  
              0.0, 1.0, 0.0); 

    // --- Desenho da Cena ---
    desenhaCuboWireframe();
    desenhaBola();

    glutSwapBuffers();
}

// --- Fun��es de Anima��o e F�sica (em Portugu�s/camelCase) ---

/**
 * Atualiza a posi��o da bola e verifica colis�es.
 */
void atualizaPosicaoBola(int valor) {
    // 1. Atualiza a posi��o da bola
    bolaX += velocidadeBolaX;
    bolaY += velocidadeBolaY;
    bolaZ += velocidadeBolaZ;

    // 2. Verifica Colis�o com as Paredes do Cubo (Eixos X, Y, Z)
    float limite = TAMANHO_CUBO - RAIO_BOLA; 

    // Colis�o no Eixo X
    if (bolaX > limite) {
        bolaX = limite;
        velocidadeBolaX = -velocidadeBolaX; 
    } else if (bolaX < -limite) {
        bolaX = -limite;
        velocidadeBolaX = -velocidadeBolaX;
    }

    // Colis�o no Eixo Y
    if (bolaY > limite) {
        bolaY = limite;
        velocidadeBolaY = -velocidadeBolaY;
    } else if (bolaY < -limite) {
        bolaY = -limite;
        velocidadeBolaY = -velocidadeBolaY;
    }

    // Colis�o no Eixo Z
    if (bolaZ > limite) {
        bolaZ = limite;
        velocidadeBolaZ = -velocidadeBolaZ;
    } else if (bolaZ < -limite) {
        bolaZ = -limite;
        velocidadeBolaZ = -velocidadeBolaZ;
    }

    // 3. Redesenha a cena
    glutPostRedisplay();

    // 4. Agenda a pr�xima atualiza��o
    glutTimerFunc(16, atualizaPosicaoBola, 0); 
}

// --- Fun��es de Inicializa��o e Main (em Portugu�s/camelCase) ---

void inicializaGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 0.5f); 
    glEnable(GL_DEPTH_TEST);

    // --- Configura��o de Ilumina��o ---
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    // Posi��o da luz
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
// NOTA: A fun��o main DEVE manter o formato (int argc, char** argv)
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
