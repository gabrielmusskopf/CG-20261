# Visualizador 3D OpenGL - Projeto de Grau A

Este projeto consiste em um visualizador de modelos 3D desenvolvido em **C++** utilizando a API gráfica **OpenGL**. A aplicação permite o carregamento de múltiplos objetos, manipulação de transformações geométricas (translação, rotação e escala), iluminação dinâmica e diferentes modos de visualização.

## 🚀 Funcionalidades

### 💡 Iluminação e Shaders
* **Modelo de Iluminação de Phong:** Implementação completa via Shaders (Vertex e Fragment) calculando os componentes:
    * **Ambiente ($k_a$):** Luz global mínima.
    * **Difusa ($k_d$):** Sombreamento baseado na posição da luz.
    * **Especular ($k_s$):** Reflexos de brilho baseados na posição da câmera.

### 🎮 Controles e Interação
* **Múltiplos Objetos:** Suporte para carregamento e exibição de várias instâncias de modelos simultaneamente.
* **Seleção:** Alternância entre objetos da cena para manipulação individual.
* **Transformações:**
    * Translação nos eixos X, Y e Z.
    * Rotação manual via teclado e mouse.
    * Rotação Automática (Toggle): Inicia/para o giro contínuo nos eixos X, Y e Z.
    * Escala uniforme para redimensionamento.

### 🎥 Sistema de Câmera e Visualização
* **Câmera Sintética:** Navegação livre pelo cenário (estilo FPS).
* **Projeção:** Alternância entre Projeção Perspectiva e Ortográfica.
* **Modo Wireframe:** Visualização da malha poligonal sobreposta ao objeto sólido.

## ⌨️ Mapeamento de Teclas

| Tecla | Ação |
| :--- | :--- |
| **W, A, S, D** | Movimentação da Câmera |
| **Scroll Mouse** | Zoom (Ajuste de FOV) |
| **Mouse Esquerdo** | Rotacionar objeto selecionado |
| **TAB** | Alternar seleção entre objetos |
| **M** | Alternar Modo Wireframe (Sólido/Malha) |
| **P** | Alternar Projeção (Perspectiva/Ortográfica) |
| **X, Y, Z** | **Toggle** Rotação automática nos eixos X, Y ou Z |
| **Setas (Dir/Esq)** | Translação no eixo X |
| **Setas (Cima/Baixo)** | Translação no eixo Z |
| **Espaço / C** | Translação no eixo Y (Sobe/Desce) |
| **Q / E** | Aumentar/Diminuir Escala |


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

## 🛠️ Tecnologias Utilizadas
* **Linguagem:** C++
* **Janelamento:** GLFW
* **Carregamento de Funções:** GLAD
* **Matemática Linear:** GLM (OpenGL Mathematics)
* **Carregamento de Modelos:** Implementação customizada para arquivos `.obj`.

## 🏗️ Estrutura do Projeto
* `/src`: Código-fonte (.cpp e .h).
* `/shaders`: Arquivos de Vertex e Fragment Shader (GLSL).
* `/assets`: Modelos 3D e texturas.

## ⚙️ Como Compilar e Rodar

O projeto utiliza **CMake** para gerenciamento de build:

```bash
# Criar diretório de build
mkdir build && cd build

# Configurar e compilar
cmake ..
make

# Executar
./GrauA
```

