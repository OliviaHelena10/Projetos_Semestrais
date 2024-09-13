/*---------------------------------------------------------------------------------------*/
/*                      SPOLOG1 PROJETO DE AVALIA��O DA DISCIPLINA                       */
/*---------------------------------------------------------------------------------------*/
/*                                                                                       */
/*                                 IDE - CODE::BLOCKS                                    */
/*                            SISTEMA OPERACIONAL - WINDOWS                              */
/*---------------------------------------------------------------------------------------*/


/* Bibliotecas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>



/* Prot�tipo das Fun��es */
void    introducao      (void);
int     ano_bissexto    (int ano);
void    dia_da_semana   (int h, char *dia_semana);
int     valida_data     (int dia, int mes, int ano);
int     formula_Zeller  (int dia, int mes, int ano);
void    carregando      (const char *str, int atraso);
void    proxima_data    (int dia, int mes, int ano, int esporte, int dias[], int *count);
void    datas_iguais    (int atletismo[], int natacao[], int rugby[], int judo[],
                        int count_a, int count_n, int count_r, int count_j);
void    arquivo_treinos (const char *nome_arquivo,
                         int atletismo[], int natacao[], int rugby[], int judo[],
                         int count_a, int count_n, int count_r, int count_j);



/* Constru��o das Fun��es */

void introducao(void)
{
    printf("---------------------------------------------------");
    printf("\n\tSejam Bem-Vindos treinadores!");
    printf("\n---------------------------------------------------");
    printf("\n\nIremos verificar calendario de treinos de seus atletas");
}



int ano_bissexto(int ano)
{
    /* Usa uma f�rmula para verificar se o ano � bissexto ou n�o */

    if ((ano % 400 == 0) || ((ano % 100 != 0) && (ano % 4 == 0)))
        return 366;         // Caso bissexto
    else
        return 365;         // Caso normal
}



int valida_data(int dia, int mes, int ano)
{
    if (dia < 1 || dia > 31) return -1;  // Dia inv�lido
    if (mes < 1 || mes > 12) return -1;  // Mes inv�lido
    if (ano < 2000) return -1;           // Ano inv�lido

    if (mes == 2) // Caso fevereiro
    {
        int max_dia = (ano_bissexto(ano) == 366) ? 29 : 28;
        if (dia > max_dia) return -1;    // situa��o de invalidez
    }

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30)
        return -1;                       // situa��o de invalidez

    return 0;                            // situa��o de validez
}



int formula_Zeller(int dia, int mes, int ano)
{
    /* Aplica uma f�rmula matem�tica para encontrar os dias da semana */

    /* Vari�veis */
    int h, j = ano / 100, k = ano % 100;

    /* Exce��o para os meses de janeiro e fevereiro  */
    if (mes < 3)
    {
        mes += 12; // janeiro = 13 e fevereiro = 14
        ano--;
    }


    /* F�rmula */
    h = (dia + ((mes + 1) * 26) / 10 + k + k / 4 + j / 4 + 5 * j) % 7;

    return h;
}



void proxima_data(int dia, int mes, int ano, int esporte, int dias[], int *count) // *count � ponteiro para a contagem de datas salvas
{
    /* Vari�veis */
    int dias_no_mes;
    int total_dias = ano_bissexto(ano), max_ano = ano + 1;


    /* Casos e Condi��es */
    while (total_dias > 0)                                      // Contagem para o per�odo de um ano
    {
        while ((mes <= 12) && (total_dias > 0))                 // Contagem para meses e dias do ano
        {
            if (mes == 2) dias_no_mes = (ano_bissexto(ano) == 366) ? 29 : 28;            // Fevereiro
            else if (mes == 4 || mes == 6 || mes == 9 || mes == 11) dias_no_mes = 30;    // Meses de 30 dias
            else dias_no_mes = 31;                                                       // Meses de 31 dias

            while ((dia <= dias_no_mes) && (total_dias > 0))    // Contagem para dias do mes e do ano
            {
                dias[*count] = dia + mes * 100 + ano * 10000;   // Salva as datas no formato DDMMAAAA
                (*count)++;

                dia += esporte;                                 // Frequ�ncia de treinos do esporte
                total_dias -= esporte;                          // Decr�scimo para rodar o loop no per�odo de 1 ano
            }

            if (dia > dias_no_mes)                              // Pr�ximo m�s reiniciando dia
            {
                dia -= dias_no_mes;                             // Inicia o pr�ximo m�s no dia correto
                mes++;
            }
        }

        ano++;          // Reinicia o ano
        mes = 1;        // Reinicia o m�s
    }
}



