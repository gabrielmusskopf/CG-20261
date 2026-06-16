# Navegador de Cena 3D OpenGL - Projeto de Grau B

Este projeto representa a evolução do visualizador desenvolvido no Grau A para um **Navegador de Cena 3D dinâmico**. O sistema foi transformado em um motor "data-driven", onde toda a configuração do ambiente (objetos, luzes e câmera) é carregada via arquivos externos, permitindo a criação de dioramas e tours virtuais complexos sem a necessidade de recompilação.

## 🚀 Funcionalidades

###  Carregamento de Cena (Data-Driven)
* **Arquivo `scene.txt`**: Define a posição inicial da câmera, parâmetros da fonte de luz e a lista de objetos a serem instanciados.
* **Múltiplos OBJs**: Suporte para carregar diversos modelos simultaneamente, permitindo compor cenários com mais de 15 objetos.

### 🎢 Animação Paramétrica
* **Curvas de Bézier**: Implementação de trajetórias suaves para objetos. Um objeto na cena segue automaticamente um percurso definido por pontos de controle cúbicos.
* **Offset de Trajetória**: A animação é calculada como um deslocamento, permitindo que o usuário mova a "base" da animação enquanto o objeto continua percorrendo a curva.

###  Iluminação e Materiais Avançados
* **Phong Shading com Structs**: O shader de fragmento foi refatorado para utilizar estruturas de `Material` e `Light`.
* **Propriedades Individuais**: Cada objeto na cena possui seus próprios coeficientes de reflexão ($k_a, k_d, k_s$) e brilho (*shininess*), definidos no arquivo de configuração.

### 🎮 Interação e Navegação
* **Câmera Fly-by**: Navegação completa pelo cenário utilizando teclado e mouse.
* **Seleção de Objetos**: Uso da tecla `TAB` para alternar entre os objetos da cena, permitindo aplicar transformações individuais ao objeto selecionado.

## 📊 Comparação: Grau A vs. Grau B

| Recurso | Grau A | Grau B |
| :--- | :--- | :--- |
| **Configuração** | Hardcoded no `main.cpp`. | Dinâmica via arquivo `scene.txt`. |
| **Foco** | Visualização de um único modelo. | Composição de cena complexa (Diorama). |
| **Animação** | Rotação automática simples. | Trajetórias paramétricas (Bézier). |
| **Materiais** | Coeficientes globais no Shader. | Materiais individuais por instância de objeto. |
| **Câmera** | Movimentação básica. | Sistema de navegação livre com controle de FOV. |
| **Escalabilidade** | Difícil de adicionar novos elementos. | Escalável (adição via arquivo de texto). |

## ⌨️ Mapeamento de Teclas

### Navegação Geral
| Tecla | Ação |
| :--- | :--- |
| **W, A, S, D** | Movimentar a câmera (Fly-by) |
| **Scroll Mouse** | Zoom in/out (Ajuste de FOV) |
| **TAB** | Alternar objeto selecionado |
| **P** | Alternar Projeção (Perspectiva/Ortográfica) |
| **M** | Alternar Modo de Visualização (Sólido/Wireframe) |
| **Q / ESC** | Encerrar aplicação |

### Controle do Objeto Selecionado
| Tecla | Ação |
| :--- | :--- |
| **Setas (Esq/Dir)** | Mover no eixo X |
| **Setas (Cima/Baixo)** | Mover no eixo Z |
| **Espaço / Ctrl Esq** | Mover no eixo Y (Subir/Descer) |
| **Mouse Esq + Arrastar** | Rotacionar livremente |
| **X, Y, Z** | Alternar rotação automática no eixo correspondente |
| **I / O** | Diminuir / Aumentar escala do objeto |

## 📄 Formato do Arquivo de Cena (`scene.txt`)

O arquivo de configuração suporta os seguintes comandos:

```text
# Configuração da Câmera
camera_pos 0 5 20
camera_yaw_pitch -90 -10
camera_fov 45

# Configuração da Luz
light_pos 0 20 0
light_color 1 1 1

# Definição de Objetos
model ../assets/Modelos3D/SuzanneSubdiv1.obj
pos 0 0 0
rot 0 180 0
scale 1 1 1
material 0.1 0.1 0.1  0.8 0.8 0.2  1.0 1.0 1.0  256.0

# Objeto com Animação Bézier (Duração e 4 pontos)
animate_bezier 10.0
p0 -10 0 0
p1 0 5 10
p2 10 0 0
p3 0 -5 -10
```

## 🏗️ Arquitetura do Sistema

1. **`loadScene`**: Parser robusto que interpreta o arquivo de texto e popula o vetor de `SceneObject`.
2. **`SceneObject`**: Estrutura que encapsula o modelo, suas transformações e agora suas propriedades de animação e material.
3. **`BezierCurve`**: Classe utilitária para cálculo de interpolação polinomial cúbica.
4. **Shaders**:
   - `phong.vert`: Processa transformações e envia normais/fragPos.
   - `phong.frag`: Implementa o modelo de Phong utilizando structs de material e luz para flexibilidade total.

## ⚙️ Como Compilar e Rodar

O projeto utiliza o **CMake** configurado para C++17:

```bash
# Criar diretório de build
mkdir build && cd build

# Configurar e compilar
cmake ..
cmake --build .

# Executar o projeto Grau B
./GrauB
```

---
*Desenvolvido como parte da disciplina de Computação Gráfica.*
