#include <glut.h> // Inclui a biblioteca GLUT para gráficos e janelas
#include <stdio.h> // Inclui a biblioteca padrão de I/O em C

// --- Configurações de Matemática ---
#ifndef M_PI
#define M_PI 3.1415 // Define PI (valor corrigido para maior precisão)
#endif

// Declarações das funções trigonométricas da biblioteca matemática
extern double cos(double x); 
extern double sin(double x); 

// --- Constantes e Variáveis Globais de Cenário ---

// Configuração do Cubo (palco)
const float TAMANHO_CUBO = 2.0f; // Define o limite espacial (metade do lado)

// Posição e Raio da Bola
float bolaX = 0.0f; // Coordenada X
float bolaY = 0.0f; // Coordenada Y
float bolaZ = 0.0f; // Coordenada Z
const float RAIO_BOLA = 0.2f; // Tamanho da esfera

// Velocidade da Bola (determinando a taxa de movimento)
float velocidadeBolaX = 0.02f; 
float velocidadeBolaY = 0.015f; 
float velocidadeBolaZ = 0.01f; 

// --- Variáveis de Material da Bola (Cor e Brilho) ---
GLfloat bolaAmbiente[] = {0.0f, 0.0f, 0.2f, 1.0f}; // Cor nas áreas escuras
GLfloat bolaDifusa[] = {0.0f, 0.0f, 1.0f, 1.0f}; // Cor principal (Azul)
GLfloat bolaEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f}; // Cor do reflexo de luz
GLfloat bolaBrilho[] = {25.0f}; // Intensidade do reflexo

// --- Variáveis de Configuração da Câmera (Ponto de Vista) ---
float distanciaCamera = 7.0f; // Distância da câmera ao centro da cena
float anguloCameraY = 45.0f; // Rotação horizontal (esquerda/direita)
float anguloCameraX = 30.0f; // Inclinação vertical (cima/baixo)

// --- Bloco de Funções de Desenho ---

void desenhaBola() {
    glPushMatrix(); // Salva o estado atual da matriz de transformação
    glTranslatef(bolaX, bolaY, bolaZ); // Move o objeto para a posição da bola

    // Aplica as propriedades de material (cores e brilho) da bola
    glMaterialfv(GL_FRONT, GL_AMBIENT, bolaAmbiente); 
    glMaterialfv(GL_FRONT, GL_DIFFUSE, bolaDifusa); 
    glMaterialfv(GL_FRONT, GL_SPECULAR, bolaEspecular);
    glMaterialfv(GL_FRONT, GL_SHININESS, bolaBrilho);

    glutSolidSphere(RAIO_BOLA, 30, 30); // Desenha a esfera sólida

    glPopMatrix(); // Restaura o estado anterior da matriz
}

void desenhaCuboWireframe() {
    glDisable(GL_LIGHTING); // Desliga a iluminação para desenhar linhas puras
    glColor3f(1.0f, 1.0f, 1.0f); // Define a cor das linhas como Branca

    glutWireCube(TAMANHO_CUBO * 2.0f); // Desenha o cubo apenas com suas arestas

    glEnable(GL_LIGHTING); // Liga a iluminação de volta para a bola
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Limpa o fundo e a profundidade da tela
    glLoadIdentity(); // Reseta a matriz de visão

    // --- Bloco de Cálculo e Posicionamento da Câmera ---
    // Cálculo trigonométrico das coordenadas da câmera com base nos ângulos
    float camX = distanciaCamera * cos(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);
    float camY = distanciaCamera * sin(anguloCameraX * M_PI / 180.0);
    float camZ = distanciaCamera * sin(anguloCameraY * M_PI / 180.0) * cos(anguloCameraX * M_PI / 180.0);

    // Posiciona a câmera no espaço (onde, para onde olha, qual o topo)
    gluLookAt(camX, camY, camZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); 

    // --- Bloco de Desenho da Cena ---
    desenhaCuboWireframe(); // Desenha o palco
    desenhaBola(); // Desenha o ator

    glutSwapBuffers(); // Exibe o quadro desenhado para a animação
}