void dia_da_semana(int h, char *dia_semana)
{
    /* Continua��o da fun��o de Zeller que indica o dia da semana */
    switch (h)
    {
        /* a fun��o strcpy copia a string apontada para dia_semana */
        case 0: strcpy(dia_semana, "S�bado"); break;
        case 1: strcpy(dia_semana, "Domingo"); break;
        case 2: strcpy(dia_semana, "Segunda-feira"); break;
        case 3: strcpy(dia_semana, "Ter�a-feira"); break;
        case 4: strcpy(dia_semana, "Quarta-feira"); break;
        case 5: strcpy(dia_semana, "Quinta-feira"); break;
        case 6: strcpy(dia_semana, "Sexta-feira"); break;
    }
}



void datas_iguais(int atletismo[], int natacao[], int rugby[], int judo[], int count_a, int count_n, int count_r, int count_j)
{
    FILE *f = fopen("DATASCOMUNS.txt", "w");  // Abre o arquivo

    if (!f)  // Caso de erro ao abrir
    {
        printf("Erro ao abrir o arquivo!\n");
        exit(1);  // For�a encerramento do programa
    }

    int intervalo_dias = -1;
    int ultimo_encontro = -1;
    int linha = 1;  // Inicializa o contador de linhas

    fprintf(f, "_____________________________________________________________________________________________________________________________________________________________________\n");
    fprintf(f, "                                                    D A T A S   C O M U N S");
    fprintf(f, "\n___________________________________________________________________________________________________________________________________________________________________\n");
    fprintf(f, "\n\t    DATAS ATLETISMO                     DATAS NATA��O                     DATAS RUGBY                     DATAS JUD�\n");
    fprintf(f, "\t    (a cada 2 dias)                    (a cada 3 dias)                  (a cada 5 dias)                 (a cada 9 dias)");
    fprintf(f, "\n___________________________________________________________________________________________________________________________________________________________________\n");

    for (int i = 0; i < count_a; i++)  // Itera sobre as datas de Atletismo
    {
        for (int j = 0; j < count_n; j++)  // Itera sobre as datas de Nata��o
        {
            for (int k = 0; k < count_r; k++)  // Itera sobre as datas de Rugby
            {
                for (int l = 0; l < count_j; l++)  // Itera sobre as datas de Jud�
                {
                    /* Compara��o de datas iguais */
                    if (atletismo[i] == natacao[j] && natacao[j] == rugby[k] && rugby[k] == judo[l])
                    {
                        int dia = atletismo[i] % 100;
                        int mes = (atletismo[i] / 100) % 100;
                        int ano = atletismo[i] / 10000;

                        /* Calculando o intervalo de dias entre encontros */
                        if (ultimo_encontro != -1)
                        {
                            int dia_anterior = ultimo_encontro % 100;
                            int mes_anterior = (ultimo_encontro / 100) % 100;
                            int ano_anterior = ultimo_encontro / 10000;

                            struct tm data_atual = {0};
                            struct tm data_anterior = {0};

                            /* Inicializando apenas os campos necess�rios */
                            data_atual.tm_mday = dia;
                            data_atual.tm_mon = mes - 1;
                            data_atual.tm_year = ano - 1900;

                            data_anterior.tm_mday = dia_anterior;
                            data_anterior.tm_mon = mes_anterior - 1;
                            data_anterior.tm_year = ano_anterior - 1900;

                            time_t t_atual = mktime(&data_atual);
                            time_t t_anterior = mktime(&data_anterior);

                            intervalo_dias = (int)difftime(t_atual, t_anterior) / (60 * 60 * 24);
                        }

                        ultimo_encontro = atletismo[i];

                        int h = formula_Zeller(dia, mes, ano);
                        char dia_semana[20];
                        dia_da_semana(h, dia_semana);           // Converte para String

                        /* Escrevendo as Datas no arquivo com a numera��o das linhas */
                        fprintf(f, "%d |    %02d/%02d/%04d -  %-13s      %02d/%02d/%04d -  %-13s      %02d/%02d/%04d -  %-13s      %02d/%02d/%04d -  %-13s\n",
                                linha++, dia, mes, ano, dia_semana,
                                dia, mes, ano, dia_semana,
                                dia, mes, ano, dia_semana,
                                dia, mes, ano, dia_semana);
                    }
                }
            }
        }
    }

    if (intervalo_dias > 0)
    {
        fprintf(f, "\n_____________________________________________________________________________________________________________________________________________________________________\n");
        fprintf(f, "\nAs modalidades se encontram a cada %d dias", intervalo_dias);
        fprintf(f, "\n_____________________________________________________________________________________________________________________________________________________________________\n");
    }

    fclose(f);  // Fecha o arquivo
}




