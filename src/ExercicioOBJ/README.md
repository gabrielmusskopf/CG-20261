# Visualizador de Objetos 3D - OpenGL

Este projeto consiste em uma aplicação gráfica robusta em C++ que utiliza a API OpenGL para renderizar modelos 3D complexos a partir de arquivos `.obj`. A aplicação foi evoluída de um modelo procedural para uma arquitetura moderna Orientada a Objetos, permitindo fácil expansão e manutenção.

## 🚀 Funcionalidades

* **Carregamento Dinâmico:** Leitura e processamento de arquivos Wavefront (`.obj`) com suporte a vértices, normais e coordenadas de textura.
* **Interação Intuitiva:** Rotação do modelo via clique e arraste do mouse e zoom dinâmico através do scroll.
* **Sistema de Câmera:** Navegação livre pelo cenário (WASD) e pontos de vista fixos programados.
* **Projeções:** Alternância em tempo real entre projeção Perspectiva e Ortográfica.

## 🏗️ Arquitetura e Componentes

O projeto está organizado em módulos independentes para separar a lógica de infraestrutura da lógica gráfica:

### 1. Janela e Inputs
* **`Window`**: Encapsula a biblioteca GLFW e o carregador GLAD. Gerencia a criação da janela, o ciclo de vida do contexto OpenGL e centraliza os *callbacks* de teclado, mouse e scroll. Utiliza o padrão de *User Pointer* para integrar funções C estáticas com a instância da classe C++.

### 2. Motor Gráfico
* **`Shader`**: Responsável por carregar arquivos de texto (`.vert` e `.frag`), compilar e linkar os programas de shader na GPU. Oferece métodos utilitários para envio de matrizes e variáveis *uniforms*.
* **`Mesh`**: Representa a unidade básica de geometria. Gerencia buffers de memória de vídeo (VAO e VBO) utilizando uma estrutura de dados `Vertex` otimizada.
* **`Model`**: Atua como o carregador de alto nível. Analisa o arquivo `.obj`, extrai as coordenadas matemáticas e instancia as malhas (`Mesh`) necessárias para a renderização.

### 3. Câmera
* **`Camera`**: Gerencia a visualização do cenário. Calcula a matriz *View* usando ângulos de Euler (Yaw/Pitch) e controla o *Field of View* (FOV) para o efeito de zoom.

## 🎮 Controles

| Tecla / Mouse | Ação |
| :--- | :--- |
| **W, A, S, D** | Movimentação da câmera pelo cenário |
| **Clique + Arrastar** | Rotaciona o objeto nos eixos X e Y |
| **Scroll Mouse** | Aproxima ou afasta a visão (Zoom/FOV) |
| **P** | Alterna entre projeção Perspectiva e Ortográfica |
| **1 a 6** | Atalhos para vistas fixas (Frente, Trás, Topo, etc.) |
| **X, Y, Z** | Ativa rotação automática no respectivo eixo |
| **Setas esquerda e direita** | Alternar objetos |
| **Q / ESC** | Encerra a aplicação |

## 🛠️ Compilação e Execução

O projeto utiliza **CMake** para o gerenciamento do build.

1.  **Pré-requisitos:**
    * Compilador C++ compatível com C++17.
    * Bibliotecas instaladas: GLFW3 e GLM.
    * Drivers de vídeo atualizados com suporte a OpenGL 4.5+.

2.  **Passos para Build:**
    ```bash
    mkdir build
    cd build
    cmake ..
    make
    ```

3.  **Execução:**
    ```bash
    ./ExercicioOBJ
    ```

## 📚 Dependências Externas

* [GLFW](https://www.glfw.org/) - Gerenciamento de janelas e contexto.
* [GLAD](https://glad.dav1d.de/) - Carregamento de extensões OpenGL.
* [GLM](https://github.com/g-truc/glm) - Matemática para gráficos 3D (matrizes e vetores).

---
*Este projeto foi desenvolvido como parte das atividades da disciplina de Computação Gráfica.*
