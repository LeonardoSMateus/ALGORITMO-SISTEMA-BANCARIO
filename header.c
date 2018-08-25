#include "header.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
Cliente *dados;
int quant_clientes=0;  // matriz global para ter controle da quantidade de usuarios
void criar_cliente_lista(){
    quant_clientes = 0;
    FILE* quantidade = fopen("quantClientes.bin", "rb");
    fread(&quant_clientes, sizeof(int), 1, quantidade);
    FILE* file = fopen("clientes.bin","rb");
    dados = malloc(1000* sizeof(Cliente)); //alocação dinamica para armazenar os dados de cada cliente
    fread(dados, sizeof(Cliente), quant_clientes+1, file);
    fclose(quantidade);
    fclose(file);
}
void atualizarDados(Cliente* clientes, int quant){
    FILE* file = fopen("clientes.bin", "wb");
    int i =0;
    for(i = 0; i < quant; i++){
        fwrite(&clientes[i], sizeof(Cliente), 1, file);
    }
    fclose(file);
}

double saldo(int id){
    int index = achar_cliente(id);//codigo pra achar o indice pelo id

    return dados[index].saldo_inicial;

}

void extratoConta(int id){
    printf("\n O saldo da conta %d e: %lf\n",id, saldo(id));

}


int saque(int id, double valor){
    int index = achar_cliente(id); //codigo pra achar o indice pelo id
    if(dados[index].saldo_inicial >= valor){
        dados[index].saldo_inicial -= valor;
     //   printf("O saldo com o saque e: %lf", dados[index].saldo_inicial);
        return 1; // concluido com sussesso
    }else{
        return 0;  // não tinha dinheiro o suficiente
    }
    atualizarDados(dados, quant_clientes);
}

int investir(int id, double valor){
    // recebe o horario atual da maquina
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    printf("\n\nA data atual %d/%d/%d\n\n",tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900); // pega a data atual do seu computador
    if(saque(id, valor)){ //vai para a funcao saque
        int index = achar_cliente(id);
        dados[index].data_investimento.tm_year = tm.tm_year; // ano do seu computador
        dados[index].data_investimento.tm_mday = tm.tm_mday; // dia do seu computador
        dados[index].data_investimento.tm_mon = tm.tm_mon; // mes do seu computador
        dados[index].valor_investido = valor; //recebe o valor da funcao
        atualizarDados(dados, quant_clientes);
        return 1;
    }else{
        return 0;
    }

}

void rendimento_valor(int id,  double juros){ // ira realizar a conta do investimento nos proximos meses
    int index = achar_cliente(id);
    struct tm data_investimento = dados[index].data_investimento;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int meses = (tm.tm_year - data_investimento.tm_year)* 12;
    meses += data_investimento.tm_mday > tm.tm_mday? 1: 0;
    meses +=  tm.tm_mon - data_investimento.tm_mon ;
    printf("\ninvestimento feito em: %d/%d/%d\n",data_investimento.tm_mday, data_investimento.tm_mon+1, data_investimento.tm_year+1900);
    printf("\nO valor obtido foi: %lf\n",((juros * meses* dados[index].valor_investido) + dados[index].valor_investido));
}

int achar_cliente(int id){ // ira encontrar os clientes cadastrados
    int i = 0;

    for(i = 0; i < quant_clientes; i++){
        if(dados[i].id == id){
            return i;
        }
    }
    //printf("\n\n Cliente nao encontrado! \n\n"); // se o cliente nao existir, vai mostrar se este nao existir
    return -1;
}
int validar_id(int id){
    int a,id_2;
    char c;
    a = achar_cliente(id);
    if(a == -1){
        return 1;
    }
    else{
       return 0 ;
    }

}



