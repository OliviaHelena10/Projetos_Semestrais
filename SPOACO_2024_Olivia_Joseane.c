/*------------------------------------------------------------*/
/*             COMPILADOR DE LINGUAGEM ASSEMBLY               */
/*------------------------------------------------------------*/
/*                                                            */
/*                    IDE - CODE::BLOCKS                      */
/*              SISTEMA OPERACIONAL - WINDOWS                 */
/*------------------------------------------------------------*/


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constantes */
#define T_CABECALHO 4
#define MAX_LINHA 100

/* Protótipo de Funcões */
unsigned char obtem_opcode(char *instrucao);
void cabecalho(FILE *output);


/* Estruturas */
typedef struct                            // Estrutura do Assembly
{
    char instrucao[4];                    // 4 espaços, sendo o último null terminator
    unsigned char opcode;
} Instrucao_Assembly;


Instrucao_Assembly lista_instrucao[] =    // Lista com todas as instruções e opcodes do Neander
{
    {"NOP", 0x00},
    {"STA", 0x10},
    {"LDA", 0x20},
    {"ADD", 0x30},
    {"OR",  0x40},
    {"AND", 0x50},
    {"NOT", 0x60},
    {"JMP", 0x80},
    {"JN",  0x90},
    {"JZ",  0xA0},
    {"HLT", 0xF0},
};


unsigned char obtem_opcode(char *instrucao)         // Encontra o opcode da instrução
{
    int i;
    for (i = 0; i < sizeof(lista_instrucao)/sizeof(Instrucao_Assembly); i++)  // itera sobre os elementos da lista de instruções
    {
        if (strcmp(instrucao, lista_instrucao[i].instrucao) == 0)             // verifica se a instrução lida está na lista de instruções
            return lista_instrucao[i].opcode;
    }
    return 0xFF;                                    // Caso de erro - instrução não encontrada
}


void cabecalho(FILE *output) // Cabeçalho do arquivo
{
    unsigned char cabecalho_arq[T_CABECALHO] = {0x03, 0x4E, 0x44, 0x52};
    fwrite(cabecalho_arq, sizeof(unsigned char), T_CABECALHO, output);
}


void processa_linha(char *linha, FILE *output) // processa uma linha do arquivo e escreve o correspondente no arquivo de saída.
{
    char instrucao[4];
    int endereco;

    if (sscanf(linha, "%s %x", instrucao, &endereco) == 2)
    {
        unsigned char opcode = obtem_opcode(instrucao);
        if (opcode != 0xFF)                              // verifica se a instrução foi encontrada
        {
            fwrite(&opcode, sizeof(unsigned char), 1, output);
            fputc(0x00, output);                         // byte extra
            unsigned char byte_endereco = (unsigned char) endereco;
            fwrite(&byte_endereco, sizeof(unsigned char), 1, output);
            fputc(0x00, output);                         // byte extra
        }
    }
    else if (sscanf(linha, "%s", instrucao) == 1)        // caso instruções sem endereço de memória
    {
        unsigned char opcode = obtem_opcode(instrucao);
        if (opcode != 0xFF)
        {
            fwrite(&opcode, sizeof(unsigned char), 1, output);
            fputc(0x00, output);                         // byte extra
        }
    }
}

int main()
{
    /* variáveis */
    char nome_arquivo_input[MAX_LINHA];
    char nome_arquivo_output[MAX_LINHA];

    /* tela inicial */
    printf("Digite o nome do arquivo .asm: ");
    scanf("%s", nome_arquivo_input);



    /* Cria o nome do arquivo de saída com extensão .mem */
    strcpy(nome_arquivo_output, strtok(nome_arquivo_input, "."));
    strcat(nome_arquivo_output, ".mem");

    FILE *input = fopen(strcat(nome_arquivo_input, ".asm"), "r");
    FILE *output = fopen(nome_arquivo_output, "wb");

    if (!input || !output)
    {
        printf("Erro ao abrir arquivos.\n");
        return 1;
    }

    cabecalho(output);

    /* Processa cada linha do arquivo de entrada */
    char linha[MAX_LINHA];
    while (fgets(linha, sizeof(linha), input))
    {
        processa_linha(linha, output);
    }

    /* Fecha o arquivo */
    fclose(input);
    fclose(output);



    /* tela finalização */
    printf("Arquivo %s gerado com sucesso.\n", nome_arquivo_output);
    return 0;
}
