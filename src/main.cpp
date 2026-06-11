#include "App.hpp"

/*
 * Trabalho 3 da disciplina SCC0250 - Computação Gráfica
 * Feito pelos alunos:
 * Gabriel Martins Monteiro   - N°USP: 14572099
 * Hélio Márcio Cabral Santos - N°USP: 14577862
 *
 *  De interatividade:
 *  - WASD e mouse: controle de câmera estilo FPS
 *  - U e I: escala a árvore
 *  - J e K: rotaciona o livro
 *  - Seta para cima e para baixo: desloca a cadeira
 *  - 1, 2, 3, 4 - ativa ou desativa luzes
 *  - t, y - translação da vela externa
 *  - z, x - luz ambinete
 *  - c, v - luz difusa
 *  - b, n - luz especular
 *  Não foram utilizadas texturas nem movimentos de câmera.
 *
 *  Sobre a organização do código:
 *  App: classe geral do código, estrutura o glfw, glad, inicializa e constrói objetos.
 *  Builder: módulo utilizado para construir modelos a partir de primitivas.
 *  Logic: módulo responsável pela lógica, ou seja, input e translação/rotação/escala segundo o teclado.
 *  Object3d: módulo responsável por ser um wrapper de VAO e VBO do OpenGL.
 *  ObjectHandler: módulo responsável por ser um handler do objeto, permitindo que ele tenha diferentes texturas.
 *  Shader: módulo responsável por ser um wrapper de shaders. Permite carregar shaders de strings e definir uniforms.
 *  Texture: módulo responsável por carregamento de texturas
 *  ObjLoader: Responsável por carregar arquivos .obj
 *
 *  Mais detalhes no README.md incluído com o projeto.
 */

int main(void) {
	App *app;

	// Inicializa a aplicação principal
	app = new App();

	// Inicia o loop principal do programa
	app->run();

	// Limpa os recursos alocados
	delete app;

	// Encerra a biblioteca GLFW antes de fechar o programa
	glfwTerminate();

	return 0;
}