void arquivo_treinos(const char *nome_arquivo, int atletismo[], int natacao[], int rugby[], int judo[], int count_a, int count_n, int count_r, int count_j)
{
    FILE *f = fopen("DATASTREINOS.txt", "w");            // Abre o arquivo

    if (!f) // Caso de erro ao abrir
    {
        printf("Erro ao abrir o arquivo %s!\n", nome_arquivo);
        exit(1); // For�a encerramento do programa
    }

    int linha = 1;  // Inicializa o contador de linhas

    /* Acessando a primeira data */
    int primeiro_dia = atletismo[0] % 100;
    int primeiro_mes = (atletismo[0] / 100) % 100;
    int primeiro_ano = atletismo[0] / 10000;
    int h_inicio = formula_Zeller(primeiro_dia, primeiro_mes, primeiro_ano);
    char dia_semana_inicio[20];
    dia_da_semana(h_inicio, dia_semana_inicio);

    /* Acessando a �ltima data */
    int ultimo_dia = atletismo[count_a - 1] % 100;
    int ultimo_mes = (atletismo[count_a - 1] / 100) % 100;
    int ultimo_ano = atletismo[count_a - 1] / 10000;

    /* Verifica qual das modalidades treina no �ltimo dia  */

    if (count_n > 0 && natacao[count_n - 1] > atletismo[count_a - 1])
    {
        ultimo_dia = natacao[count_n - 1] % 100;
        ultimo_mes = (natacao[count_n - 1] / 100) % 100;
        ultimo_ano = natacao[count_n - 1] / 10000;
    }

    if (count_r > 0 && rugby[count_r - 1] > natacao[count_n - 1] && rugby[count_r - 1] > atletismo[count_a - 1])
    {
        ultimo_dia = rugby[count_r - 1] % 100;
        ultimo_mes = (rugby[count_r - 1] / 100) % 100;
        ultimo_ano = rugby[count_r - 1] / 10000;
    }

    if (count_j > 0 && judo[count_j - 1] > rugby[count_r - 1] && judo[count_j - 1] > natacao[count_n - 1] && judo[count_j - 1] > atletismo[count_a - 1])
    {
        ultimo_dia = judo[count_j - 1] % 100;
        ultimo_mes = (judo[count_j - 1] / 100) % 100;
        ultimo_ano = judo[count_j - 1] / 10000;
    }

    int h_fim = formula_Zeller(ultimo_dia, ultimo_mes, ultimo_ano);
    char dia_semana_fim[20];
    dia_da_semana(h_fim, dia_semana_fim);

    /* Escrevendo no arquivo com a numera��o das linhas */
    fprintf(f, "_____________________________________________________________________________________________________________________________________________________________________\n");
    fprintf(f, "\t\t\t\tAs modalidades treinam de %02d/%02d/%04d (%s) a %02d/%02d/%04d (%s)\n",
            primeiro_dia, primeiro_mes, primeiro_ano, dia_semana_inicio,
            ultimo_dia, ultimo_mes, ultimo_ano, dia_semana_fim);
    fprintf(f, "_____________________________________________________________________________________________________________________________________________________________________\n");
    fprintf(f, "\n\t    DATAS ATLETISMO                    DATAS NATA��O                     DATAS RUGBY                     DATAS JUD�\n");
    fprintf(f, "\t    (a cada 2 dias)                   (a cada 3 dias)                  (a cada 5 dias)                  (a cada 9 dias)");
    fprintf(f, "\n___________________________________________________________________________________________________________________________________________________________________\n");

    int max_count = count_a > count_n ? count_a : count_n;
    max_count = max_count > count_r ? max_count : count_r;
    max_count = max_count > count_j ? max_count : count_j;

    for (int i = 0; i < max_count; i++)
    {
        char dia_semana[20];

        fprintf(f, "%02d |\t ", linha++);  // Adiciona a numera��o da linha

        if (i < count_a)                        // Caso Atletismo
        {
            int dia = atletismo[i] % 100;
            int mes = (atletismo[i] / 100) % 100;
            int ano = atletismo[i] / 10000;
            int h = formula_Zeller(dia, mes, ano);
            dia_da_semana(h, dia_semana); // Converte para String
            fprintf(f, "%02d/%02d/%04d -  %-13s\t", dia, mes, ano, dia_semana);
        }
        else
        {
            fprintf(f, "%-33s", " "); // Espa�amento
        }

        if (i < count_n) // Caso Nata��o
        {
            int dia = natacao[i] % 100;
            int mes = (natacao[i] / 100) % 100;
            int ano = natacao[i] / 10000;
            int h = formula_Zeller(dia, mes, ano);
            dia_da_semana(h, dia_semana); // Converte para String
            fprintf(f, "%02d/%02d/%04d -  %-13s\t", dia, mes, ano, dia_semana);
        }
        else
        {
            fprintf(f, "%-37s", " "); // Espa�amento
        }

        if (i < count_r) // Caso Rugby
        {
            int dia = rugby[i] % 100;
            int mes = (rugby[i] / 100) % 100;
            int ano = rugby[i] / 10000;
            int h = formula_Zeller(dia, mes, ano);
            dia_da_semana(h, dia_semana); // Converte para String
            fprintf(f, "%02d/%02d/%04d -  %-13s\t", dia, mes, ano, dia_semana);
        }
        else
        {
            fprintf(f, "%-37s", " "); // Espa�amento
        }

        if (i < count_j) // Caso Jud�
        {
            int dia = judo[i] % 100;
            int mes = (judo[i] / 100) % 100;
            int ano = judo[i] / 10000;
            int h = formula_Zeller(dia, mes, ano);
            dia_da_semana(h, dia_semana); // Converte para String
            fprintf(f, "%02d/%02d/%04d -  %-13s", dia, mes, ano, dia_semana);
        }

        fprintf(f, "\n"); // Quebra de linha ap�s cada linha de treinos
    }

    fclose(f); // Fecha o arquivo
}


