# Trabalho 3 da disciplina SCC0250 - Computação Gráfica

Trabalho 3 da disciplina SCC0250 - Computação Gráfica
Feito pelos alunos:
- Gabriel Martins Monteiro   - N°USP: 14572099
- Hélio Márcio Cabral Santos - N°USP: 14577862

O trabalho cumpre todos os requisitos. Ele utiliza somente as bibliotecas
padrão (STL), glfw (gerenciamento de janelas), GLAD (carregamento de funções de
OpenGL), funções matemática (glm) e STB para carreagmento de imagens. A glm não
está incluída no projeto, mas basta clonar o repositório do github na pasta
external/, de forma que fique external/glm/(repositório).

A cena representa uma casa no estilo medieval rodeado de um pequeno bosque.

De interatividade:
- WASD e mouse: controle de câmera estilo FPS
- U e I: escala a árvore
- J e K: rotaciona o livro
- Seta para cima e para baixo: desloca a cadeira

Os objetos foram carregados através do obj loader próprio **ObjLoader** e estão
todos na pasta obj/

Sobre a organização do código:

- **App**: classe geral do código, estrutura o glfw, glad, inicializa e constrói objetos.
- **Builder**: módulo utilizado para construir modelos a partir de primitivas.
- **Logic**: módulo responsável pela lógica, ou seja, input e translação/rotação/escala segundo o teclado.
- **Object3d**: módulo responsável por ser um wrapper de VAO e VBO do OpenGL.
- **ObjectHandler**: módulo responsável por ser um handler do objeto, permitindo que ele tenha diferentes texturas.
- **Shader**: módulo responsável por ser um wrapper de shaders. Permite carregar shaders de strings e definir uniforms.
- **Texture**: módulo responsável por carregamento de texturas
- **ObjLoader**: Responsável por carregar arquivos .obj

# Compilação

O projeto foi feito em C++ e depende das bibliotecas glfw, glm, GLAD e STB.
Além da própria biblioteca padrão de C++ (STL). Também se utiliza cmake como
build system. O script run_build.sh é meramente um wrapper que compila e
executa o programa.

Para compilar, recomendo que configure o ambiente instalando o glfw com o seu
instalador de pacotes. Por restante, basta seguir o seguinte processo:

```
$ git clone https://github.com/GabrielMtins/Trabalho-CG-1.git
$ git submodule init
$ git submodule update
$ cd Trabalho-Cg-1
$ ./run_build.sh
```
