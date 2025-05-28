#include<stdio.h>
#include <windows.h>

int main(){
    SetConsoleOutputCP(CP_UTF8); 
    int idade;
    char nome[50];
    /* char leva %s ou %c?*/
    printf("Informe seu nome:");
    scanf("%s", &nome);
    printf("Informe sua idade:");
    scanf("%d", &idade);
    printf("Olá me chamo %s e tenho %d anos", nome, idade);

}