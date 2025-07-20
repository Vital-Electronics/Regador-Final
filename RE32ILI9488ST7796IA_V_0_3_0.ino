/*

Regador - Profissional (IA)

*  MODELO = R  E32 ST7796  IA
*           |  |   |       |_ Inteligencia Artificial
*           |  |   |_ Driver do TFT   ( PODE SER O ILI9488 / ST7796 )
*           |  |_ Processador ESP32 VROOM
*           |_ Regador                                                                                 
                        
Due Core - Multi Tarefas (default 0.  Core 2 é o 1)
TFT.Touch Display 4" 480X320 - Driver:  ILI9488 / ST7796
Designed in 10/07/2025

Pinagem Utilizada:
===============================================|
|   TFT       |      TOUCH      |    SD CARD   |   
===============================================|
| CS    17    |     CS    21    |   CS    5    | 
| RST   16    |     CLK   18    |   MOSI  23   |
| DC    14    |     DIN   23    |   MISO  19   | 
| MOSI  23    |     DOUT  19    |   CLS   18   | 
| SCK   18    |     IRQ   **    |              |  
| SCK   18    |     IRQ   **    |              |  
| MISO  19*   |                 |              | 
| LED   3V3   |                 |              | 
===============================================|
*  Pin 19 (nao obrigatório)                    | 
** Pin 34 (nao obrigatório)                    | 
===============================================|


PREVISAO DO TEMPO
https://home.openweathermap.org/
USUARIO   = Vital-Electronics
email     = chicaovital56@gmail.com
SENHA     = Openweather1!
CHAVE API = 150ba34a9a214de99aaaeb09bfb6760f

gitHub
Signed in as :  Vital-Electronics
email: gmail
Senha: Senhagithub1!


*/



#define FIRMWARE_VERSION "MOD: RE32ILI9488IA-V.0.3.0"
const char* URL_VERSION = "https://raw.githubusercontent.com/Vital-Electronics/Regador-Final/main/versionILI9488.txt";
const char* binURL = "https://raw.githubusercontent.com/Vital-Electronics/Regador-Final/main/firmwareILI9488.bin";



//const char* URL_VERSION = "https://raw.githubusercontent.com/Vital-Electronics/Regador-Final/main/versionST7796.txt";
//const char* binURL = "https://raw.githubusercontent.com/Vital-Electronics/Regador-Final/main/firmwareST7796.bin";


volatile float Volume_Atual_cisterna;
double Volume_Maximo_Cisterna = 25.0;
volatile double Quantidade_Usada_Agua = 0.0;
int largura_anterior = 0;  //usada na tela standby para limpar melhor a inf. anterior
#define NUM_ZONAS 7



#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL5QQxUWSIe"
#define BLYNK_TEMPLATE_NAME "Central Garagem"
#define BLYNK_AUTH_TOKEN "diwKGzXa6XRuuN3CY2ANy80afClaZ59M" 
#include <BlynkSimpleEsp32.h>
char auth[] = BLYNK_AUTH_TOKEN;





//---------------------------------------------------------------------------------------------------------------------------------------------------------          Memoria s_da_cidade        ----------------------
#include <EEPROM.h>
#define EEPROM_SIZE 4096
#define POSICAO_DOSAGEM_ACUMULADA            0

#define POSICAO_DOMINGO_PROGRAMADO          40
bool  Domingo_Programado;
#define POSICAO_SEGUNDA_PROGRAMADO          41  
bool  Segunda_Programado;
#define POSICAO_TERCA_PROGRAMADO            42  
bool  Terca_Programado;
#define POSICAO_QUARTA_PROGRAMADO           43  
bool  Quarta_Programado;
#define POSICAO_QUINTA_PROGRAMADO           44  
bool  Quinta_Programado;
#define POSICAO_SEXTA_PROGRAMADO            45  
bool  Sexta_Programado;
#define POSICAO_SABADO_PROGRAMADO           46 
bool  Sabado_Programado; 

#define POSICAO_MINUTOS_ZONA_1              2515 
int Minutos_Zona_1;
#define POSICAO_MINUTOS_ZONA_2              2519 
int Minutos_Zona_2;
#define POSICAO_MINUTOS_ZONA_3              2523 
int Minutos_Zona_3;
#define POSICAO_MINUTOS_ZONA_4              2527 
int Minutos_Zona_4;
#define POSICAO_MINUTOS_ZONA_5              2531 
int Minutos_Zona_5;
#define POSICAO_MINUTOS_ZONA_6              2535 
int Minutos_Zona_6;
#define POSICAO_MINUTOS_ZONA_7              2539 
int Minutos_Zona_7;
#define POSICAO_ULTIMO_DIA_EXECUTADO        1030 //int




#define POSICAO_DOSAGEM_RESET                8         
#define POSICAO_DOSAGEM_REFERENCIA          12   
#define POSICAO_TAMANHO_PISCINA             16      
#define POSICAO_CONCENTRACAO_CLORO          20   
#define POSICAO_HORA_PROGRAMADA             24      
#define POSICAO_MINUTO_PROGRAMADO           28    
#define POSICAO_SEGUNDO_PROGRAMADO          32   
#define POSICAO_TIMEZONE_OFFSET             36 



#define POSICAO_ML_PER_REV_PADRAO           47    
#define POSICAO_ML_PER_REV_AJUSTADO         51  
#define POSICAO_SSID_WIFI                   60           
#define POSICAO_SENHA_WIFI                  316          
#define POSICAO_NOME_CIDADE                 572         
#define POSICAO_PAIS_DA_CIDADE              636         
#define POSICAO_LATITUDE_CIDADE             640         
#define POSICAO_LONGITUDE_CIDADE            644                                   
#define POSICAO_CALIB_TOUCH_X_MIN           648                                  
#define POSICAO_CALIB_TOUCH_X_MAX           652                                  
#define POSICAO_CALIB_TOUCH_Y_MIN           656                                  
#define POSICAO_CALIB_TOUCH_Y_MAX           660
#define TAMANHO_PREVISAO_CLIMA              44
#define NUM_DIAS_FORECAST                   7
#define POSICAO_PREVISAO_CLIMA_0            720 
#define POSICAO_PREVISAO_CLIMA_1            764 
#define POSICAO_PREVISAO_CLIMA_2            808
#define POSICAO_PREVISAO_CLIMA_3            852
#define POSICAO_PREVISAO_CLIMA_4            896
#define POSICAO_PREVISAO_CLIMA_5            940
#define POSICAO_PREVISAO_CLIMA_6            984
#define POSICAO_MODO_AUTOMATICO             1028  
#define POSICAO_ALERTA_LIGADO               1029 

#define POSICAO_HISTORICO_DOSAGEM           1040  // daqui pra frente consumira 4 bytes por dia = 1460 posicoes para guardar 1 ano de histórico
#define POSICAO_DOSAGEM_DIARIA              2500  // proxima disponivel 2508
#define POSICAO_ULTIMA_ZERADA_DOSAGEM_ACUM  2508  // proxima disponivel 
#define POSICAO_PRIMEIRO_ACESSO             2514  // boleana 

#define POSICAO_ALTURA_CISTERNA             2545  // float (4 bytes)
#define POSICAO_proxima_Livre               2549  // proxima disponivel 


#define NUM_DIAS_HISTORICO            365
#define TAMANHO_REGISTRO_HIST         4




//--------------------------------------------------------------------------------------------------------------------------------------------------------          Variáveis         -----------------------
struct PrevisaoClimaDia {
  uint32_t  dataTimestamp; // Timestamp do dia
  float     tempMin;
  float     tempMax;
  uint8_t   condicaoTempo;  // Enum ou código (chuva, limpo, etc)
  uint8_t   probChuva;      // 0-100 (%)
  float     chuvaMM;
  float     ventoMS;
  uint8_t   umidade;        // 0-100
  float     uvi;
  uint32_t  nascerSol; // Timestamp UTC do nascer do sol
  uint32_t  porSol;    // Timestamp UTC do pôr do sol              
};


int     DosagemDiaria;
double  DosagemAcumulada;
int     DosagemReferencia;



bool  Modo_Operacao_Automatico;
float Tamanho_Piscina;
float Concentracao_Cloro;
bool  Alerta_ligado = false;
bool Primeiro_Acesso;

int Dosagem_hora_programada;
int Dosagem_minuto_programado;
volatile int progressoBombeamento = 0; // 0 a 100 (%) tem que ser volatile pois vai ser utilizada por 2 cores diferentes.

typedef enum {
  TELA_STANDBY,
  TELA_MENU_PRINCIPAL,
  TELA_MENU_AJUSTES,
  TELA_MENU_BOMBEAR,
  TELA_MENU_DEFINIR_LOCAL,
  TELA_MENU_AJUSTES_WIFI,
  TELA_DIGITAR_SENHA,
  TELA_MENU_CALIBRAR_BOMBA,
  TELA_MENU_AJUSTE_DOSAGEM_DIARIA,
  TELA_MENU_AJUSTES_OUTROS,
  TELA_CONFIRMAR_ACAO,
  TELA_MENU_MASTER_RESET,
  TELA_MENU_INFORMACOES,
  TELA_DIGITAR_CIDADE,
  TELA_CALIBRAR_TOUCH,
  TELA_PREVISAO_TEMPO,
  TELA_AGENDAR_DOSAGEM,
  TELA_DADOS_HISTORICOS,
  TELA_MENU_ATUALIZAR_FIRMWARE,
  TELA_PROGRESSO_BOMBEAMENTO,
  TELA_NIVEL_CISTERNA
} tela_atual_t;
volatile tela_atual_t tela_atual = TELA_STANDBY;
volatile bool pedirTelaPrincipal = false;

unsigned long ultimoToque = 0;              // Marca o último toque
unsigned long tempoLimiteSemToque = 60000;  // 30 segundos sem toque
unsigned long ultimoUpdateMenu = 0;         // Última atualização do menu principal

//Calibracao do touch tft
int calib_x_min = 300;
int calib_x_max = 3950;
int calib_y_min = 200;
int calib_y_max = 3900;

typedef struct {
  int x_raw;
  int y_raw;
  int x_ref;
  int y_ref;
} PontoCalibracao;

PontoCalibracao pontosCalibracao[5];  // 5 pontos de calibração
uint8_t etapaCalibracao = 0;
bool calibrandoTouch = false;

// Coordenadas de referência na tela para os círculos
const int coordsCalibracao[5][2] = {
  { 6, 6 },      // canto superior esquerdo
  { 474, 6 },    // canto superior direito
  { 240, 160 },  // centro
  { 6, 314 },    // canto inferior esquerdo
  { 474, 314 }   // canto inferior direito
};


//Variaveis tela standby
int xstandby = 10;
int ystandby = 10;
int dxstandby = 2;  // Velocidade horizontal
int dystandby = 2;  // Velocidade vertical

bool bombeando = false;
bool sugando = false;
int frameAtual = 1;



//-------------------------------------------------------------------------------------------------------------------------------------------------------         TFT         -----------------------------
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "Free_Fonts.h"
#include "Logotipos.h"
TFT_eSPI tft;
#define TOUCH_CS 21
XPT2046_Touchscreen ts(TOUCH_CS);

// Desenhar o Logo com Raio Laser
#define BMP_WIDTH 60
#define BMP_HEIGHT 46
#define POS_X 210
#define POS_Y 80
#define ORIGEM_X 480
#define ORIGEM_Y 320

#include <math.h>

const char* keysMaiusculas[4][10] = {
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" },
  { "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P" },
  { "A", "S", "D", "F", "G", "H", "J", "K", "L", "#+=" },
  { "SHIF", "Z", "X", "C", "   ", "V", "B", "N", "M", "<=" }
};

const char* keysMinusculas[4][10] = {
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" },
  { "q", "w", "e", "r", "t", "y", "u", "i", "o", "p" },
  { "a", "s", "d", "f", "g", "h", "j", "k", "l", "#+=" },
  { "SHIF", "z", "x", "c", "   ", "v", "b", "n", "m", "<=" }
};

const char* keysSimbolos[4][10] = {
  { "!", "@", "#", "$", "%", "^", "&", "*", "(", ")" },
  { "1", "2", "3", "4", "5", "6", "7", "8", "9", "0" },
  { "-", "=", "+", "[", "]", "{", "}", "", "", "#+=" },
  { "SHIF", ";", ":", "\"", "   ", ",", ".", "<", ">", "<=" }
};

const char* dias_semana_pt[7] = { "Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab" };
const char* meses_pt[12] = { "Jan", "Fev", "Mar", "Abr", "Mai", "Jun", "Jul", "Ago", "Set", "Out", "Nov", "Dez" };


#define stepPin 32
#define dirPin 33
#define enablePin 27
const int stepsPerRevolution = 200;  // 200 Steps per revolution = 1,5ml/revolution (padrao).
float mlPerRevolutionPadrao;
float mlPerRevolutionAjustado;
float TotalBombeado = 0.0;



//--------------------------------------------------------------------------------------------------------------------------------------          WiFi          ----------------------------

#include "WiFi.h"
char ssidSelecionado[32] = "";  // Nome da rede selecionada
char wifiSenha[64] = "";        // Senha digitada pelo usuário
int redeSelecionadaIndex = -1;  // Índice da rede escaneada
uint8_t modoTeclado = 0;        // 0 = maiúsculas, 1 = minúsculas, 2 = símbolos
uint8_t modoAlfabeto = 0;       // 0 = maiusculas, 1 = minusculas
int redesDisponiveis = -1;      // -1 significa: nenhum scan ainda feito
unsigned long wifiDesconectadoDesde = 0;

#include <HTTPClient.h>
#include <Update.h>

#include <ArduinoJson.h>
#include <time.h>
const char* apiKey = "150ba34a9a214de99aaaeb09bfb6760f";  // OpenWeatherMap
String cidade = "Lisboa";
long timezoneOffset;
unsigned long ultimoUpdateHora = 0;
String horaAtualTela = "";
char cidade_digitada[50] = "";
String pais_da_cidade = "";     // Código ISO do país, ex: "BR", "US"
bool horaSincronizada = false;  // Sempre inicia como falsa ao ligar

// Obter informacoes climaticas
String climaDescricao, climaPrincipal;
float temperaturaAtual, temperaturaMin, temperaturaMax;
int umidade, pressao, nuvens, visibilidade, ventoVelocidade;
String nascerDoSol, porDoSol;
float cidade_latitude;
float cidade_longitude;

unsigned long tempoInicioSistema = millis();
bool precisaObterForecast = true;

PrevisaoClimaDia previsoes[7];

unsigned long proximaTentativaNTP = 0;





#include <HCSR04.h>  //by gemgine
HCSR04 hc(32, 33);  //initialisation class HCSR04 (trig pin , echo pin)
float nivel;
float distancia;
float altura_cisterna;// Ajuste fino para calibrar o volume - original 258 - quanto maior a altura da cisterna ele vai marcar mais volume de agua.
#define qtdmedidas 100  //Quantidade de medidas para tirar média
bool rodar_nivel = true;
void VerificarNivelCisterna();
const float LITROS_POR_CM = 14000.0 / 134.5; // Aproximadamente 104.0892 Litros/cm

#include <Wire.h>
#include <DFRobot_MCP23017.h>
// Objeto I2C dedicado
TwoWire I2CBUS = TwoWire(0);
// Endereço correto (conforme scanner)
DFRobot_MCP23017 mcp(I2CBUS, 0x20);




//----------------------------------------------------------------------------------------------------------------------------------          Tasks Due Core          ------------------------------------------
TaskHandle_t handleNivelCisterna = NULL;
TaskHandle_t handleMonitorBombeado = NULL;
TaskHandle_t handleMonitorSugado = NULL;
void Tarefa_SincronizaHora(void* pvParameters);



BLYNK_WRITE(V4) {  // Horta/Fundos
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPA3, x);
}

BLYNK_WRITE(V8) {  // Escada
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPA4, x);
}

BLYNK_WRITE(V2) {  // Coqueiro
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPA5, x);
}

BLYNK_WRITE(V7) {  // Bananeira
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPA6, x);
}

BLYNK_WRITE(V25) {  // Oliveira
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPA7, x);
}

BLYNK_WRITE(V3) {  // Fonte
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPB7, x);
}

BLYNK_WRITE(V6) {  // Rampa
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPB6, x);
}

BLYNK_WRITE(V9) {  // Chafariz
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPB5, x);
}

BLYNK_WRITE(V10) {  // Luzes do Jardim
  int x = param.asInt();
  mcp.digitalWrite(mcp.eGPB4, x);
}




