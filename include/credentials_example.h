/*
 * INSTRUÇÕES DE CONFIGURAÇÃO:
 * 
 * 1. Copie este arquivo para "credentials.h"
 * 2. Preencha suas credenciais reais
 * 3. NUNCA commite o arquivo credentials.h no git!
 * 
 * Para obter as credenciais do Spotify:
 * 1. Acesse https://developer.spotify.com/dashboard
 * 2. Crie um novo aplicativo
 * 3. Copie o Client ID e Client Secret
 * 4. Em "Edit Settings", adicione o Redirect URI:
 *    - Para IP: http://[IP_DO_ESP]/callback/
 *    - Para mDNS: http://arduino.local/callback/
 * 5. Use o getRefreshToken.cpp para obter o Refresh Token
 */

#ifndef CREDENTIALS_H
#define CREDENTIALS_H

// ===== CONFIGURAÇÕES DE WIFI =====
#define WIFI_SSID "SEU_WIFI_SSID"
#define WIFI_PASSWORD "SUA_SENHA_WIFI"

// ===== CONFIGURAÇÕES DO SPOTIFY =====
#define SPOTIFY_CLIENT_ID "SEU_CLIENT_ID_AQUI"
#define SPOTIFY_CLIENT_SECRET "SEU_CLIENT_SECRET_AQUI"
#define SPOTIFY_REFRESH_TOKEN "SEU_REFRESH_TOKEN_AQUI"

// ===== CONFIGURAÇÕES REGIONAIS =====
#define SPOTIFY_MARKET "BR"

#endif // CREDENTIALS_H