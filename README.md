# Instagram

Alunos: Gabriel Clério Wottawa

OBS.:

Caso tenha problema com as dependências, logo depois ao abrir o projeto no VS, faça a configuração do diretório das dependencias em:
Propriedades > C/C++ > Geral > Diretórios de Inclusão Adicionais > ... Adiciocione os diretórios GLEW/include, GLFW/include, irrKlang/include, SOIL/include e /glm da   pasta /dependencies deste projeto, como mostra a imagem a seguir;

![image](https://user-images.githubusercontent.com/56985063/137815948-9e756cca-e031-4c78-9dcc-0c3033e817c4.png)

Propriedades > Vinculador > Geral > Diretórios de Biblioteca Adicionais > ... Adiciocione os diretórios GLEW/lib, GLFW/lib, irrKlang/lib e SOIL/lib da pasta /dependencies deste projeto, como mostra a imagem a seguir;

![image](https://user-images.githubusercontent.com/56985063/137816043-0b762679-746c-4d48-86d0-6de995b4e680.png)

Propriedades > Vinculador > Entrada > Dependências Adicionais > ... Adicione as bibliotecas glfw3.lib, glew32s.lib, opengl32.lib, irrKlang.lib e SOIL.lib, como mostra a imagem a seguir.

![image](https://user-images.githubusercontent.com/56985063/137816090-50ff047e-f2b1-47fd-9cbb-c59c5bb59c4f.png)
