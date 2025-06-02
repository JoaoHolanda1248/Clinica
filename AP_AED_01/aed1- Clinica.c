#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

// ENUMs

//Enumeração para especialidades dos médicos cadastrados
typedef enum{
    CLINICO,
    PEDIATRA,
    CARDIOLOGISTA,
    ORTOPEDISTA,
    GINECOLOGISTA,
    NEUROLOGISTA,
    DERMATOLOGISTA,
    PSIQUIATRA,
    OFTALMOLOGISTA,
    ENDOCRINOLOGISTA
} Esp;

//Enumerações para o status das consultas cadastradas
typedef enum{
    AGENDADA,
    REALIZADA,
    CANCELADA,
    ADIADA,
    REMARCADA
} Status;

// Structs

//Struct para médicos
typedef struct{
    char nome[100];
    char crm[20];
    Esp esp;
    char tel[20];
} Med;

//Struct para pacientes
typedef struct{
    char nome[100];
    char cpf[15];
    char tel[20];
} Pac;

//Struct para consultas
typedef struct{
    int id;
    char cpfPac[15];
    char crmMed[20];
    char data[11];
    char hora[6];
    Status stat;
} Cons;

// Variáveis globais
Med* meds = NULL;
int qtdMeds = 0;

Pac* pacs = NULL;
int qtdPacs = 0;

Cons* cons = NULL;
int qtdCons = 0;

// Funções para strings enums
const char* getEsp(Esp e){
    const char* espStr[] = {
        "CLINICO","PEDIATRA","CARDIOLOGISTA","ORTOPEDISTA","GINECOLOGISTA",
        "NEUROLOGISTA","DERMATOLOGISTA","PSIQUIATRA","OFTALMOLOGISTA","ENDOCRINOLOGISTA"
    };
    return espStr[e];
}

const char* getStat(Status s){
    const char* statStr[] = {
        "AGENDADA","REALIZADA","CANCELADA","ADIADA","REMARCADA"
    };
    return statStr[s];
}

// Função para converter string para Esp
Esp strToEsp(const char* str){
    for(int i = 0; i <= ENDOCRINOLOGISTA; i++){
        if(strcmp(str, getEsp(i)) == 0)
            return i;
    }
    return CLINICO; // padrão se não achar
}

// Função para converter string para Status
Status strToStat(const char* str){
    for(int i = 0; i <= REMARCADA; i++){
        if(strcmp(str, getStat(i)) == 0)
            return i;
    }
    return AGENDADA; // padrão
}

// --- SALVAR E CARREGAR ARQUIVOS ---

// Função para salvar os médicos
void salvarMeds(){
    FILE* f = fopen("medicos.txt","w");
    if(!f){
        printf("Erro ao salvar medicos.\n");
        return;
    }
    for(int i=0; i<qtdMeds; i++){
        fprintf(f, "%s|%s|%s|%s\n", meds[i].nome, meds[i].crm, meds[i].tel, getEsp(meds[i].esp));
    }
    fclose(f);
}

// Função para carregar médicos salvos
void carregarMeds(){
    FILE* f = fopen("medicos.txt","r");
    if(!f) return; // se não existir, ignora

    char linha[256];
    while(fgets(linha, sizeof(linha), f)){
        meds = realloc(meds, (qtdMeds+1)*sizeof(Med));
        Med* m = &meds[qtdMeds];
        // formato: nome|crm|tel|esp\n
        char* token = strtok(linha, "|");
        if(token) strcpy(m->nome, token);
        token = strtok(NULL, "|");
        if(token) strcpy(m->crm, token);
        token = strtok(NULL, "|");
        if(token) strcpy(m->tel, token);
        token = strtok(NULL, "|\n");
        if(token) m->esp = strToEsp(token);

        qtdMeds++;
    }
    fclose(f);
}

// Funções para salvar pacientes
void salvarPacs(){
    FILE* f = fopen("pacientes.txt","w");
    if(!f){
        printf("Erro ao salvar pacientes.\n");
        return;
    }
    for(int i=0; i<qtdPacs; i++){
        fprintf(f, "%s|%s|%s\n", pacs[i].nome, pacs[i].cpf, pacs[i].tel);
    }
    fclose(f);
}

//Função para carregar os pacientes cadastrados
void carregarPacs(){
    FILE* f = fopen("pacientes.txt","r");
    if(!f) return;

    char linha[256];
    while(fgets(linha, sizeof(linha), f)){
        pacs = realloc(pacs, (qtdPacs+1)*sizeof(Pac));
        Pac* p = &pacs[qtdPacs];
        // formato: nome|cpf|tel\n
        char* token = strtok(linha, "|");
        if(token) strcpy(p->nome, token);
        token = strtok(NULL, "|");
        if(token) strcpy(p->cpf, token);
        token = strtok(NULL, "|\n");
        if(token) strcpy(p->tel, token);

        qtdPacs++;
    }
    fclose(f);
}

