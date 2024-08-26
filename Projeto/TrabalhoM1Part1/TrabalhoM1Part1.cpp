/*
Alunos: Vanessa Bastos da Luz e Yuri Rodrigues

Professor: Rodrigo Lyra

Matéria: Computação Gráfica

Trabalho M1-1

Informações de uso:
											 ^
Para movimentar o objeto utilize as setas <- | ->

Para aumentar e diminuir o objeto utilize as teclas "+" e "-"

Para rotacionar o objeto para esquerda utilize a tecla "e"

Para rotacionar o objeto para direita utilize a tecla "d"
*/



#include <iostream>
#include <GL/freeglut.h>
#include <vector>
using vertice = std::pair<double, double>;
using lista_vertices = std::vector<vertice>;
using aresta = std::pair<int, int>;
using lista_arestas = std::vector<aresta>;

struct Poligono {
	double tamanhoLado;
	int numLados;
	vertice posicao;
	vertice escala;
	double rotacao;
	lista_vertices vertices;
	lista_arestas arestas;
};

Poligono criar_poligono(double posicao_x, double posicao_y, double tamanho_lado, int num_lados);
void desenhar(Poligono poligono);
void movimentar(Poligono& poligono, double distancia, double angulo);
void escalar(Poligono& poligono, double escala_x, double escala_y);
void rotacionar(Poligono& poligono, double angulo);
void display();
void redraw(int value);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);

Poligono pentagono;
int delay = 10;

int main(int argc, char** argv) {

	pentagono = criar_poligono(128, 128, 30, 5);

	glutInit(&argc, argv);

	glutInitWindowSize(512, 512);

	glutCreateWindow("Desenhando uma linha");
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glOrtho(0, 256, 0, 256, -1, 1);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(keyboard_special);
	glutTimerFunc(10, redraw, 0);

	glutMainLoop();

	return 0;
}


void display(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	desenhar(pentagono);

	glFlush();
}


void redraw(int value) {
	glutPostRedisplay();
	glutTimerFunc(delay, redraw, 0);
}

Poligono criar_poligono(double posicao_x, double posicao_y, double tamanho_lado, int num_lados) {
	Poligono novo_poligono;
	novo_poligono.numLados = num_lados;
	novo_poligono.posicao.first = posicao_x;
	novo_poligono.posicao.second = posicao_y;
	novo_poligono.tamanhoLado = tamanho_lado;

	novo_poligono.escala.first = 1;
	novo_poligono.escala.second = 1;

	novo_poligono.rotacao = 0;

	float angulo = 0;
	float passo_angulo = ((360 / float(num_lados)) * 3.1415926536) / 180.0;

	float apothem = tamanho_lado / (2 * tan(3.1416 / float(num_lados)));
	posicao_x -= tamanho_lado / 2.0;
	posicao_y -= apothem;

	novo_poligono.vertices.push_back(vertice(posicao_x, posicao_y));
	std::cout << "Vertices:\n";
	std::cout << 0 << " - " << posicao_x << " - " << posicao_y << "\n";
	for (int i = 1; i < num_lados; i++) {
		posicao_x = posicao_x + tamanho_lado * cos(angulo);
		posicao_y = posicao_y + tamanho_lado * sin(angulo);
		novo_poligono.vertices.push_back(vertice(posicao_x, posicao_y));
		std::cout << i << " - " << posicao_x << " - " << posicao_y << "\n";
		angulo += passo_angulo;
	}

	std::cout << "Arestas:\n";
	for (int i = 0; i < num_lados; i++) {
		novo_poligono.arestas.push_back(aresta(i, (i + 1) % num_lados));
		std::cout << i << " - " << (i + 1) % num_lados << "\n";
	}


	return novo_poligono;
}

void movimentar(Poligono& poligono, double distancia, double angulo) {
	double dx = distancia * cos(angulo);
	double dy = distancia * sin(angulo);

	// Atualiza a posição do polígono
	poligono.posicao.first += dx;
	poligono.posicao.second += dy;

	// Movimenta os vértices do polígono
	for (auto& vert : poligono.vertices) {
		vert.first += dx;
		vert.second += dy;
	}
}

void escalar(Poligono& poligono, double escala_x, double escala_y, char botao) {
	double centro_x = poligono.posicao.first;
	double centro_y = poligono.posicao.second;

	if (botao == '+')
		// Escala os vértices em relação ao centro do polígono
		for (auto& vert : poligono.vertices) {
			vert.first = centro_x + (vert.first - centro_x) * escala_x;
			vert.second = centro_y + (vert.second - centro_y) * escala_y;
		}
	else if (botao == '-')
		for (auto& vert : poligono.vertices) {
			vert.first = centro_x + (vert.first - centro_x) / escala_x;
			vert.second = centro_y + (vert.second - centro_y) / escala_y;
		}

	// Atualiza a escala do polígono
	poligono.escala.first *= escala_x;
	poligono.escala.second *= escala_y;
}

void rotacionar(Poligono& poligono, double angulo) {
	double centro_x = poligono.posicao.first;
	double centro_y = poligono.posicao.second;

	// Rotaciona os vértices em torno do centro do polígono
	for (auto& vert : poligono.vertices) {
		double x_novo = centro_x + (vert.first - centro_x) * cos(angulo) - (vert.second - centro_y) * sin(angulo);
		double y_novo = centro_y + (vert.first - centro_x) * sin(angulo) + (vert.second - centro_y) * cos(angulo);
		vert.first = x_novo;
		vert.second = y_novo;
	}

	// Atualiza a rotação do polígono
	poligono.rotacao += angulo;
}

void keyboard(unsigned char key, int x, int y) {
	std::cout << key;
	switch (key) {
	case 27:
		exit(0);
		break;
	case '+':
		escalar(pentagono, 1.1, 1.1, '+');
		break;
	case '-':
		escalar(pentagono, 1.1, 1.1, '-');
		break;
	case 'd':
		rotacionar(pentagono, 30);
		break;
	case 'e':
		rotacionar(pentagono, -30);
		break;
	}
}

void keyboard_special(int key, int x, int y) {
	std::cout << key;
	switch (key) {
	case GLUT_KEY_DOWN:
		movimentar(pentagono, 10, (270 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_UP:
		movimentar(pentagono, 10, (90 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_RIGHT:
		movimentar(pentagono, 10, (0 / 180.0) * 3.1416);
		break;

	case GLUT_KEY_LEFT:
		movimentar(pentagono, 10, (180 / 180.0) * 3.1416);
		break;

	}
}

void desenhar(Poligono poligono) {
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_LINES);
	for (int i = 0; i < poligono.arestas.size(); i++) {
		float v_o = poligono.arestas[i].first;
		float v_d = poligono.arestas[i].second;
		glVertex2f(poligono.vertices[v_o].first, poligono.vertices[v_o].second);
		glVertex2f(poligono.vertices[v_d].first, poligono.vertices[v_d].second);
	}
	glEnd();
}