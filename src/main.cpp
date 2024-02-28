
    

// ----------------------------
// Bibliotecas Padrão
// ----------------------------

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

// ----------------------------
// Bibliotecas Adicionais - cada uma delas precisará ser instalada.
// ----------------------------

#include <SpotifyArduino.h>
// Biblioteca para conexão com a API do Spotify

// Instale do Github
// https://github.com/witnessmenow/spotify-api-arduino

// incluindo a variável "spotify_server_cert"
// o cabeçalho está incluído como parte da biblioteca SpotifyArduino
#include <SpotifyArduinoCert.h>

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
// Biblioteca para controle do display OLED

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//------- Substitua o seguinte! ------

char ssid[] = "MATHEUS ";         // SSID da sua rede (nome)
char password[] = "12213490"; // senha da sua rede

char clientId[] = "";     // Your client ID of your spotify APP
char clientSecret[] = ""; // Your client Secret of your spotify APP (Do Not share this!)

// Country code, including this is advisable
#define SPOTIFY_MARKET "BR"

#define SPOTIFY_REFRESH_TOKEN ""

//------- ---------------------- ------

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 100; // Tempo entre as solicitações (1 minuto)
unsigned long requestDueTime;               // hora em que a solicitação é devida

void setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Aguardar conexão
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Conectado a ");
    Serial.println(ssid);
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    // Lidar com a verificação HTTPS
#if defined(ESP8266)
    client.setFingerprint(SPOTIFY_FINGERPRINT); // Estas expiram a cada poucos meses
#elif defined(ESP32)
    client.setCACert(spotify_server_cert);
#endif
    // ... ou não!
    //client.setInsecure();

    // Se você deseja habilitar algumas depurações extras
    // descomente a linha "#define SPOTIFY_DEBUG" em SpotifyArduino.h

    Serial.println("Atualizando Tokens de Acesso");
    if (!spotify.refreshAccessToken())
    {
        Serial.println("Falha ao obter os tokens de acesso");
    }
}

void printCurrentlyPlayingToSerial(CurrentlyPlaying currentlyPlaying)
{
    // Use os detalhes neste método ou, se você quiser armazená-los
    // certifique-se de copiá-los (usando algo como strncpy)
    // const char* artist =

    Serial.println("--------- Reprodução Atual ---------");

    Serial.print("Está reproduzindo: ");
    if (currentlyPlaying.isPlaying)
    {
        Serial.println("Sim");
    }
    else
    {
        Serial.println("Não");
    }

    Serial.print("Faixa: ");
    Serial.println(currentlyPlaying.trackName);
    Serial.print("URI da Faixa: ");
    Serial.println(currentlyPlaying.trackUri);
    Serial.println();

    Serial.println("Artistas: ");
    for (int i = 0; i < currentlyPlaying.numArtists; i++)
    {
        Serial.print("Nome: ");
        Serial.println(currentlyPlaying.artists[i].artistName);
        Serial.print("URI do Artista: ");
        Serial.println(currentlyPlaying.artists[i].artistUri);
        Serial.println();
    }

    Serial.print("Álbum: ");
    Serial.println(currentlyPlaying.albumName);
    Serial.print("URI do Álbum: ");
    Serial.println(currentlyPlaying.albumUri);
    Serial.println();

    if (currentlyPlaying.contextUri != NULL)
    {
        Serial.print("URI do Contexto: ");
        Serial.println(currentlyPlaying.contextUri);
        Serial.println();
    }

    long progress = currentlyPlaying.progressMs; // duração passada na música
    long duration = currentlyPlaying.durationMs; // Duração da música
    Serial.print("Tempo decorrido da música (ms): ");
    Serial.print(progress);
    Serial.print(" de ");
    Serial.println(duration);
    Serial.println();

    float percentage = ((float)progress / (float)duration) * 100;
    int clampedPercentage = (int)percentage;
    Serial.print("<");
    for (int j = 0; j < 50; j++)
    {
        if (clampedPercentage >= (j * 2))
        {
            Serial.print("=");
        }
        else
        {
            Serial.print("-");
        }
    }
    Serial.println(">");
    Serial.println();

    // estará na ordem do mais largo para o mais estreito
    // currentlyPlaying.numImages é o número de imagens que
    // estão armazenadas
    for (int i = 0; i < currentlyPlaying.numImages; i++)
    {
        Serial.println("------------------------");
        Serial.print("Imagem do Álbum: ");
        Serial.println(currentlyPlaying.albumImages[i].url);
        Serial.print("Dimensões: ");
        Serial.print(currentlyPlaying.albumImages[i].width);
        Serial.print(" x ");
        Serial.print(currentlyPlaying.albumImages[i].height);
        Serial.println();
    }
    Serial.println("------------------------");

display.clearDisplay();

  // Exibe o nome da música
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println(currentlyPlaying.trackName);

  // Exibe o nome do artista
  display.setTextSize(1);
  display.setCursor(0, 20);
  display.println(currentlyPlaying.artists[0].artistName);

    //display.drawLine(0, 30, SCREEN_WIDTH, 30, SSD1306_WHITE);
    
    long progress1 = currentlyPlaying.progressMs;
    long duration1 = currentlyPlaying.durationMs;
    int progressBarWidth = map(progress, 0, duration, 0, SCREEN_WIDTH);

    display.fillRect(0, 31, progressBarWidth, 1, SSD1306_WHITE);

    display.display();
}
void loop()
{
    if (millis() > requestDueTime)
    {
        Serial.print("Heap Livre: ");
        Serial.println(ESP.getFreeHeap());

        Serial.println("obtendo a música atualmente reproduzida:");
        // O mercado pode ser excluído se você quiser, por exemplo, spotify.getCurrentlyPlaying()
        int status = spotify.getCurrentlyPlaying(printCurrentlyPlayingToSerial, SPOTIFY_MARKET);
        if (status == 200)
        {
            Serial.println("Obtido com sucesso a música atualmente reproduzida");
        }
        else if (status == 204)
        {
            Serial.println("Não parece ter nada sendo reproduzido");
        }
        else
        {
            Serial.print("Erro: ");
            Serial.println(status);
        }
        requestDueTime = millis() + delayBetweenRequests;
    }
}