// --- Bloco de Animação e Lógica de Física ---

void atualizaPosicaoBola(int valor) {
    // 1. Atualiza a posição
    bolaX += velocidadeBolaX; 
    bolaY += velocidadeBolaY; 
    bolaZ += velocidadeBolaZ; 

    // 2. Verifica Colisão
    float limite = TAMANHO_CUBO - RAIO_BOLA; // Calcula o ponto de colisão

    // Lógica de Colisão nos 3 Eixos: se ultrapassar, reposiciona e inverte a velocidade
    if (bolaX > limite) { bolaX = limite; velocidadeBolaX = -velocidadeBolaX; } 
    else if (bolaX < -limite) { bolaX = -limite; velocidadeBolaX = -velocidadeBolaX; }

    if (bolaY > limite) { bolaY = limite; velocidadeBolaY = -velocidadeBolaY; } 
    else if (bolaY < -limite) { bolaY = -limite; velocidadeBolaY = -velocidadeBolaY; }

    if (bolaZ > limite) { bolaZ = limite; velocidadeBolaZ = -velocidadeBolaZ; } 
    else if (bolaZ < -limite) { bolaZ = -limite; velocidadeBolaZ = -velocidadeBolaZ; }

    // 3. Agenda e Redesenha
    glutPostRedisplay(); // Solicita um novo desenho (chama display())
    glutTimerFunc(16, atualizaPosicaoBola, 0); // Agenda a próxima atualização (aprox. 60 FPS)
}

// --- Bloco de Inicialização OpenGL e Iluminação ---

void inicializaGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 0.5f); // Define a cor de fundo (Cinza Escuro)
    glEnable(GL_DEPTH_TEST); // Habilita o z-buffering para renderização 3D correta

    // Configuração da Iluminação
    glEnable(GL_LIGHTING); // Habilita o sistema de iluminação
    glEnable(GL_LIGHT0); // Habilita a primeira fonte de luz

    GLfloat posicaoLuz[] = {TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, TAMANHO_CUBO * 1.5f, 1.0f}; // Posição da fonte de luz
    glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz); // Aplica a posição

    // Definição das cores e componentes da luz
    GLfloat luzAmbiente[] = {0.2f, 0.2f, 0.2f, 1.0f}; 
    GLfloat luzDifusa[] = {0.8f, 0.8f, 0.8f, 1.0f};
    GLfloat luzEspecular[] = {1.0f, 1.0f, 1.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa); 
    glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular); 
}

// --- Bloco de Redimensionamento e Projeção ---

void remodela(int largura, int altura) {
    if (altura == 0) altura = 1; // Previne erro de divisão por zero
    float aspecto = (float)largura / (float)altura; // Calcula a proporção L/A da janela

    glViewport(0, 0, largura, altura); // Define a área de desenho
    glMatrixMode(GL_PROJECTION); // Entra no modo de projeção
    glLoadIdentity(); // Reseta a matriz
    gluPerspective(60.0f, aspecto, 0.1f, 100.0f); // Define a perspectiva 3D
    glMatrixMode(GL_MODELVIEW); // Volta ao modo de modelo/visão
}

// --- Bloco da Função Principal (main) ---

int main(int argc, char** argv) {
    glutInit(&argc, argv); // Inicializa o GLUT (necessário para argumentos)
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // Configura o display
    glutInitWindowSize(800, 600); // Define o tamanho da janela
    glutCreateWindow("Bola Dentro do Cubo"); // Cria a janela

    glutDisplayFunc(display); // Associa a função de desenho
    glutReshapeFunc(remodela); // Associa a função de redimensionamento
    glutTimerFunc(16, atualizaPosicaoBola, 0); // Inicia a animação

    inicializaGL(); // Chama as configurações iniciais do OpenGL

    glutMainLoop(); // Inicia o loop de eventos principal

    return 0; // Encerra o programa
}
