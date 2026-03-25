## Compilação

```bash
gcc main.c lexical.c -o lex

./lex entrada.c saida.c
```

**main.c**

O programa começa verificando se o usuário forneceu os arquivos de entrada e saída via linha de comando.
Em seguida, abre o arquivo de entrada para leitura e o de saída para escrita, mostrando erros se não conseguir.
Depois, chama a função analisar_arquivo para realizar a análise léxica do conteúdo.
Por fim, fecha os arquivos e exibe uma mensagem indicando que a análise foi concluída.