void carregando(const char *str, int atraso)
{
    /* Cria uma tela de carregamento */

    while (*str)                // Enquanto n�o chegar ao fim da string
    {
        putchar(*str);          // Imprime o caractere atual
        fflush(stdout);         // Garante que o caractere seja exibido imediatamente
        sleep(atraso);          // Atraso em segundos
        str++;                  // Avan�a para o pr�ximo caractere
    }
}

int main()
{
    /* Vari�veis */
    int dia, mes, ano;
    int atletismo[1000], natacao[1000], rugby[1000], judo[1000];
    int count_a = 0, count_n = 0, count_r = 0, count_j = 0;
    int freq_atletismo = 2, freq_natacao = 3, freq_rugby = 5, freq_judo = 9;



    /* Tela Inicial do Programa */
    introducao();

    printf("\n\nDigite o dia de inicio do seu treino: ");
    scanf("%i", &dia);

    printf("Digite o mes de inicio do seu treino: ");
    scanf("%i", &mes);

    printf("Digite o ano de inicio do seu treino: ");
    scanf("%i", &ano);

    system("cls");



    /* Confere o retorno de valida_data */
    if (valida_data(dia, mes, ano) == -1) // caso de invalidez
    {
        printf("\n\nData %02d/%02d/%04d invalida!\n", dia, mes, ano);
        exit(1);   // for�a o programa a fechar
    }


    /* Aplica as fun��es de c�lculo e manipula��o das datas */
    proxima_data(dia, mes, ano, freq_atletismo, atletismo, &count_a);
    proxima_data(dia, mes, ano, freq_natacao, natacao, &count_n);
    proxima_data(dia, mes, ano, freq_rugby, rugby, &count_r);
    proxima_data(dia, mes, ano, freq_judo, judo, &count_j);



    /* Fun��es geradoras de Relat�rio */
    datas_iguais(atletismo, natacao, rugby, judo,
                           count_a, count_n, count_r, count_j);
    arquivo_treinos("DATASTREINOS.txt",
                             atletismo, natacao, rugby, judo,
                             count_a, count_n, count_r, count_j);


    /* Fim do Programa */
    const char *mensagem = "...";
    printf("\n\n\t\t\tGerando relatorio ");
    carregando(mensagem, 1);

    system("cls");
    printf("\n_________________________________________________________");
    printf("\n\n\nRelatorios gerados com sucesso! \n");
    printf("Confira seus arquivos na pasta\n\n");
    printf("\n_________________________________________________________");
    printf("\n\nNao se esqueca dos materiais necessarios para as aulas!");
    getch();

    system("cls");
    printf("Ate mais!\n\n");
    sleep(1);
    return 0;
}
