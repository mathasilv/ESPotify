/*******************************************************************
    Imprime informações sobre o dispositivo Spotify ativo atualmente
    no monitor serial usando um ESP32 ou ESP8266

    OBSERVAÇÃO: É necessário obter um token de atualização para usar este exemplo
    Use o exemplo "getRefreshToken" para obtê-lo.

    Placas compatíveis:
      - Qualquer placa ESP8266 ou ESP32
    Componentes:
    Placa de desenvolvimento estilo ESP32 D1 Mini* - http://s.click.aliexpress.com/e/C6ds4my

 *  * = Afiliado

    Se você achar o que faço útil e gostaria de me apoiar,
    considere se tornar um patrocinador no Github
    https://github.com/sponsors/witnessmenow/


    Escrito por Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Bibliotecas padrão
// ----------------------------

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif

#include <WiFiClientSecure.h>

// ----------------------------
// Bibliotecas adicionais - cada uma delas precisará ser instalada.
// ----------------------------

#include <SpotifyArduino.h>
// Biblioteca para conectar-se à API do Spotify

// Instale do Github
// https://github.com/witnessmenow/spotify-api-arduino

// Inclui uma variável "spotify_server_cert"
// O cabeçalho é incluído como parte da biblioteca SpotifyArduino
#include <SpotifyArduinoCert.h>

#include <ArduinoJson.h>
// Biblioteca usada para fazer o parse de JSON das respostas da API

// Pesquise por "Arduino Json" no Gerenciador de Bibliotecas do Arduino
// https://github.com/bblanchon/ArduinoJson

//------- Substitua o seguinte! ------

char ssid[] = "MATHEUS ";         // your network SSID (name)
char password[] = "12213490"; // your network password

char clientId[] = "68933654dc544df38ece84a8e49faeca";     // Your client ID of your spotify APP
char clientSecret[] = "e5122fc865b1482881cbe85288f632d0"; // Your client Secret of your spotify APP (Do Not share this!)

// Country code, including this is advisable
#define SPOTIFY_MARKET "BR" // Country code, including this is advisable

#define SPOTIFY_REFRESH_TOKEN "AQA_QXYzJVrdmmcTOv2y2SwZtyhGp8AfHrX4Y--bJUkhkLtpTcHElaioffY5kiXdTkygAc6u6aa9fv5KQ58riPQzqvrK4A-Wj7ouam3BxxN2N8euUoNGc2vsjMc87OoMWfw"

//-----------------------------------

WiFiClientSecure client;
SpotifyArduino spotify(client, clientId, clientSecret, SPOTIFY_REFRESH_TOKEN);

unsigned long delayBetweenRequests = 60000; // Tempo entre as solicitações (1 minuto)
unsigned long requestDueTime;               // Hora da próxima solicitação

void setup()
{

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Aguarda a conexão
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

    // Lida com a verificação HTTPS
#if defined(ESP8266)
    client.setFingerprint(SPOTIFY_FINGERPRINT); // Estas expiram a cada alguns meses
#elif defined(ESP32)
    client.setCACert(spotify_server_cert);
#endif
    // ... ou não!
    //client.setInsecure();

    // Se você quiser ativar uma depuração extra
    // descomente a linha "#define SPOTIFY_DEBUG" em SpotifyArduino.h

    Serial.println("Atualizando os Tokens de Acesso");
    if (!spotify.refreshAccessToken())
    {
        Serial.println("Falha ao obter os tokens de acesso");
    }
}

void printPlayerDetailsToSerial(PlayerDetails playerDetails)
{
    Serial.println("--------- Detalhes do Player ---------");

    Serial.print("ID do Dispositivo: ");
    Serial.println(playerDetails.device.id);

    Serial.print("Nome do Dispositivo: ");
    Serial.println(playerDetails.device.name);

    Serial.print("Tipo do Dispositivo: ");
    Serial.println(playerDetails.device.type);

    Serial.print("Está Ativo: ");
    if (playerDetails.device.isActive)
    {
        Serial.println("Sim");
    }
    else
    {
        Serial.println("Não");
    }

    Serial.print("Está Restrito: ");
    if (playerDetails.device.isRestricted)
    {
        Serial.println("Sim, de acordo com a documentação da API, \"nenhum comando da Web API será aceito por este dispositivo\"");
    }
    else
    {
        Serial.println("Não");
    }

    Serial.print("Sessão Privada: ");
    if (playerDetails.device.isPrivateSession)
    {
        Serial.println("Sim");
    }
    else
    {
        Serial.println("Não");
    }

    Serial.print("Volume (percentual): ");
    Serial.println(playerDetails.device.volumePercent);

    Serial.print("Progresso (ms): ");
    Serial.println(playerDetails.progressMs);

    Serial.print("Está Tocando: ");
    if (playerDetails.isPlaying)
    {
        Serial.println("Sim");
    }
    else
    {
        Serial.println("Não");
    }

    Serial.print("Modo Aleatório: ");
    if (playerDetails.shuffleState)
    {
        Serial.println("Ligado");
    }
    else
    {
        Serial.println("Desligado");
    }

    Serial.print("Modo de Repetição: ");
    switch (playerDetails.repeateState)
    {
    case repeat_track:
        Serial.println("faixa");
        break;
    case repeat_context:
        Serial.println("contexto");
        break;
    case repeat_off:
        Serial.println("desativado");
        break;
    }

    Serial.println("------------------------");
}

void loop()
{
    if (millis() > requestDueTime)
    {
        Serial.print("Heap Livre: ");
        Serial.println(ESP.getFreeHeap());

        Serial.println("Obtendo detalhes do player:");
        // O mercado pode ser excluído se você quiser, por exemplo, spotify.getPlayerDetails()
        int status = spotify.getPlayerDetails(printPlayerDetailsToSerial, SPOTIFY_MARKET);
        if (status == 200)
        {
            Serial.println("Detalhes do player obtidos com sucesso");
        }
        else if (status == 204)
        {
            Serial.println("Nenhum player ativo?");
        }
        else
        {
            Serial.print("Erro: ");
            Serial.println(status);
        }

        requestDueTime = millis() + delayBetweenRequests;
    }
}
