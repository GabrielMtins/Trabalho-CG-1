# Trabalho 1 da disciplina SCC0250 - Computação Gráfica

Trabalho 1 da disciplina SCC0250 - Computação Gráfica
Feito pelos alunos:
- Gabriel Martins Monteiro   - N°USP: 14572099
- Hélio Márcio Cabral Santos - N°USP: 14577862

O trabalho cumpre todos os requisitos. Ele utiliza somente as bibliotecas
padrão (STL), glfw (gerenciamento de janelas), GLAD (carregamento de funções de
OpenGL) e funções matemática (glm). A glm não está incluída no projeto, mas
basta clonar o repositório do github na pasta external/, de forma que fique
external/glm/(repositório).

A cena possui 6 objetos:
- Mesa
- Super Nintendo
- Controle
- Monitor
- Pacman
- Livro

Todos devidamente distintos.

A cena representa um desenvolvedor nos anos 90 testando as funções de escala do
super nintendo. Na mesa, há um livro de programação em C (escrito C nele,
construído com primitivas), há um Super Nintendo e um monitor. No plano de
frente, há um controle de Super Nintendo, que o desenvolvedor utiliza para
testar a função de escala do videogame. No monitor, há um objeto que é um
Pacman 2d, composto somente de primitivas, sem utilizar textura. A pessoa pode
apertar um botão do controle para que o Pacman escale no monitor.

Nota: NÃO foram utilizados efeitos de iluminação (phong shading, directional lighting,
diffuse, specular, ambient). O projeto meramente utiliza cores por faces definidos
por uniforms para
que o volume dos objetos possa ser propriamente visto.

De interatividade:
- P: mostra a malha ou retorna ao modo normal de visualização.
- Q: aperta o botão do controle e faz o pacman escalar
- A: gira o controle para uma direção
- D: gira o controle para a outra direção
- I, J, K, L: servem como setinhas e podem ser utilizadas para mover o livro

Os objetos são complexos (feitos em código com auxílio da classe Builder). Cada
objeto tem sua matriz de transformação (model) própria. Cada objeto tem utiliza
variedades de translações, rotações e escalas.

O teclado é utilizado para translação do livro. O teclado é utilizado para
translação do botão do controle e para a escala do pacman. O teclado é
utilizado para rotação do controle. Os objetos fazem sentido na cena, como dito
anteriormente. Não foram utilizadas texturas nem movimentos de câmera.
 
Sobre a organização do código:

- **App**: classe geral do código, estrutura o glfw, glad, inicializa e constrói objetos.
- **Builder**: módulo utilizado para construir modelos a partir de primitivas.
- **Logic**: módulo responsável pela lógica, ou seja, input e translação/rotação/escala segundo o teclado.
- **Object3d**: módulo responsável por ser um wrapper de VAO e VBO do OpenGL.
- **ObjectHandler**: módulo responsável por ser um handler do objeto, permitindo que ele tenha diferentes cores.
- **Shader**: módulo responsável por ser um wrapper de shaders. Permite carregar shaders de strings e definir uniforms.

# Compilação

O projeto foi feito em C++ e depende das bibliotecas glfw, glm e GLAD. Além da
própria biblioteca padrão de C++ (STL).

Para compilar, recomendo que configure o ambiente instalando o glfw com o seu
instalador de pacotes. Por restante, basta seguir o seguinte processo:

```
$ git clone https://github.com/GabrielMtins/Trabalho-CG-1.git
$ git submodule init
$ cd Trabalho-Cg-1
$ make
$ make run
```