//Função para salvar consultas
void salvarCons(){
    FILE* f = fopen("consultas.txt","w");
    if(!f){
        printf("Erro ao salvar consultas.\n");
        return;
    }
    for(int i=0; i<qtdCons; i++){
        fprintf(f, "%d|%s|%s|%s|%s|%s\n", cons[i].id, cons[i].cpfPac, cons[i].crmMed, cons[i].data, cons[i].hora, getStat(cons[i].stat));
    }
    fclose(f);
}

//Função para carregar consultas
void carregarCons(){
    FILE* f = fopen("consultas.txt","r");
    if(!f) return;

    char linha[256];
    while(fgets(linha, sizeof(linha), f)){
        cons = realloc(cons, (qtdCons+1)*sizeof(Cons));
        Cons* c = &cons[qtdCons];
        char* token = strtok(linha, "|");
        if(token) c->id = atoi(token);
        token = strtok(NULL, "|");
        if(token) strcpy(c->cpfPac, token);
        token = strtok(NULL, "|");
        if(token) strcpy(c->crmMed, token);
        token = strtok(NULL, "|");
        if(token) strcpy(c->data, token);
        token = strtok(NULL, "|");
        if(token) strcpy(c->hora, token);
        token = strtok(NULL, "|\n");
        if(token) c->stat = strToStat(token);

        qtdCons++;
    }
    fclose(f);
}

// Funções existentes...

// Função para cadastrar os médicos
void cadMed(){
    meds = realloc(meds, (qtdMeds+1)*sizeof(Med));
    Med* m = &meds[qtdMeds];

    printf("Nome: "); 
    scanf(" %[^\n]", m->nome);
    printf("CRM: "); 
    scanf(" %s", m->crm);
    printf("Telefone: "); 
    scanf(" %s", m->tel);
    printf("Especialidade:\n");
    printf("0-CLINICO | 1-PEDIATRA | 2-CARDIOLOGISTA | 3-ORTOPEDISTA | 4-GINECOLOGISTA\n");
    printf("5-NEUROLOGISTA | 6-DERMATOLOGISTA | 7-PSIQUIATRA | 8-OFTALMOLOGISTA | 9-ENDOCRINOLOGISTA\n");
    printf("Escolha: ");
    int e; scanf("%d", &e);
    m->esp = e;

    qtdMeds++;
    printf("Medico cadastrado com sucesso!\n");
}

// Função para deletar médicos
void delMed(){
    char crm[20];
    printf("Informe o CRM do medico a deletar: ");
    scanf(" %s", crm);

    for(int i=0; i<qtdMeds; i++){
        if(strcmp(meds[i].crm, crm) == 0){
            for(int j=i; j<qtdMeds-1; j++){
                meds[j] = meds[j+1];
            }
            qtdMeds--;
            meds = realloc(meds, qtdMeds*sizeof(Med));
            printf("Medico removido com sucesso.\n");
            return;
        }
    }
    printf("Medico nao encontrado.\n");
}

// Função para edição de médicos
void editMed() {
    char crm[20];
    printf("Informe o CRM do medico a editar: ");
    scanf(" %s", crm);

    for (int i = 0; i < qtdMeds; i++) {
        if (strcmp(meds[i].crm, crm) == 0) {
            printf("Medico encontrado. Informe os novos dados:\n");

            printf("Novo nome: ");
            scanf(" %[^\n]", meds[i].nome);

            printf("Novo CRM: ");
            scanf(" %s", meds[i].crm); // Caso não deseje permitir alteração do CRM, comente esta linha

            printf("Novo telefone: ");
            scanf(" %s", meds[i].tel);

            printf("Nova especialidade:\n");
            printf("0-CLINICO | 1-PEDIATRA | 2-CARDIOLOGISTA | 3-ORTOPEDISTA | 4-GINECOLOGISTA\n");
            printf("5-NEUROLOGISTA | 6-DERMATOLOGISTA | 7-PSIQUIATRA | 8-OFTALMOLOGISTA | 9-ENDOCRINOLOGISTA\n");
            printf("Escolha: ");
            int e; 
            scanf("%d", &e);
            meds[i].esp = e;

            printf("Medico atualizado com sucesso.\n");
            return;
        }
    }

    printf("Medico nao encontrado.\n");
}

