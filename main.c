#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct
    {
        char Nome[50];
        int Codigo;
        double Saldo;

    }Cliente;

typedef struct 
    {
        int CodCliente_origem;
        int CodCliente_destino;
        int TipoOp;
        double valor;      
    }Operacoes;

void cadastraCliente(char Nome[],double saldo);
void selecionaCliente(void);
void DepositaFundos(int codigo,double valor);
void SacarFundos(int codigo,double valor);
void TransfereFundos(int codOrigem,int codDestino,double valor);
void MostraSaldo(int codigo);
void MostraExtrato(int codCliente);
int excluiCliente(int codCliente);
void salvaDados(void);
void Menu();

// Funções de suporte
void addClienteLiasta(Cliente cliente);
void printCliente(Cliente cliente);
void lerClientes(void);
void lerOperacoes(void);
void carregaCliente(char Nome[],double saldo,int codigo);
void novaOperacao(int tipoOper,int origem,int destino,int valor);
void limpa_stadin(void);
int acharCliente(int codigo);

//Variaveis globais
int quantClientes;
int quantOperacoes=0 ;
Cliente ListaClientes[200];
Operacoes HistoricoOperacoes[200];
int clienteCont=0;

int main(void) {
    lerClientes();
    lerOperacoes();
    Menu();
    salvaDados();
  return 0;
}
void limpa_stadin(void) {
    int c = getchar();

    while (c != '\n' && c != EOF)
        c = getchar();
}
int acharCliente(int codigo){
    for(int index=0; index<quantClientes; index++){
        if(ListaClientes[index].Codigo == codigo){
            return index;
        }

    }
    printf("Cliente Não Encontrado!!!\n");
    return -1;

}
void addClienteLista(Cliente cliente){
    ListaClientes[quantClientes] = cliente;
    quantClientes++;
}
void printCliente(Cliente cliente){
    printf("\n-->Nome jCompleto: %s",cliente.Nome);
    printf("\n-->Código: %d",cliente.Codigo);
    printf("\n-->Saldo: R$ %f \n",cliente.Saldo);
}
void printOperacao(Operacoes op){
    printf("#######################################\n");
    printf("-->Tipo de operação: %d\n",op.TipoOp);
    printf("-->Origem: %d\n",op.CodCliente_origem);
    printf("-->Destino: %d\n",op.CodCliente_destino);
    printf("-->Valor:R$ %lf\n",op.valor);
    printf("#######################################\n");
}
void lerClientes(void){
    int len = 255;
    char buffer[len];
    int maiorCod = 0;;
    double saldo;
    FILE *file;
    if(!(file= fopen("clientes.txt", "r"))){
        printf("clientes.txt não encontrado!!!!\n");
        return;
    }
    //lê cada linha do arquio clientes.txt
    while(fgets(buffer,len,file)){
        int index = 0,codigo,etapas=0,nomeIndex = 0;
        char nome[100],strSaldo[100],strCodigo[10];
        //enquanto não for lido um caracter ; fazemos o loop que coverte 
        //os caracteres em suas respectivas variaveis 
        // strSaldo -> double Saldo;
        // strCodigo -> int codigo;
        while(buffer[index] != ';'){
            if(buffer[index] == ' '){
                //caso o caracter for um espaço significa que chegamos aos
                //ao fim de um parametro e vamos nos preparar para o 
                //proximo
                nomeIndex = 0 ;
                etapas++;
            }
            else{
                if(etapas==0){
                    //etapa 0 é responsavel por ler e salvar o nome
                    nome[nomeIndex] = buffer[index];
                    nomeIndex++;
                }
                else if(etapas == 1){
                    //etapa 1 ler e salvar o codigo do cliente que depois
                    // será convertido para double
                    strCodigo[nomeIndex] = buffer[index];
                    nomeIndex++;
                   
                    }
                else{
                    //le os caracteres restatantes que representam o saldo
                    strSaldo[nomeIndex] = buffer[index];
                    nomeIndex++;
                }
            }
            index++;
            }
        sscanf(strSaldo,"%lf",&saldo);
        codigo = atoi(strCodigo);

        carregaCliente(nome,saldo,codigo);

        //reseta os arrays 
        memset(nome,0,sizeof(nome));
        memset(strCodigo,0,sizeof(strCodigo));
    }
    fclose(file);
    printf("Clientes carregados com sucesso!!\n");
}
void lerOperacoes(){
    int len = 255;
    char buffer[len];
    FILE *file;
    if(!(file = fopen("historico.txt","r"))){
        printf("Historico não encontrado!!!!");
        return;
    }
    while(fgets(buffer,len,file)){
       int index = 0,origem,tipo,destino,etapas=0,valorIndex=0;
       char tipoOp,codigoOrigem,codigoDestino,valor[100];
       double valorOp;
       while(buffer[index] != ';'){
            if(buffer[index] == ' '){
                etapas++;
            }
            else{
                if(etapas == 0){
                    tipoOp= buffer[index];
                }
                else if(etapas == 1){
                    codigoOrigem =buffer[index];
                }
                else if(etapas == 2){
                    codigoDestino = buffer[index];
                }
                else{
                    valor[valorIndex] = buffer[index];
                    valorIndex++;
                }
            }
       index++;
       }
       sscanf(valor,"%lf",&valorOp);
       tipo = (int)tipoOp - 48;
       origem = (int)codigoOrigem-48;
       destino = (int)codigoDestino-48;
       novaOperacao(tipo,origem,destino,valorOp);
       memset(valor,0,sizeof(valor));
    }
    fclose(file);
    printf("Histórico de operações carregados com sucesso!!\n");
}
void carregaCliente(char Nome[],double saldo,int codigo){
    // Função responsavel por fazer a criação dos clientes
    // Salvos no arquivo clientes.txt 
    if(codigo>clienteCont){
        clienteCont = codigo;
    }
    Cliente novoCliente;

    novoCliente.Saldo = saldo;
    novoCliente.Codigo = codigo;
    strcpy(novoCliente.Nome,Nome);

    ListaClientes[quantClientes] = novoCliente;
    quantClientes++;
}
void novaOperacao(int tipoOper,int origem,int destino,int valor){

    Operacoes operacao;
    operacao.CodCliente_origem = origem;
    operacao.CodCliente_destino = destino;
    operacao.TipoOp = tipoOper;
    operacao.valor = valor;

    HistoricoOperacoes[quantOperacoes] = operacao; 
    quantOperacoes++;
}
void cadastraCliente(char Nome[],double saldo){

    Cliente novoCliente;

    novoCliente.Saldo = saldo;
    novoCliente.Codigo = ++clienteCont;
    strcpy(novoCliente.Nome,Nome);

    addClienteLista(novoCliente);
    novaOperacao(0,novoCliente.Codigo, novoCliente.Codigo,novoCliente.Saldo);
    printCliente(novoCliente);
}
void MostraSaldo(int codigo){
    int clienteIndex = acharCliente(codigo);
    printf("-->Cliente: %s\n",ListaClientes[clienteIndex].Nome);
    printf("-->Saldo atual:R$ %lf\n",ListaClientes[clienteIndex].Saldo);

}
void TodasOperacoes(void){
    for(int i=0;i<quantOperacoes;i++){
        printOperacao(HistoricoOperacoes[i]);
    }
}
void DepositaFundos(int codigo,double valor){
    int clienteIndex = acharCliente(codigo);
    if(valor<0){
        printf("-->Valor digitado é invalido ou incorreto Tente novamente!!\n");
        return;
    }
    Cliente cliente = ListaClientes[clienteIndex];
    cliente.Saldo += valor;
    ListaClientes[clienteIndex] = cliente;
    novaOperacao(1, codigo, codigo,valor);
}
void SacarFundos(int codigo,double valor){
    int clienteIndex = acharCliente(codigo);
    if(valor<0 ||(ListaClientes[clienteIndex].Saldo-valor)<0 ){
        printf("-->Valor digitado é invalido ou Saldo insuficiente Tente novamente!!\n");
        return;
    }
    ListaClientes[clienteIndex].Saldo -= valor;
    novaOperacao(2, ListaClientes[clienteIndex].Codigo, ListaClientes[clienteIndex].Codigo,valor);
    MostraSaldo(codigo);
}
void TransfereFundos(int codOrigem,int codDestino,double valor){

    int clienteOrigemIndex = acharCliente(codOrigem);
    int clienteDestinoIndex = acharCliente(codDestino);
    int comfirme,result;

    if(clienteDestinoIndex ==-1 ){
        printf("-->Código de cliente invalido ou incorreto\n");
        return;
    }
    if(valor<0 ||(ListaClientes[clienteOrigemIndex].Saldo-valor)<0 ){
        printf("-->Valor digitado é invalido ou Saldo insuficiente Tente novamente!!\n");
        return;
    }
    printf("-->Cliente de origem: \n");
    printf("Nome Completo: %s\nCódigo: %d\n",ListaClientes[clienteOrigemIndex].Nome,ListaClientes[clienteOrigemIndex].Codigo);

    printf("-->Cliente de destino: \n");
    printf("Nome Completo: %s\nCódigo: %d\n",ListaClientes[clienteDestinoIndex].Nome,ListaClientes[clienteDestinoIndex].Codigo);

    printf("-->Confirma transação?(1-Sim/2-Não): ");
    result=scanf(" %d",&comfirme);
    if(result==0){
        limpa_stadin();
        }

    if(comfirme!=1){
        printf("transação cancelada!!!");
        return;
    }

    ListaClientes[clienteDestinoIndex].Saldo += valor;
    ListaClientes[clienteOrigemIndex].Saldo -= valor;
    printf("Transação realizada com sucesso!!!\n");
    novaOperacao(3, codOrigem,codDestino,valor);

}
int excluiCliente(int codCliente){
    int clienteIndex = acharCliente(codCliente);
    Cliente cliente = ListaClientes[clienteIndex];
    if(ListaClientes[clienteIndex].Saldo>0){
        printf("-->O saldo da conta precisa estar zerado\n");
        return -1;
    }
    //remove o Cliente da ListaClientes movendo todos os cliente um index para a esquerda
    // ou seja mover o proximo cliente para o index atual até o ultimo cliente 
    for(int i=clienteIndex; i<quantClientes-1; i++){
            ListaClientes[i] = ListaClientes[i + 1];
        }
    quantClientes--;
    for(int index =0;index<quantClientes;index++){
        printf("%s\n",ListaClientes[index].Nome);
    }
    printf("-->Cliente %s excluido com sucesso\n",cliente.Nome);
    novaOperacao(4, codCliente, codCliente, 0);
    return 1;
}
void mostraExtrato(int codCliente){
    printf("-->Operações de %s\n",ListaClientes[acharCliente(codCliente)].Nome);
    for(int index=0;index<quantOperacoes;index++){
        if(HistoricoOperacoes[index].CodCliente_origem == codCliente ||HistoricoOperacoes[index].CodCliente_destino == codCliente){
            printOperacao(HistoricoOperacoes[index]);
        }
    }
    printf("-->Fim das Operações\n");
}
void selecionaCliente(void){

    int codigoCliente = -1,opcao = -1,clienteIndex = -1,result;
    double valor;

    do{
        printf("-->Digite um código de cliente valido: ");
        result=scanf(" %d",&codigoCliente);
        if(result==0){
            limpa_stadin();
        }
        clienteIndex = acharCliente(codigoCliente);
    }while(codigoCliente<0 || clienteIndex == -1);
    do{
        valor = 0;
        printf("Menu\n");
        printf("1. Depositar fundos\n2. Transferir fundos\n3. Sacar fundos\n4. Mostrar saldo\n5. Mostrar extrato\n6. Excluir cliente\n7. Voltar ao menu anterior\nOpção: ");
        result=scanf(" %d",&opcao);
        if(result==0){
            limpa_stadin();
        }
        switch(opcao){
            case 1:
                printf("-->Digite um valor positivo e diferente de zero\nvalor: R$ ");
                result=scanf(" %lf",&valor);
                if(result==0){
                    limpa_stadin();
                }
                DepositaFundos(codigoCliente,valor);
                break;
            case 2:

                printf("-->Digite o codigo da conta de destino da transferencia");
                int codDestino;
                result=scanf(" %d",&codDestino);
                if(result==0){
                    limpa_stadin();
                }
                printf("-->Digite um valor positivo e diferente de zero\n-->valor: R$ ");
                result=scanf(" %lf",&valor);
                if(result==0){
                    limpa_stadin();
                }
                TransfereFundos(codigoCliente,codDestino,valor);
                break;  
            case 3:
                printf("-->Digite um valor positivo e diferente de zero\nvalor: R$ ");
                result=scanf(" %lf",&valor);
                 if(result==0){
                    limpa_stadin();
                }
                SacarFundos(codigoCliente,valor);
                break;
            case 4:
                MostraSaldo(codigoCliente);
                break;
            case 5:
                mostraExtrato(codigoCliente);
                break;
            case 6:
                if(excluiCliente(codigoCliente)!= -1){
                opcao = 7;
                }
                break;
        }
        
    }while(opcao !=7);
}
void salvaDados(void){
    FILE *file = fopen("clientes.txt","w");
    for(int index=0;index<quantClientes;index++){
            fprintf(file,"%s %d %f;\n",ListaClientes[index].Nome,ListaClientes[index].Codigo,ListaClientes[index].Saldo);
    
    }
    fclose(file);
    file = fopen("historico.txt","w");
    Operacoes op;
    for(int index=0;index<quantOperacoes;index++){
        op = HistoricoOperacoes[index];
        fprintf(file,"%d %d %d %lf;\n",op.TipoOp,op.CodCliente_origem,op.CodCliente_destino,op.valor);
    
    }
    fclose(file);
    printf("-->Dados salvos com sucesso!!!");
}
void Menu(void){
    int opcao,result;
    do{
        printf("\nMenu\n");
        printf("1. Cadastra Cliente\n2. Seleciona Cliente\n3. Histórico de todas operações\n4. Salvar Dados\n9. Sair\n");
        printf("Opção: ");
        result = scanf(" %d",&opcao);
        if(result==0){
            limpa_stadin();
        }
        switch(opcao){
            case 1:
                printf("-->Digite seu nome: ");
                char Nome[100];
                double saldo = 0;
                result = scanf(" %s",Nome);
                if(result==0){
                    limpa_stadin();
                }
                while(saldo<=0){
                    printf("-->Digite um saldo que seja positivo e diferente de zero \n-->Digite seu saldo: ");
                    result=scanf(" %lf",&saldo);
                    if(result==0){
                        limpa_stadin();
                }
                };
                cadastraCliente(Nome,saldo);
                break;
            case 2:
                selecionaCliente();
                break;
            case 3:
                TodasOperacoes();
                break;
            case 4:
                salvaDados();  
                break; 
 
        }
        printf("\n");
    }
    while(opcao != 9);
}