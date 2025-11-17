#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

/* ======================= Config DLL ======================= */
static HMODULE g_hDll = NULL;

/* Conven��o de chamada (Windows): __stdcall */
#ifndef CALLCONV
#  define CALLCONV WINAPI
#endif

/* ======================= Assinaturas da DLL ======================= */
typedef int (CALLCONV *AbreConexaoImpressora_t)(int, const char *, const char *, int);
typedef int (CALLCONV *FechaConexaoImpressora_t)(void);
typedef int (CALLCONV *ImpressaoTexto_t)(const char *, int, int, int);
typedef int (CALLCONV *Corte_t)(int);
typedef int (CALLCONV *ImpressaoQRCode_t)(const char *, int, int);
typedef int (CALLCONV *ImpressaoCodigoBarras_t)(int, const char *, int, int, int);
typedef int (CALLCONV *AvancaPapel_t)(int);
typedef int (CALLCONV *AbreGavetaElgin_t)(int, int, int);
typedef int (CALLCONV *AbreGaveta_t)(int, int, int);
typedef int (CALLCONV *SinalSonoro_t)(int, int, int);
typedef int (CALLCONV *ImprimeXMLSAT_t)(const char *, int);
typedef int (CALLCONV *ImprimeXMLCancelamentoSAT_t)(const char *, const char *, int);
typedef int (CALLCONV *InicializaImpressora_t)(void);

/* ======================= Ponteiros ======================= */
static AbreConexaoImpressora_t        AbreConexaoImpressora        = NULL;
static FechaConexaoImpressora_t       FechaConexaoImpressora       = NULL;
static ImpressaoTexto_t               ImpressaoTexto               = NULL;
static Corte_t                        Corte                        = NULL;
static ImpressaoQRCode_t              ImpressaoQRCode              = NULL;
static ImpressaoCodigoBarras_t        ImpressaoCodigoBarras        = NULL;
static AvancaPapel_t                  AvancaPapel                  = NULL;
static AbreGavetaElgin_t              AbreGavetaElgin              = NULL;
static AbreGaveta_t                   AbreGaveta                   = NULL;
static SinalSonoro_t                  SinalSonoro                  = NULL;
static ImprimeXMLSAT_t                ImprimeXMLSAT                = NULL;
static ImprimeXMLCancelamentoSAT_t    ImprimeXMLCancelamentoSAT    = NULL;
static InicializaImpressora_t         InicializaImpressora         = NULL;

/* ======================= Configura��o ======================= */
static int   g_tipo      = 1;
static char  g_modelo[64] = "i9";
static char  g_conexao[128] = "USB";
static int   g_parametro = 0;
static int   g_conectada = 0;

