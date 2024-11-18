#include <WiFi.h>
#include <HTTPClient.h>
#include <DFRobotDFPlayerMini.h>
#include <SoftwareSerial.h>

// Configuración de red Wi-Fi
const char* ssid = "TU_SSID";
const char* password = "TU_PASSWORD";

// Clave de la API de OpenAI
const String apiKey = "TU_API_KEY";

// Configuración del DFPlayer Mini
SoftwareSerial mySerial(16, 17); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

// Inicializar variables
String prompt = "Explícame la ley de Newton";
String openaiResponse;

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600);

  // Configurar DFPlayer Mini
  if (!myDFPlayer.begin(mySerial)) {
    Serial.println("Error al inicializar el DFPlayer");
    while (true);
  }
  myDFPlayer.volume(25); // Ajustar volumen (0-30)

  // Conectar a Wi-Fi
  conectarWiFi();
}

void loop() {
  // Obtener respuesta desde OpenAI
  openaiResponse = obtenerRespuesta(prompt);
  
  // Reproducir la respuesta en voz alta
  if (openaiResponse != "") {
    Serial.println("Respuesta recibida: " + openaiResponse);
    reproducirTexto(openaiResponse);
  }
  
  delay(10000); // Esperar antes de la siguiente interacción
}

void conectarWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Conectando a Wi-Fi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("\nConectado a la red Wi-Fi");
}

String obtenerRespuesta(String prompt) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("https://api.openai.com/v1/chat/completions");
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", "Bearer " + apiKey);

    // Formato del mensaje para la API
    String requestBody = "{\"model\": \"gpt-4\", \"messages\": [{\"role\": \"system\", \"content\": \"Eres un asistente que explica conceptos de física.\"}, {\"role\": \"user\", \"content\": \"" + prompt + "\"}]}";

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      int startIndex = response.indexOf("\"content\":\"") + 11;
      int endIndex = response.indexOf("\"", startIndex);
      String reply = response.substring(startIndex, endIndex);
      return reply;
    } else {
      Serial.println("Error en la solicitud: " + String(httpResponseCode));
    }

    http.end();
  }
  return "";
}

void reproducirTexto(String texto) {
  // Aquí deberás convertir el texto en audio utilizando un módulo TTS.
  // Si usas DFPlayer, asegúrate de tener audios pregrabados con las respuestas más comunes.
  myDFPlayer.play(1); // Esto es solo un ejemplo; necesitarás un módulo TTS para leer texto dinámicamente.
}

