## Compilação

```bash
gcc main.c lexical.c -o lex

./lex entrada.c saida.c
```


1. description 

Um processamento léxico básico é feito reconhecendo diferentes tipos de tokens, registrando suas posições e escrevendo tudo em uma saída organizada. Ele separa números, identificadores, palavras reservadas, literais, comentários, operadores, separadores e indicadores, garantindo que cada elemento do código-fonte seja corretamente classificado para uso em etapas posteriores.



**lexical.c**


- match

Verifica se um caractere corresponde a um padrão de expressão regular. Retorna 1 se casar, 0 caso contrário.

- ok_separador

Confere se o caractere é um separador como (){};,.

- ok_operador

Confere se o caractere é um operador como + - * / = < >.

- ok_indicador

Confere se o caractere é um indicador como # ou @.

- ok_reservada

Verifica se uma string corresponde a uma palavra reservada da linguagem (como if, int, return).

- registrar

Escreve no arquivo de saída a classe do token, seu valor e sua posição (linha e coluna).

- analisar_arquivo

Lê o arquivo de entrada caractere por caractere, identifica tokens (numerais, identificadores, palavras reservadas, literais, comentários, operadores, separadores e indicadores) e registra cada um usando a função registrar. Mantém contadores de linha e coluna para rastrear a posição de cada token.

**main.c**

O programa começa verificando se o usuário forneceu os arquivos de entrada e saída via linha de comando.
Em seguida, abre o arquivo de entrada para leitura e o de saída para escrita, mostrando erros se não conseguir.
Depois, chama a função analisar_arquivo para realizar a análise léxica do conteúdo.
Por fim, fecha os arquivos e exibe uma mensagem indicando que a análise foi concluída.

#### updates 

- Compilação

```bash
make

make run 

```

Para limpar 

```bash
make clean

```
