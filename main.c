#include <stdio.h>
#include <stdlib.h>
#include "header.h"
int main()
{
  int opcao =100;
    criar_cliente_lista();
	while(opcao!=sair){
		cabecalho();
		printf("%d Criar conta \n", criar_conta); // cria a conta para o usuario
		printf("%d Login \n", login);
        printf("%d Acesso Administrador\n",login_adm);    // faz o login para o usuario, para que ele possa usar as funçoes de sua conta
		printf("%d Sair \n", sair);// fecha o programa
		scanf("%d", &opcao);

		switch(opcao){
			case criar_conta:
				cabecalho();
				inserir();
				cabecalho();
				break;
			case login:
				cabecalho();
				entrar();
				cabecalho();
				break;
            case login_adm:
                cabecalho();
                entrar_adm();
                cabecalho();
			case sair:

				break;
			default:
				printf("Comando invalido....\n");
 				break;
		}
	}
	return 0;
}
