void initWiFi() {
  WiFi.begin(ssid, pass);
  
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print("."); 
  }
  
  Serial.print("\nIP Address: ");
  Serial.println(WiFi.localIP());
}


//looking if it's connected.
void initIoTHub() {
  if (!Esp32MQTTClient_Init((const uint8_t *) connectionString)){
    _connected = false;
    return;
  }
  _connected = true; 
}
