# ESPotify - Display de Música Spotify para ESP8266/ESP32

Sistema embarcado para exibição em tempo real da música atualmente em reprodução no Spotify, utilizando microcontroladores ESP8266 ou ESP32 com display OLED.

## Descrição

Este projeto permite visualizar informações da música que está sendo reproduzida na sua conta do Spotify em um display OLED 128x32. São exibidos o nome da faixa, artista e uma barra de progresso da reprodução.

## Requisitos de Hardware

- Microcontrolador ESP8266 (NodeMCU v2) ou ESP32
- Display OLED SSD1306 128x32 pixels (interface I2C)
- Cabos de conexão

### Esquema de Conexão (I2C)

| Display OLED | ESP8266/ESP32 |
|--------------|---------------|
| VCC          | 3.3V          |
| GND          | GND           |
| SDA          | GPIO4 (D2)    |
| SCL          | GPIO5 (D1)    |

## Requisitos de Software

- PlatformIO IDE ou Arduino IDE
- Conta de desenvolvedor Spotify

### Bibliotecas Utilizadas

- SpotifyArduino - Comunicação com API do Spotify
- ArduinoJson - Parsing de respostas JSON
- Adafruit SSD1306 - Controle do display OLED
- Adafruit GFX - Biblioteca gráfica

## Configuração

### 1. Criar Aplicativo no Spotify Developer

1. Acesse https://developer.spotify.com/dashboard
2. Faça login com sua conta Spotify
3. Clique em "Create App"
4. Preencha os dados do aplicativo
5. Em "Redirect URIs", adicione:
   - Para uso com IP: `http://[IP_DO_ESP]/callback/`
   - Para uso com mDNS: `http://arduino.local/callback/`
6. Anote o Client ID e Client Secret

### 2. Configurar Credenciais

1. Copie o arquivo de exemplo:
   ```
   cp include/credentials_example.h include/credentials.h
   ```

2. Edite o arquivo `include/credentials.h` com suas credenciais:
   ```cpp
   #define WIFI_SSID "nome_da_sua_rede"
   #define WIFI_PASSWORD "senha_da_sua_rede"
   #define SPOTIFY_CLIENT_ID "seu_client_id"
   #define SPOTIFY_CLIENT_SECRET "seu_client_secret"
   #define SPOTIFY_REFRESH_TOKEN "seu_refresh_token"
   ```

### 3. Obter Refresh Token

O Refresh Token é necessário para autenticação com a API do Spotify.

1. Abra o arquivo `include/getRefreshToken.cpp`
2. Configure suas credenciais WiFi e Spotify
3. Compile e faça upload para o ESP
4. Abra o Serial Monitor (115200 baud)
5. Anote o endereço IP exibido
6. Acesse o IP pelo navegador
7. Clique no link de autorização
8. Autorize o aplicativo na página do Spotify
9. O Refresh Token será exibido na tela e no Serial Monitor
10. Copie o token para o arquivo de credenciais

### 4. Compilar e Fazer Upload

Usando PlatformIO:
```
pio run -t upload
```

Usando Arduino IDE:
1. Instale as bibliotecas necessárias pelo Library Manager
2. Selecione a placa correta (NodeMCU 1.0 ou ESP32 Dev Module)
3. Compile e faça upload

## Estrutura do Projeto

```
ESPotify/
├── src/
│   └── main.cpp              # Código principal
├── include/
│   ├── credentials_example.h # Template de credenciais
│   ├── credentials.h         # Credenciais (não versionado)
│   └── getRefreshToken.cpp   # Utilitário para obter token
├── lib/
│   ├── spotify-api-arduino/  # Biblioteca Spotify
│   ├── ArduinoJson-6.x/      # Biblioteca JSON
│   └── ...                   # Outras bibliotecas
├── platformio.ini            # Configuração PlatformIO
└── README.md                 # Este arquivo
```

## Configurações Avançadas

### Alterar Intervalo de Atualização

No arquivo `src/main.cpp`, modifique a variável:
```cpp
unsigned long delayBetweenRequests = 5000; // Valor em milissegundos
```

Valores recomendados:
- 5000 (5 segundos) - Atualização frequente
- 10000 (10 segundos) - Balanço entre atualização e consumo
- 30000 (30 segundos) - Economia de requisições

### Habilitar Debug

Para depuração, descomente a linha no arquivo `lib/spotify-api-arduino/src/SpotifyArduino.h`:
```cpp
#define SPOTIFY_DEBUG 1
```

Atenção: O modo debug exibe tokens no Serial Monitor. Não utilize em ambientes não seguros.

### Usar ESP32

No arquivo `platformio.ini`, comente a seção do ESP8266 e descomente a seção do ESP32:
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

## Solução de Problemas

### Erro "Falha ao obter os tokens de acesso"

- Verifique se o Client ID e Client Secret estão corretos
- Confirme se o Refresh Token é válido
- Verifique a conexão com a internet

### Display não liga

- Confirme as conexões I2C
- Verifique se o endereço I2C está correto (padrão: 0x3C)
- Teste com um scanner I2C para confirmar o endereço

### Não conecta ao WiFi

- Verifique o nome da rede e senha
- Confirme se a rede é 2.4GHz (ESP8266 não suporta 5GHz)
- Aproxime o dispositivo do roteador

### Erro 429 (Too Many Requests)

- Aumente o intervalo entre requisições
- Aguarde alguns minutos antes de tentar novamente

## Limitações

- O ESP8266 possui memória limitada, podendo apresentar instabilidade com uso prolongado
- A API do Spotify possui limite de requisições por minuto
- Certificados SSL podem expirar e necessitar atualização

## Licença

Este projeto utiliza a biblioteca SpotifyArduino, licenciada sob LGPL 2.1.

## Referências

- Documentação da API do Spotify: https://developer.spotify.com/documentation/web-api
- Biblioteca SpotifyArduino: https://github.com/witnessmenow/spotify-api-arduino
- Documentação PlatformIO: https://docs.platformio.org

## Autor

Projeto baseado na biblioteca SpotifyArduino de Brian Lough.