//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$&&&&&&&$$$$$$$$$$&&&&&$$$$$$$$$$$$$$$$$$$     SETUP     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("VITAL Electronics - IOT Solutions", 240, 10);
  tft.setFreeFont(FSB9);
  tft.drawString("Regador Inteligente", 240, 40);
  tft.setFreeFont(FF0);
  tft.drawString(String(FIRMWARE_VERSION), 240, 60);

  tft.setTextColor(TFT_WHITE);
  tft.setFreeFont(FF1);

  desenharComRaios();

  ts.begin();
  ts.setRotation(1);  // Alinhado com a tela

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Verificando Sistema....", 240, 140);
  tft.setCursor(15, 50);
  tft.setFreeFont(FF1);
  tft.println("");
  tft.println("");
  tft.println("");
  tft.println("");
  tft.println("");
  tft.println("");
  tft.println("");
  delay(500);

  if (!EEPROM.begin(EEPROM_SIZE)) {
    Serial.println("Falha ao inicializar EEPROM");
    return;
  }


  tft.println("");
  tft.print("EEPROM (Memoria)");
  for (int d; d < 25; d++) {
    tft.print(".");
    delay(10);
  }
  tft.println("OK");

  tft.println("");
  tft.print("CPU - Dual Core - 240 MHz");
  for (int e; e < 14; e++) {
    tft.print(".");
    delay(30);
  }
  tft.println("OK");

  tft.println("");
  tft.print("Bomba");
  for (int f; f < 34; f++) {
    tft.print(".");
    delay(10);
  }
  tft.println("OK");


  tft.println("");
  tft.print("Tela - TFT 480x320(Touch)");
  for (int g; g < 14; g++) {
    tft.print(".");
    delay(10);
  }
  tft.println("OK");


  //delay(1000);
  altura_cisterna = EEPROM.readFloat(POSICAO_ALTURA_CISTERNA);
  VerificarNivelCisterna();

  WiFi.mode(WIFI_STA);
  Conexao_WiFi_Setup();
  Mostrar_Menu_Principal();
  tela_atual = TELA_MENU_PRINCIPAL;


  DosagemAcumulada =        EEPROM.readDouble(POSICAO_DOSAGEM_ACUMULADA);
  DosagemDiaria =           EEPROM.readInt(POSICAO_DOSAGEM_DIARIA);
  DosagemReferencia =       EEPROM.readInt(POSICAO_DOSAGEM_REFERENCIA);
  mlPerRevolutionPadrao =   EEPROM.readFloat(POSICAO_ML_PER_REV_PADRAO);
  mlPerRevolutionAjustado = EEPROM.readFloat(POSICAO_ML_PER_REV_AJUSTADO);
  Primeiro_Acesso =         EEPROM.read(POSICAO_PRIMEIRO_ACESSO) != 0; 

  cidade =                  EEPROM.readString(POSICAO_NOME_CIDADE);              // Nome da cidade (com acento, para lógica/API)
  Serial.print("SETUP: cidade da EEPROM = [");
  Serial.print(cidade);
  Serial.println("]");
  cidade_latitude =         EEPROM.readFloat(POSICAO_LATITUDE_CIDADE);  // Latitude salva
  cidade_longitude =        EEPROM.readFloat(POSICAO_LONGITUDE_CIDADE);                     // Longitude salva
  EEPROM.get(POSICAO_TIMEZONE_OFFSET, timezoneOffset);
  horaSincronizada = false;  // Sempre inicia como falsa ao ligar
  EEPROM.get(POSICAO_MODO_AUTOMATICO, Modo_Operacao_Automatico);
  EEPROM.get(POSICAO_ALERTA_LIGADO, Alerta_ligado);
  
  Dosagem_hora_programada = EEPROM.readInt(POSICAO_HORA_PROGRAMADA);
  Dosagem_minuto_programado = EEPROM.readInt(POSICAO_MINUTO_PROGRAMADO);
  


  // 1. Checa conexão Wi-Fi para sincronizar hora
  if (WiFi.status() == WL_CONNECTED) {
    configTime(timezoneOffset, 0, "pool.ntp.org");  // Configura o RTC/NTP para o timezone correto
    // 2. Tenta sincronizar por até 10 segundos
    time_t now = time(nullptr);
    int tentativas = 0;
    while (now < 100000 && tentativas < 20) {  // Valor inválido
      delay(500);
      now = time(nullptr);
      tentativas++;
    }

    // 3. Verifica resultado
    if (now < 100000) {
      Serial.println("NÃO conseguiu sincronizar hora via NTP! Vai usar RTC interno.");
      horaSincronizada = false;
    } else {
      struct tm* tm_info = localtime(&now);
      char buf[32];
      strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", tm_info);
      Serial.print("Hora SINCRONIZADA: ");
      Serial.println(buf);
      horaSincronizada = true;
    }

     Blynk.config(auth);
     Blynk.connect();  // tenta conexão com o servidor Blynk


  } else {
    Serial.println("Sem conexão Wi-Fi. Usando RTC interno.");
    horaSincronizada = false;
  }






  // Tarefa Core 1 para sincronizar hora todo dia as 4:00am
  xTaskCreatePinnedToCore(Tarefa_SincronizaHora, "SincronizaHoraTask", 4096, NULL, 1, NULL, 1);

  xTaskCreatePinnedToCore(TarefaVerificarNivelCisterna, "NivelCisterna", 4096, NULL, 1, &handleNivelCisterna, 1);
  



  String nomeSemAcentos = removerAcentos(cidade);
  nomeSemAcentos.toCharArray(cidade_digitada, sizeof(cidade_digitada));


  calib_x_min = EEPROM.readInt(POSICAO_CALIB_TOUCH_X_MIN);
  calib_x_max = EEPROM.readInt(POSICAO_CALIB_TOUCH_X_MAX);
  calib_y_min = EEPROM.readInt(POSICAO_CALIB_TOUCH_Y_MIN);
  calib_y_max = EEPROM.readInt(POSICAO_CALIB_TOUCH_Y_MAX);
  // VALIDAÇÃO DE VALORES: Se não faz sentido, volta ao padrão!
  if (calib_x_min < 100 || calib_x_max > 4095 || calib_x_min >= calib_x_max || calib_y_min < 100 || calib_y_max > 4095 || calib_y_min >= calib_y_max) {
    calib_x_min = 300;
    calib_x_max = 3950;
    calib_y_min = 200;
    calib_y_max = 3900;
  }


  LerPrevisoesDaEEPROM(previsoes, 7);


  I2CBUS.begin(4, 22); // SDA=4, SCL=22
  mcp.begin();

  Serial.println("MCP23017 inicializado com sucesso!");
  mcp.pinMode(mcp.eGPA3, OUTPUT);  //Rele1
  mcp.pinMode(mcp.eGPA4, OUTPUT);  //Rele2
  mcp.pinMode(mcp.eGPA5, OUTPUT);  //Rele3
  mcp.pinMode(mcp.eGPA6, OUTPUT);  //Rele4
  mcp.pinMode(mcp.eGPA7, OUTPUT);  //Rele5
  mcp.pinMode(mcp.eGPB7, OUTPUT);  //Rele6
  mcp.pinMode(mcp.eGPB6, OUTPUT);  //Rele7
  mcp.pinMode(mcp.eGPB5, OUTPUT);  //Rele8 - Luzes do Jardim
  mcp.pinMode(mcp.eGPB4, OUTPUT);  //Rele9 - Chafariz
  

}
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
//$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$     LOOP     $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
void loop() {

    if (pedirTelaPrincipal) {
      tela_atual = TELA_MENU_PRINCIPAL;
      Mostrar_Menu_Principal();
      pedirTelaPrincipal = false; 
    }

  if (precisaObterForecast && (millis() - tempoInicioSistema > 3000)) {
    if (WiFi.status() == WL_CONNECTED) {
        obterForecastClimatico(cidade); // Executa requisição apenas se estiver conectado!
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.drawString("Sem conexao Wi-Fi!", 100, 40);
        Serial.println("Sem conexao Wi-Fi. Forecast NÃO solicitado.");
    }
    precisaObterForecast = false;
  }
  


    // Se hora NÃO está sincronizada E Wi-Fi está conectado, tente sincronizar
  if (!horaSincronizada && WiFi.status() == WL_CONNECTED && millis() > proximaTentativaNTP) {
      Serial.println("Tentando sincronizar hora NTP...");
      configTime(timezoneOffset, 0, "pool.ntp.org");
      delay(1200); // Dá um tempinho para resposta do NTP
      time_t now = time(nullptr);
      if (now > 100000) {
          horaSincronizada = true;
          Serial.println("Hora sincronizada com sucesso!");
      } else {
          Serial.println("Falha ao sincronizar. Tentando novamente em 30s.");
          proximaTentativaNTP = millis() + 30000; // nova tentativa em 30s
      }
  }




  if (ts.touched()) { ultimoToque = millis(); }  // Atualiza temporizador de inatividade
  
  handleTouch();
  

   // MOSTRAR TELA DE PROGRESSO BOMBEAMENTO
  if (tela_atual == TELA_PROGRESSO_BOMBEAMENTO) {
    // Só chama uma vez ao entrar na tela:
    Mostrar_Tela_Progresso_Bombeamento();
    return; 
  }


  if (tela_atual == TELA_STANDBY) { Mostrar_Menu_Standby(); }


 if ((tela_atual != TELA_STANDBY) && (millis() - ultimoToque > tempoLimiteSemToque) && !calibrandoTouch) { tft.fillScreen(TFT_BLACK);  tela_atual = TELA_STANDBY;  Mostrar_Menu_Standby(); }



  if (tela_atual == TELA_MENU_DEFINIR_LOCAL) {
    if (millis() - ultimoUpdateHora >= 1000) {  // 1 segundo
      horaAtualTela = imprimirHoraAtual();
      ultimoUpdateHora = millis();

      tft.fillRoundRect(300, 55, 100, 30, 10, TFT_BLACK);
      tft.setTextDatum(TC_DATUM);
      tft.drawString(horaAtualTela, 230, 60);
      tft.drawString(removerAcentos(cidade) + ", " + pais_da_cidade, 230, 90);
      delay(5);
    }
  }



  if (tela_atual == TELA_MENU_PRINCIPAL) {
      if (millis() - ultimoUpdateHora >= 1000) {
          String completa;
          if (horaSincronizada) {
              completa = imprimirHoraAtual(); // só chama se sincronizada, nunca trava!
          } else {
              completa = "Erro em obter Hora!";
              // Opcional: aqui você pode tentar re-sincronizar, se quiser.
          }
          tft.setTextColor(TFT_YELLOW);
          tft.setFreeFont(FF1);
          tft.fillRoundRect(0, 0, 400, 60, 7, TFT_BLACK);
          tft.setTextDatum(TC_DATUM);
          tft.drawString(completa, 230, 10);
          ultimoUpdateHora = millis();

          // Mostra ícone Wi-Fi
          if (WiFi.status() == WL_CONNECTED) {
              wifiDesconectadoDesde = 0;
              int sinal = WiFi.RSSI();
              if (sinal >= -50) {
                  tft.drawBitmap(5, 5, Sinal_100_45x45, 45, 45, TFT_WHITE);
              } else if (sinal >= -60) {
                  tft.drawBitmap(5, 5, Sinal_75_45x45, 45, 45, TFT_WHITE);
              } else if (sinal >= -70) {
                  tft.drawBitmap(5, 5, Sinal_50_45x45, 45, 45, TFT_WHITE);
              } else if (sinal >= -80) {
                  tft.drawBitmap(5, 5, Sinal_25_45x45, 45, 45, TFT_WHITE);
              } else {
                  tft.drawBitmap(5, 5, Sinal_0_45x45, 45, 45, TFT_WHITE);
              }
          } else {
              if (wifiDesconectadoDesde == 0) {
                  wifiDesconectadoDesde = millis();
              }
              tft.fillRoundRect(0, 0, 60, 60, 5, TFT_BLACK);
              tft.drawRoundRect(5, 5, 55, 55, 10, TFT_RED);
              tft.drawBitmap(10, 10, No_WiFi_45x45, 45, 45, TFT_YELLOW);
          }
      }
  }




  Verificar_Hora_e_Dosar();

  Blynk.run();

}





//###############################################     Menu:  STANDBY     ##############################################################
void Mostrar_Menu_Standby() {
  Serial.println("Executando Mostrar_Menu_Standby");

  TotalBombeado = 0.0;
  int velocidade_animacao = 150;

  float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
  char buf[24];
  dtostrf(volume_m3, 4, 1, buf);
  strcat(buf, " m3");

  static unsigned long ultimaAnimacao = 0;
  static int largura_anterior = 0;

  if (millis() - ultimaAnimacao >= velocidade_animacao) {
    ultimaAnimacao = millis();

    tft.setFreeFont(FSS24);
    tft.setTextDatum(MC_DATUM);

    int largura = tft.textWidth(buf);
    int altura = tft.fontHeight();

    // Apaga texto anterior
    tft.fillRect(xstandby - largura_anterior / 2, ystandby - altura / 2, largura_anterior, altura, TFT_BLACK);

    // Atualiza posição
    xstandby += dxstandby;
    ystandby += dystandby;

    // Colisão horizontal
    if (xstandby - largura / 2 < 0) {
      xstandby = largura / 2;
      dxstandby = -dxstandby;
    } else if (xstandby + largura / 2 > tft.width()) {
      xstandby = tft.width() - largura / 2;
      dxstandby = -dxstandby;
    }

    // Colisão vertical
    if (ystandby - altura / 2 < 0) {
      ystandby = altura / 2;
      dystandby = -dystandby;
    } else if (ystandby + altura / 2 > tft.height()) {
      ystandby = tft.height() - altura / 2;
      dystandby = -dystandby;
    }

    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.drawString(buf, xstandby, ystandby);

    // Salva largura atual para próxima limpeza
    largura_anterior = largura;
  }

  // Reset datum e texto
  tft.setTextDatum(TL_DATUM);
  tft.setTextSize(1);
}