Cliente* ler_dados(){ // criando um cliente


    fflush(stdin);
    Cliente* tempStruct = malloc(sizeof(Cliente));
    char c;
    int id_valido;
    int id_novo;
    do{
        c = 'n';

        printf("Digite ID conta:\t");
        fflush(stdin);
        scanf("%d", &id_novo);
        id_valido = validar_id(id_novo);
        if(id_valido){
             (*tempStruct).id = id_novo;
                fflush(stdin);
            printf("Digite Nome:\t");
            gets((*tempStruct).nome);
            fflush(stdin);
            printf("Digite Data de Nascimento:\t");
            scanf("%d", &(*tempStruct).data_nascimento);
            fflush(stdin);
            printf("Digite uma senha de 4 numeros:\t");
            int i =0;
            for(i = 0; i < 4; i++){
                int temp = getch()-48;
                (*tempStruct).senha_num[i] = temp;
                printf("*");
            }
            printf("\n");
            fflush(stdin);
            printf("Digite uma senha de 3 letras:\t");
            for(i = 0; i < 3; i++){
                int temp = getch();
                (*tempStruct).senha_let[i] = temp;
                printf("*");
            }
            printf("\n");
            fflush(stdin);
            printf("Digite seu saldo inicial:\t");
            scanf("%lf", &(*tempStruct).saldo_inicial);
            fflush(stdin);
            return tempStruct;
        }
        else{
            fflush(stdin);
            printf("ID invalido, Deseja continuar a operacao com um ID diferente ?\n S- SIM \t N - NAO \n");
            scanf("%c",&c);
            if(c == 'n'){
                return NULL;
            }

        }
           fflush(stdin);
    }while(!id_valido );

}
int inserir(){

    Cliente* clien = ler_dados();
    if(clien){
        FILE* file = fopen("clientes.bin","ab");
        FILE* quantidade = fopen("quantClientes.bin", "wb");
        dados[quant_clientes] = *clien;
        fwrite(&dados[quant_clientes], sizeof(dados[quant_clientes]),1, file);


        quant_clientes++; // aumentando a quantidade de clientes na variavel global
        fwrite(&quant_clientes, sizeof(int), 1, quantidade);
        fclose(file);
        fclose(quantidade);
    }

	return 0;
}

int modificar_cliente(int id){
    if(id > quant_clientes){
        return 0;
    }else{
        dados[id] = *ler_dados();
        return 1;
    }

}

void mostrar_cliente(int id){

    int index = achar_cliente(id);
    printf("\nNome: %s\n", dados[index].nome);
    printf("\nSaldo: %lf\n", dados[index].saldo_inicial);



}
void mostrar_clientes(){

    int i = 0;
    for(i = 0; i < quant_clientes; i++){
          mostrar_cliente(dados[i].id);
    }
}





int transferencia(int id_a, int id_b, double valor){
  int a,b;

  b = achar_cliente(id_b); // ira chamar uma funcao para buscar o cliente
  if(b >= 0){
    if(saque(id_a, valor)){
        dados[b].saldo_inicial += valor; // ira transferir o dinheiro do banco para o cliente
         atualizarDados(dados, quant_clientes);
        return 1;
    }
  }

  return 0;
}

void emprestimo(int id_a, double valor){
  int a;

  a = achar_cliente(id_a); // ira chamar uma funcao para buscar o cliente
  dados[a].saldo_inicial += valor; // ira adicionar o valor a conta do cliente
  atualizarDados(dados, quant_clientes);
}

void cambio(int id_a){
  int a;
  double valor;
  char moeda;
  double converte_valor;
  a = achar_cliente(id_a); // ira chamar uma funcao para buscar o cliente
  fflush(stdin);
  printf("Converter para qual moeda: \n D - Dolar \n\n E - Euro \n "); // escolhe a moeda
  scanf("%c",&moeda);
  printf("\n  valor em real a ser convertido :\t"); // ira trocar o valor em real para a moeda escolhida
  scanf("%lf",&valor);

  if(moeda == 'D' || moeda == 'd'){
    converte_valor = valor /3.14; // ira converter o valor de real para dolar
    dados[a].saldo_dolar = converte_valor; // ira adicionar o valor convertido da "conta" do cliente(real) em sua conta dolar
    dados[a].saldo_inicial -= valor; // ira retirar o valor, em real, da "conta" (real)
  }
  if(moeda == 'E' || moeda == 'e'){
     converte_valor = valor /3.74; // ira converter o valor de real para euro
    dados[a].saldo_euro = converte_valor;// ira adicionar o valor convertido da "conta" do cliente(real) em sua conta Euro
    dados[a].saldo_inicial -= valor;// ira retirar o valor, em real, da "conta" (real)
  }
  atualizarDados(dados, quant_clientes);
}

void mostra_cambio(int id_a){
    fflush(stdin);
  int a;
  char moeda;
  a = achar_cliente(id_a);
  if(a == -1){
    printf("Cliente nao encontrado");
  }
  else{
    printf("Moeda a ser mostrada\n Euro - Digite 'E'\n Dolar 'D'\n");
    scanf("%c",&moeda);
    system("cls");
    if(moeda == 'E' || moeda == 'e'){
        printf("Seu Saldo em Euro: %lf\n",dados[a].saldo_euro);
    }
    if(moeda == 'D' || moeda == 'd'){
        printf("Seu Saldo em Dolar: %lf\n",dados[a].saldo_dolar);
    }
  }
}