// Função para listar os médicos
void listMed() {
    if (qtdMeds == 0) {
        printf("Nenhum medico cadastrado.\n");
        return;
    }

    printf("Lista de Medicos:\n");
    for (int i = 0; i < qtdMeds; i++) {
        printf("Nome: %s\n", meds[i].nome);
        printf("CRM: %s\n", meds[i].crm);
        printf("Telefone: %s\n", meds[i].tel);
        printf("Especialidade: ");
        switch (meds[i].esp) {
            case 0: printf("CLINICO\n"); break;
            case 1: printf("PEDIATRA\n"); break;
            case 2: printf("CARDIOLOGISTA\n"); break;
            case 3: printf("ORTOPEDISTA\n"); break;
            case 4: printf("GINECOLOGISTA\n"); break;
            case 5: printf("NEUROLOGISTA\n"); break;
            case 6: printf("DERMATOLOGISTA\n"); break;
            case 7: printf("PSIQUIATRA\n"); break;
            case 8: printf("OFTALMOLOGISTA\n"); break;
            case 9: printf("ENDOCRINOLOGISTA\n"); break;
            default: printf("DESCONHECIDA\n"); break;
        }
        printf("-------------------------\n");
    }
}

// Função cadastrar os pacientes
void cadPac(){
    pacs = realloc(pacs, (qtdPacs+1)*sizeof(Pac));
    Pac* p = &pacs[qtdPacs];

    printf("Nome: "); 
    scanf(" %[^\n]", p->nome);
    printf("CPF: "); 
    scanf(" %s", p->cpf);
    printf("Telefone: "); 
    scanf(" %s", p->tel);

    qtdPacs++;
    printf("Paciente cadastrado com sucesso!\n");
}

// Função para deletar os pacientes 
void delPac(){
    char cpf[15];
    printf("Informe o CPF do paciente a deletar: ");
    scanf(" %s", cpf);

    for(int i=0; i<qtdPacs; i++){
        if(strcmp(pacs[i].cpf, cpf) == 0){
            for(int j=i; j<qtdPacs-1; j++){
                pacs[j] = pacs[j+1];
            }
            qtdPacs--;
            pacs = realloc(pacs, qtdPacs*sizeof(Pac));
            printf("Paciente removido com sucesso.\n");
            return;
        }
    }
    printf("Paciente nao encontrado.\n");
}

// Função para editar os pacientes
void editPac() {
    char cpf[15];

    printf("Informe o CPF do paciente a editar: ");
    scanf(" %s", cpf);

    for(int i = 0; i < qtdPacs; i++) {
        if(strcmp(pacs[i].cpf, cpf) == 0) {
            printf("Paciente encontrado. Informe os novos dados:\n");

            printf("Novo nome: ");
            scanf(" %[^\n]", pacs[i].nome);

            printf("Novo CPF: ");
            scanf(" %s", pacs[i].cpf);

            printf("Novo telefone: ");
            scanf(" %s", pacs[i].tel);

            printf("Paciente atualizado com sucesso.\n");
            return;
        }
    }

    printf("Paciente nao encontrado.\n");
}

// Função para listar os pacientes
void listPac() {
    if (qtdPacs == 0) {
        printf("Nenhum paciente cadastrado.\n");
        return;
    }

    printf("Lista de Pacientes:\n");
    for (int i = 0; i < qtdPacs; i++) {
        printf("Nome: %s\n", pacs[i].nome);
        printf("CPF: %s\n", pacs[i].cpf);
        printf("Telefone: %s\n", pacs[i].tel);
        printf("-------------------------\n");
    }
}

// Função para agendar consultas
void agCons(){
    cons = realloc(cons, (qtdCons+1)*sizeof(Cons));
    Cons* c = &cons[qtdCons];

    c->id = (qtdCons > 0) ? cons[qtdCons-1].id + 1 : 1; // incremento do ID
    listPac();
    printf("CPF do Paciente: "); 
    scanf(" %s", c->cpfPac);
    listMed();
    printf("CRM do Medico: "); 
    scanf(" %s", c->crmMed);
    printf("Data (DD/MM/AAAA): "); 
    scanf(" %s", c->data);
    printf("Hora (HH:MM): "); 
    scanf(" %s", c->hora);
    c->stat = AGENDADA;

    qtdCons++;
    printf("Consulta agendada com sucesso!\n");
}

// Função para deletar consultas
void delCons(){
    int id;
    printf("Informe o ID da consulta a deletar: ");
    scanf("%d", &id);

    for(int i=0; i<qtdCons; i++){
        if(cons[i].id == id){
            for(int j=i; j<qtdCons-1; j++){
                cons[j] = cons[j+1];
            }
            qtdCons--;
            cons = realloc(cons, qtdCons*sizeof(Cons));
            printf("Consulta removida com sucesso.\n");
            return;
        }
    }
    printf("Consulta nao encontrada.\n");
}

