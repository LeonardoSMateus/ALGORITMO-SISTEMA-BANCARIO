#include<time.h>

struct Cliente_Info{ // struct para armazenar as informações de cada cliente
	int id;
	char nome[100];
	int data_nascimento;
	double saldo_inicial;
	double saldo_euro;
	double saldo_dolar;
	struct tm data_investimento;
	double valor_investido;
	int senha_num[4];
	char senha_let[4];
} typedef Cliente;

typedef enum{ // enumeração para menu principal
	criar_conta=1,
	login,
	login_adm,
	sair
}opcoes;

typedef enum{ // enumerando as funçoes apos o usuario fazer sua login em sua conta
	fazer_deposito=1,
	fazer_saque,
	extrato_conta,
	fazer_transferencia,
	fazer_emprestimo,
	fazer_cambio,
	fazer_investimento,
	visualizar_ganhos,
	mostrar_moeda,
	sair_conta
}opcao_conta;

typedef enum{
	listar_conta=1,
	pesquisar_conta,
	sair_conta_adm
}opcao_adm;

void atualizarDados(Cliente* clientes, int quant);

int achar_cliente(int id);

void depositar(int nconta, double valor);

void aplicacao();

void entrar();

int transferencia(int id_a, int id_b, double valor);

void emprestimo(int id_a, double valor);

void cambio(int id_a);

int validar_id(int id);

//felipe

Cliente* ler_dados();

void criar_cliente_lista();

void mostrar_cliente(int id);

int inserir();

int investir(int id, double valor);

int modificar_cliente(int id);

void mostrar_clientes();

int deixar();

double saldo(int id);

int saque(int id, double valor);

void rendimento_valor(int id,  double juros);

void extratoConta(int id);

void listar();

void pesquisar();

void cabecalho();

void entrar_adm();