void depositar(int nconta, double valor){

	int a = achar_cliente(nconta); // ira chamar uma funcao para buscar o cliente


    dados[a].saldo_inicial += valor; // ira depositar o valor inserido em sua conta
    atualizarDados(dados, quant_clientes);
}

double rendimentos[6] = {1.05,1.044,1.022,1.02,1.09,1.065};

void aplicacao(int nconta){
	int escolha, casa;
	double valor, rendeu;

	int cont = achar_cliente(nconta); // ira chamar uma funcao para buscar o cliente

		if(dados[cont].id == nconta){
			do{
				printf("\n\n\n");
				printf("Escolhe sue invetimento:\n");
				printf("1- Acao da Petrobras \n 2-Poupanca \n 3- Ouro \n 4- Acao da Samsung \n 5- Sair \n");
				scanf("%d", &escolha);

				printf("Qual o valor investimento:\t");
				scanf("%lf", &valor);

				srand(time(NULL));
				if(escolha == 1){
					casa = rand()%6;
					rendeu = valor * rendimentos[casa];
				}
				if(escolha == 2){
					casa = rand()%6;
					rendeu = valor * rendimentos[casa];
				}
				if(escolha == 3){
					casa = rand()%6;
					rendeu = valor * rendimentos[casa];
				}
				if(escolha == 4){
					casa = rand()%6;
					rendeu = valor * rendimentos[casa];
				}

				dados[cont].saldo_inicial -= valor;
				dados[cont].saldo_inicial += rendeu;

			}while(valor >= dados[cont].saldo_inicial && escolha != 5);
		}
}
int comparePwdNum(int *s1, int *s2, int size){
    int i =0;
    int igual = 1;
    for(i = 0; i < size; i++){
        if(s1[i] != s2[i]){
            igual = 0;
        }
    }
    return igual;
}

/////funçoes leo
void listar(){

	char nome[30];

	cabecalho();


    int i = 0;
    for(i = 0; i < quant_clientes; i++){

                printf("ID: \t %d \n", dados[i].id);
                printf("Nome:\t %s \n", dados[i].nome);
                printf("Data nascimento:\t %d\n", dados[i].data_nascimento);
                printf("Saldo inicial:\t %lf \n", dados[i].saldo_inicial);
                printf("Saldo Euro:\t %lf \n", dados[i].saldo_euro);
                printf("Saldo Dolar:\t %lf \n", dados[i].saldo_dolar);
                printf("---------------------------------\n\n");


	}

	getch();
}

void pesquisar(){



	char nome[30];

	cabecalho();




    fflush(stdin);
    printf("Digite o nome a pesquisar:\t");
    gets(nome);
    int i = 0;
    for(i = 0; i < quant_clientes; i++){
        if(strcmp(nome, dados[i].nome) == 0){
                printf("ID: \t %d \n", dados[i].id);
                printf("Nome:\t %s \n", dados[i].nome);
                printf("Data nascimento:\t %d\n", dados[i].data_nascimento);
                printf("Saldo inicial:\t %lf \n", dados[i].saldo_inicial);
                printf("Saldo Euro:\t %lf \n", dados[i].saldo_euro);
                printf("Saldo Dolar:\t %lf \n", dados[i].saldo_dolar);
                printf("---------------------------------\n\n");
        }

	}


	getch();
}

void cabecalho(){
	system("cls");
	printf("-----------------------------------------------\n");
	printf("\t\t OWL BANK \n");
	printf("-----------------------------------------------\n");
}