/* ======================= Utilidades ======================= */
#define LOAD_FN(h, name)                                                         \
    do {                                                                         \
        name = (name##_t)GetProcAddress((HMODULE)(h), #name);                    \
        if (!(name)) {                                                           \
            fprintf(stderr, "Falha ao resolver s�mbolo %s (erro=%lu)\n",         \
                    #name, GetLastError());                                      \
            return 0;                                                            \
        }                                                                        \
    } while (0)

static void flush_entrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ======================= Fun��es para manipular a DLL ======================= */
static int carregarFuncoes(void)
{
    g_hDll = LoadLibraryA("E1_Impressora01.dll");
    if (!g_hDll) {
        fprintf(stderr, "Erro ao carregar E1_Impressora01.dll (erro=%lu)\n", GetLastError());
        return 0;
    }

    LOAD_FN(g_hDll, AbreConexaoImpressora);
    LOAD_FN(g_hDll, FechaConexaoImpressora);
    LOAD_FN(g_hDll, ImpressaoTexto);
    LOAD_FN(g_hDll, Corte);
    LOAD_FN(g_hDll, ImpressaoQRCode);
    LOAD_FN(g_hDll, ImpressaoCodigoBarras);
    LOAD_FN(g_hDll, AvancaPapel);
    LOAD_FN(g_hDll, AbreGavetaElgin);
    LOAD_FN(g_hDll, AbreGaveta);
    LOAD_FN(g_hDll, SinalSonoro);
    LOAD_FN(g_hDll, ImprimeXMLSAT);
    LOAD_FN(g_hDll, ImprimeXMLCancelamentoSAT);
    LOAD_FN(g_hDll, InicializaImpressora);

    return 1;
}

static void liberarBiblioteca(void)
{
    if (g_hDll) {
        FreeLibrary(g_hDll);
        g_hDll = NULL;
    }
}

/* ======================= Fun��es a serem implementadas pelos alunos ======================= */

static void exibirMenu(void)
{
    // TODO: implementar exibi��o do menu principal com as op��es de impress�o
        
}

static void configurarConexao(int *tipo,char *modelo, char *conexao, int *parametro,int *config)
{
    // TODO: pedir ao usu�rio tipo, modelo, conex�o e par�metro
    printf("#############################################\n"
           "##         Configurar Conexao:             ##\n"
           "##                                         ##\n"
           "##   1 - USB                               ##\n"
           "##   2 - RS232                             ##\n"
           "##   3 - TCP/IP                            ##\n"
           "##   4 - Bluetooth                         ##\n"
           "##   5 - Impressoras acopladas(Android)    ##\n"
           "##                                         ##\n"
           "#############################################\n\n");
    scanf("%i",tipo);
    
    if(*tipo == 5){
        modelo[0] = '\0';
        conexao[0] = '\0';
        printf("\nImpressora Acoplada selecionada, configuracao terminada!\n\n");
    }else{
    printf("#####################################\n"
           "##                                 ##\n"
           "##   Digite o modelo da conexao:   ##\n" 
           "##                                 ##\n"
           "#####################################\n\n");
    scanf("%s",modelo);

    
    printf("########################################\n"
           "##                                    ##\n"
           "##  Digite a conexao da impressora:   ##\n"
           "##                                    ##\n"
           "########################################\n\n");
    scanf("%s",conexao);
    }

    if(*tipo == 1||*tipo == 4||*tipo == 5){
        *parametro = 0;
    }else{
    printf("########################################\n"
           "##                                    ##\n"
           "##   Digite o parametro da conexao:   ##\n"
           "##                                    ##\n"
           "########################################\n\n");
    scanf("%i",parametro);    
    }
    g_tipo = *tipo;
    strcpy(g_modelo, modelo);
    strcpy(g_conexao, conexao);
    g_parametro = *parametro;
    printf("\nConexao configurada com sucesso!\n\n");
    *config = 1;
}

static void abrirConexao(int *config )
{
    // TODO: chamar AbreConexaoImpressora e validar retorno
    if(*config==1){
        int r = AbreConexaoImpressora(g_tipo, g_modelo, g_conexao, g_parametro);
    }else{
        printf("\nConfigure a conexao da impressora primeiro!\n\n");
    }
}

static void fecharConexao(void)
{
    // TODO: chamar FechaConexaoImpressora e tratar retorno
}

static void imprimirTexto(void)
{
    // TODO: solicitar texto do usu�rio e chamar ImpressaoTexto
    // incluir AvancaPapel e Corte no final
}

static void imprimirQRCode(void)
{
    // TODO: solicitar conte�do do QRCode e chamar ImpressaoQRCode(texto, 6, 4)
    // incluir AvancaPapel e Corte no final
}

static void imprimirCodigoBarras(void)
{
    // TODO: usar ImpressaoCodigoBarras(8, "{A012345678912", 100, 2, 3)
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLSAT(void)
{
    // TODO: ler o arquivo ./XMLSAT.xml e enviar via ImprimeXMLSAT
    // incluir AvancaPapel e Corte no final
}

static void imprimirXMLCancelamentoSAT(void)
{
    // TODO: ler o arquivo ./CANC_SAT.xml e chamar ImprimeXMLCancelamentoSAT
    // incluir AvancaPapel e Corte no final
    
	/*usar assinatura abaixo:
        "Q5DLkpdRijIRGY6YSSNsTWK1TztHL1vD0V1Jc4spo/CEUqICEb9SFy82ym8EhBRZ"
        "jbh3btsZhF+sjHqEMR159i4agru9x6KsepK/q0E2e5xlU5cv3m1woYfgHyOkWDNc"
        "SdMsS6bBh2Bpq6s89yJ9Q6qh/J8YHi306ce9Tqb/drKvN2XdE5noRSS32TAWuaQE"
        "Vd7u+TrvXlOQsE3fHR1D5f1saUwQLPSdIv01NF6Ny7jZwjCwv1uNDgGZONJdlTJ6"
        "p0ccqnZvuE70aHOI09elpjEO6Cd+orI7XHHrFCwhFhAcbalc+ZfO5b/+vkyAHS6C"
        "YVFCDtYR9Hi5qgdk31v23w==";
        */
}

static void abrirGavetaElginOpc(void)
{
    // TODO: chamar AbreGavetaElgin(1, 50, 50)
}

static void abrirGavetaOpc(void)
{
    // TODO: chamar AbreGaveta(1, 5, 10)
}

static void emitirSinalSonoro(void)
{
    // TODO: chamar SinalSonoro(4, 50, 5)
}

/* ======================= Fun��o principal ======================= */
int main(void)
{
    int tipo,parametro,opcao,config,r;
    char modelo[20],conexao[25];
    if (!carregarFuncoes()) {
        return 1;
    }
	
    while (1) {
        
        //construir o menu e chamar as fun�oes aqui!!!
        printf("###################################\n##                               ##\n##      Bem vindo ao menu!       ##\n##                               ##\n##  1 - Configurar Conexao       ##\n##  2 - Abrir Conexao            ##\n##  3 - Impressao Texto          ##\n##  4 - Impressao QRCode         ##\n##  5 - Impressao Cod Barras     ##\n##  6 - Impressao XML SAT        ##\n##  7 - Impressao XML Canc SAT   ##\n##  8 - Abrir Gaveta Elgin       ##\n##  9 - Abrir Gaveta             ##  \n## 10 - Sinal Sonoro             ##\n##  0 - Fechar Conexao e Sair    ##\n##                               ##\n###################################\n");
        scanf("%i",&opcao);
        switch (opcao) {
        case 1:
            printf("\nOpcao escolhida: Configurar Conexao!\n\n");
            opcao = -1;
            configurarConexao(&tipo,modelo,conexao,&parametro,&config);
            break;
        case 2:
            printf("\nOpcao escolhida: Abrir Conexao!\n\n");
            abrirConexao(&config);
            break;
        case 3:
            printf("\nOpcao escolhida: Impressao Texto!\n\n");
            imprimirTexto();
            break;
        case 4:
            printf("\nOpcao escolhida: Impressao QRCode!\n\n");
            imprimirQRCode();
            break;
        case 5:
            printf("\nOpcao escolhida: Impressao Cod Barras!\n\n");
            imprimirCodigoBarras();
            break;
        case 6:
            printf("\nOpcao escolhida: Impressao XML SAT!\n\n");
            imprimirXMLSAT();
            break;
        case 7:
            printf("\nOpcao escolhida: Impressao XML Canc SAT!\n\n");
            imprimirXMLCancelamentoSAT();
            break;
        case 8:
            printf("\nOpcao escolhida: Abrir Gaveta Elgin!\n\n");
            abrirGavetaElginOpc();
            break;
        case 9:
            printf("\nOpcao escolhida: Abrir Gaveta!\n\n");
            abrirGavetaOpc();
            break;
        case 10:
            printf("\nOpcao escolhida: Sinal Sonoro!\n\n");
            emitirSinalSonoro();
            break;
        case 0:
            printf("\nOpcao escolhida: Fechar Conexao e Sair!\n\n");
            fecharConexao();
            return 0 ;
            break;
        default:
            printf("\nOpcao Invalida!\n\n");
            break;
    }
    }
}