// Função para alterar o status da consulta
void altStatCons(){
    int id;
    printf("Informe o ID da consulta: ");
    scanf("%d", &id);

    for(int i=0; i<qtdCons; i++){
        if(cons[i].id == id){
            printf("Status atual: %s\n", getStat(cons[i].stat));
            printf("Escolha novo status:\n");
            printf("0-AGENDADA | 1-REALIZADA | 2-CANCELADA | 3-ADIADA | 4-REMARCADA\n");
            int s; 
            scanf("%d", &s);
            cons[i].stat = s;
            printf("Status atualizado com sucesso!\n");
            return;
        }
    }
    printf("Consulta nao encontrada.\n");
}

// Função para listar as consultas
void listCons() {
    if (qtdCons == 0) {
        printf("Nenhuma consulta agendada.\n");
        return;
    }

    printf("Lista de Consultas:\n");
    for (int i = 0; i < qtdCons; i++) {
        printf("ID: %d\n", cons[i].id);
        printf("CPF do Paciente: %s\n", cons[i].cpfPac);
        printf("CRM do Medico: %s\n", cons[i].crmMed);
        printf("Data: %s\n", cons[i].data);
        printf("Hora: %s\n", cons[i].hora);
        printf("Status: %s\n", getStat(cons[i].stat));
        printf("-------------------------\n");
    }
}

// Relatórios

// Relatório por pacientes
void relPac(const char* cpf){
    printf("Consultas do paciente %s:\n", cpf);
    for(int i=0; i<qtdCons; i++){
        if(strcmp(cons[i].cpfPac, cpf) == 0){
            printf("ID: %d | Medico: %s | Data: %s | Hora: %s | Status: %s\n",
                cons[i].id, cons[i].crmMed, cons[i].data,
                cons[i].hora, getStat(cons[i].stat));
        }
    }
}

// Relatório por médicos
void relMed(const char* crm){
    printf("Consultas do medico %s:\n", crm);
    for(int i=0; i<qtdCons; i++){
        if(strcmp(cons[i].crmMed, crm) == 0){
            printf("ID: %d | Paciente: %s | Data: %s | Hora: %s | Status: %s\n",
                cons[i].id, cons[i].cpfPac, cons[i].data,
                cons[i].hora, getStat(cons[i].stat));
        }
    }
}

// Relatório por especialidade
void relEsp(){
    printf("Escolha especialidade:\n");
    printf("0-CLINICO | 1-PEDIATRA | 2-CARDIOLOGISTA | 3-ORTOPEDISTA | 4-GINECOLOGISTA\n");
    printf("5-NEUROLOGISTA | 6-DERMATOLOGISTA | 7-PSIQUIATRA | 8-OFTALMOLOGISTA | 9-ENDOCRINOLOGISTA\n");
    int e; scanf("%d", &e);

    printf("Consultas da especialidade %s:\n", getEsp(e));
    for(int i=0; i<qtdCons; i++){
        // buscar medico e verificar especialidade
        for(int j=0; j<qtdMeds; j++){
            if(strcmp(cons[i].crmMed, meds[j].crm) == 0 && meds[j].esp == e){
                printf("ID: %d | Paciente: %s | Medico: %s | Data: %s | Hora: %s | Status: %s\n",
                    cons[i].id, cons[i].cpfPac, cons[i].crmMed, cons[i].data, cons[i].hora, getStat(cons[i].stat));
            }
        }
    }
}

// Relatório por data
void relData(){
    char data[11];
    printf("Informe data (DD/MM/AAAA): ");
    scanf(" %s", data);
    printf("Consultas na data %s:\n", data);
    for(int i=0; i<qtdCons; i++){
        if(strcmp(cons[i].data, data) == 0){
            printf("ID: %d | Paciente: %s | Medico: %s | Hora: %s | Status: %s\n",
                cons[i].id, cons[i].cpfPac, cons[i].crmMed, cons[i].hora, getStat(cons[i].stat));
        }
    }
}

// Relatório por status da consulta
void relStatus(){
    printf("Escolha status:\n");
    printf("0-AGENDADA | 1-REALIZADA | 2-CANCELADA | 3-ADIADA | 4-REMARCADA\n");
    int s; scanf("%d", &s);
    printf("Consultas com status %s:\n", getStat(s));
    for(int i=0; i<qtdCons; i++){
        if(cons[i].stat == s){
            printf("ID: %d | Paciente: %s | Medico: %s | Data: %s | Hora: %s\n",
                cons[i].id, cons[i].cpfPac, cons[i].crmMed, cons[i].data, cons[i].hora);
        }
    }
}