void entrar(){ // funcao que chama as outras e da opções para o usuario
	int num,num2, senhan1[4], cont,id_valido;
	char senhal[5];
    char c;
    int id_novo;

    cabecalho();
    printf("Digite ID conta:\t");
	scanf("%d", &num);
	fflush(stdin);
	printf("Digite a senha de numeros:\t");
    int i =0;
	for(i = 0; i < 4; i++){

        senhan1[i] = getch()-48;
        printf("*");
	}

	fflush(stdin);
	printf("\n");
	printf("Digite a senha de letras:\t");
	for(i = 0; i < 3; i++){
        int temp = getch();
        senhal[i] = temp;
        printf("*");
	}
	senhal[i] = '\0';
	printf("\n");
	fflush(stdin);

	for(cont=0;cont<quant_clientes;cont++){
		if(num == dados[cont].id){
         //   printf("compare %d ", comparePwdNum(senhan1, dados[cont].senha_num));
			if(comparePwdNum(senhan1, dados[cont].senha_num,4) == 1){

				if(strcmp(senhal,dados[cont].senha_let)==0){
					int op = 100;
					while(op!=sair_conta){
						cabecalho();
						printf("%d Fazer deposito \n", fazer_deposito);
						printf("%d Fazer saque \n", fazer_saque);
                        printf("%d Tirar extrato \n", extrato_conta);
						printf("%d Fazer transferencia \n", fazer_transferencia);
						printf("%d Fazer emprestimo \n", fazer_emprestimo);
						printf("%d Fazer cambio \n", fazer_cambio);
						printf("%d Fazer invesmento \n", fazer_investimento);
						printf("%d Visualizar rendimentos \n", visualizar_ganhos);
                        printf("%d Mostrar saldo em Euro ou Dolar \n",mostrar_moeda);
						printf("%d sair \n", sair_conta);
						scanf("%d", &op);
                        double valor;

						switch(op){
							case fazer_deposito:
                                cabecalho();
                                printf("Qual valor do deposito:\t");
                                scanf("%lf", &valor);
								depositar(num, valor);
                                getch();
								break;
							case fazer_saque:
                                cabecalho();
								printf("Digite o valor:\t");
								scanf("%lf", &valor);

								if(saque(num, valor)){
                                    cabecalho();
                                    printf("\nSaque efetuado com sussesso!\n");
								}else{
                                    cabecalho();
                                    printf("\nErro ao efetuar o saque\n");
								}
                                getch();
								break;

                            case extrato_conta:
                                cabecalho();
                                extratoConta(num);
                                getch();
                                break;
							case fazer_transferencia:
                                cabecalho();
								printf("\n Digite a conta que deseja transferir:\t");
								scanf("%d", &num2);
								printf("\n Digite o valor a ser transferido:\t");
								scanf("%lf",&valor);
								if(transferencia(num, num2,valor)){
                                    printf("\n Transferencia concluida com sussesso\n");
								}else{
                                    printf("\n Erro ao transferir\n");
								}
                                getch();
								break;
							case fazer_emprestimo:
                                cabecalho();
                                printf("Digite o valor do emprestimo:\t");
                                scanf("%lf", &valor);

								emprestimo(num, valor);
                                getch();
								break;
							case fazer_cambio:
								cabecalho();
								cambio(num);
                                getch();
								break;
							case fazer_investimento:
                                cabecalho();
                                printf("Digite o valor a ser investido: ");
                                scanf("%lf",&valor);
                                system("cls");
                                if(investir(num,valor)){
                                    printf("\nInvestimento feito com susseso!\n");

                                }else{
                                    printf("\nErro ao investir\n");
                                }
                                    getch();

								break;
                            case visualizar_ganhos:
                                cabecalho();
                                rendimento_valor(num, 1.5);
                                getch();
                                break;
                            case mostrar_moeda:
                                cabecalho();
                                mostra_cambio(num);
                                getch();
                                break;
							case sair_conta:

								break;
							default:
								cabecalho();
								printf("Comando invalido....");
								break;
						}
					}
				}
			}
		}
	}
}

void entrar_adm(){
	int num, senhan1[8], cont;
	char senhal[6], senhapermanente[4] = {'l','k','\0'};

	cabecalho();
	printf("Digite ID Administrador:\t");
	scanf("%d", &num);
    int i =0;
    printf("Digite a senha de numeros:\t");
	for(i = 0; i < 8; i++){

        senhan1[i] = getch()-48;
        printf("*");
	}

	fflush(stdin);
	printf("\n");
	printf("Digite a senha de letras:\t");
	for(i = 0; i < 2; i++){
        int temp = getch();
        senhal[i] = temp;
        printf("*");
	}
	senhal[i] = '\0';
	printf("\n");


	fflush(stdin);

	fflush(stdin);
    int senha[] = {2,4,1,1,2,0,1,7};
	for(cont=0;cont<quant_clientes;cont++){
		if(num == 15122017){

			if(comparePwdNum(senhan1, senha,8) ==1){
                    printf("passou");

				if(strcmp(senhal,senhapermanente)==0){
					int op = 100;
					while(op!=sair){

						cabecalho();

						printf("%d Listar conta \n", listar_conta);
						printf("%d Pesquisar conta \n", pesquisar_conta);
						printf("%d Sair \n", sair_conta_adm);
						scanf("%d", &op);

						switch(op){
							case listar_conta:
                                cabecalho();
								listar();
								break;

							case pesquisar_conta:
								cabecalho();
								pesquisar();
								break;

							case sair_conta_adm:
								break;

							default:
								printf("Comando invalido....");
								break;
						}
					}
				}
			}
		}
	}
}



