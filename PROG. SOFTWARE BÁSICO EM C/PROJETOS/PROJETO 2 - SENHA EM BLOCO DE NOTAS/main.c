#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

// ------------------CONSTANTES---------------------//
#define ENTER 13 //VALOR DA TECLA ENTER
#define BACKSPACE 8	//VALOR DA TECLA BACKSPACE
#define MAX 50 //VALOR M�XIMO DA SENHA



int main(){
	
//---------ABERTURA E VERIFICA��O DA EXIST�NCIA DO ARQUIVO SENHA.TXT---------//
	char senhaCorreta[MAX]; 
	
	FILE *f = NULL;
	f = fopen ("senha.txt", "r"); //Abre o arquivo senha.txt para leitura
	if(f == NULL){
		printf("Arquivo nao encontrado!"); //Se n�o  existir, d� como N�o Encontrado e fecha o programa
		return 1;
	}else{
		fgets(senhaCorreta, sizeof(senhaCorreta), f); //Se existir, pega o que est� escrito no arquivo e fecha
		fclose(f);
	}

//--------DEFINI��O DAS VARI�VEIS DE SENHA E CONTADOR DE CARACTERES-----//
	char caracter;
	char senha[MAX];
	int i = 0;
	
	printf("Digite a senha: ");

//-------ENTRADA DE DADOS DA SENHA DIGITADOS PELO USU�RIO---------//
	while(1){ //Loop para pegar caracter por caracter
		
		caracter = getch(); //pega o caracter digitado pelo usu�rio
				
		if(caracter == ENTER){	//Se digitar ENTER, termina a entrada de dados e forma a senha para compara��o
		
			senha[i] = '\0'; //Marca o final da string
			break;
			
		}else if(caracter == BACKSPACE){ //Se digitar BACKSPACE, apaga o �ltimo caracter que o usu�rio digitou
			
			if (i > 0){
				i--; //Comando que diminui a cadeia de caracteres do vetor senha[]
				printf("\b \b"); //Comando respons�vel por apagar visualmente o caracter
				senha[i] = '\0'; //Marca o final da String
			}
			
		
		}else if(i > MAX - 1){	//Limitador de carecteres (Equivalente ao valor de MAX, que � 50)
			senha[i] = '\0'; //Marca o final da String
		}else{
			
			senha[i] = caracter; //grava o caracter digitado pelo usu�rio
			i++; //Passa para o pr�ximo caracter
			putchar('*'); //Adiciona visualmente o Asterisco para indicar que um caracter foi digitado
			
		}
		

	}
	
//----------------COMPARA��O DA SENHA DIGITADA E SENHA DO ARQUIVO---------------
	
	if(strcmp(senha, senhaCorreta) == 0){ //strcmp() compara a senha digitada pelo usu�rio com a senha que foi lida no arquivo senha.txt
    	printf("\n\nSenha correta! Acesso permitido.");
	}else{
	    printf("\n\nSenha incorreta! Acesso negado.");
	}
	
	return 0;
}