int main(){
    
    SetConsoleOutputCP(CP_UTF8);

    // Chamando as funções para carregar os objetos salvos
    carregarMeds();
    carregarPacs();
    carregarCons();

    int op = 0, opcao, opcao2, opcao3;
    // Laço de repetição para o usuário permanecer no código e escolher as operações
    do {
        printf(" _________________________________\n");
        printf("\n     Clinica Médica UFERSA      \n");
        printf(" _________________________________\n");
        printf(" |1 - Gerenciamento de médicos   |\n");
        printf(" |2 - Gerenciamento de pacientes |\n");
        printf(" |3 - Gerenciar consultas        |\n");
        printf(" |4 - Verificar relatórios       |\n");
        printf(" |5 - Sair                       |\n");
        printf(" _________________________________\n");
        printf("   Escolha uma opcao: ");
        scanf("%d", &op);
        switch (op)
        {
        case 1:
            do{
                printf("-- Gerenciamento de Médicos --\n");
                printf("[1] Adicionar médicos\n");
                printf("[2] Atualizar médico\n");
                printf("[3] Remover médico\n");
                printf("[4] Visualizar médicos\n");
                printf("[5] Voltar para o menu principal\n");
                scanf("%d", &opcao);

                switch (opcao)
                {
                case 1: cadMed(); break;

                case 2: listMed(); editMed(); break;

                case 3: listMed(); delMed(); break;

                case 4: listMed(); break;

                case 5: break;

                default:
                    printf("Opção inválida");
                    break;
                }

                
            }while(opcao != 5);
            break;
            

        case 2:
            do{
            printf(" --- Gerenciamento de pacientes --- \n");
            printf("[1] Adicionar paciente\n");
            printf("[2] Atualizar paciente\n");
            printf("[3] Remover paciente\n");
            printf("[4] Visualizar paciente\n");
            printf("[5] Voltar para o menu principal\n");
            scanf("%d", &opcao2);
            switch (opcao2)
            {
                case 1: cadPac(); break;

                case 2: listPac(); editPac(); break;

                case 3: listPac(); delPac(); break;

                case 4: listPac(); break;

                case 5: break;
                
                default:
                    printf("Opção inválida");
                    break;
                }
            }while(opcao2 != 5);
            
            break;
        case 3:
            do
            {
            printf(" --- Gerenciamento de consultas --- \n");
            printf("[1] Agendar consulta\n");
            printf("[2] Cancelar consulta\n");
            printf("[3] Listar consultas\n");
            printf("[4] Atualizar status das consultas\n");
            printf("[5] Voltar para o menu principal\n");
            scanf("%d", &opcao3);
            switch (opcao3)
            {
                case 1: agCons(); break;

                case 2: listCons(); delCons(); break;

                case 3: listCons(); break;

                case 4: listCons(); altStatCons(); break;

                case 5: break;

                default:
                    printf("Opção inválida");
                    break;
                }
                
            } while (opcao3 != 5);
        
            break;

        case 4:
            {
                int r;
                do{
                    printf("\n--- Relatorios ---\n");
                    printf("[1] Por paciente\n");
                    printf("[2] Por medico\n");
                    printf("[3] Por especialidade\n");
                    printf("[4] Por data\n");
                    printf("[5] Por status\n");
                    printf("[6] Voltar para o menu principal\n");
                    printf("Escolha: ");
                    scanf("%d", &r);
                    if(r == 1){
                        listPac();
                        char cpf[15];
                        printf("Informe CPF do paciente: ");
                        scanf(" %s", cpf);
                        relPac(cpf);
                    } else if(r == 2){
                        listMed();
                        char crm[20];
                        printf("Informe CRM do medico: ");
                        scanf(" %s", crm);
                        relMed(crm);
                    } else if(r == 3){
                        relEsp();
                    } else if(r == 4){
                        relData();
                    } else if(r == 5){
                        relStatus();
                    } else if (r == 6){
                        break;
                    } else {
                        printf("Opcao invalida.\n");
                    }
                }while(r != 6);
                
            } break;
        case 5:
            salvarMeds();
            salvarPacs();
            salvarCons();
            printf("Saindo do programa. Dados salvos.\n");
            break;
        default:
            printf("Opção inválida");
            break;
        }

        
    } while(op != 5);

    // Liberação das memórias alocadas para evitar vazamento de memória 
    free(meds);
    free(pacs);
    free(cons);

    return 0;
}