//###############################################     Menu:  PRINCIPAL     ############################################################
void Mostrar_Menu_Principal() {
  Serial.println("Executando Mostrar_Menu_Principal ");

  Domingo_Programado = (bool)EEPROM.read(POSICAO_DOMINGO_PROGRAMADO);
  Segunda_Programado = (bool)EEPROM.read(POSICAO_SEGUNDA_PROGRAMADO);
  Terca_Programado = (bool)EEPROM.read(POSICAO_TERCA_PROGRAMADO);
  Quarta_Programado = (bool)EEPROM.read(POSICAO_QUARTA_PROGRAMADO);
  Quinta_Programado = (bool)EEPROM.read(POSICAO_QUINTA_PROGRAMADO);
  Sexta_Programado = (bool)EEPROM.read(POSICAO_SEXTA_PROGRAMADO);
  Sabado_Programado = (bool)EEPROM.read(POSICAO_SABADO_PROGRAMADO);

  DosagemDiaria = EEPROM.readInt(POSICAO_DOSAGEM_DIARIA);
  Dosagem_hora_programada = EEPROM.readInt(POSICAO_HORA_PROGRAMADA);
  Dosagem_minuto_programado = EEPROM.readInt(POSICAO_MINUTO_PROGRAMADO);
  DosagemAcumulada = EEPROM.readDouble(POSICAO_DOSAGEM_ACUMULADA);

  String ssidAtual;
  String ipStr;

  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FF0);

  int barra_x = 5;
  int barra_y = 70;
  int barra_largura = 50;
  int barra_altura = 220;

  // Calcula percentual preenchido
  float percentual = Volume_Atual_cisterna / (Volume_Maximo_Cisterna * 1000.0);
  if (percentual < 0) percentual = 0;
  if (percentual > 1) percentual = 1;

  // Altura da barra preenchida
  int preenchido = (int)(barra_altura * percentual);

  //Barra volume
  tft.drawRoundRect(barra_x, barra_y, barra_largura, barra_altura, 5, TFT_GREEN);
  tft.fillRoundRect(barra_x + 2, barra_y + barra_altura - preenchido + 2, barra_largura - 4, preenchido - 4 > 0 ? preenchido - 4 : 0, 3, TFT_CYAN);
  tft.setTextDatum(MC_DATUM);
  tft.setFreeFont(FSB9);
  tft.setTextColor(TFT_BLACK, TFT_CYAN);
  float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
  tft.drawString(String(volume_m3, 1), 30, 270);
 
  tft.drawRoundRect(100, 60, 155, 40, 5, TFT_GREEN);     tft.drawRoundRect(255, 60, 110, 40, 5, TFT_GREEN);
  tft.drawRoundRect(100, 100, 155, 40, 5, TFT_GREEN);     tft.drawRoundRect(255, 100, 110, 40, 5, TFT_GREEN);
  tft.setFreeFont(FF1);
  tft.setTextDatum(ML_DATUM);
   tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Agua ACUM (L)",  110, 78);
  tft.drawString("Hora REGA", 110, 118);
  tft.setTextDatum(MR_DATUM);
  tft.drawString(String(DosagemAcumulada,0), 345, 78);

  char horarioCompleto[6]; // "hh:mm" + '\0'
  sprintf(horarioCompleto, "%02d:%02d", Dosagem_hora_programada, Dosagem_minuto_programado);
  tft.drawString(horarioCompleto, 345, 118);

  // ====== Quadrados de zonas e dias ======
  int quadrado_tam = 40;
  int espaco = 2;
  int x_inicial = 85;
  int y_zonas  = 130;   // linha de cima
  int y_dias   = 200;   // linha de baixo

  bool zonas[7] = {
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_1) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_2) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_3) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_4) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_5) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_6) > 0,
  EEPROM.readInt(POSICAO_MINUTOS_ZONA_7) > 0
  };

  // Agora, dias REALMENTE lidos da EEPROM:
  bool dias[7]  = {
    Segunda_Programado,
    Terca_Programado,
    Quarta_Programado,
    Quinta_Programado,
    Sexta_Programado,
    Sabado_Programado,
    Domingo_Programado
  };

  // Nomes dos dias
  const char* nomes_dias[7] = {"S", "T", "Q", "Q", "S", "S", "D"}; // S=Seg, T=Ter, Q=Qua, Q=Qui, S=Sex, S=Sab, D=Dom


  tft.setTextDatum(ML_DATUM);
  tft.drawString("Zonas",  x_inicial,  y_zonas+35);
  tft.drawString("Dias",   x_inicial,  y_dias+35);

  for (int i = 0; i < 7; i++) {
    int x = x_inicial + i * (quadrado_tam + espaco);

    // ----- Linha de zonas (em cima) -----
    tft.drawRoundRect(x, y_zonas+50, quadrado_tam, quadrado_tam, 5,  TFT_WHITE);
    if (zonas[i]) {
      tft.fillRoundRect(x+2, y_zonas+2+50, quadrado_tam-4, quadrado_tam-4, 5, TFT_GREEN);
      tft.setTextColor(TFT_BLACK, TFT_GREEN);
    } else {
      tft.fillRoundRect(x+2, y_zonas+2+50, quadrado_tam-4, quadrado_tam-4, 5, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FF1);
    tft.drawString(String(i+1), x + quadrado_tam/2, y_zonas + (quadrado_tam/2)+49);

    // ----- Linha de dias (embaixo) -----
    tft.drawRoundRect(x, y_dias+50, quadrado_tam, quadrado_tam-5, 5, TFT_WHITE);
    if (dias[i]) {
      tft.fillRoundRect(x+2, y_dias+2+50, quadrado_tam-4, quadrado_tam-9, 5, TFT_YELLOW);
      tft.setTextColor(TFT_BLACK, TFT_YELLOW);
    } else {
      tft.fillRoundRect(x+2, y_dias+2+50, quadrado_tam-4, quadrado_tam-9, 5, TFT_BLACK);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
    }
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FF1);
    tft.drawString(nomes_dias[i], x + (quadrado_tam/2)-2, y_dias + (quadrado_tam/2)+46);
  }
  // ====== FIM DOS QUADRADOS ======

  tft.setFreeFont(FF0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //Botao Ajustes
  tft.fillRoundRect(405, 5, 55, 55, 7, TFT_LIGHTGREY);
  tft.drawBitmap(410, 10, Settings45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("AJUSTES", 433, 66);  // Título

  //Botao Bomba
  tft.fillRoundRect(405, 90, 55, 55, 7, TFT_CYAN);  //Botao BombeaR
  tft.drawBitmap(410, 95, Bomba45x45, 45, 45, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("BOMBA", 433, 151);  // Título

  //Botao INFO
  tft.fillRoundRect(405, 175, 55, 55, 7, TFT_ORANGE);  //Botao Sugar
  tft.drawBitmap(410, 180, Info45x45, 45, 45, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("INFO", 433, 236);  // Título

  //Botao Reiniciar
  tft.fillRoundRect(405, 260, 55, 55, 7, TFT_RED);  //Botao Reinciar
  tft.drawBitmap(410, 265, Reiniciar45x45, 45, 45, TFT_WHITE);

  //Rodapé
  tft.setCursor(0, 305);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FF0);
  tft.print(FIRMWARE_VERSION);

  if (WiFi.status() == WL_CONNECTED) {
      ssidAtual = WiFi.SSID();
      IPAddress ip = WiFi.localIP();
      ipStr = ip.toString();
  } else {
      ssidAtual = "-= ERRO =-";
      ipStr    =  "-= ERRO =-";
  }
}

//###############################################     Menu:  AJUSTES WIFI     #########################################################
void Mostrar_Menu_Ajustes_WIFi() {
  Serial.println("Executando Mostrar_Menu_Ajustes_WIFi");

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes Wi-Fi
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, WiFi45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - Wi-Fi", 210, 12);  // Título

  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_YELLOW);
  tft.setCursor(40, 100);
  tft.print("Escaneando redes disponíveis.");
  tft.setCursor(40, 150);
  tft.print("Aguarde............");

  delay(100);  // Pequena pausa antes do scan

  WiFi.disconnect(true);
  delay(200);

  redesDisponiveis = WiFi.scanNetworks();  // Armazena globalmente
  delay(200);                              // Garantia extra para os dados estarem prontos

  // Limpa área da mensagem de carregamento
  tft.fillRect(0, 40, 480, 280, TFT_BLACK);
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);
  tft.drawBitmap(425, 5, WiFi45x45, 45, 45, TFT_WHITE);


  if (redesDisponiveis == 0) {
    tft.setTextColor(TFT_RED);
    tft.setCursor(10, 100);
    tft.print("Nenhuma rede encontrada");
    delay(1500);
    tela_atual = TELA_MENU_AJUSTES;
    Mostrar_Menu_Ajustes();
    return;
  }

  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 60);
  tft.printf("%d redes encontradas:", redesDisponiveis);


  for (int i = 0; i < redesDisponiveis && i < 5; i++) {
    String ssid = WiFi.SSID(i);
    int rssi = WiFi.RSSI(i);

    if (ssid.length() == 0) {
      ssid = "<HIDDEN>";
    }

    int linhaY = 105 + i * 35;
    tft.setCursor(10, linhaY);
    tft.printf("%2d) %-24.24s | %ddBm", i + 1, ssid.c_str(), rssi);
  }



  //Botao D3 - Atualizar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Refresh45x45, 45, 45, TFT_BLACK);

  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);

  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);
}
//###############################################     Menu:  DIGITAR CIDADE     #######################################################
void Mostrar_Tela_Digitar_Cidade() {
  Serial.println("Executando Mostrar_Tela_Digitar_Cidade");
  tft.fillScreen(TFT_BLACK);

  cidade_digitada[0] = '\0';  // <-- limpa o conteúdo anterior
  memset(cidade_digitada, 0, sizeof(cidade_digitada));

  // Cabeçalho
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.fillRect(0, 0, 480, 30, TFT_BLUE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Digite o nome da Cidade", 239, 2);


  // Campo de senha
  tft.setFreeFont(FF1);
  tft.drawRect(10, 60, 450, 30, TFT_GREEN);
  tft.setCursor(20, 80);
  tft.print("Cidade:");
  tft.println(removerAcentos(String(cidade_digitada)));

  // Chama o teclado
  desenharTeclado();
}
//###############################################     Menu:  AJUSTES     #######·######################################################
void Mostrar_Menu_Ajustes() {
  Serial.println("Executando Mostrar_Menu_Ajustes");

  wifiSenha[0] = '\0';  // Limpa a senha temporária
  ssidSelecionado[0] = '\0';

  //Limpa a atela
  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_DARKGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Settings45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes", 210, 12);  // Título


  tft.setFreeFont(FF0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);



  //Botao A1
  tft.fillRoundRect(20, 65, 55, 55, 7, TFT_BLUE);  //Botao Sugar
  tft.drawBitmap(25, 70, WiFi45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("WI-FI", 48, 126);  // Título
  //Botao A2
  tft.fillRoundRect(20, 150, 55, 55, 7, TFT_BLUE);  //Botao Sugar
  tft.drawBitmap(25, 155, Mapa45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("LOCAL", 48, 211);  // Título
  //Botao A3
  tft.fillRoundRect(20, 235, 55, 55, 7, TFT_BLUE);  //Botao Sugar
  tft.drawBitmap(25, 240, Graficos45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("DADOS", 48, 296);  // Título


  //Botao B1
  tft.fillRoundRect(110, 65, 55, 55, 7, TFT_BLUE);  //Botao 
  tft.drawBitmap(115, 70, Calibrar45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("CALIBRAR BOMBA", 138, 126);  // Título
  //Botao B2
  tft.fillRoundRect(110, 150, 55, 55, 7, TFT_BLUE);  //Botao 
  tft.drawBitmap(115, 155, Tela45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("CALIBRAR TOUCH", 138, 211);  // Título
  //Botao B3
  tft.fillRoundRect(110, 235, 55, 55, 7, TFT_BLUE);  //Botao 
  tft.drawBitmap(115, 240, Update45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SOBRE", 138, 296);  // Título





  //Botao C1
  tft.fillRoundRect(200, 65, 55, 55, 7, TFT_BLUE);  
  tft.drawBitmap(205, 70, Dosagem45x45, 45, 45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("DOSAGEM", 228, 126);
  //Botao C2
  tft.fillRoundRect(200, 150, 55, 55, 7, TFT_BLUE);   
  tft.drawBitmap(205, 155, Agenda45x45, 45,45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("AGENDA", 228, 211);
  //Botao C3
  tft.fillRoundRect(200, 235, 55, 55, 7, TFT_RED);  
  tft.drawBitmap(205, 240, Reset45x45, 45, 45, TFT_YELLOW);
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("RESET FABRICA", 229, 296);

  
  //Botao D1
  tft.fillRoundRect(290, 65, 55, 55, 7, TFT_BLUE);  
  tft.drawBitmap(295, 70, Nivel_Cisterna45x45, 45, 45, TFT_WHITE);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("CISTERNA", 318, 126);
  
  /*
  //Botao D2
  tft.fillRoundRect(200, 150, 55, 55, 7, TFT_BLUE);   
  tft.drawBitmap(205, 155, Agenda45x45, 45,45, TFT_WHITE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("AGENDA", 228, 211);
  //Botao D3
  tft.fillRoundRect(200, 235, 55, 55, 7, TFT_RED);  
  tft.drawBitmap(205, 240, Reset45x45, 45, 45, TFT_YELLOW);
  tft.setTextDatum(TC_DATUM);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString("RESET FABRICA", 229, 296);
  */




  //Botao - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);

  //Botao - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);
}
//###############################################     Menu:  CALIBRAR BOMBA    ########################################################
void Mostrar_Menu_Calibrar_Bomba() {
  Serial.println("Executando Mostrar_Menu_Calibrar_Bomba");
  mlPerRevolutionAjustado = EEPROM.readFloat(POSICAO_ML_PER_REV_AJUSTADO);

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes Calirar Bomba
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Calibrar45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - Calibrar Vazao", 210, 12);


  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);


  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(FF1);


  //Mensagem
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Pressione (+) ou (-)", 205, 55);
  tft.drawString("para ajustar em 1% os litros/min", 205, 75);


  tft.setFreeFont(FF3);
  //Botao (+)
  tft.drawRoundRect(340, 175, 40, 40, 7, TFT_YELLOW);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-", 359, 193);
  //Botao (-)
  tft.drawRoundRect(410, 175, 40, 40, 7, TFT_YELLOW);
  tft.drawString("+", 429, 193);


  tft.setFreeFont(FF2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRect(140, 120, 90, 35, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.setCursor(10, 145);
  tft.println("Padrao:          L/min");
  tft.drawRect(140, 180, 90, 35, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.setCursor(10, 205);
  tft.println("Ajustado:        L/min");

  tft.setTextDatum(CR_DATUM);
  tft.drawString(String(mlPerRevolutionPadrao, 0), 215, 135);
  tft.setTextDatum(CR_DATUM);
  tft.drawString(String(mlPerRevolutionAjustado, 0), 215, 196);
}
//###############################################     Menu:  DOSAGEM DIARIA     #######################################################
void Mostrar_Menu_Ajuste_Dosagem_diaria() {
  Serial.println("Executando Mostrar_Menu_Ajuste_Dosagem_diaria");

  Minutos_Zona_1 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_1);
  Minutos_Zona_2 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_2);
  Minutos_Zona_3 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_3);
  Minutos_Zona_4 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_4);
  Minutos_Zona_5 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_5);
  Minutos_Zona_6 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_6);
  Minutos_Zona_7 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_7);
  
 
  
  
  DosagemDiaria = EEPROM.readInt(POSICAO_DOSAGEM_DIARIA);  // Use readInt em vez de readFloat
  DosagemReferencia = EEPROM.readInt(POSICAO_DOSAGEM_REFERENCIA);
  
   tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes Dosagem Diaria
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Dosagem45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - Rega", 210, 12);


  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  tft.setFreeFont(FF1);
  tft.setTextDatum(ML_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);


  // Painel
  tft.drawRoundRect( 5, 40,  50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 40,  150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205,  40, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290,  40, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340,  40, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 70,  50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 70,  150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205,  70, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290,  70, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340,  70, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 100, 50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 100, 150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205, 100, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290, 100, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340, 100, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 130, 50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 130, 150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205, 130, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290, 130, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340, 130, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 160, 50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 160, 150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205, 160, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290, 160, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340, 160, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 190, 50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 190, 150, 30 ,5, TFT_GREEN);  tft.drawRoundRect(205, 190, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290, 190, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340, 190, 30,30, 5, TFT_GREEN);
  tft.drawRoundRect( 5, 220, 50, 30, 5, TFT_GREEN);  tft.drawRoundRect( 55, 220, 150, 30, 5, TFT_GREEN);  tft.drawRoundRect(205, 220, 50,30, 5, TFT_GREEN);    tft.drawRoundRect(290, 220, 30, 30, 5, TFT_GREEN);   tft.drawRoundRect(340, 220, 30,30, 5, TFT_GREEN);


  tft.drawString("1", 25,  54);                     tft.drawString("Horta",     60,   54);              tft.drawString("-", 300,  54);  tft.drawString("+", 350,  54);           
  tft.drawString("2", 25,  84);                     tft.drawString("Escada",    60,   84);              tft.drawString("-", 300,  85);  tft.drawString("+", 350,  85);
  tft.drawString("3", 25, 114);                     tft.drawString("Coqueiro",  60,  114);              tft.drawString("-", 300,  114); tft.drawString("+", 350,  114);
  tft.drawString("4", 25, 144);                     tft.drawString("Bananeira", 60,  144);              tft.drawString("-", 300,  144); tft.drawString("+", 350,  144);
  tft.drawString("5", 25, 174);                     tft.drawString("Oliveira",  60,  174);              tft.drawString("-", 300,  174); tft.drawString("+", 350,  174);
  tft.drawString("6", 25, 204);                     tft.drawString("Fonte",     60,  204);              tft.drawString("-", 300,  204); tft.drawString("+", 350,  204);
  tft.drawString("7", 25, 234);                     tft.drawString("Rampa",     60,  234);              tft.drawString("-", 300,  234); tft.drawString("+", 350,  234);
    

  tft.setTextDatum(MC_DATUM);
  tft.drawString(String(Minutos_Zona_1), 230,  54);  
  tft.drawString(String(Minutos_Zona_2), 230,  85); 
  tft.drawString(String(Minutos_Zona_3), 230, 114); 
  tft.drawString(String(Minutos_Zona_4), 230, 144);
  tft.drawString(String(Minutos_Zona_5), 230, 174); 
  tft.drawString(String(Minutos_Zona_6), 230, 204); 
  tft.drawString(String(Minutos_Zona_7), 230, 234); 

}





//###############################################     Menu:  DIGITAR SENHA     ########################################################
void Mostrar_Tela_Digitar_Senha() {
  Serial.println("Executando Mostrar_Tela_Digitar_Senha");
  tft.fillScreen(TFT_BLACK);

  // Cabeçalho
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.fillRect(0, 0, 480, 30, TFT_BLUE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Digite a Senha", 239, 2);

  // SSID Selecionado
  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 50);
  tft.print("SSID: ");
  tft.println(ssidSelecionado);

  // Campo de senha
  tft.drawRect(10, 60, 450, 30, TFT_GREEN);
  tft.setCursor(20, 80);
  tft.print("Senha: ");
  tft.println(wifiSenha);

  // Chama o teclado
  desenharTeclado();
}
//###############################################     Menu:  FUNCAO RESET     #########################################################
void ResetarESP() {
  Serial.println("Executando ResetarESP");
  ESP.restart();
}
//###############################################     Menu:  BOMBEAR     ##############################################################
void Mostrar_Menu_Bombear() {
  Serial.println("Executando Mostrar_Menu_Bombear");

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_CYAN);  //Botao Ajustes
  tft.drawBitmap(425, 5, Bomba45x45, 45, 45, TFT_BLACK);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Bombear / Sugar", 210, 12);  // Título


  tft.setFreeFont(FF2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRoundRect(292, 90, 115, 55, 7, TFT_GREEN);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Bombear", 349, 115);  // Título

  tft.drawRoundRect(292, 165, 115, 55, 7, TFT_GREEN);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Sugar", 349, 190);  // Título

  tft.drawBitmap(48, 60, Turbina3_80x80, 80, 80, TFT_WHITE);
  tft.setFreeFont(FF2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MR_DATUM);
  tft.drawString("mL", 195, 265);
  tft.drawRoundRect(40, 240, 120, 50, 10, TFT_BLUE);


  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Home
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);
}
//###############################################     Menu:  INFORMACOES       ########################################################
void Mostrar_Menu_Informacoes() {
  Serial.println("Executando Mostrar_Menu_Informacoes");

  cidade =                  EEPROM.readString(POSICAO_NOME_CIDADE);
  Serial.print("MENU INFORMACOES: cidade da EEPROM = [");
  Serial.print(EEPROM.readString(POSICAO_NOME_CIDADE));
  Serial.println("]");
  
  pais_da_cidade =     EEPROM.readString(POSICAO_PAIS_DA_CIDADE);  //  CORRIGIR%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
  Tamanho_Piscina =    EEPROM.readFloat(POSICAO_TAMANHO_PISCINA);
  Concentracao_Cloro = EEPROM.readFloat(POSICAO_CONCENTRACAO_CLORO);
  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_ORANGE);
  tft.drawBitmap(425, 5, Info45x45, 45, 45, TFT_BLACK);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Informacoes", 210, 12);

  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Salvar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  // Painel
  tft.setFreeFont(FF1);
  tft.setTextDatum(TR_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRoundRect(5, 50, 250, 40, 7, TFT_GREEN);
  tft.drawRoundRect(255, 50, 50, 40, 7, TFT_GREEN);
  tft.drawRoundRect(325, 52, 35, 35, 7, TFT_YELLOW);
  tft.drawRoundRect(380, 52, 35, 35, 7, TFT_YELLOW);
  
  tft.drawRoundRect(5, 100, 250, 40, 7, TFT_GREEN);
  tft.drawRoundRect(255, 100, 50, 40, 7, TFT_GREEN);
  tft.drawRoundRect(325, 102, 35, 35, 7, TFT_YELLOW);
  tft.drawRoundRect(380, 102, 35, 35, 7, TFT_YELLOW);
  
  tft.drawRoundRect(5, 150, 120, 40, 7, TFT_GREEN);
  tft.drawRoundRect(125, 150, 350, 40, 7, TFT_GREEN);

  tft.drawString("Tam. Piscina  (M3):", 245, 65);
  tft.drawString(String(Tamanho_Piscina,0), 295, 65);
  tft.drawString("-", 348, 63);
  tft.drawString("+", 402, 63);
  tft.drawString("Conc. Sol. Cloro %:", 245, 115);
  tft.drawString(String(Concentracao_Cloro,0), 295, 115);
  tft.drawString("-", 348, 113);
  tft.drawString("+", 402, 113);
  tft.drawString("Cidade:", 115, 165);
  tft.drawString(removerAcentos(cidade) + ", " + pais_da_cidade, 445, 165);

  
  tft.fillRoundRect(45, 220, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(50, 225, Weather45x45, 45, 45, TFT_WHITE);
  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Previsao", 70, 281);  // Título

  Serial.print("Cidade bruta: [");
  Serial.print(cidade);
  Serial.println("]");
  Serial.print("Cidade sem acento: [");
  Serial.print(removerAcentos(cidade));
  Serial.println("]");

 

}
//###############################################     Menu:  FUNCAO MASTER RESET     ##################################################
void Mostrar_Menu_Master_Reset() {
  Serial.println("Executando Mostrar_Menu_Master_reset");

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_RED);  //Botao Ajustes
  tft.drawBitmap(425, 5, Reset45x45, 45, 45, TFT_YELLOW);
  tft.fillRect(5, 7, 405, 30, TFT_RED);
  tft.setTextColor(TFT_YELLOW, TFT_RED);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("RESET de FABRICA", 210, 12);  // Título

  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Ao CONFIRMAR esta selecao,", 230, 70);
  tft.drawString("todos os valores serao ajustados ao", 230, 95);
  tft.drawString("PADRAO de FABRICA.", 230, 120);
  tft.drawString("CUIDADO!", 230, 170);

  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);
}
//###############################################     Menu:  AJUSTE LOCAL     #########################################################
void Mostrar_Menu_Definir_Local() {
  Serial.println("Executando Mostrar_Menu_Definir_Local");
  cidade = EEPROM.readString(POSICAO_NOME_CIDADE);
  pais_da_cidade = EEPROM.readString(POSICAO_PAIS_DA_CIDADE);

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes Calirar Bomba
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Mapa45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - LOCAL", 210, 12);

  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setFreeFont(FF1);

  horaAtualTela = imprimirHoraAtual();
  ultimoUpdateHora = millis();

  tft.fillRoundRect(300, 55, 100, 30, 10, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString(horaAtualTela, 230, 60);
  tft.drawString(removerAcentos(cidade) + ", " + pais_da_cidade, 230, 90);
  delay(5);

  //Botao Buscar Cidade
  tft.fillRoundRect(55, 180, 55, 55, 10, TFT_CYAN);
  tft.drawBitmap(60, 185, Lupa45x45, 45, 45, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("BUSCAR CIDADE", 83, 241);  // Título


}

//###############################################     Menu:  OUTROS     ###############################################################
void Mostrar_Menu_Outros() {
  Serial.println("Executando Mostrar_Menu_Outros");

  // Cabeçalho
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.fillRect(0, 0, 480, 30, TFT_BLUE);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - Outros", 239, 2);  // Título

  //Botao Voltar
  tft.fillRoundRect(330, 245, 125, 55, 10, TFT_GREEN);
  tft.setCursor(355, 280);
  tft.setFreeFont(FSB12);
  tft.setTextColor(TFT_BLACK, TFT_BLACK);
  tft.print("Voltar");



  //Caixa - Ajustes LIVRE1
  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  tft.drawRect(10, 70, 100, 150, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.drawBitmap(20, 80, Construcao80x80, 80, 80, TFT_WHITE);
  tft.setCursor(40, 200);
  tft.print("LIVRE1");

  //Caixa - Ajustes LIVRE2
  tft.drawRect(130, 70, 100, 150, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.drawBitmap(140, 80, Construcao80x80, 80, 80, TFT_WHITE);
  tft.setCursor(141, 190);
  tft.print("Botao2");
  tft.setCursor(148, 210);
  tft.print("LIVRE2");

  //Caixa - Ajustes LIVRE3
  tft.drawRect(250, 70, 100, 150, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.drawBitmap(260, 80, Construcao80x80, 80, 80, TFT_WHITE);
  tft.setCursor(256, 190);
  tft.print("Botao3");
  tft.setCursor(272, 210);
  tft.print("LIVRE3");

  //Caixa - Ajustes LIVRE4
  tft.drawRect(370, 70, 100, 150, TFT_GREEN);  //(x, y, Largura, Altura, Angulo arredondamento, Cor)
  tft.drawBitmap(380, 80, MasterReset80x80, 80, 80, TFT_RED);
  tft.setCursor(390, 190);
  tft.print("RESET");
  tft.setCursor(380, 210);
  tft.print("FABRICA");
}
//###############################################     Menu:  CALIBRAR TOUCH     #######################################################
void Mostrar_Menu_Calibrar_Touch() {
  Serial.println("Executando Mostrar_Menu_Calibrar_Touch");
  tft.fillScreen(TFT_BLACK);

  tft.setFreeFont(FSB12);
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Calibrar Touch", 240, 30);

  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Toque no circulo Vermelho", 240, 65);

  // Desenha todos os círculos, destaca o da vez
  for (int i = 0; i < 5; i++) {
    uint16_t cor = (i == etapaCalibracao) ? TFT_RED : TFT_LIGHTGREY;
    tft.fillCircle(coordsCalibracao[i][0], coordsCalibracao[i][1], 6, cor);
    tft.drawCircle(coordsCalibracao[i][0], coordsCalibracao[i][1], 6, TFT_WHITE);
  }

   //Botao D2 - Voltar
  tft.fillRoundRect(210, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(215, 265, Voltar45x45, 45, 45, TFT_BLACK);

  calibrandoTouch = true;
}
//###############################################     Menu:  PREVISAO DO TEMPO     ####################################################
void Mostrar_Menu_Previsao_Tempo() {
  tft.fillScreen(TFT_BLACK);

  // Cabeçalho e botões
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);
  tft.drawBitmap(425, 5, Weather45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Previsao do Tempo", 240, 12);
  tft.setFreeFont(FF1);
  tft.setTextDatum(TC_DATUM);
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  const int numDias = 4; // Hoje + 3 dias
  const int x0 = 80;
  const int y0 = 60;
  const int w_col = 95;
  const int h_titulo = 20;
  const int h_linha = 22;
  const int espacamento = 4;

  // Nomes das linhas
  const char* nomes[] = { "Min C", "Max C", "Umid", "Chuva", "Prob", "UV" };
  const int numCampos = sizeof(nomes) / sizeof(nomes[0]); // Deve ser 6

  // Ler previsões da EEPROM
  PrevisaoClimaDia previsoes[4];
  LerPrevisoesDaEEPROM(previsoes, numDias); // Garante que lê os dados gravados

  // --- Linha com datas ---
  for (int d = 0; d < numDias; d++) {
    time_t ts = previsoes[d].dataTimestamp;
    struct tm *tm_dia = gmtime(&ts);
    char dataStr[12];
    snprintf(dataStr, sizeof(dataStr), "%02d/%02d", tm_dia->tm_mday, tm_dia->tm_mon + 1);

    const char* nomesSemana[] = {"DOM", "SEG", "TER", "QUA", "QUI", "SEX", "SAB"};
    int xCentro = x0 + d * w_col + w_col / 2;

    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setFreeFont(FF1);
    tft.setTextDatum(TC_DATUM); // Centraliza horizontal e verticalmente
    tft.drawString(dataStr, xCentro, y0);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(nomesSemana[tm_dia->tm_wday], xCentro, y0 + h_titulo - 6);
  }

  // --- Conteúdo das linhas (dados das previsões) ---
  for (int l = 0; l < numCampos; l++) {
    int yLinha = y0 + h_titulo + 16 + l * (h_linha + espacamento);

    // Nome da linha (à esquerda)
    tft.setTextDatum(TL_DATUM); // Alinhado à esquerda
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setFreeFont(FF1);
    tft.drawString(nomes[l], x0 - 75, yLinha+7); // Ajuste a margem lateral para evitar corte

    // Fundo claro como "caixa"
    tft.fillRect(x0 - 5, yLinha - 2, numDias * w_col + 5, h_linha + 4, TFT_BLACK);

    // Dados por dia
    for (int d = 0; d < numDias; d++) {
      String valor = "-";
      switch(l) {
        case 0: valor = String(previsoes[d].tempMin - 273.15, 1); break; // Celsius
        case 1: valor = String(previsoes[d].tempMax - 273.15, 1); break;
        case 2: valor = String(previsoes[d].umidade) + "%"; break;
        case 3: valor = String(previsoes[d].chuvaMM, 1) + "mm"; break;
        case 4: valor = String(previsoes[d].probChuva) + "%"; break;
        case 5: 
          if (isnan(previsoes[d].uvi) || previsoes[d].uvi < 0 || previsoes[d].uvi > 12) {
            valor = "-";
          } else {
            valor = String(previsoes[d].uvi, 1); // Mostra com 1 casa decimal
          }
          break;
      }

      tft.setTextDatum(TC_DATUM); // Centraliza horizontal e verticalmente
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawString(valor, x0 + d * w_col + w_col / 2, yLinha + 7);
    }
  }

  // --- Linhas verticais (divisórias entre dias)
  for (int d = 1; d < numDias; d++) {
    int x = x0 + d * w_col;
    tft.drawLine(x, y0 + h_titulo + 10, x, y0 + h_titulo + 16 + numCampos * (h_linha + espacamento), TFT_GREEN);
  }

  // --- Linhas horizontais (para separar os campos)
  for (int l = 0; l <= numCampos; l++) {
    int y = y0 + h_titulo + 16 + l * (h_linha + espacamento);
    tft.drawLine(x0 - 5, y, x0 + numDias * w_col, y, TFT_GREEN);
  }

  tft.setFreeFont(FF1);
  tft.setTextDatum(TL_DATUM); // Centraliza horizontal e verticalmente
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(removerAcentos(cidade) + ", " + pais_da_cidade, 10, 280);

}
//###############################################     Menu:  AGENDAR dosagem     ######################################################
void Mostrar_Menu_Agendar_Hora_Dosagem(){
  Serial.println("Executando Mostrar_Menu_Agendar_dosagem");

  Domingo_Programado = (bool)EEPROM.read(POSICAO_DOMINGO_PROGRAMADO);
  Segunda_Programado = (bool)EEPROM.read(POSICAO_SEGUNDA_PROGRAMADO);
  Terca_Programado = (bool)EEPROM.read(POSICAO_TERCA_PROGRAMADO);
  Quarta_Programado = (bool)EEPROM.read(POSICAO_QUARTA_PROGRAMADO);
  Quinta_Programado = (bool)EEPROM.read(POSICAO_QUINTA_PROGRAMADO);
  Sexta_Programado = (bool)EEPROM.read(POSICAO_SEXTA_PROGRAMADO);
  Sabado_Programado = (bool)EEPROM.read(POSICAO_SABADO_PROGRAMADO);

  Dosagem_hora_programada = EEPROM.readInt(POSICAO_HORA_PROGRAMADA);
  Dosagem_minuto_programado = EEPROM.readInt(POSICAO_MINUTO_PROGRAMADO);
  
  
  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Agenda45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("AGENDAR - Rega", 210, 12);  // Título

  tft.setFreeFont(FF0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  //Botao SEGUNDA
  tft.drawRoundRect(15, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Segunda_Programado) {
    tft.drawBitmap(21, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(21, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SEG", 42, 75);

  //Botao TERCA
  tft.drawRoundRect(80, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Terca_Programado) {
    tft.drawBitmap(86, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(86, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("TER", 107, 75);

  //Botao QUARTA
  tft.drawRoundRect(145, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Quarta_Programado) {
    tft.drawBitmap(151, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(151, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("QUA", 172, 75);

  //Botao QUINTA
  tft.drawRoundRect(210, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Quinta_Programado) {
    tft.drawBitmap(216, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(216, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("QUI", 237, 75);

  //Botao SEXTA
  tft.drawRoundRect(275, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Sexta_Programado) {
    tft.drawBitmap(281, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(281, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SEX", 302, 75);

  //Botao SABADO
  tft.drawRoundRect(340, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Sabado_Programado) {
    tft.drawBitmap(346, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(346, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("SAB", 367, 75);

  //Botao DOMINGO
  tft.drawRoundRect(405, 70, 55, 55, 7, TFT_GREENYELLOW);
  if (Domingo_Programado) {
    tft.drawBitmap(411, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
  } else {
    tft.drawBitmap(411, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
  }
  tft.setTextDatum(TC_DATUM);
  tft.drawString("DOM", 432, 75);


  tft.drawRoundRect(5, 160, 100, 50, 7, TFT_ORANGE);   tft.drawRoundRect(105, 160, 70, 50, 7, TFT_ORANGE);     tft.drawRoundRect(190, 165, 40, 40, 7, TFT_GREEN);    tft.drawRoundRect(240, 165, 40, 40, 7, TFT_GREEN);
  tft.drawRoundRect(5, 210, 100, 50, 7, TFT_ORANGE);   tft.drawRoundRect(105, 210, 70, 50, 7, TFT_ORANGE);     tft.drawRoundRect(190, 215, 40, 40, 7, TFT_GREEN);    tft.drawRoundRect(240, 215, 40, 40, 7, TFT_GREEN);


  tft.setTextDatum(MR_DATUM);
  tft.drawString("HORA:",    95, 185);                                                                         tft.drawString("-",    214, 184);                 tft.drawString("+",    265, 184);
  tft.drawString("MINUTO:",  95, 235);                                                                         tft.drawString("-",    214, 234);                 tft.drawString("+",    265, 234);

  tft.setFreeFont(FF2);
  
  char horaFormatada[3]; // 2 dígitos + terminador '\0'
  sprintf(horaFormatada, "%02d", Dosagem_hora_programada);
  tft.drawString(horaFormatada, 155, 185);
  char minutoFormatado[3]; // 2 dígitos + terminador '\0'
  sprintf(minutoFormatado, "%02d", Dosagem_minuto_programado);
  tft.drawString(minutoFormatado, 155, 235);
  
}
//###############################################     Menu:  Atualizar FIRMWARE     ###################################################
void Mostrar_Menu_Atualizar_Firmaware(){
  Serial.println("Executando Mostrar_Menu_Atualizar_Firmware");

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho 
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);
  tft.drawBitmap(425, 5, Update45x45, 45, 45, TFT_BLACK);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Sobre", 210, 12);

  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);


  tft.setFreeFont(FF0);
  tft.setTextDatum(TL_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawRoundRect(5, 50, 400, 150, 5, TFT_GREEN);
  tft.setFreeFont(FF1);
  tft.drawString("Regador Inteligente - MOD: RE32ST7796-IA", 10, 60);
  tft.setFreeFont(FF0);

  tft.drawString("Referencias:", 10, 85);
  tft.drawString("Weather Forecast: from: www.openweather.com                  ", 10, 110);
  tft.drawString("                                                             ", 10, 135);
  tft.drawString("                                                             ", 10, 150);
  tft.drawString("Design by FVital & Filhos - July/2025", 10, 175);
  

  
  tft.fillRoundRect(145, 220, 55, 55, 7, TFT_DARKGREY);  //Botao Ajustes
  tft.drawBitmap(150, 225, Update45x45, 45, 45, TFT_WHITE);
  tft.setFreeFont(FF1);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("ATUALIZAR FIRMWARE", 170, 281);  // Título

  

}
//###############################################     Menu:  PROGRESSO Bombeamento     ################################################
void Mostrar_Tela_Progresso_Bombeamento() {
    int x = 40, y = 120, largura = 400, altura = 40;
    static int ultimoPercentual = -1;

    // Garante % dentro de 0~100
    int percentual = progressoBombeamento;
    if (percentual < 0) percentual = 0;
    if (percentual > 100) percentual = 100;

    // Calcula pixel final
    int totalPixels = (percentual * largura) / 100;

    // Só redesenha se o percentual mudou
    if (percentual != ultimoPercentual) {
        // Preenche toda a barra já atingida (sem confiar no "últimoPixel")
        for (int i = 0; i < totalPixels; i++) {
            tft.drawFastVLine(x + i, y + 2, altura - 4, TFT_CYAN);
        }
        ultimoPercentual = percentual;
    }

    // Atualiza o texto do percentual sempre
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FF1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(String(percentual) + "%", x + largura/2, y - altura/2);

    // Reset se sair da tela
    if (tela_atual != TELA_PROGRESSO_BOMBEAMENTO) {
        ultimoPercentual = -1;
    }
}
//###############################################     Menu:  DADOS Histõricos     ###########³³³³³#####################################
void Mostrar_Menu_Dados_Historicos() {
  Serial.println("Executando Mostrar_Menu_Dados_Historicos");

  tft.fillScreen(TFT_BLACK);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("HISTORICO DE DOSAGEM", 210, 12);
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Graficos45x45, 45, 45, TFT_WHITE);
  tft.setFreeFont(FF0);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  // Ajuste para 5 colunas, centralizadas e mais largas
  const int num_colunas = 5;
  const int largura_col = 76;
  const int altura_dia = 35;
  const int altura_dose = 35;
  const int espaco_col = 0;
  const int base_x = 89; // Ajuste para centralizar (calcule conforme sua tela)
  const int y_dia = 80;
  const int y_dose = 115;

  int xColuna[num_colunas];
  for (int i = 0; i < num_colunas; i++) {
    xColuna[i] = base_x + i * (largura_col + espaco_col);
    tft.drawRoundRect(xColuna[i], y_dia, largura_col, altura_dia, 7, TFT_GREEN);
    tft.drawRoundRect(xColuna[i], y_dose, largura_col, altura_dose, 7, TFT_GREEN);
  }

  tft.setTextDatum(MR_DATUM);
  tft.setFreeFont(FF1);
  tft.drawString("Dose(L)", base_x - 8, 130); // Ajuste aqui para alinhar texto "Dose-L"
  tft.setFreeFont(FF1);

  DosagemAcumulada = EEPROM.readDouble(POSICAO_DOSAGEM_ACUMULADA);
  tft.drawRoundRect(5,   180, 160, 40, 7, TFT_GREEN);
  tft.drawString("Dosagem ACUM: ", 160, 200);
  tft.drawRoundRect(165, 180, 120, 40, 7, TFT_GREEN);
  tft.drawString(String((float)DosagemAcumulada, 0), 245, 200);
  tft.drawString("lt", 280, 200);
  tft.fillRoundRect(305, 180,  40,  40, 7, TFT_RED);
  tft.drawBitmap(310, 185, Reset30x30, 30, 30, TFT_YELLOW);

 // tft.drawString("Desde de:", 110, 240);

  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  // === MOSTRAR DATAS E VALORES HISTÓRICOS NOS RETÂNGULOS ===

  time_t agora = time(nullptr);
  struct tm *hoje = localtime(&agora);
  int diaAtual = hoje->tm_yday;

  for (int i = 0; i < num_colunas; i++) {
    // time_t para cada dia retroativo (mais recente à esquerda)
    time_t tDia = agora - (i * 86400);
    struct tm *dataHistorica = localtime(&tDia);

    char strData[6];
    sprintf(strData, "%02d/%02d", dataHistorica->tm_mday, dataHistorica->tm_mon + 1);

    int diaHistorico = diaAtual - i;
    if (diaHistorico < 0) diaHistorico += NUM_DIAS_HISTORICO;

    int posicao = POSICAO_HISTORICO_DOSAGEM + (diaHistorico * TAMANHO_REGISTRO_HIST);
    int dose_ml = EEPROM.readInt(posicao);
    float dose_litros = dose_ml;
   
    // Linha de cima: data
    tft.setFreeFont(FF1);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString(strData, xColuna[i] + largura_col/2, y_dia + 17);

    // Linha de baixo: dose em litros (só mostra se realmente tem valor!)
    tft.setFreeFont(FF1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    if (fabs(dose_litros) < 0.05f || isnan(dose_litros)) {
      tft.drawString("", xColuna[i] + largura_col/2, y_dose + 18);
    } else {
      
     tft.drawString(String(dose_litros, 0), xColuna[i] + largura_col/2, y_dose + 18);
      
      
    }
  }
}




//===============================================     FUNCOES AUXILIARES     ========================================================
void desenharComRaios() {
  for (int y = 0; y < BMP_HEIGHT; y++) {
    for (int x = 0; x < BMP_WIDTH; x++) {
      int byteIndex = (y * ((BMP_WIDTH + 7) / 8)) + (x / 8);
      uint8_t byte = pgm_read_byte(&VE_60x46[byteIndex]);

      if (byte & (0x80 >> (x % 8))) {
        int telaX = POS_X + x;
        int telaY = POS_Y + y;

        // Desenha o raio
        tft.drawLine(ORIGEM_X, ORIGEM_Y, telaX, telaY, TFT_YELLOW);
        delay(1);  // Diminua se quiser mais rápido

        // Apaga o raio (opcional)
        tft.drawLine(ORIGEM_X, ORIGEM_Y, telaX, telaY, TFT_BLACK);

        // Acende o pixel
        tft.drawPixel(telaX, telaY, TFT_YELLOW);
      }
    }
  }
}
bool isInside(int px, int py, int bx, int by, int bw, int bh) {  // Função para verificar se o toque está dentro de uma área
  return (px >= bx && px <= bx + bw && py >= by && py <= by + bh);
}
void connectWiFi() {
  Serial.println("Executando connectWiFi");
  tft.fillRect(0, 30, 480, 230, TFT_BLACK);

  String ssid = EEPROM.readString(POSICAO_SSID_WIFI);
  String pass = EEPROM.readString(POSICAO_SENHA_WIFI);

  if (ssid.length() == 0) {
    Serial.println("SSID vazio!");
    return;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());

  int timeout = 10000;  // 10 segundos
  unsigned long start = millis();

  tft.fillRect(0, 100, 480, 220, TFT_BLACK);
  tft.setCursor(10, 100);
  tft.setTextColor(TFT_YELLOW);
  tft.print("   Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
    delay(500);
    tft.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    tft.println("");
    tft.println("");
    Serial.println("\nConectado!");
    tft.println("");
    tft.println("        Conectado com sucesso!");
    //tft.println(WiFi.localIP());
    delay(2000);
    Mostrar_Menu_Principal();
    tela_atual = TELA_MENU_PRINCIPAL;
  } else {
    Serial.println("\nFalha na conexao.");
    tft.println("");
    tft.println("");
    tft.println("");
    tft.println("  Falha na conexao.");
    tft.println("");
    tft.println("  Tente novamente.......");
    EEPROM.write(POSICAO_SSID_WIFI, 0);
    EEPROM.write(POSICAO_SENHA_WIFI, 0);
    EEPROM.commit();
    delay(2000);
    Mostrar_Menu_Ajustes();
    tela_atual = TELA_MENU_AJUSTES;
  }
}
void desenharTeclado() {
  Serial.println("Executando desenharTeclado");
  const int btnW = 45;
  const int btnH = 35;
  const int startX = 10;
  const int startY = 105;
  const int gap = 2;

  tft.setFreeFont(FF1);

  const char*(*keys)[10];  // Ponteiro para linha de teclas

  if (modoTeclado == 0) {
    keys = keysMaiusculas;
  } else if (modoTeclado == 1) {
    keys = keysMinusculas;
  } else {
    keys = keysSimbolos;
  }

  for (int row = 0; row < 4; row++) {
    for (int col = 0; col < 10; col++) {
      const char* key = keys[row][col];
      if (key != nullptr && strlen(key) > 0) {
        int x = startX + col * (btnW + gap);
        int y = startY + row * (btnH + gap);

        if (y + btnH > tft.height()) continue;

        // Define a cor baseada na tecla
        uint16_t corFundo = TFT_LIGHTGREY;

        if (strcmp(key, "SHIF") == 0) {
          corFundo = TFT_NAVY;  // Destaque para Shift
        } else if (strcmp(key, "<=") == 0) {
          corFundo = TFT_RED;  // Destaque para Backspace
        } else if (strcmp(key, "#+=") == 0) {
          corFundo = TFT_ORANGE;  // Destaque para Símbolos
        }

        tft.fillRoundRect(x, y, btnW, btnH, 5, corFundo);
        tft.drawRoundRect(x, y, btnW, btnH, 5, TFT_DARKGREY);  // Borda

        int16_t txtWidth = tft.textWidth(key);
        int16_t txtHeight = tft.fontHeight();
        tft.setCursor(x + (btnW - txtWidth) / 2, y + (btnH - txtHeight) / 2 + 10);

        // Texto em branco nas teclas especiais
        if (corFundo == TFT_NAVY || corFundo == TFT_RED || corFundo == TFT_ORANGE) {
          tft.setTextColor(TFT_WHITE);
        } else {
          tft.setTextColor(TFT_BLACK);
        }

        tft.print(key);
      }
    }
  }

  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);
}
void Conexao_WiFi_Setup() {
  Serial.println("Executando Conexao_WiFi_Setup");
  tft.fillRect(0, 0, 480, 320, TFT_BLACK);

  String ssid = EEPROM.readString(POSICAO_SSID_WIFI);
  String pass = EEPROM.readString(POSICAO_SENHA_WIFI);

  if (ssid.length() == 0) {
    Serial.println("SSID vazio!");
    return;
  }

  WiFi.begin(ssid.c_str(), pass.c_str());

  int timeout = 10000;  // 10 segundos
  unsigned long start = millis();

  tft.drawBitmap(380, 10, TryingWIFI_80x80, 80, 80, TFT_WHITE);
  tft.setCursor(10, 150);
  tft.setTextColor(TFT_YELLOW);

  tft.setTextDatum(TC_DATUM);
  tft.drawString("Tentando Conexao com:", 240, 140);
  tft.drawString(ssid, 240, 160);
  tft.setCursor(10, 190);


  while (WiFi.status() != WL_CONNECTED && millis() - start < timeout) {
    delay(500);
    tft.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nConectado!");
    tft.drawBitmap(380, 10, TryingWIFI_80x80, 80, 80, TFT_BLACK);  //Apago o loto anterior
    tft.drawBitmap(380, 10, WiFi_OK_80x80, 80, 80, TFT_GREEN);
    tft.println("");
    tft.println("");
    tft.println("");
    tft.println("");
    tft.println("           Conectado com sucesso!");
    //tft.println(WiFi.localIP());
    delay(2000);
    Mostrar_Menu_Principal();
    tela_atual = TELA_MENU_PRINCIPAL;
  } else {
    Serial.println("\nFalha na conexao.");
    tft.println("");
    tft.println("Falha na conexao.");
    tft.drawBitmap(380, 10, TryingWIFI_80x80, 80, 80, TFT_BLACK);  //Apago o loto anterior
    tft.drawBitmap(380, 10, WiFi_NAO_OK_80x80, 80, 80, TFT_RED);
    delay(2500);
    Mostrar_Menu_Principal();
    tela_atual = TELA_MENU_PRINCIPAL;
  }
}
bool DesejaContinuar() {

  // Tela de confirmação
  tft.fillScreen(TFT_BLACK);
  tft.setFreeFont(FF2);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Deseja Continuar?", 225, 120);

  // Botões
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.fillRoundRect(110, 180, 80, 40, 10, TFT_GREEN);  // Botão SIM
  tft.drawString("SIM", 149, 198);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.fillRoundRect(260, 180, 80, 40, 10, TFT_GREEN);  // Botão NÃO
  tft.drawString("NAO", 299, 198);

  // Espera por toque
  while (true) {
    uint16_t x, y;
    if (tft.getTouch(&x, &y)) {
      ultimoToque = millis();  // Atualiza temporizador de inatividade
      if (isInside(x, y, 110, 180, 80, 40)) {
        return true;
      } else if (isInside(x, y, 260, 180, 80, 40)) {
        return false;
      }
    }
  }
}
bool obterTimezoneDaCidade(String cidade) {
  HTTPClient http;
  // Geo API: busca cidade por nome, retorna array com resultados
  String url = "http://api.openweathermap.org/geo/1.0/direct?q=" + urlencode(cidade) + "&limit=1&appid=" + apiKey;

  http.begin(url);
  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println(">>> Payload recebido:");
    Serial.println(payload);
    DynamicJsonDocument doc(2048);
    DeserializationError error = deserializeJson(doc, payload);

    if (error) {
      Serial.println("Erro ao parsear JSON.");
      return false;
    }

    if (doc.size() == 0) {
      Serial.println("Cidade não encontrada.");
      return false;
    }



    // Pega o primeiro resultado do array (melhor match)
    cidade_latitude = doc[0]["lat"];
    cidade_longitude = doc[0]["lon"];
    pais_da_cidade = doc[0]["country"].as<String>();
    String nomeCidadeOficial = doc[0]["name"].as<String>();

    Serial.printf("País: %s\n", pais_da_cidade.c_str());
    Serial.printf("Lat: %.4f, Lon: %.4f\n", cidade_latitude, cidade_longitude);
    Serial.printf("Nome oficial salvo: %s\n", nomeCidadeOficial.c_str());

    // Grava na EEPROM o nome oficial (com acento)
    for (int i = 0; i < 64; i++) EEPROM.write(POSICAO_NOME_CIDADE + i, 0);
    EEPROM.writeString(POSICAO_NOME_CIDADE, nomeCidadeOficial);

    EEPROM.writeFloat(POSICAO_LATITUDE_CIDADE, cidade_latitude);
    EEPROM.writeFloat(POSICAO_LONGITUDE_CIDADE, cidade_longitude);
    EEPROM.commit();

    // Remover acentos do nome oficial e salvar no array cidade_digitada
    String nomeSemAcentos = removerAcentos(nomeCidadeOficial);
    nomeSemAcentos.toCharArray(cidade_digitada, sizeof(cidade_digitada));

    return true;
  } else {
    Serial.printf("Erro HTTP: %d\n", httpCode);
    return false;
  }
}
void configurarRelogioNTP(long offsetSegundos) {
  long gmtOffset_sec = offsetSegundos;
  int daylightOffset_sec = 0;  // Ajuste se sua cidade usar horário de verão

  Serial.printf("Configurando NTP com offset %ld segundos\n", gmtOffset_sec);
  configTime(gmtOffset_sec, daylightOffset_sec, "pool.ntp.org", "time.nist.gov");

  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    Serial.println("Hora sincronizada com sucesso.");
  } else {
    Serial.println("Erro obter HORA");
  }
}
String imprimirHoraAtual() {
  
  EEPROM.get(POSICAO_TIMEZONE_OFFSET, timezoneOffset);
  //configTime(timezoneOffset, 0, "pool.ntp.org");
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Erro ao obter hora.");
    return "Erro ao obter hora";
  }

  char buffer[64];
  // Monta a string manualmente em português:
  sprintf(buffer, "%s, %02d %s %04d %02d:%02d:%02d",
          dias_semana_pt[timeinfo.tm_wday],
          timeinfo.tm_mday,
          meses_pt[timeinfo.tm_mon],
          timeinfo.tm_year + 1900,
          timeinfo.tm_hour,
          timeinfo.tm_min,
          timeinfo.tm_sec);

  Serial.println(buffer);
  return String(buffer);
}
String urlencode(String str) {
  String encoded = "";
  char c;
  char code0;
  char code1;
  for (int i = 0; i < str.length(); i++) {
    c = str.charAt(i);
    if (isalnum(c)) {
      encoded += c;
    } else {
      code1 = (c & 0xf) + '0';
      if ((c & 0xf) > 9) code1 = (c & 0xf) - 10 + 'A';
      c = (c >> 4) & 0xf;
      code0 = c + '0';
      if (c > 9) code0 = c - 10 + 'A';
      encoded += '%';
      encoded += code0;
      encoded += code1;
    }
  }
  return encoded;
}
float calcularDosagemCloroHoje(const PrevisaoClimaDia previsoes[]) {
    // 1. Lê volume da piscina da EEPROM (em m³)
    float volumePiscina = EEPROM.readFloat(POSICAO_TAMANHO_PISCINA);
    // 2. Lê concentração de cloro ativo da solução da EEPROM (ex: 0.14 para 14%)
    float concentracaoCloro = EEPROM.readFloat(POSICAO_CONCENTRACAO_CLORO); // valor entre 0.12 e 0.15, por exemplo

    // 3. Dados do forecast de hoje (previsoes[0])
    float indiceUV = previsoes[0].uvi;
    float temp = ((previsoes[0].tempMax + previsoes[0].tempMin) / 2.0f) - 273.15f;
    float horasSol = float(previsoes[0].porSol - previsoes[0].nascerSol) / 3600.0f;

    // 4. Calcula perda de cloro (ppm)
    float perdaCloro = 0.18f * (indiceUV / 7.0f) * (temp / 25.0f) * horasSol;

    // 5. Converte para gramas de cloro ativo
    float gramasCloro = perdaCloro * volumePiscina; // 1ppm = 1g/m³

    // 6. Calcula quantidade de solução de cloro a dosar, considerando a concentração lida
    float litrosSolução = gramasCloro / (concentracaoCloro * 10.0f); // Exemplo: 0.14*1000 = 140g/L para 14%
    float mL_Solução = litrosSolução * 1000.0f;

    Serial.print("Vol Piscina: ");
    Serial.println(volumePiscina,0);
    Serial.print("% Cloro: ");
    Serial.println(concentracaoCloro,0);
    Serial.print("UV: ");
    Serial.println(indiceUV,1);
    Serial.print("Temp: ");
    Serial.println(temp,1);
    Serial.print("horasSol: ");
    Serial.println(horasSol,1);
    Serial.print("perdaCloro: ");
    Serial.println(perdaCloro,1);
    Serial.print("gramasCloro: ");
    Serial.println(gramasCloro,1);
    Serial.print("litrosSolução: ");
    Serial.println(litrosSolução,1);
    Serial.print("mL_Solução: ");
    Serial.println(mL_Solução,1);

    
    
    if (Modo_Operacao_Automatico){
        EEPROM.put(POSICAO_DOSAGEM_DIARIA, int(mL_Solução));
      }
    EEPROM.put(POSICAO_DOSAGEM_REFERENCIA, int(mL_Solução));
    EEPROM.commit();

    Serial.print("Dosagem referencia guardada da memoria: ");
    int teste = EEPROM.readInt(POSICAO_DOSAGEM_REFERENCIA);
     
    Serial.println(teste);
    return mL_Solução; // mL de solução a dosar hoje
}
String dataFormatadaPortugues(struct tm* tm_info) {
  char buffer[16];
  // Exemplo: "Seg, 17 Jun 2024 16:45"
  sprintf(buffer, "%02d/%02d/%04d %02d:%02d",
          tm_info->tm_mday, tm_info->tm_mon + 1, tm_info->tm_year + 1900,
          tm_info->tm_hour, tm_info->tm_min);

  String resultado = String(dias_semana_pt[tm_info->tm_wday]) + ", " + String(tm_info->tm_mday) + " " + String(meses_pt[tm_info->tm_mon]) + " " + String(tm_info->tm_year + 1900) + " " + String(tm_info->tm_hour) + ":" + (tm_info->tm_min < 10 ? "0" : "") + String(tm_info->tm_min);
  return resultado;
}
String removerAcentos(String str) {
  String saida = "";
  for (uint16_t i = 0; i < str.length();) {
    uint8_t c = str[i];
    // UTF-8 multibyte para acentuados comuns
    if (c >= 0xC3) {
      uint8_t next = str[i + 1];
      if (c == 0xC3) {
        switch (next) {
          case 0xA1: saida += 'a'; break;  // á
          case 0xA0: saida += 'a'; break;  // à
          case 0xA2: saida += 'a'; break;  // â
          case 0xA3: saida += 'a'; break;  // ã
          case 0xA7: saida += 'c'; break;  // ç
          case 0xA9: saida += 'e'; break;  // é
          case 0xA8: saida += 'e'; break;  // è
          case 0xAA: saida += 'e'; break;  // ê
          case 0xAB: saida += 'e'; break;  // ë
          case 0xAD: saida += 'i'; break;  // í
          case 0xAC: saida += 'i'; break;  // ì
          case 0xAE: saida += 'i'; break;  // î
          case 0xAF: saida += 'i'; break;  // ï
          case 0xB3: saida += 'o'; break;  // ó
          case 0xB2: saida += 'o'; break;  // ò
          case 0xB4: saida += 'o'; break;  // ô
          case 0xB5: saida += 'o'; break;  // õ
          case 0xBA: saida += 'u'; break;  // ú
          case 0xB9: saida += 'u'; break;  // ù
          case 0xBB: saida += 'u'; break;  // û
          case 0xBC: saida += 'u'; break;  // ü
          case 0x81: saida += 'A'; break;  // Á
          case 0x80: saida += 'A'; break;  // À
          case 0x82: saida += 'A'; break;  // Â
          case 0x83: saida += 'A'; break;  // Ã
          case 0x87: saida += 'C'; break;  // Ç
          case 0x89: saida += 'E'; break;  // É
          case 0x88: saida += 'E'; break;  // È
          case 0x8A: saida += 'E'; break;  // Ê
          case 0x8B: saida += 'E'; break;  // Ë
          case 0x8D: saida += 'I'; break;  // Í
          case 0x8C: saida += 'I'; break;  // Ì
          case 0x8E: saida += 'I'; break;  // Î
          case 0x8F: saida += 'I'; break;  // Ï
          case 0x93: saida += 'O'; break;  // Ó
          case 0x92: saida += 'O'; break;  // Ò
          case 0x94: saida += 'O'; break;  // Ô
          case 0x95: saida += 'O'; break;  // Õ
          case 0x9A: saida += 'U'; break;  // Ú
          case 0x99: saida += 'U'; break;  // Ù
          case 0x9B: saida += 'U'; break;  // Û
          case 0x9C: saida += 'U'; break;  // Ü
          default:
            // ignora caracteres desconhecidos
            break;
        }
        i += 2;  // Pula dois bytes
        continue;
      }
    }
    // Se não for UTF-8 especial, adiciona normalmente
    saida += (char)str[i];
    i++;
  }
  return saida;
}
bool obterForecastClimatico(String cidade) {
  // Garante que você já tem cidade_latitude e cidade_longitude válidos
  if (cidade_latitude == 0 || cidade_longitude == 0) {
    Serial.println("Latitude/Longitude inválidas!");
    return false;
  }
  String url = "https://api.openweathermap.org/data/3.0/onecall?lat="
               + String(cidade_latitude, 6) + "&lon=" + String(cidade_longitude, 6) + "&appid=" + String(apiKey);
  Serial.println("URL utilizada para consulta:");
  Serial.println(url);

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  if (httpCode != 200) {
    Serial.printf("Erro HTTP: %d\n", httpCode);
    http.end();
    return false;
  }
  String payload = http.getString();

  Serial.println("=== JSON bruto recebido (OneCall - Obter Forecast) ===");
  Serial.println(payload);

  http.end();

  // Parse do JSON
  StaticJsonDocument<7000> doc;  // Tamanho ajustável
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.println("Erro no parse do JSON");
    return false;
  }

  //Pega o timezone e grava na EEPROM
  if (!doc["timezone_offset"].isNull()) {
    int timezoneOffset = doc["timezone_offset"].as<int>();
    EEPROM.writeLong(POSICAO_TIMEZONE_OFFSET, timezoneOffset);
    EEPROM.commit();
    Serial.print("Timezone offset salvo na EEPROM: ");
    Serial.println(timezoneOffset);
  }

  // Pega o índice UV atual
  if (doc["current"]["uvi"].isNull()) {
    Serial.println("Não veio índice UV no JSON.");
    return false;
  }
  float forecast_uv_index = doc["current"]["uvi"].as<float>();
  Serial.print("Índice UV atual: ");
  Serial.println(forecast_uv_index, 1);

  // Para pegar a previsão diária de UV (opcional, até 7 dias)
  float forecast_uv_index_diario[7];
  for (int i = 0; i < 7; i++) {
    if (!doc["daily"][i].isNull() && !doc["daily"][i]["uvi"].isNull()) {
      forecast_uv_index_diario[i] = doc["daily"][i]["uvi"].as<float>();
    } else {
      forecast_uv_index_diario[i] = -1;  // valor inválido
    }
  }

 
  PrevisaoClimaDia previsoes[NUM_DIAS_FORECAST];
  
    for (int i = 0; i < NUM_DIAS_FORECAST; i++) {
      PrevisaoClimaDia p;
      p.dataTimestamp = doc["daily"][i]["dt"].as<uint32_t>();
      p.tempMin = doc["daily"][i]["temp"]["min"].as<float>();
      p.tempMax = doc["daily"][i]["temp"]["max"].as<float>();
      p.condicaoTempo = doc["daily"][i]["weather"][0]["id"].as<uint8_t>();
      p.probChuva = doc["daily"][i]["pop"].isNull() ? 0 : doc["daily"][i]["pop"].as<float>() * 100;
      p.chuvaMM = doc["daily"][i]["rain"].isNull() ? 0 : doc["daily"][i]["rain"].as<float>();
      p.ventoMS = doc["daily"][i]["wind_speed"].as<float>();
      p.umidade = doc["daily"][i]["humidity"].as<uint8_t>();
      p.uvi = doc["daily"][i]["uvi"].isNull() ? -1 : doc["daily"][i]["uvi"].as<float>();
      p.nascerSol = doc["daily"][i]["sunrise"].as<uint32_t>();
      p.porSol    = doc["daily"][i]["sunset"].as<uint32_t>();

      previsoes[i] = p;
    }
  GravarPrevisoesNaEEPROM(previsoes, NUM_DIAS_FORECAST);

 
  return true;
}
void GravarPrevisoesNaEEPROM(PrevisaoClimaDia previsoes[], int numDias) {
   for (int i = 0; i < numDias; i++) {
    int endereco = POSICAO_PREVISAO_CLIMA_0 + i * TAMANHO_PREVISAO_CLIMA;
    EEPROM.put(endereco, previsoes[i]);
  }
  
  EEPROM.commit();  // importante para ESP32/ESP8266
  
  PrevisaoClimaDia previsoesLocais[NUM_DIAS_FORECAST];
  LerPrevisoesDaEEPROM(previsoes, NUM_DIAS_FORECAST);
  float dosagemHoje = calcularDosagemCloroHoje(previsoes);
    
  Serial.println("OBtido Forecast 3.0 e gravado na EEPROM!!!!!");
  Serial.println("Dosagem referencia Calculada e guardada na memoria!!!!!");

}
void LerPrevisoesDaEEPROM(PrevisaoClimaDia previsoes[], int numDias) {
  for (int i = 0; i < numDias; i++) {
    int endereco = POSICAO_PREVISAO_CLIMA_0 + i * TAMANHO_PREVISAO_CLIMA;
    EEPROM.get(endereco, previsoes[i]);
    Serial.printf("Lido Dia %d | Endereço: %d | Timestamp: %lu | Temp Min: %.2f | Temp Max: %.2f\n",
                  i, endereco, previsoes[i].dataTimestamp, previsoes[i].tempMin, previsoes[i].tempMax);
  }
}
void Verificar_Hora_e_Dosar() {

  if (!horaSincronizada) return;

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int horaAtual = timeinfo.tm_hour;
  int minutoAtual = timeinfo.tm_min;
  int segundoAtual = timeinfo.tm_sec;
  int diaHoje = timeinfo.tm_mday;
  int diaSemana = timeinfo.tm_wday;

  int ultimoDiaExecutado = EEPROM.readInt(POSICAO_ULTIMO_DIA_EXECUTADO);

  // Verifica se o dia está habilitado
  bool diaHabilitado = false;
  switch (diaSemana) {
    case 0: diaHabilitado = EEPROM.readBool(POSICAO_DOMINGO_PROGRAMADO);  break;
    case 1: diaHabilitado = EEPROM.readBool(POSICAO_SEGUNDA_PROGRAMADO);  break;
    case 2: diaHabilitado = EEPROM.readBool(POSICAO_TERCA_PROGRAMADO);    break;
    case 3: diaHabilitado = EEPROM.readBool(POSICAO_QUARTA_PROGRAMADO);   break;
    case 4: diaHabilitado = EEPROM.readBool(POSICAO_QUINTA_PROGRAMADO);   break;
    case 5: diaHabilitado = EEPROM.readBool(POSICAO_SEXTA_PROGRAMADO);    break;
    case 6: diaHabilitado = EEPROM.readBool(POSICAO_SABADO_PROGRAMADO);   break;
  }

  if (diaHabilitado &&
      horaAtual == Dosagem_hora_programada &&
      minutoAtual == Dosagem_minuto_programado &&
      ultimoDiaExecutado != diaHoje) {

    // Verifica condição climática para abortar a rega
    float chuvaHoje = previsoes[0].chuvaMM;
    int   probChuvaHoje = previsoes[0].probChuva;

    if (chuvaHoje >= 5.0 || probChuvaHoje >= 70) {
      Serial.println(">> Rega abortada devido à previsão ou ocorrência de chuva significativa.");
      return;
    }

    if (handleNivelCisterna != NULL) vTaskSuspend(handleNivelCisterna);

    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(TC_DATUM);
    tft.setFreeFont(FF1);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.drawString("Rega em andamento... Aguarde", 240, 75);
    tft.drawRect(39, 119, 402, 42, TFT_CYAN);
    //delay(100);
    Mostrar_Tela_Progresso_Bombeamento();
    tela_atual = TELA_PROGRESSO_BOMBEAMENTO;

    xTaskCreatePinnedToCore(TarefaRegaUnica, "RegaUnica", 4096, NULL, 2, NULL, 1);
    EEPROM.writeInt(POSICAO_ULTIMO_DIA_EXECUTADO, diaHoje);
    EEPROM.commit();
  }
}
void Mostrar_Menu_Nivel_Cisterna() {
  Serial.println("Executando Mostrar_Menu_Nivel_Cisterna");

  altura_cisterna = EEPROM.readFloat(POSICAO_ALTURA_CISTERNA);

  tft.fillScreen(TFT_BLACK);

  // Cabeçalho Ajustes Calirar Bomba
  tft.fillRoundRect(420, 0, 55, 55, 7, TFT_LIGHTGREY);  //Botao Ajustes
  tft.drawBitmap(425, 5, Nivel_Cisterna45x45, 45, 45, TFT_WHITE);
  tft.fillRect(5, 7, 405, 30, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.setFreeFont(FSB12);
  tft.setTextDatum(TC_DATUM);
  tft.drawString("Ajustes - Nivel Cisterna", 210, 12);


  //Botao D3 - Confirmar
  tft.fillRoundRect(290, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(295, 265, Confirmar45x45, 45, 45, TFT_BLACK);
  //Botao D2 - Voltar
  tft.fillRoundRect(355, 260, 55, 55, 7, TFT_GREENYELLOW);
  tft.drawBitmap(360, 265, Voltar45x45, 45, 45, TFT_BLACK);
  //Botao D1 - HOME
  tft.fillRoundRect(420, 260, 55, 55, 7, TFT_GREENYELLOW);  //Botao Reinciar
  tft.drawBitmap(425, 265, Home45x45, 45, 45, TFT_BLACK);

  tft.setFreeFont(FF1);
  tft.setTextDatum(MR_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);



  //Altura Cisterna
  tft.drawRoundRect(5,   70, 200, 40, 5, TFT_GREEN);
  tft.drawRoundRect(205, 70, 75, 40, 5, TFT_GREEN);
  tft.drawString("Volume Atual(m3):", 195, 90);
  float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
  tft.drawString(String(volume_m3, 2), 270, 90);

    //Botao (+)
  tft.drawRoundRect(350, 70, 40, 40, 7, TFT_YELLOW);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("-", 369, 88);
  //Botao (-)
  tft.drawRoundRect(420, 70, 40, 40, 7, TFT_YELLOW);
  tft.drawString("+", 439, 88);


}
void VerificarNivelCisterna() {
  distancia = calcularmedia();
  nivel = altura_cisterna - distancia;
    if (nivel < 0) {
      nivel = 0; // Garante que o nível em cm não seja negativo (cisterna vazia)
    }

  Volume_Atual_cisterna = nivel * LITROS_POR_CM;
  float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
  Serial.print("Volume Cisterna: ");
  Serial.println(Volume_Atual_cisterna, 0);
  Blynk.virtualWrite(V12, volume_m3);
}
float calcularmedia() {
  float somaMedidas = 0;
  float resultado = 0;
  for (int index = 0; index < qtdmedidas; index++) {
    delay(50);
    somaMedidas += hc.dist();
  }
  resultado = (float)somaMedidas / qtdmedidas;
  return resultado;
}




void handleTouch() {
  //Serial.println("Executando handleTouch");
  if (ts.touched()) {
    TS_Point p = ts.getPoint();
    int x = map(p.x, calib_x_min, calib_x_max, 0, tft.width());
    int y = map(p.y, calib_y_min, calib_y_max, 0, tft.height());

    // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    // BLOCO CALIBRAÇÃO TOUCH (adicione aqui)
    if (tela_atual == TELA_CALIBRAR_TOUCH && calibrandoTouch) {
      
      
          if (x > 210 && x < 265 && y > 260 && y < 315) {
            calibrandoTouch = false;
            etapaCalibracao = 0;
            tela_atual = TELA_MENU_AJUSTES;
            Mostrar_Menu_Ajustes();
            return;
          }
      
      
      
      // Armazene o ponto capturado
      pontosCalibracao[etapaCalibracao].x_raw = p.x;
      pontosCalibracao[etapaCalibracao].y_raw = p.y;
      pontosCalibracao[etapaCalibracao].x_ref = coordsCalibracao[etapaCalibracao][0];
      pontosCalibracao[etapaCalibracao].y_ref = coordsCalibracao[etapaCalibracao][1];

      etapaCalibracao++;
      delay(500);  // Anti-bounce

      if (etapaCalibracao >= 5) {

        calib_x_min = min(pontosCalibracao[0].x_raw, pontosCalibracao[3].x_raw);
        calib_x_max = max(pontosCalibracao[1].x_raw, pontosCalibracao[4].x_raw);
        calib_y_min = min(pontosCalibracao[0].y_raw, pontosCalibracao[1].y_raw);
        calib_y_max = max(pontosCalibracao[3].y_raw, pontosCalibracao[4].y_raw);

        tft.fillScreen(TFT_BLACK);
        tft.setFreeFont(FF2);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_YELLOW, TFT_BLACK);
        tft.drawString("Salvando.....", 230, 160);
        delay(1000);
        EEPROM.writeInt(POSICAO_CALIB_TOUCH_X_MIN, calib_x_min);
        EEPROM.writeInt(POSICAO_CALIB_TOUCH_X_MAX, calib_x_max);
        EEPROM.writeInt(POSICAO_CALIB_TOUCH_Y_MIN, calib_y_min);
        EEPROM.writeInt(POSICAO_CALIB_TOUCH_Y_MAX, calib_y_max);
        EEPROM.commit();



        calibrandoTouch = false;
        etapaCalibracao = 0;
        // Aqui você pode salvar os dados, se desejar
        tela_atual = TELA_MENU_AJUSTES;
        Mostrar_Menu_Ajustes();
      } else {
        Mostrar_Menu_Calibrar_Touch();
      }
      return;  // Impede o fluxo normal enquanto calibra
    }


    switch (tela_atual) {
      case TELA_STANDBY:  //=====================================================================MENU STANDBY
        if (isInside(x, y, 0, 0, 480, 320)) {
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        }
        break;


      case TELA_MENU_PRINCIPAL:                //==================================================MENU PRINCIPAL
        if (isInside(x, y, 405, 5, 55, 55)) {  //Botao Ajustes
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 405, 90, 55, 55)) {  //Botao Bombear
          Mostrar_Menu_Bombear();
          tela_atual = TELA_MENU_BOMBEAR;
        } else if (isInside(x, y, 405, 175, 55, 55)) {  //Botao Informacoes
          Mostrar_Menu_Informacoes();
          tela_atual = TELA_MENU_INFORMACOES;
        } else if (isInside(x, y, 405, 260, 55, 55)) {  //Botao Reiniciar
          bool pergunta = DesejaContinuar();
          if (!pergunta) {
            ResetarESP();
          } else {
            Mostrar_Menu_Principal();
            tela_atual = TELA_MENU_PRINCIPAL;
          }
        } else if (isInside(x, y, 100, 60, 265, 40)) {  //botao oculto Agua Acumulada
          Mostrar_Menu_Dados_Historicos();
          tela_atual = TELA_DADOS_HISTORICOS;
        } else if (isInside(x, y, 100, 100, 265, 40)) { //Boao oculto Agendar Hora
          Mostrar_Menu_Agendar_Hora_Dosagem();
          tela_atual = TELA_AGENDAR_DOSAGEM; 
        } else if (isInside(x, y, 85, 182, 280, 40)){
          Mostrar_Menu_Ajuste_Dosagem_diaria();
          tela_atual = TELA_MENU_AJUSTE_DOSAGEM_DIARIA;       
        } else if (isInside(x, y, 85, 250, 280, 40)){
          Mostrar_Menu_Agendar_Hora_Dosagem();
          tela_atual = TELA_AGENDAR_DOSAGEM;
        } else if (isInside(x, y, 0, 0, 0, 0)){
          //Colocar funcao para ajuste cisterna
        } else if (isInside(x, y, 5, 70, 220, 50)){
          Mostrar_Menu_Nivel_Cisterna();
          tela_atual = TELA_NIVEL_CISTERNA;
        }
        break;



      case TELA_MENU_AJUSTES:                    //==================================================MENU AJUSTES
        if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 420, 260, 55, 55)) {  //Botao HOME
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 20, 65, 55, 55)) {  //Botao A1 - Ajustes WiFi
          Serial.println("Ajustes WiFi selecionado");
          Mostrar_Menu_Ajustes_WIFi();
          tela_atual = TELA_MENU_AJUSTES_WIFI;
        } else if (isInside(x, y, 20, 150, 55, 55)) {  //Botao A2 - Ajustes HORA
          Serial.println("Ajustes Hora Selecionado");
          Mostrar_Menu_Definir_Local();
          tela_atual = TELA_MENU_DEFINIR_LOCAL;
        } else if (isInside(x, y, 20, 235, 55, 55)) {  //Botao A3 - Ajustes DADOS
          Serial.println("Ajustes Local Selecionado");
          Mostrar_Menu_Dados_Historicos();
          tela_atual = TELA_DADOS_HISTORICOS;
        } else if (isInside(x, y, 110, 65, 55, 55)) {  //Botao B1 - Ajustes Calibrar Bomba
          Serial.println("Ajustes Calibrar selecionado");
          Mostrar_Menu_Calibrar_Bomba();
          tela_atual = TELA_MENU_CALIBRAR_BOMBA;
        } else if (isInside(x, y, 110, 150, 55, 55)) {  //Botao B2 - Ajustes Calibrar TELA
          Serial.println("Ajustes Tela selecionado");
          Mostrar_Menu_Calibrar_Touch();
          tela_atual = TELA_CALIBRAR_TOUCH;
        } else if (isInside(x, y, 110, 235, 55, 55)) {  //Botao B3 - Ajustes SOBRE (FIRMWARE)
          Serial.println("Ajustes Dados selecionado");
          Mostrar_Menu_Atualizar_Firmaware();
          tela_atual = TELA_MENU_ATUALIZAR_FIRMWARE;
        } else if (isInside(x, y, 200, 65, 55, 55)) {  //Botao C1 - Ajustes DOSAGEM
          Serial.println("Ajustes Dosagem selecionado");
          Mostrar_Menu_Ajuste_Dosagem_diaria();
          tela_atual = TELA_MENU_AJUSTE_DOSAGEM_DIARIA;
        } else if (isInside(x, y, 200, 150, 55, 55)) {  //Botao C2 - Ajustes AGENDA
          Serial.println("Ajustes Agenda selecionado");
          Mostrar_Menu_Agendar_Hora_Dosagem();
          tela_atual = TELA_AGENDAR_DOSAGEM;
        } else if (isInside(x, y, 200, 235, 55, 55)) {  //Botao C3 - Ajustes MASTER RESET
          Serial.println("Ajustes MASTER RESET selecionado");
          Mostrar_Menu_Master_Reset();
          tela_atual = TELA_MENU_MASTER_RESET;
        } else if (isInside(x, y, 290, 65, 45, 45)){
          Mostrar_Menu_Nivel_Cisterna();
          tela_atual = TELA_NIVEL_CISTERNA;
        }
        break;



      case TELA_MENU_BOMBEAR:                    //====================================================MENU BOMBEAR
        if (isInside(x, y, 420, 260, 55, 55)) {  // Botao Voltar
          TotalBombeado = 0.0;
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 292, 90, 115, 55)) {
          bombeando = true;
        } else if (isInside(x, y, 292, 165, 115, 55)) {  //botao nombear vermelho
          sugando = true;
        }
        break;





      case TELA_MENU_DEFINIR_LOCAL:                       //=============================================MENU DEFINIR LOCAL
        if (isInside(x, y, 420, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 55, 180, 55, 55)){
          Mostrar_Tela_Digitar_Cidade();
          tela_atual = TELA_DIGITAR_CIDADE;
        }
        break;



      case TELA_MENU_AJUSTES_OUTROS:              //=======================================================MENU AJUSTES OUTROS
        if (isInside(x, y, 330, 245, 125, 55)) {  //Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 370, 70, 100, 150)) {
          Serial.println("Contagem regressive Menu Ajustar Outros Selecionado");

          for (int o = 0; o < 10; o++) {
            tft.fillRect(150, 135, 200, 100, TFT_BLACK);
            tft.setTextColor(TFT_WHITE, TFT_BLACK);
            tft.setFreeFont(FSB12);
            tft.setCursor(200, 150);
            tft.println("Tem Certeza??");
            tft.println("");
            tft.println("");
            tft.setCursor(200, 170);
            tft.print(10 - o);
            tft.println(" Seg");
            delay(1000);
          }


          EEPROM.put(POSICAO_DOSAGEM_ACUMULADA, 0);  // Dosagem Acumulada
          EEPROM.put(10, 25);                        // Dosagem Diaria
          EEPROM.put(POSICAO_ML_PER_REV_AJUSTADO, mlPerRevolutionPadrao);
          Domingo_Programado = false;
          Segunda_Programado = false;
          Terca_Programado = false;
          Quarta_Programado = false;
          Quinta_Programado = false;
          Sexta_Programado = false;
          Sabado_Programado = false;
          EEPROM.put(POSICAO_DOMINGO_PROGRAMADO, Domingo_Programado);
          EEPROM.put(POSICAO_SEGUNDA_PROGRAMADO, Segunda_Programado);
          EEPROM.put(POSICAO_TERCA_PROGRAMADO, Terca_Programado);
          EEPROM.put(POSICAO_QUARTA_PROGRAMADO, Quarta_Programado);
          EEPROM.put(POSICAO_QUINTA_PROGRAMADO, Quinta_Programado);
          EEPROM.put(POSICAO_SEXTA_PROGRAMADO, Sexta_Programado);
          EEPROM.put(POSICAO_SABADO_PROGRAMADO, Sabado_Programado);
          EEPROM.commit();
          delay(100);
          ResetarESP();
        }
        break;




      case TELA_MENU_AJUSTES_WIFI:               //==========================================================MENU AJUSTES WIFI
        if (isInside(x, y, 420, 260, 55, 55)) {  // Botão HOME
          Conexao_WiFi_Setup();
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Conexao_WiFi_Setup();
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botao Atualizar
          Mostrar_Menu_Ajustes_WIFi();
          tela_atual = TELA_MENU_AJUSTES_WIFI;
        } else {
          // Detecta clique nas redes (até 5 opções)
          for (int i = 0; i < redesDisponiveis && i < 5; i++) {
            int linhaY = 105 + i * 35;
            if (isInside(x, y, 10, linhaY - 10, 460, 25)) {
              String ssid = WiFi.SSID(i);
              if (ssid.length() > 0) {
                strcpy(ssidSelecionado, ssid.c_str());
                redeSelecionadaIndex = i;
                Serial.println("Rede selecionada: " + ssid);
                Mostrar_Tela_Digitar_Senha();
                tela_atual = TELA_DIGITAR_SENHA;
                break;
              }
            }
          }
        }
        break;







      case TELA_MENU_AJUSTE_DOSAGEM_DIARIA:      //============================================================MENU AJUSTES DOSAGEM DIARIA
        if (isInside(x, y, 420, 260, 55, 55)) {  //Botao HOME
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  // Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botao Confirmar
          bool pergunta = DesejaContinuar();
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
            if (!pergunta) {
              EEPROM.put(POSICAO_MINUTOS_ZONA_1, Minutos_Zona_1);
              EEPROM.put(POSICAO_MINUTOS_ZONA_2, Minutos_Zona_2);
              EEPROM.put(POSICAO_MINUTOS_ZONA_3, Minutos_Zona_3);
              EEPROM.put(POSICAO_MINUTOS_ZONA_4, Minutos_Zona_4);
              EEPROM.put(POSICAO_MINUTOS_ZONA_5, Minutos_Zona_5);
              EEPROM.put(POSICAO_MINUTOS_ZONA_6, Minutos_Zona_6);
              EEPROM.put(POSICAO_MINUTOS_ZONA_7, Minutos_Zona_7);
              EEPROM.commit();
              
              tft.fillRect(0, 60, 480, 200, TFT_BLACK);  //apaga
              tft.setTextColor(TFT_YELLOW, TFT_BLACK);
              tft.setTextDatum(MC_DATUM);
              tft.drawString("Salvando Dados...!", 240, 150);
              delay(2000);
              Mostrar_Menu_Principal();
              tela_atual = TELA_MENU_PRINCIPAL;
            } else {
              Mostrar_Menu_Ajuste_Dosagem_diaria();
              tela_atual = TELA_MENU_AJUSTE_DOSAGEM_DIARIA;
            }



        }  else if (isInside(x, y, 290, 40, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_1 = Minutos_Zona_1 - 1;
          if (Minutos_Zona_1 <= 0) { Minutos_Zona_1 = 0; }
          tft.fillRoundRect(206, 41, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_1), 230, 54);
        } else if (isInside(x, y, 350, 40, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_1 = Minutos_Zona_1 + 1;
          if (Minutos_Zona_1 <= 0) { Minutos_Zona_1 = 0; }
          tft.fillRoundRect(206, 41, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_1), 230, 54);






        }  else if (isInside(x, y, 290, 70, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_2 = Minutos_Zona_2 - 1;
          if (Minutos_Zona_2 <= 0) { Minutos_Zona_2 = 0; }
          tft.fillRoundRect(206, 71, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_2), 230, 84);

        } else if (isInside(x, y, 350, 70, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_2 = Minutos_Zona_2 + 1;
          if (Minutos_Zona_2 <= 0) { Minutos_Zona_2 = 0; }
          tft.fillRoundRect(206, 71, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_2), 230, 84);





        }  else if (isInside(x, y, 290, 100, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_3 = Minutos_Zona_3 - 1;
          if (Minutos_Zona_3 <= 0) { Minutos_Zona_3 = 0; }
          tft.fillRoundRect(206, 101, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_3), 230, 114);

        } else if (isInside(x, y, 350, 100, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_3 = Minutos_Zona_3 + 1;
          if (Minutos_Zona_3 <= 0) { Minutos_Zona_3 = 0; }
          tft.fillRoundRect(206, 101, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_3), 230, 114);






         }  else if (isInside(x, y, 290, 130, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_4 = Minutos_Zona_4 - 1;
          if (Minutos_Zona_4 <= 0) { Minutos_Zona_4 = 0; }
          tft.fillRoundRect(206, 131, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_4), 230, 144);

        } else if (isInside(x, y, 350, 130, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_4 = Minutos_Zona_4 + 1;
          if (Minutos_Zona_4 <= 0) { Minutos_Zona_4 = 0; }
          tft.fillRoundRect(206, 131, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_4), 230, 144);



        }  else if (isInside(x, y, 290, 160, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_5 = Minutos_Zona_5 - 1;
          if (Minutos_Zona_5 <= 0) { Minutos_Zona_5 = 0; }
          tft.fillRoundRect(206, 161, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_5), 230, 174);

        } else if (isInside(x, y, 350, 160, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_5 = Minutos_Zona_5 + 1;
          if (Minutos_Zona_5 <= 0) { Minutos_Zona_5 = 0; }
          tft.fillRoundRect(206, 161, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_5), 230, 174);



        }  else if (isInside(x, y, 290, 190, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_6 = Minutos_Zona_6 - 1;
          if (Minutos_Zona_6 <= 0) { Minutos_Zona_6 = 0; }
          tft.fillRoundRect(206, 191, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_6), 230, 204);

        } else if (isInside(x, y, 350, 190, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_6 = Minutos_Zona_6 + 1;
          if (Minutos_Zona_6 <= 0) { Minutos_Zona_6 = 0; }
          tft.fillRoundRect(206, 191, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_6), 230, 204);




        }  else if (isInside(x, y, 290, 220, 30, 30)) {
          // Botao (-) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_7 = Minutos_Zona_7 - 1;
          if (Minutos_Zona_7 <= 0) { Minutos_Zona_7 = 0; }
          tft.fillRoundRect(206, 221, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_7), 230, 234);

        } else if (isInside(x, y, 350, 220, 30, 30)) {
          // Botao (+) Zona 1
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          Minutos_Zona_7 = Minutos_Zona_7 + 1;
          if (Minutos_Zona_7 <= 0) { Minutos_Zona_7 = 0; }
          tft.fillRoundRect(206, 221, 48, 28, 5, TFT_BLACK);  //Apaga
          tft.setTextDatum(MC_DATUM);
          tft.drawString(String(Minutos_Zona_7), 230, 234);  



        
        }   
        break;





      case TELA_MENU_CALIBRAR_BOMBA:             //==============================================================MENU AJUSTES CALIBRAR BOMBA
        if (isInside(x, y, 420, 260, 55, 55)) {  //Bota Home
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botao Confirmar
          bool pergunta = DesejaContinuar();
          if (!pergunta) {
            EEPROM.put(POSICAO_ML_PER_REV_AJUSTADO, mlPerRevolutionAjustado);
            EEPROM.commit();
            tft.fillRect(0, 50, 400, 200, TFT_BLACK);  //Limpa a area
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setFreeFont(FF1);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Salvando Dados...!", 240, 150);
            delay(2000);
            Mostrar_Menu_Ajustes();
            tela_atual = TELA_MENU_AJUSTES;
          } else {
            Mostrar_Menu_Calibrar_Bomba();
            tela_atual = TELA_MENU_CALIBRAR_BOMBA;
          }
        } else if (isInside(x, y, 410, 175, 40, 40)) {  //Botao (-%)
          Serial.println("+1% Selecionado");
          mlPerRevolutionAjustado = (mlPerRevolutionAjustado * 1.01);
          tft.fillRect(141, 181, 88, 33, TFT_BLACK);  //Limpa a area
          tft.setFreeFont(FF2);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.setTextDatum(CR_DATUM);
          tft.drawString(String(mlPerRevolutionAjustado, 0), 215, 196);
        } else if (isInside(x, y, 340, 175, 40, 40)) {  // Botao +1%
          Serial.println("-1% Selecionado");
          mlPerRevolutionAjustado = (mlPerRevolutionAjustado / 1.01);
          tft.fillRect(141, 181, 88, 33, TFT_BLACK);  //Limpa a area
          tft.setFreeFont(FF2);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          tft.setTextDatum(CR_DATUM);
          tft.drawString(String(mlPerRevolutionAjustado, 0), 215, 196);
        }
        break;




      case TELA_DIGITAR_SENHA:                   //================================================================MENU AJUSTES DIGITAR SENHA
        if (isInside(x, y, 420, 260, 55, 55)) {  //Botao HOME
          Conexao_WiFi_Setup();
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 290, 260, 55, 55)) {  // Botão confirmar
          Serial.println("Salvando SSID e Senha...");
          EEPROM.writeString(POSICAO_SSID_WIFI, ssidSelecionado);
          EEPROM.writeString(POSICAO_SENHA_WIFI, wifiSenha);
          EEPROM.commit();
          connectWiFi();                                // Tenta conectar
        } else if (isInside(x, y, 355, 260, 55, 55)) {  // Botão Voltar
          Conexao_WiFi_Setup();
          tela_atual = TELA_MENU_AJUSTES_WIFI;
          Mostrar_Menu_Ajustes_WIFi();
        } else {
          const int btnW = 45;
          const int btnH = 35;
          const int startX = 10;
          const int startY = 105;
          const int gap = 2;

          const char*(*keys)[10];
          if (modoTeclado == 0) keys = keysMaiusculas;
          else if (modoTeclado == 1) keys = keysMinusculas;
          else keys = keysSimbolos;

          for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 10; col++) {
              int xKey = startX + col * (btnW + gap);
              int yKey = startY + row * (btnH + gap);

              if (isInside(x, y, xKey, yKey, btnW, btnH)) {
                const char* tecla = keys[row][col];

                if (strcmp(tecla, "SHIF") == 0) {
                  modoAlfabeto = !modoAlfabeto;  // Alterna entre 0 e 1
                  modoTeclado = modoAlfabeto;    // Atualiza o layout para maius/minus
                  desenharTeclado();
                } else if (strcmp(tecla, "#+=") == 0) {
                  if (modoTeclado == 2) {
                    modoTeclado = modoAlfabeto;  // Retorna ao layout anterior (maiúscula ou minúscula)
                  } else {
                    modoTeclado = 2;  // Vai para símbolos
                  }
                  desenharTeclado();
                } else if (strcmp(tecla, "   ") == 0) {
                  int len = strlen(wifiSenha);
                  if (len < sizeof(wifiSenha) - 1) {
                    wifiSenha[len] = ' ';
                    wifiSenha[len + 1] = '\0';
                  }
                } else if (strcmp(tecla, "<=") == 0) {
                  int len = strlen(wifiSenha);
                  if (len > 0) wifiSenha[len - 1] = '\0';
                } else if (strlen(tecla) == 1) {
                  int len = strlen(wifiSenha);
                  if (len < sizeof(wifiSenha) - 1) {
                    wifiSenha[len] = tecla[0];
                    wifiSenha[len + 1] = '\0';
                  }
                }

                // Atualiza o campo de senha na tela
                tft.fillRect(100, 60, 360, 30, TFT_BLACK);  // Apaga o texto anterior
                tft.drawRect(10, 60, 450, 30, TFT_GREEN);   // Redesenha o campo da senha
                tft.setCursor(100, 80);
                tft.setTextColor(TFT_WHITE);
                tft.setFreeFont(FF1);
                tft.print(wifiSenha);
                break;
              }
            }
          }
        }
        break;


      case TELA_MENU_MASTER_RESET:               //==================================================================MENU MASTER RESET
        if (isInside(x, y, 420, 260, 55, 55)) {  //Bota Home
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 290, 260, 55, 55)) {
          bool pergunta = DesejaContinuar();
          if (!pergunta) {
            EEPROM.put(POSICAO_MINUTOS_ZONA_1, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_2, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_3, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_4, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_5, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_6, 0);
            EEPROM.put(POSICAO_MINUTOS_ZONA_7, 0);
            
            
            
            
            
            EEPROM.put(POSICAO_DOSAGEM_ACUMULADA, 0.0);
            EEPROM.put(POSICAO_DOSAGEM_DIARIA, 500);
            EEPROM.put(POSICAO_DOSAGEM_RESET, 0);
            EEPROM.put(POSICAO_DOMINGO_PROGRAMADO, 1);  // MEMORIA-POsicao 31 = Domingo bool
            EEPROM.put(POSICAO_SEGUNDA_PROGRAMADO, 1);  // MEMORIA-POsicao 32 = Segunda bool
            EEPROM.put(POSICAO_TERCA_PROGRAMADO, 1);    // MEMORIA-POsicao 33 = Terça bool
            EEPROM.put(POSICAO_QUARTA_PROGRAMADO, 1);   // MEMORIA-POsicao 34 = Quarta bool
            EEPROM.put(POSICAO_QUINTA_PROGRAMADO, 1);   // MEMORIA-POsicao 35 = Quinta bool
            EEPROM.put(POSICAO_SEXTA_PROGRAMADO, 1);    // MEMORIA-POsicao 36 = Sexta bool
            EEPROM.put(POSICAO_SABADO_PROGRAMADO, 1);   // MEMORIA-POsicao 37 = Sabado bool

            EEPROM.put(POSICAO_HORA_PROGRAMADA, 8);
            EEPROM.put(POSICAO_MINUTO_PROGRAMADO, 0);
            EEPROM.put(POSICAO_SEGUNDO_PROGRAMADO, 0);  // MEMORIA-POsicao 40 = Segundo Programado

            EEPROM.put(POSICAO_ML_PER_REV_PADRAO, 325.0f);
            EEPROM.put(POSICAO_ML_PER_REV_AJUSTADO, 325.0f);

            String ssid_Master_Reset = "MOVISTAR-WIFI6-8C30";
            String pass_Master_Reset = "ee7Ymedg79aUcjViXm3a";
            EEPROM.writeString(POSICAO_SSID_WIFI, ssid_Master_Reset);   //MEMORIA-POsicao 60 = ssid EEPROM.writeChar(address, 'A');  / EEPROM.readChar(address)
            EEPROM.writeString(POSICAO_SENHA_WIFI, pass_Master_Reset);  //MEMORIA-POsicao 100 = pass

            EEPROM.put(POSICAO_TAMANHO_PISCINA, 50.0f);         // Exemplo: 80 m³
            EEPROM.put(POSICAO_CONCENTRACAO_CLORO, 14.0f);      // Exemplo: 14%
            EEPROM.put(POSICAO_DOSAGEM_REFERENCIA, 500);  //MEMORIA-Posiçao 220 = Dosagem Referencia (IA)

            EEPROM.writeInt(POSICAO_CALIB_TOUCH_X_MIN, 300);
            EEPROM.writeInt(POSICAO_CALIB_TOUCH_X_MAX, 3950);
            EEPROM.writeInt(POSICAO_CALIB_TOUCH_Y_MIN, 200);
            EEPROM.writeInt(POSICAO_CALIB_TOUCH_Y_MAX, 3900);
            EEPROM.writeInt(POSICAO_ULTIMO_DIA_EXECUTADO, -1);
            EEPROM.write(POSICAO_ULTIMO_DIA_EXECUTADO, -1);

            EEPROM.put(POSICAO_PRIMEIRO_ACESSO, 1);   

            EEPROM.writeFloat(POSICAO_ALTURA_CISTERNA, 266.92);
            EEPROM.commit();

            EEPROM.commit();

            tft.fillRect(0, 0, 480, 320, TFT_BLACK);  //apaga
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Restaurando dados de fabrica!", 240, 150);
            delay(2000);
            ESP.restart();

          } else {
            Mostrar_Menu_Ajustes();
            tela_atual = TELA_MENU_AJUSTES;
          }
        }
        break;





      case TELA_MENU_INFORMACOES:                //====================================================================MENU INFORMACOES
        if (isInside(x, y, 420, 260, 55, 55)) {                      //Botao HOME
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {                    //Botal Salvar
          bool pergunta = DesejaContinuar();
          if (!pergunta) {
            tft.setFreeFont(FF1);
            tft.setTextDatum(MC_DATUM);
            tft.fillScreen(TFT_BLACK);
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.drawString("Salvando........", 230, 160);
            delay(350);
            EEPROM.put(POSICAO_TAMANHO_PISCINA,Tamanho_Piscina );         // Exemplo: 80 m³
            EEPROM.put(POSICAO_CONCENTRACAO_CLORO, Concentracao_Cloro);      // Exemplo: 14%
            EEPROM.commit();
            Mostrar_Menu_Informacoes();
            tela_atual = TELA_MENU_INFORMACOES;
          } else {
            Mostrar_Menu_Informacoes();
            tela_atual = TELA_MENU_INFORMACOES;
          }
        } else if (isInside(x, y, 325, 52, 35, 35)){ //Botao - piscima
          Tamanho_Piscina = Tamanho_Piscina - 1;
            if (Tamanho_Piscina < 0) { Tamanho_Piscina = 0; }
          tft.setFreeFont(FF1);
          tft.setTextDatum(TR_DATUM);
          tft.fillRoundRect(256, 51, 48, 38, 7,  TFT_BLACK);
          tft.drawString(String(Tamanho_Piscina,0), 295, 65);

        } else if (isInside(x, y, 380, 52, 35, 35)){ //Botal + Piscina
          Tamanho_Piscina = Tamanho_Piscina + 1;
          tft.setFreeFont(FF1);
          tft.setTextDatum(TR_DATUM);
          tft.fillRoundRect(256, 51, 48, 38, 7, TFT_BLACK);
          tft.drawString(String(Tamanho_Piscina,0), 295, 65);

        } else if (isInside(x, y, 325, 102, 35, 35)){
          Concentracao_Cloro = Concentracao_Cloro -1;
            if (Concentracao_Cloro < 0) { Concentracao_Cloro = 1; }
          tft.setFreeFont(FF1);
          tft.setTextDatum(TR_DATUM);
          tft.fillRoundRect(256, 101, 48, 38, 7, TFT_BLACK);
          tft.drawString(String(Concentracao_Cloro,0), 295, 115);

        } else if (isInside(x, y, 380, 102, 35, 35)){
          Concentracao_Cloro = Concentracao_Cloro +1;
            if (Concentracao_Cloro > 100) { Concentracao_Cloro = 100; }
          tft.setFreeFont(FF1);
          tft.setTextDatum(TR_DATUM);
          tft.fillRoundRect(256, 101, 48, 38, 7, TFT_BLACK);
          tft.drawString(String(Concentracao_Cloro,0), 295, 115);
        } else if (isInside(x, y, 5, 220, 115, 55)){
          Mostrar_Menu_Previsao_Tempo();
          tela_atual = TELA_PREVISAO_TEMPO;
        }
        break;






      

      case TELA_PREVISAO_TEMPO:                       //=================================================================MENU PREVISAO TEMPO
        if (isInside(x, y, 420, 260, 55, 55)) {  //Botao HOME
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Informacoes();
          tela_atual = TELA_MENU_INFORMACOES;
        } 
        break;



      case TELA_DIGITAR_CIDADE:
        if (isInside(x, y, 420, 260, 55, 55)) {  // Botão HOME
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;

        } else if (isInside(x, y, 290, 260, 55, 55)) {  // Botão confirmar
          bool pergunta = DesejaContinuar();
          if (!pergunta) {
            tft.fillScreen(TFT_BLACK);
            tft.setFreeFont(FF1);
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.drawString("Procurando cidade......", 230, 160);
            Serial.println("Salvando nome da cidade...");
            Serial.print(">>> Buscando cidade: '");
            Serial.print(cidade_digitada);
            Serial.println("'");

            // Busca com limite 5 cidades
            HTTPClient http;
            String url_geo = "http://api.openweathermap.org/geo/1.0/direct?q=" + urlencode(String(cidade_digitada)) + "&limit=5&appid=" + apiKey;
            http.begin(url_geo);
            int httpCode = http.GET();
            if (httpCode > 0) {
              String payload = http.getString();
              DynamicJsonDocument doc(4096);
              DeserializationError error = deserializeJson(doc, payload);
              if (!error && doc.size() > 0) {
                int idxEscolhida = 0;

                // Se veio mais de uma cidade, exibe opções e espera escolha do usuário
                if (doc.size() > 1) {
                  tft.fillScreen(TFT_BLACK);
                  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                  tft.setFreeFont(FF1);
                  tft.setTextDatum(TL_DATUM);
                  tft.drawString("Escolha a cidade:", 10, 10);

                  int n = doc.size();
                  int btnY = 50;
                  char cidadesMostradas[10][32]; // Até 10 cidades únicas
                  int countMostradas = 0;
                  int indexMapa[10];             // Guarda o índice original de cada cidade exibida

                  // Filtra e mostra só cidade+país únicos
                  for (int i = 0; i < n; i++) {
                    String nome = doc[i]["name"].as<String>();
                    String pais = doc[i]["country"].as<String>();
                    String chave = nome + "," + pais;
                    bool duplicado = false;
                    for (int j = 0; j < countMostradas; j++) {
                      if (chave == String(cidadesMostradas[j])) {
                        duplicado = true;
                        break;
                      }
                    }
                    if (duplicado) continue;
                    chave.toCharArray(cidadesMostradas[countMostradas], 32);
                    indexMapa[countMostradas] = i; // Guarda o índice do JSON
                    countMostradas++;

                    // Desenha o botão
                    String desc = nome + ", " + pais;
                    tft.fillRoundRect(10, btnY, 440, 40, 7, TFT_NAVY);
                    tft.setTextColor(TFT_WHITE, TFT_NAVY);
                    tft.drawString(desc, 20, btnY + 10);
                    btnY += 50;
                  }

                  // Aguarda o toque do usuário numa das opções exibidas
                  bool selecionou = false;
                  int idxEscolhidaFiltrada = 0;
                  while (!selecionou) {
                    if (ts.touched()) {
                      TS_Point p = ts.getPoint();
                      int ysel = map(p.y, calib_y_min, calib_y_max, 0, tft.height());
                      for (int i = 0; i < countMostradas; i++) {
                        int y1 = 50 + i * 50;
                        if (ysel > y1 && ysel < y1 + 40) {
                          idxEscolhidaFiltrada = i; // Posição na lista filtrada
                          selecionou = true;
                          break;
                        }
                      }
                      delay(300); // anti-bounce
                    }
                    yield();
                  }
                  // Índice real no JSON:
                  idxEscolhida = indexMapa[idxEscolhidaFiltrada];
                }

                // Usa o índice escolhido (ou 0 se só uma cidade)
                float lat = doc[idxEscolhida]["lat"];
                float lon = doc[idxEscolhida]["lon"];
                String nomeOficial = doc[idxEscolhida]["name"].as<String>();
                String pais = doc[idxEscolhida]["country"].as<String>();

                // Salva como já fazia
                for (int i = 0; i < 64; i++) EEPROM.write(POSICAO_NOME_CIDADE + i, 0);
                EEPROM.writeString(POSICAO_NOME_CIDADE, nomeOficial);
                EEPROM.writeFloat(POSICAO_LATITUDE_CIDADE, lat);
                EEPROM.writeFloat(POSICAO_LONGITUDE_CIDADE, lon);
                for (int i = 0; i < 4; i++) EEPROM.write(POSICAO_PAIS_DA_CIDADE + i, 0);
                EEPROM.writeString(POSICAO_PAIS_DA_CIDADE, pais);
                EEPROM.commit();

                strcpy(cidade_digitada, nomeOficial.c_str());
                cidade_latitude = lat;
                cidade_longitude = lon;

                tft.fillScreen(TFT_BLACK);
                tft.setFreeFont(FF1);
                tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                tft.setTextDatum(MC_DATUM);
                tft.drawString("Cidade Encontrada", 230, 140);
                tft.drawString("Salvando....", 230, 190);
                delay(2000);

                obterForecastClimatico(nomeOficial);
                Mostrar_Menu_Definir_Local();
                tela_atual = TELA_MENU_DEFINIR_LOCAL;

              } else {
                tft.fillScreen(TFT_BLACK);
                tft.setFreeFont(FF1);
                tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                tft.setTextDatum(MC_DATUM);
                tft.drawString("Cidade nao encontrada!", 230, 150);
                tft.drawString("Verifique a correta digitacao.", 230, 190);
                tft.drawString("Por Favor, tente Novamente", 230, 230);
                delay(2000);
                Mostrar_Menu_Definir_Local();
                tela_atual = TELA_MENU_DEFINIR_LOCAL;
                http.end();
              }
            } else {
              tft.fillScreen(TFT_BLACK);
              tft.setFreeFont(FF1);
              tft.setTextColor(TFT_YELLOW, TFT_BLACK);
              tft.setTextDatum(MC_DATUM);
              tft.drawString("Falha na consulta GeoAPI!", 230, 190);
              delay(2000);
              http.end();
            }

          } else {
            Mostrar_Menu_Ajustes();
            tela_atual = TELA_MENU_AJUSTES;
          }

        } else if (isInside(x, y, 355, 260, 55, 55)) {  // Botão Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;

        } else {
          const int btnW = 45;
          const int btnH = 35;
          const int startX = 10;
          const int startY = 105;
          const int gap = 2;

          const char*(*keys)[10];
          if (modoTeclado == 0) keys = keysMaiusculas;
          else if (modoTeclado == 1) keys = keysMinusculas;
          else keys = keysSimbolos;

          for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 10; col++) {
              int xKey = startX + col * (btnW + gap);
              int yKey = startY + row * (btnH + gap);

              if (isInside(x, y, xKey, yKey, btnW, btnH)) {
                const char* tecla = keys[row][col];

                if (strcmp(tecla, "SHIF") == 0) {
                  modoAlfabeto = !modoAlfabeto;
                  modoTeclado = modoAlfabeto;
                  desenharTeclado();
                } else if (strcmp(tecla, "#+=") == 0) {
                  if (modoTeclado == 2) modoTeclado = modoAlfabeto;
                  else modoTeclado = 2;
                  desenharTeclado();
                } else if (strcmp(tecla, "   ") == 0) {
                  int len = strlen(cidade_digitada);
                  if (len < sizeof(cidade_digitada) - 1) {
                    cidade_digitada[len] = ' ';
                    cidade_digitada[len + 1] = '\0';
                  }
                } else if (strcmp(tecla, "<=") == 0) {
                  int len = strlen(cidade_digitada);
                  if (len > 0) cidade_digitada[len - 1] = '\0';
                } else if (strlen(tecla) == 1) {
                  int len = strlen(cidade_digitada);
                  if (len < sizeof(cidade_digitada) - 1) {
                    cidade_digitada[len] = tecla[0];
                    cidade_digitada[len + 1] = '\0';
                  }
                }

                // Atualiza o campo de cidade na tela
                tft.fillRect(100, 60, 360, 30, TFT_BLACK);
                tft.drawRect(10, 60, 450, 30, TFT_GREEN);
                tft.setCursor(100, 80);
                tft.setTextColor(TFT_WHITE);
                tft.setFreeFont(FF1);
                tft.println(removerAcentos(String(cidade_digitada)));
                break;
              }
            }
          }
        }
        break;




      case TELA_AGENDAR_DOSAGEM:  //=========================================================================================MENU AGENDAR DOSAGEM
        if (isInside(x, y, 420, 260, 55, 55)) {  //Bota Home
          Mostrar_Menu_Principal();
          tela_atual = TELA_MENU_PRINCIPAL;
        } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
          Mostrar_Menu_Ajustes();
          tela_atual = TELA_MENU_AJUSTES;
        } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botao Confirmar
          bool pergunta = DesejaContinuar();
          tft.setFreeFont(FF1);
          tft.setTextColor(TFT_WHITE, TFT_BLACK);
          if (!pergunta) {
            EEPROM.put(POSICAO_DOMINGO_PROGRAMADO, Domingo_Programado);
            EEPROM.put(POSICAO_SEGUNDA_PROGRAMADO, Segunda_Programado);
            EEPROM.put(POSICAO_TERCA_PROGRAMADO, Terca_Programado);
            EEPROM.put(POSICAO_QUARTA_PROGRAMADO, Quarta_Programado);
            EEPROM.put(POSICAO_QUINTA_PROGRAMADO, Quinta_Programado);
            EEPROM.put(POSICAO_SEXTA_PROGRAMADO, Sexta_Programado);
            EEPROM.put(POSICAO_SABADO_PROGRAMADO, Sabado_Programado);
            EEPROM.put(POSICAO_HORA_PROGRAMADA, Dosagem_hora_programada);
            EEPROM.put(POSICAO_MINUTO_PROGRAMADO, Dosagem_minuto_programado);
            EEPROM.commit();
            tft.fillRect(0, 60, 480, 200, TFT_BLACK);  //apaga
            tft.setTextColor(TFT_YELLOW, TFT_BLACK);
            tft.setTextDatum(MC_DATUM);
            tft.drawString("Salvando Dados...!", 240, 150);
            delay(2000);
            Mostrar_Menu_Principal();
            tela_atual = TELA_MENU_PRINCIPAL;
          } else {
            Mostrar_Menu_Agendar_Hora_Dosagem();
            tela_atual = TELA_AGENDAR_DOSAGEM;
          }
        } else if (isInside(x, y, 15, 70, 55, 55)) {  // Botao SEGUNDA
          Segunda_Programado = !Segunda_Programado;
          if (Segunda_Programado) {
            tft.fillRect(20, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(21, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(20, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(21, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 80, 70, 55, 55)) {
          Terca_Programado = !(Terca_Programado);
          if (Terca_Programado) {
            tft.fillRect(85, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(86, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(85, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(86, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 145, 70, 55, 55)) {
          Quarta_Programado = !(Quarta_Programado);
          if (Quarta_Programado) {
            tft.fillRect(150, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(151, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(150, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(151, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 210, 70, 55, 55)) {
          Quinta_Programado = !(Quinta_Programado);
          if (Quinta_Programado) {
            tft.fillRect(215, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(216, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(215, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(216, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 275, 70, 55, 55)) {
          Sexta_Programado = !(Sexta_Programado);
          if (Sexta_Programado) {
            tft.fillRect(280, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(281, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(280, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(281, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 340, 70, 55, 55)) {
          Sabado_Programado = !(Sabado_Programado);
          if (Sabado_Programado) {
            tft.fillRect(345, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(346, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(345, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(346, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }
        } else if (isInside(x, y, 405, 70, 55, 55)) {
          Domingo_Programado = !(Domingo_Programado);
          if (Domingo_Programado) {
            tft.fillRect(410, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(411, 85, ON45x45, 45, 45, TFT_GREENYELLOW);
          } else {
            tft.fillRect(410, 95, 45, 25, TFT_BLACK);  //apaga
            tft.drawBitmap(411, 85, OFF45x45, 45, 45, TFT_LIGHTGREY);
          }

        } else if (isInside(x, y, 190, 165, 40, 40)){ //-Hora
          Dosagem_hora_programada = (Dosagem_hora_programada + 23) % 24;
          tft.fillRoundRect(106, 161, 68, 48, 7, TFT_BLACK);  //apaga
          char horaFormatada[3]; // 2 dígitos + terminador '\0'
          sprintf(horaFormatada, "%02d", Dosagem_hora_programada);
          tft.drawString(horaFormatada, 155, 185);
        
        } else if (isInside(x, y, 240, 165, 40, 40)){  // + Hora
          Dosagem_hora_programada = (Dosagem_hora_programada + 1) % 24;
          tft.fillRoundRect(106, 161, 68, 48, 7, TFT_BLACK);  //apaga
          char horaFormatada[3]; // 2 dígitos + terminador '\0'
          sprintf(horaFormatada, "%02d", Dosagem_hora_programada);
          tft.drawString(horaFormatada, 155, 185);
        
        }else if (isInside(x, y, 190, 215, 40, 40)){  // - min
          Dosagem_minuto_programado = (Dosagem_minuto_programado + 59) % 60;
          char minutoFormatado[3]; // 2 dígitos + terminador '\0'
          sprintf(minutoFormatado, "%02d", Dosagem_minuto_programado);
          tft.fillRoundRect(106, 211, 68, 48, 7, TFT_BLACK);  //apaga
          tft.drawString(minutoFormatado, 155, 235);
        
        }else if (isInside(x, y, 240, 215, 40, 40)){  // + min
          Dosagem_minuto_programado = (Dosagem_minuto_programado + 1) % 60;
          char minutoFormatado[3]; // 2 dígitos + terminador '\0'
          sprintf(minutoFormatado, "%02d", Dosagem_minuto_programado);
          tft.fillRoundRect(106, 211, 68, 48, 7, TFT_BLACK);  //apaga
          tft.drawString(minutoFormatado, 155, 235);
        }
        break;



      case TELA_DADOS_HISTORICOS:               //=============================================================================MENU DADOS HISTORICOS
              if (isInside(x, y, 420, 260, 55, 55)) {         //Bota Home
                Mostrar_Menu_Principal();
                tela_atual = TELA_MENU_PRINCIPAL;
              } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
                Mostrar_Menu_Ajustes();
                tela_atual = TELA_MENU_AJUSTES;
              } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botal Confirmar
                bool pergunta = DesejaContinuar();
                    if (!pergunta) {
                      EEPROM.put(POSICAO_DOSAGEM_ACUMULADA, 0.0);
                      EEPROM.commit();
                      tft.fillScreen(TFT_BLACK);
                      tft.setFreeFont(FF1);
                      tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                      tft.setTextDatum(MC_DATUM);
                      tft.drawString("Salvando Dados...!", 240, 150);
                      delay(2000);
                      Mostrar_Menu_Principal();
                      tela_atual = TELA_MENU_PRINCIPAL;
                    } else {
                      Mostrar_Menu_Dados_Historicos();
                      tela_atual = TELA_DADOS_HISTORICOS;
                    }
              }else if ( isInside(x, y, 305, 185, 40, 40)){
                tft.fillRoundRect(166, 181, 85, 38, 7, TFT_BLACK);
                 tft.setTextDatum(MR_DATUM);
                DosagemAcumulada = 0;
                tft.drawString(String((float)DosagemAcumulada, 0), 245, 200);
              }
              break;


      case TELA_MENU_ATUALIZAR_FIRMWARE:               //========================================================================MENU ATUALIZAR FIRMWARE
              if (isInside(x, y, 420, 260, 55, 55)) {         //Bota Home
                Mostrar_Menu_Principal();
                tela_atual = TELA_MENU_PRINCIPAL;
              } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
                Mostrar_Menu_Ajustes();
                tela_atual = TELA_MENU_AJUSTES;
              } else if (isInside(x, y, 145, 220, 55, 55)){
                checar_e_atualizar_firmware();//Buscar Firmware
                Serial.println("buscando Firmware");
                Mostrar_Menu_Principal();
                tela_atual = TELA_MENU_PRINCIPAL;
              } 
              break;




      case TELA_NIVEL_CISTERNA:             //==============================================================MENU AJUSTES CALIBRAR BOMBA
            if (isInside(x, y, 420, 260, 55, 55)) {  //Bota Home
              Mostrar_Menu_Principal();
              tela_atual = TELA_MENU_PRINCIPAL;
            } else if (isInside(x, y, 355, 260, 55, 55)) {  //Botao Voltar
              Mostrar_Menu_Ajustes();
              tela_atual = TELA_MENU_AJUSTES;
            } else if (isInside(x, y, 290, 260, 55, 55)) {  //Botao Confirmar
              bool pergunta = DesejaContinuar();
                  if (!pergunta) {
                    altura_cisterna = (Volume_Atual_cisterna / LITROS_POR_CM) + distancia;
                    EEPROM.writeFloat(POSICAO_ALTURA_CISTERNA, altura_cisterna);
                    EEPROM.commit();
                    tft.fillRect(0, 50, 400, 200, TFT_BLACK);  //Limpa a area
                    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
                    tft.setFreeFont(FF1);
                    tft.setTextDatum(MC_DATUM);
                    tft.drawString("Salvando Dados...Aguarde!", 240, 150);
                    VerificarNivelCisterna();  // <-- ESSA LINHA RESOLVE O PROBLEMA
                    Mostrar_Menu_Ajustes();
                    tela_atual = TELA_MENU_AJUSTES;
                  } 
            } else if (isInside(x, y, 350, 70, 40, 40)) {  //Botao (-%)
                Volume_Atual_cisterna = (Volume_Atual_cisterna / 1.001);
                tft.fillRoundRect(206, 71, 73, 38, 5, TFT_BLACK);  //Limpa a area
                tft.setFreeFont(FF1);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.setTextDatum(MR_DATUM);
                float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
                tft.drawString(String(volume_m3, 2), 270, 90);
              } else if (isInside(x, y, 420, 70, 40, 40)) {  // Botao +1%
                Volume_Atual_cisterna = (Volume_Atual_cisterna * 1.001);
                tft.fillRoundRect(205, 71, 73, 38, 5, TFT_BLACK);  //Limpa a area
                tft.setFreeFont(FF1);
                tft.setTextColor(TFT_WHITE, TFT_BLACK);
                tft.setTextDatum(MR_DATUM);
                float volume_m3 = ((int)(Volume_Atual_cisterna * 10 / 1000.0 + 0.5)) / 10.0;
                tft.drawString(String(volume_m3, 2), 270, 90);
            }


            break;  









    }
    delay(100);  // Evita múltiplas detecções rápidas
  }
}



//==============================================================================================     FUNCOES CORE 1     ========================================================
void Funcao_Mostrar_Total_Bombeado(void* pvParameters) {
  int frameAtual = 1;
  int ciclos = 0;
  int totalAnterior = -1;
  tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_BLACK);
  tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_BLACK);
  tft.setTextFont(4);  // Fonte interna padrão
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MR_DATUM);
  while (1) {
    // Pisca seta a cada 250ms
    if (frameAtual == 1) {
      tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_WHITE);
      frameAtual = 0;
    } else {
      tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_BLACK);
      frameAtual = 1;
    }
    // Atualiza o número apenas a cada 1s (4 ciclos de 250ms)
    if (ciclos % 4 == 0 || TotalBombeado != totalAnterior) {
      tft.fillRoundRect(41, 241, 118, 48, 10, TFT_BLACK);  // Limpa a área do número
      tft.drawString(String(int(TotalBombeado)), 145, 265);
      totalAnterior = TotalBombeado;
    }
    ciclos++;
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
  // --- Antes de sair, sempre limpe a seta (caso precise no seu sistema) ---
  tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_BLACK);
  tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_BLACK);
}
void Funcao_Mostrar_Total_Sugado(void* pvParameters) {
  int frameAtual = 1;
  int ciclos = 0;
  int totalAnterior = -1;
  tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_BLACK);
  tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_BLACK);
  tft.setTextFont(4);  // Fonte interna padrão
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextDatum(MR_DATUM);
  while (1) {
    // Pisca seta a cada 250ms
    if (frameAtual == 1) {
      tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_WHITE);
      frameAtual = 0;
      delay(10);
    } else {
      tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_BLACK);
      frameAtual = 1;
      delay(10);
    }
    // Atualiza o número apenas a cada 1s (4 ciclos de 250ms)
    if (ciclos % 4 == 0 || TotalBombeado != totalAnterior) {
      tft.fillRoundRect(41, 241, 118, 48, 10, TFT_BLACK);  // Limpa a área do número
      delay(10);
      tft.drawString(String(int(TotalBombeado)), 145, 265);
      totalAnterior = TotalBombeado;
    }
    ciclos++;
    vTaskDelay(250 / portTICK_PERIOD_MS);
  }
  // --- Antes de sair, sempre limpe a seta (caso precise no seu sistema) ---
  tft.drawBitmap(90, 160, Seta_Cima_45x45, 45, 45, TFT_BLACK);
  tft.drawBitmap(90, 160, Seta_Baixo_45x45, 45, 45, TFT_BLACK);
}
void Tarefa_SincronizaHora(void* pvParameters) {
  for (;;) {
    time_t agora = time(nullptr);
    struct tm* tm_info = localtime(&agora);

    // Se for 4:00 AM e a hora ainda não foi sincronizada neste dia
    static int ultimoDiaSync = -1;
    if (tm_info->tm_hour == 4 && tm_info->tm_mday != ultimoDiaSync) {
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("[SYNC] Sincronizando hora via NTP...");
        configTime(timezoneOffset, 0, "pool.ntp.org");

        // Espera sincronizar
        time_t nowCheck = time(nullptr);
        int tentativas = 0;
        while (nowCheck < 100000 && tentativas < 20) {
          delay(500);
          nowCheck = time(nullptr);
          tentativas++;
        }
        if (nowCheck >= 100000) {
          Serial.println("[SYNC] Falha ao sincronizar hora.");
          horaSincronizada = false;
        } else {
          Serial.println("[SYNC] Hora sincronizada com sucesso.");
          horaSincronizada = true;
          ultimoDiaSync = tm_info->tm_mday;
        }
      } else {
        Serial.println("[SYNC] Sem Wi-Fi. Hora NÃO sincronizada.");
        horaSincronizada = false;
      }
    }
    vTaskDelay(pdMS_TO_TICKS(3600000));  // Checa a cada 1 hora
  }
}
void Executando_Bombeamento_Segundo_Plano(void *param) {
  // Leia o volume na própria Task
  int volume_ml = EEPROM.readInt(POSICAO_DOSAGEM_DIARIA);
  float mlPerRevolutionAjustado = EEPROM.readFloat(POSICAO_ML_PER_REV_AJUSTADO);
  int totalSteps = (volume_ml / mlPerRevolutionAjustado) * stepsPerRevolution;
       
  digitalWrite(enablePin, LOW); // Liga o motor
  digitalWrite(dirPin, HIGH);   // Direção

  float mlBombeado = 0.0;
  float mlBombeadoAnterior = 0.0;
  progressoBombeamento = 0; // zera o progresso no início

  for (int i = 0; i < totalSteps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(600);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(600);

    // Atualiza ml bombeado a cada passo
    mlBombeado += (mlPerRevolutionAjustado / stepsPerRevolution);

        // Quando passar mais 1,5ml, atualiza progresso
        if ((mlBombeado - mlBombeadoAnterior) >= 1.50) {
          int percentual = (int)((mlBombeado * 100.0) / volume_ml);
          if (percentual > 100) percentual = 100;
          if (percentual < 0) percentual = 0;
          progressoBombeamento = percentual;
          mlBombeadoAnterior = mlBombeado;
        }

      if (i % 100 == 0) vTaskDelay(1);  // <--- Pequena pausa a cada 100 passos
  
  }
  digitalWrite(enablePin, HIGH); // Desliga o motor

    // Gerar a data no formato AAMMDD (ex: 250627 para 27/06/2025)
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      uint16_t data = ((timeinfo.tm_year + 1900) % 100) * 10000
                    + (timeinfo.tm_mon + 1) * 100
                    + timeinfo.tm_mday;
      
      DosagemAcumulada = EEPROM.readDouble(POSICAO_DOSAGEM_ACUMULADA);
      DosagemAcumulada += ((double)volume_ml)/1000.00;
      EEPROM.writeDouble(POSICAO_DOSAGEM_ACUMULADA, DosagemAcumulada);
      
      // *** Adicione este bloco ***
      int diaDoAno = timeinfo.tm_yday; // 0-364
      int posicao = POSICAO_HISTORICO_DOSAGEM + (diaDoAno * TAMANHO_REGISTRO_HIST);
      
      
      Serial.print("Dia do ano: ");
      Serial.print(diaDoAno);
      Serial.print(" | Posição: ");
      Serial.print(posicao);
      Serial.print(" | Valor volume_ml: ");
      Serial.println(volume_ml);
            
      EEPROM.writeInt(posicao, volume_ml);
 
      EEPROM.commit();
      pedirTelaPrincipal = true;
    }

  vTaskDelete(NULL); // Termina a Task
}
void checar_e_atualizar_firmware() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setFreeFont(FF1);
  tft.drawString("Verificando nova versao...", 240, 120);

  Serial.printf("Heap antes do OTA: %d\n", ESP.getFreeHeap());

  // 1. Checa versão remota
  HTTPClient http;
  http.begin(URL_VERSION);
  int code = http.GET();
  Serial.print("HTTP GET version.txt: ");
  Serial.println(code);

  if (code == HTTP_CODE_OK) {
    String remoteVersion = http.getString();
    remoteVersion.trim(); // remove espaços e \n

    Serial.printf("Versao remota: [%s], Versao local: [%s]\n", remoteVersion.c_str(), FIRMWARE_VERSION);

    if (remoteVersion != FIRMWARE_VERSION) {
      tft.drawString("Nova versao encontrada!", 240, 150);
      delay(1000);
      tft.drawString("Baixando firmware OTA...", 240, 180);

      // 2. Baixa e atualiza firmware .bin
      http.end();
      HTTPClient httpBin;
      httpBin.begin(binURL);
      int codeBin = httpBin.GET();
      Serial.print("HTTP GET bin: ");
      Serial.println(codeBin);
      if (codeBin == HTTP_CODE_OK) {
        int tamanho = httpBin.getSize();
        WiFiClient* stream = httpBin.getStreamPtr();
        if (Update.begin(tamanho)) {
          Update.writeStream(*stream);
          if (Update.end() && Update.isFinished()) {
            Serial.println("Update OK, reiniciando...");
            tft.drawString("Update OK! Reiniciando...", 240, 210);
            delay(1500);
            ESP.restart();
          } else {
            Serial.println("Erro ao finalizar update");
            tft.drawString("Erro ao finalizar update!", 240, 210);
          }
        } else {
          Serial.println("Erro ao iniciar update");
          tft.drawString("Erro ao iniciar update!", 240, 210);
        }
      } else {
        Serial.println("Erro ao baixar arquivo .bin");
        tft.drawString("Erro ao baixar arquivo!", 240, 210);
      }
      httpBin.end();
    } else {
      Serial.println("Firmware ja esta atualizado.");
      tft.drawString("Firmware ja atualizado!", 240, 180);
      delay(1500);
    }
  } else {
    Serial.println("Erro ao buscar version.txt!");
    tft.drawString("Erro ao buscar versao!", 240, 180);
    delay(1500);
  }
  http.end();
}
void TarefaVerificarNivelCisterna(void *pvParameters) {
  while (1) {
    VerificarNivelCisterna(); // Executa a medição
    vTaskDelay(120000 / portTICK_PERIOD_MS); // Espera 2 minutos
  }
}
void TarefaRegaUnica(void *pvParameters) {
  
  Minutos_Zona_1 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_1);
  Minutos_Zona_2 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_2);
  Minutos_Zona_3 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_3);
  Minutos_Zona_4 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_4);
  Minutos_Zona_5 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_5);
  Minutos_Zona_6 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_6);
  Minutos_Zona_7 = EEPROM.readInt(POSICAO_MINUTOS_ZONA_7);
  mlPerRevolutionAjustado = EEPROM.readFloat(POSICAO_ML_PER_REV_AJUSTADO);

  double volume_litros = (Minutos_Zona_1 + Minutos_Zona_2 + Minutos_Zona_3 +
                          Minutos_Zona_4 + Minutos_Zona_5 + Minutos_Zona_6 +
                          Minutos_Zona_7) * (mlPerRevolutionAjustado / 60.0);

  double litrosBombeados = 0.0; 
  
  if (Minutos_Zona_1 > 0) {
    mcp.digitalWrite(mcp.eGPA3, HIGH);
    Blynk.virtualWrite(V4, 1);  
    vTaskDelay(Minutos_Zona_1 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPA3, LOW);
     Blynk.virtualWrite(V4, 0);  
    litrosBombeados += Minutos_Zona_1 * (mlPerRevolutionAjustado/60);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;    
  }
  
  if (Minutos_Zona_2 > 0) {
    mcp.digitalWrite(mcp.eGPA4, HIGH);
    Blynk.virtualWrite(V8, 1);
    vTaskDelay(Minutos_Zona_2 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPA4, LOW);
    Blynk.virtualWrite(V8, 0);
    litrosBombeados += Minutos_Zona_2 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }
  if (Minutos_Zona_3 > 0) {
    mcp.digitalWrite(mcp.eGPA5, HIGH);
    Blynk.virtualWrite(V2, 1);
    vTaskDelay(Minutos_Zona_3 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPA5, LOW);
    Blynk.virtualWrite(V2, 0);
    litrosBombeados += Minutos_Zona_3 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }
  if (Minutos_Zona_4 > 0) {
    mcp.digitalWrite(mcp.eGPA6, HIGH);
    Blynk.virtualWrite(V7, 1);
    vTaskDelay(Minutos_Zona_4 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPA6, LOW);
    Blynk.virtualWrite(V7, 0);
    litrosBombeados += Minutos_Zona_4 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }
  if (Minutos_Zona_5 > 0) {
    mcp.digitalWrite(mcp.eGPA7, HIGH);
    Blynk.virtualWrite(V5, 1);
    vTaskDelay(Minutos_Zona_5 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPA7, LOW);
    Blynk.virtualWrite(V5, 0);
   litrosBombeados += Minutos_Zona_5 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }
  if (Minutos_Zona_6 > 0) {
    mcp.digitalWrite(mcp.eGPB7, HIGH);
    Blynk.virtualWrite(V3, 1);
    vTaskDelay(Minutos_Zona_6 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPB7, LOW);
    Blynk.virtualWrite(V3, 0);
   litrosBombeados += Minutos_Zona_6 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }
  if (Minutos_Zona_7 > 0) {
    mcp.digitalWrite(mcp.eGPB6, HIGH);
    Blynk.virtualWrite(V6, 1);
    vTaskDelay(Minutos_Zona_7 * 60000 / portTICK_PERIOD_MS);
    mcp.digitalWrite(mcp.eGPB6, LOW);
    Blynk.virtualWrite(V6, 0);
   litrosBombeados += Minutos_Zona_7 * (mlPerRevolutionAjustado/60.0);
    int percentual = (int)((litrosBombeados * 100.0) / volume_litros);
    if (percentual > 100) percentual = 100;
    if (percentual < 0) percentual = 0;
    progressoBombeamento = percentual;
    
  }

   // Gerar a data no formato AAMMDD (ex: 250627 para 27/06/2025)
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
      uint16_t data = ((timeinfo.tm_year + 1900) % 100) * 10000
                    + (timeinfo.tm_mon + 1) * 100
                    + timeinfo.tm_mday;
      
      DosagemAcumulada = EEPROM.readDouble(POSICAO_DOSAGEM_ACUMULADA);
      DosagemAcumulada += ((double)litrosBombeados);
      EEPROM.writeDouble(POSICAO_DOSAGEM_ACUMULADA, DosagemAcumulada);
      
      // *** Adicione este bloco ***
      int diaDoAno = timeinfo.tm_yday; // 0-364
      int posicao = POSICAO_HISTORICO_DOSAGEM + (diaDoAno * TAMANHO_REGISTRO_HIST);
      
      
      Serial.print("Dia do ano: ");
      Serial.print(diaDoAno);
      Serial.print(" | Posição: ");
      Serial.print(posicao);
      Serial.print(" | Valor volume Lt: ");
      Serial.println(litrosBombeados);
            
      EEPROM.writeInt(posicao, litrosBombeados);
 
      EEPROM.commit();
    }
    
    pedirTelaPrincipal = true;  
    if (handleNivelCisterna != NULL) vTaskResume(handleNivelCisterna);
  
  vTaskDelete(NULL);
}






//***********************************************************************************************  EM CONSTRUCAO    *************************************************************











