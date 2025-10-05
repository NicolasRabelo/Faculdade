#include <glut.h> // Inclui a biblioteca GLUT para gr�ficos e janelas
#include <stdio.h> // Inclui a biblioteca padr�o de I/O em C

// --- Configura��es de Matem�tica ---
#ifndef M_PI
#define M_PI 3.1415 // Define PI (valor corrigido para maior precis�o)
#endif

// Declara��es das fun��es trigonom�tricas da biblioteca matem�tica
extern double cos(double x); 
extern double sin(double x); 

// --- Constantes e Vari�veis Globais de Cen�rio ---

// Configura��o do Cubo (palco)
const float TAMANHO_CUBO = 2.0f; // Define o limite espacial (metade do lado)

// Posi��o e Raio da Bola
float bolaX = 0.0f; // Coordenada X
float bolaY = 0.0f; // Coordenada Y
float bolaZ = 0.0f; // Coordenada Z
const float RAIO_BOLA = 0.2f; // Tamanho da esfera

// Velocidade da Bola (determinando a taxa de movimento)
float velocidadeBolaX = 0.02f; 
float velocidadeBolaY = 0.015f; 
float velocidadeBolaZ = 0.01f; 

// --- Vari�veis de Material da Bola (Cor e Brilho) ---
GLfloat bolaAmbiente[] = {0.0f, 0.0f, 0.2f, 1.0f}; // Cor nas �reas escuras
GLfloat bolaDifusa[] = {0.0f, 0.0f, 1.0f, 1.0f}; // Cor principal (Azul)
GLfloat bolaEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor do reflexo de luz
GLfloat bolaBrilho[] = {25.0f}; // Intensidade do reflexo

// --- Vari�veis de Configura��o da C�mera (Ponto de Vista) ---
float distanciaCamera = 7.0f; // Dist�ncia da c�mera ao centro da cena
float anguloCameraY = 45.0f; // Rota��o horizontal (esquerda/direita)
float anguloCameraX = 30.0f; // Inclina��o vertical (cima/baixo)

// --- Bloco de Fun��es de Desenho ---

void desenhaBola() {
    glPushMatrix(); // Salva o estado atual da matriz de transforma��o
    glTranslatef(bolaX, bolaY, bolaZ); // Move o objeto para a posi��o da bola

    // Aplica as propriedades de material (cores e brilho) da bola
    glMaterialfv(GL_FRONT, GL_AMBIENT, bolaAmbiente); 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bolaDifusa); 
    glMaterialfv(GL_FRONT, GL_SPECULAR, bolaEspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bolaBrilho);

    glutSolidSphere(RAIO_BOLA, 30, 30); // Desenha a esfera s�lida

    glPopMatrix(); // Restaura o estado anterior da matriz
}

void desenhaCuboWireframe() {
    glDisable(GL_LIGHTING); // Desliga a ilumina��o para desenhar linhas puras
    glColor3f(1.0f, 1.0f, 1.0f); // Define a cor das linhas como Branca

    glutWireCube(TAMANHO_CUBO * 2.0f); // Desenha o cubo apenas com suas arestas

    glEnable(GL_LIGHTING); // Liga a ilumina��o de volta para a bola
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o fundo e a profundidade da tela
    glLoadIdentity(); // Reseta a matriz de vis�o

    // --- Bloco de C�lculo e Posicionamento da C�mera ---
    // C�lculo trigonom�trico das coordenadas da c�mera com base nos �ngulos
    float camX = distanciaCamera * cos(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);
    float camY = distanciaCamera * sin(anguloCameraX * M_PI / 180.0);
    float camZ = distanciaCamera * sin(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);

    // Posiciona a c�mera no espa�o (onde, para onde olha, qual o topo)
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

    // --- Bloco de Desenho da Cena ---
    desenhaCuboWireframe(); // Desenha o palco
    desenhaBola(); // Desenha o ator

    glutSwapBuffers(); // Exibe o quadro desenhado para a anima��o
}

// --- Bloco de Anima��o e L�gica de F�sica ---

void atualizaPosicaoBola(int valor) {
    // 1. Atualiza a posi��o
    bolaX += velocidadeBolaX; 
    bolaY += velocidadeBolaY; 
    bolaZ += velocidadeBolaZ; 

    // 2. Verifica Colis�o
    float limite = TAMANHO_CUBO - RAIO_BOLA; // Calcula o ponto de colis�o

    // L�gica de Colis�o nos 3 Eixos: se ultrapassar, reposiciona e inverte a velocidade
    if (bolaX > limite) { bolaX = limite; velocidadeBolaX = -velocidadeBolaX; } 
    else if (bolaX < -limite) { bolaX = -limite; velocidadeBolaX = -velocidadeBolaX; }

    if (bolaY > limite) { bolaY = limite; velocidadeBolaY = -velocidadeBolaY; } 
    else if (bolaY < -limite) { bolaY = -limite; velocidadeBolaY = -velocidadeBolaY; }

    if (bolaZ > limite) { bolaZ = limite; velocidadeBolaZ = -velocidadeBolaZ; } 
    else if (bolaZ < -limite) { bolaZ = -limite; velocidadeBolaZ = -velocidadeBolaZ; }

    // 3. Agenda e Redesenha
    glutPostRedisplay(); // Solicita um novo desenho (chama display())
    glutTimerFunc(16, atualizaPosicaoBola, 0); // Agenda a pr�xima atualiza��o (aprox. 60 FPS)
}

// --- Bloco de Inicializa��o OpenGL e Ilumina��o ---

void inicializaGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 0.5f); // Define a cor de fundo (Cinza Escuro)
    glEnable(GL_DEPTH_TEST); // Habilita o z-buffering para renderiza��o 3D correta

    // Configura��o da Ilumina��o
    glEnable(GL_LIGHTING); // Habilita o sistema de ilumina��o
    glEnable(GL_LIGHT0); // Habilita a primeira fonte de luz

    GLfloat posicaoLuz[] = {TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, 1.0f}; // Posi��o da fonte de luz
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz); // Aplica a posi��o

    // Defini��o das cores e componentes da luz
    GLfloat luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat luzDifusa[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular); 
}

// --- Bloco de Redimensionamento e Proje��o ---

void remodela(int largura, int altura) {
    if (altura == 0) altura = 1; // Previne erro de divis�o por zero
    float aspecto = (float)largura / (float)altura; // Calcula a propor��o L/A da janela

    glViewport(0, 0, largura, altura); // Define a �rea de desenho
    glMatrixMode(GL_PROJECTION); // Entra no modo de proje��o
    glLoadIdentity(); // Reseta a matriz
    gluPerspective(60.0f, aspecto, 0.1f, 100.0f); // Define a perspectiva 3D
    glMatrixMode(GL_MODELVIEW); // Volta ao modo de modelo/vis�o
}

// --- Bloco da Fun��o Principal (main) ---

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT (necess�rio para argumentos)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o display
    glutInitWindowSize(800, 600); // Define o tamanho da janela
    glutCreateWindow("Bola Dentro do Cubo"); // Cria a janela

    glutDisplayFunc(display); // Associa a fun��o de desenho
    glutReshapeFunc(remodela); // Associa a fun��o de redimensionamento
    glutTimerFunc(16, atualizaPosicaoBola, 0); // Inicia a anima��o

    inicializaGL(); // Chama as configura��es iniciais do OpenGL

    glutMainLoop(); // Inicia o loop de eventos principal

    return 0; // Encerra o programa
}
