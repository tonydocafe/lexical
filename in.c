#include <stdio.h>  

int main() {
    
    int idade;
    char nome[50];

    
    printf("Digite seu nome: ");
    scanf("%49s", nome); 

    printf("Digite sua idade: ");
    scanf("%d", &idade);

    
    if (idade >= 18) {
        printf("Olá %s, você é maior de idade!\n", nome);
    } else {
        printf("Olá %s, você é menor de idade!\n", nome);
    }

    
    printf("Contando até 5:\n");
    for (int i = 1; i <= 5; i++) {
        printf("%d ", i);
    }
    printf("\n");

    return 0; 
}