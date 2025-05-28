#include <stdio.h>
#include <windows.h>

/*Free, igualar a 0/nulo, abrir com a*/
// Parte para cadastrar, editar, excluir e mostrar os médicos

enum Especialidade
{
    CARDIOLOGISTA,
    CLINICO,
    PEDIATRA,
    OFTAMOLOGISTA,
    ORDONTOLOGISTA

};

struct Medico
{
    char Nome[100];
    char CRM[100];
    char Telefone[15];
    enum Especialidade Espec;
    char Oficio[50];
};

void addmedico()
{
    FILE *arquivo = fopen("medico.bin", "ab");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Medico medico;
    char continuar;
    
    do
    {
        printf("Nome do médico: ");
        scanf(" %s", medico.Nome);

        printf("CRM: ");
        scanf(" %s", medico.CRM);

        printf("Telefone: ");
        scanf(" %s", medico.Telefone);
        
        

        fwrite(&medico, sizeof(struct Medico), 1, arquivo);

        printf("Deseja cadastrar outro médico? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    fclose(arquivo);

}

void lermedicos()
{
    FILE *arquivo = fopen("medico.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Medico medico;

    printf("Médicos cadastrados:\n");

    while (fread(&medico, sizeof(struct Medico), 1, arquivo))
    {
        printf("Nome: %s\n", medico.Nome);
        printf("CRM: %s\n", medico.CRM);
        printf("Especialidade: %d\n", medico.Oficio);
        printf("Telefone: %s\n", medico.Telefone);
        printf("----------------------------\n");
    }

    fclose(arquivo);
}

void deletemedico()
{
    FILE *arquivo = fopen("medico.bin", "r+b");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    FILE *temp = fopen("temp.bin", "wb");
    if (temp == NULL)
    {
        printf("Erro ao criar o arquivo temporário!\n");
        fclose(arquivo);
        return;
    }

    struct Medico medico;

    printf("Médicos cadastrados:\n");
    while (fread(&medico, sizeof(struct Medico), 1, arquivo))
    {
        printf("Nome: %s\n", medico.Nome);
        printf("CRM: %s\n", medico.CRM);
        printf("Especialidade: %d\n", medico.Espec);
        printf("Telefone: %s\n", medico.Telefone);
        printf("----------------------------\n");
    }

    char NomeBusca[100];
    int encontrado = 0;

    rewind(arquivo); // voltar ao início para a edição

    printf("Digite o nome do médico que deseja excluir: ");
    scanf(" %99s", NomeBusca); // espaço ignora \n pendente

    while (fread(&medico, sizeof(struct Medico), 1, arquivo))
    {
        if (strcmp(medico.Nome, NomeBusca) != 0)
        {
            // Se não for o médico que queremos excluir, escrevemos no novo arquivo
            fwrite(&medico, sizeof(struct Medico), 1, temp);
        }
        else
        {
            encontrado = 1;
        }
    }

    fclose(arquivo);
    fclose(temp);

    // Substitui o arquivo original pelo temporário
    if (remove("medico.bin") != 0 || rename("temp.bin", "medico.bin") != 0)
    {
        printf("Erro ao substituir o arquivo original.\n");
        return;
    }

    if (encontrado)
    {
        printf("Médico removido com sucesso!\n");
    }
    else
    {
        printf("Médico não encontrado!\n");
    }
}

void editmedico()
{
    FILE *arquivo = fopen("medico.bin", "r+b");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Medico medico;

    printf("Médicos cadastrados:\n");
    while (fread(&medico, sizeof(struct Medico), 1, arquivo))
    {
        printf("Nome: %s\n", medico.Nome);
        printf("CRM: %s\n", medico.CRM);
        printf("Especialidade: %d\n", medico.Espec);
        printf("Telefone: %s\n", medico.Telefone);
        printf("----------------------------\n");
    }

    char NomeBusca[100];
    int encontrado = 0;
    int op_medico;

    rewind(arquivo); // voltar ao início para a edição

    printf("Digite o nome do médico que deseja editar: ");
    scanf(" %99s", NomeBusca); // espaço ignora \n pendente

    while (fread(&medico, sizeof(struct Medico), 1, arquivo))
    {
        if (strcmp(medico.Nome, NomeBusca) == 0)
        {
            encontrado = 1;
            char continuar;

            do
            {
                printf("\nO que você deseja mudar em %s:\n", medico.Nome);
                printf("[1] Nome\n");
                printf("[2] CRM\n");
                printf("[3] Especialidade\n");
                printf("[4] Telefone\n");
                printf("Escolha uma opção: ");

                int opc;
                scanf("%d", &opc);

                switch (opc)
                {
                case 1:
                    printf("Novo Nome: ");
                    scanf(" %99s", medico.Nome);
                    break;
                case 2:
                    printf("Novo CRM: ");
                    scanf(" %99s", medico.CRM);
                    break;
                case 3:
                    printf("Especialidade: ");
                    printf("[1] CARDIOLOGISTA\n");
                    printf("[2] CLINICO\n");
                    printf("[3] PEDIATRA\n");
                    printf("[4] OFTAMOLOGISTA\n");
                    printf("[5] ORDONTOLOGISTA\n");
                    scanf("%d", op_medico);
                    switch (op_medico)
                    {
                    case 1:
                        medico.Espec = CARDIOLOGISTA;
                        break;

                    case 2:
                        medico.Espec = CLINICO;
                        break;

                    case 3:
                        medico.Espec = PEDIATRA;
                        break;

                    case 4:
                        medico.Espec = OFTAMOLOGISTA;
                        break;

                    case 5:
                        medico.Espec = ORDONTOLOGISTA;
                        break;

                    default:
                        printf("Especialidade não cadastrada");
                        break;
                    }
                    break;
                case 4:
                    printf("Novo Telefone: ");
                    scanf(" %14s", medico.Telefone);
                    break;
                default:
                    printf("Opção inválida!\n");
                    break;
                }

                printf("Deseja editar outro campo deste médico? (s/n): ");
                scanf(" %c", &continuar);

            } while (continuar == 's' || continuar == 'S');

            fseek(arquivo, -sizeof(struct Medico), SEEK_CUR);
            fwrite(&medico, sizeof(struct Medico), 1, arquivo);
            printf("Médico atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado)
    {
        printf("Nome não encontrado!\n");
    }

    fclose(arquivo);
}

struct Paciente
{
    char Nome[100];
    char CPF[14];
    char telefone[15]
};

void addpaciente()
{
    FILE *arquivo = fopen("paciente.bin", "ab");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Paciente paciente;
    char continuar;

    do
    {
        printf("Nome do paciente: ");
        scanf(" %s", paciente.Nome);

        printf("CPF: ");
        scanf(" %s", paciente.CPF);

        printf("Telefone: ");
        scanf(" %s", paciente.telefone);

        fwrite(&paciente, sizeof(struct Paciente), 1, arquivo);

        printf("Deseja cadastrar outro paciente? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    fclose(arquivo);
}

void lerpaciente()
{
    FILE *arquivo = fopen("paciente.bin", "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Paciente paciente;

    printf("Pacientes cadastrados:\n");

    while (fread(&paciente, sizeof(struct Paciente), 1, arquivo))
    {
        printf("Nome: %s\n", paciente.Nome);
        printf("CPF: %s\n", paciente.CPF);
        printf("Telefone: %s\n", paciente.telefone);
        printf("----------------------------\n");
    }

    fclose(arquivo);
}

void editpaciente()
{
    FILE *arquivo = fopen("paciente.bin", "r+b");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Paciente paciente;

    printf("Pacientes cadastrados:\n");
    while (fread(&paciente, sizeof(struct Paciente), 1, arquivo))
    {
        printf("Nome: %s\n", paciente.Nome);
        printf("CRM: %s\n", paciente.CPF);
        printf("Telefone: %s\n", paciente.telefone);
        printf("----------------------------\n");
    }

    char NomeBusca[100];
    int encontrado = 0;

    rewind(arquivo); // voltar ao início para a edição

    printf("Digite o nome do paciente que deseja editar: ");
    scanf(" %99s", NomeBusca); // espaço ignora \n pendente

    while (fread(&paciente, sizeof(struct Paciente), 1, arquivo))
    {
        if (strcmp(paciente.Nome, NomeBusca) == 0)
        {
            encontrado = 1;
            // Switch case para o user escolher oq mudar
            printf("Novo Nome: ");
            scanf(" %s", paciente.Nome);
            printf("Novo CPF: ");
            scanf(" %s", paciente.CPF);
            printf("Novo Telefone: ");
            scanf(" %s", paciente.telefone);

            fseek(arquivo, -sizeof(struct Paciente), SEEK_CUR);
            fwrite(&paciente, sizeof(struct Paciente), 1, arquivo);
            printf("Paciente atualizado com sucesso!\n");
            break;
        }
    }

    if (!encontrado)
    {
        printf("Nome não encontrado!\n");
    }

    fclose(arquivo);
}

enum Status
{
    AGENDADO,
    REALIZADO,
    CANCELADO
};

struct Consulta
{
    char ID[100];
    struct Paciente paciente;
    struct Medico medico;
    char data_hora[100];
    enum Status status;
};

void addconsulta()
{
    FILE *arquivo = fopen("consulta.bin", "ab");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Consulta consulta;
    char continuar;
    do
    {
        printf("Identificador da consulta: ");
        scanf(" %s", consulta.ID);
        ///////////////////////////////
        // Lista dos pacientes disponíveis
        FILE *arquivo = fopen("paciente.bin", "rb");
        if (arquivo == NULL)
        {
            printf("Erro ao abrir o arquivo!\n");
            return;
        }

        struct Paciente paciente;

        printf("Pacientes cadastrados:\n");

        while (fread(&paciente, sizeof(struct Paciente), 1, arquivo))
        {
            printf("Nome: %s\n", paciente.Nome);
            printf("CPF: %s\n", paciente.CPF);
            printf("Telefone: %s\n", paciente.telefone);
            printf("----------------------------\n");
        }

        fclose(arquivo);
        // Lista dos médicos disponíveis

        //////////////////////////////////
        printf("Data da consulta: ");
        scanf(" %s", consulta.data_hora);

        printf("Deseja cadastrar outra consulta? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    fclose(arquivo);
}

int main()
{
    SetConsoleOutputCP(CP_UTF8);

    /*
        Filtro de paciente e aluno precisa de condição, ou um ou outro

    */
    int opcao, opcao2;
    do
    {
        printf(" ==== Olá seja bem vindo ao Sistema online de saúde - SOS ===== \n");
        printf(" ==== Oque você deseja?  ===== \n");
        printf("[1] Gerenciamento de médicos\n");
        printf("[2] Gerenciamento de pacientes\n");
        printf("[3] Gerenciar consultas\n");
        printf("[4] Verificar relatórios\n");
        printf("[5] Sair\n");
        scanf("%d", &opcao);
        switch (opcao)
        {
        case 1:
            printf("Seja bem vindo ao gerenciamento de médicos, oque você deseja efetivar?\n");
            printf("[1] Adicionar médicos\n");
            printf("[2] Atualizar médico\n");
            printf("[3] Remover médico\n");
            printf("[4] Visualizar médicos\n");
            printf("[5] Voltar para o menu principal\n");
            scanf("%d", &opcao2);

            switch (opcao2)
            {
            case 1:
                addmedico();
                break;

            case 2:
                editmedico();
                break;

            case 3:
                deletemedico();
                break;

            case 4:
                printf("Médicos cadastrados");
                lermedicos();
            default:
                break;
            }

            break;

        case 2:
            printf("Seja bem vindo ao Gerenciamento de pacientes, oque você deseja efetivar?\n");
            printf("[1] Adicionar paciente\n");
            printf("[2] Atualizar paciente\n");
            printf("[3] Remover paciente\n");
            printf("[4] Visualizar paciente\n");
            printf("[5] Voltar para o menu principal\n");
            scanf("%d", &opcao2);
            switch (opcao2)
            {
            case 1:
                addpaciente();
                break;
            case 2:
                editpaciente();
                break;
            case 3:
                break;
            case 4:
                lerpaciente();
                break;
            default:
                break;
            }
            break;
        case 3:
            // Expplicar melhor essa parte
            printf("Seja bem vindo ao Gerenciamento de consultas, oque você deseja efetivar?\n");
            printf("[1] Agendar consulta\n");
            printf("[2] Cancelar consulta\n");
            printf("[3] Listar consultas\n");
            printf("[4] Atualizar consultas\n");
            printf("[5] Voltar para o menu principal\n");
            break;

        case 4:
            printf("Seja bem vindo ao Gerenciamento de relatórios, oque você deseja efetivar?\n");
            // Listar por médico e por paciente
            printf("[1] Listagem de consultas de médico específico\n");
            printf("[2] Gerar relatório filtrado por especialidade\n");
            printf("[3] Mostrar consultas do dia atual\n");
            printf("[5] Voltar para o menu principal\n");
            break;
        case 5:
            printf("VOLTE SEMPRE");
            printf(":3");
            break;
        default:
            printf("Opção inválida");
            break;
        }

    } while (opcao != 5);
}
