/*********
EJEMPLO GET REQUEST FOR INPUT ESP32.
*********/

#include <Arduino.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <AsyncTCP.h>
  #include <SPIFFS.h>
  #include <FS.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>


AsyncWebServer server(80);
//User y password wifi AP
const char* userAdmin = "admin";
const char* passwordAdmin = "admin";

// REPLACE WITH YOUR NETWORK CREDENTIALS
const char* ssid = "NOMBRE_RED_WIFI";
const char* password = "PASSWORD_RED_WIFI";
// CREAR ACCESS POINT PARA LA CONFIGURACIÓN DE RED WIFI
const char* ssidAP     = "ESP32-Access-Point";
const char* passwordAP = "123456789";

const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";

void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}

void setup() {
    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    if (!SPIFFS.begin()) {
      Serial.println("SPIFFS failed to mount !\r\n");
    }
    String myFile = "/index.html";

    if(SPIFFS.exists(myFile)) {
      Serial.println(F("myFile founded on   SPIFFS"));   //ok
      File file = SPIFFS.open(myFile, "r");
      Serial.println(F("mi archivo"));   //ok
    }
    WiFi.softAP(ssidAP, passwordAP);
    IPAddress IP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(IP);

  // Enviar pagina login al navegador
   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(SPIFFS, "/index.html", "text/html");
    });
  // Cwrgar estilo de login localmente
   server.on("/style1", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/all.min.css", "text/css");
  });
  // Cwrgar estilo de login localmente
  server.on("/style2", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.min.css", "text/css");
    });  
  
  // Cargar estilo de login localmente
   server.on("/style3", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/bootstrap.bundle.min.js", "text/js");
  });
  // Cargar logo login
   server.on("/logo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/logo.png", "image/png");
    });
 // Cargar logo tablesa
 server.on("/logo_tablesa", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send(SPIFFS, "/logo_tablesa.png", "image/png");
  });
  

  // Send a GET request to <ESP_IP>/get?input1=<inputMessage>
 server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request){  
    String inputMessage1;
    String inputMessage2;
    inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
    if(inputMessage1.equals(userAdmin) && inputMessage2.equals(passwordAdmin) ){
      String formulario =                                        //Construcción del formulario HTML de respuesta en cadena de texto
     "<!DOCTYPE HTML>"
      "<html>"
      "<style>"
          "body {"
        "background: #0080bb;"
        "background: linear-gradient(to right, #0080bb, #33AEFF);"
      "}"
      "@media (min-width: 1400px)"
      ".container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl" 
      "{"
        "zoom: 0;"
       "}"
       "@media (min-width: 768px)"
      ".container, .container-md, .container-sm" 
      "{"
        "zoom: 2;"
      "}"
      ".btn-login {"
        "  font-size: 0.9rem;"
        " letter-spacing: 0.05rem;"
        " padding: 0.75rem 1rem;"
        "}"
        ".btn-google {"
          "color: white !important;"
          " background-color: #ea4335;"
          "}"
      ".btn-facebook {"
        "color: white !important;"
        "background-color: #0080bb;"
        "}"
      ".styleLogo{"
          "width:25rem;"
      "}"
      ".styleLogoTablesa"
      "{"
          "max-width: 14%;"
      "}"
      ".margin-img"
      "{"
          "text-align: end;"
       "}"
      
      "</style>"
      "<head>"
            "<link rel='stylesheet' href='style1'/>"
            "<link rel='stylesheet' href='style2'/>"
            "<link rel='stylesheet' href='style3'/>"
            "<title>CONFIGURACI&Oacute;N</title>"
          "</head>"
      "<body>"
        "<div class='container'>"
          "<div class='row'>"
            "<div class='col-sm-9 col-md-12 col-lg-5 mx-auto'>"
              "<div class='card border-0 shadow rounded-3 my-5'>"
                "<div class='card-body p-4 p-sm-5'>"
                  "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N DE RED WIFI</h5>"
                  "<form  action='/connection'>"
                "<div class='form-floating mb-3'>"
                "<input name='input1' type='text' class='form-control' id='floatingInput'  placeholder='Nombre red' required='true'>"
                "<label for='floatingInput'>'Escriba el nombre de la red wifi'</label>"
              "</div>"
              "<div class='form-floating mb-3'>"
                "<input name='input2' type='password' class='form-control' id='floatingPassword' placeholder='Password' required='true'>"
                "<label for='floatingPassword'>Constrase&ntilde;a</label>"
              "</div>" 
                "<div>"
                "<label for='mac'>"
                "<strong>MAC: </strong>"
                + WiFi.macAddress() + 
                "</label>"
                "</div>"
              "<div class='d-grid mt-3'>"
                "<button class='btn btn-primary btn-login text-uppercase fw-bold' type='submit'>Conectar</button>"
              "</div>"
            "</form>"
            "<div class='col-md-12 mt-4 margin-img'>"
               "<small>Powered by </small><img src='logo_tablesa' class='styleLogoTablesa'>"
            "</div>"
          "</div>"
        "</div>"
      "</div>"
    "</div>"
  "</div>"
"</body>"
"</html>";                                               //Fin del documento HTML
    request->send(200, "text/html", formulario);  
  }else{
    int IPLocal = WiFi.softAPIP();;
    String IPRedirect = inet_ntoa(IPLocal);
    request->send(404, "text/html", "<script>alert ('Usuario y/o Contrasenia incorrectos');window.location.href='http://"+IPRedirect+"'</script>");
  }
  });

//Tercer documento html para mostrar el mensaje de conectado exitosamente o error al conectar con la red wifi
 server.on("/connection", HTTP_GET, [] (AsyncWebServerRequest *request){  
    const char* nombreRedWifi;
    const char* passwordRedWifi;
    
    String inputMessage1;
    String inputMessage2;
    inputMessage1 = request->getParam(PARAM_INPUT_1)->value();
    inputMessage2 = request->getParam(PARAM_INPUT_2)->value();

    nombreRedWifi = inputMessage1.c_str();
    passwordRedWifi = inputMessage2.c_str();
    
    WiFi.begin(nombreRedWifi, passwordRedWifi);
    if(WiFi.waitForConnectResult() == WL_CONNECTED) {
       Serial.println("Conexión exitosa");
      Serial.print("IP Address: ");
      Serial.println(WiFi.localIP());
      int IPLocal = WiFi.localIP();
      String IPRedirect = inet_ntoa(IPLocal);
      String formulario =                                        //Construcción del formulario HTML de respuesta en cadena de texto
         "<!DOCTYPE HTML>"
          "<html>"
          "<style>"
              "body {"
            "background: #0080bb;"
            "background: linear-gradient(to right, #0080bb, #33AEFF);"
          "}"
          "@media (min-width: 1400px)"
          ".container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl" 
          "{"
            "zoom: 0;"
           "}"
           "@media (min-width: 768px)"
          ".container, .container-md, .container-sm" 
          "{"
            "zoom: 2;"
          "}"
          ".btn-login {"
            "  font-size: 0.9rem;"
            " letter-spacing: 0.05rem;"
            " padding: 0.75rem 1rem;"
            "}"
            ".btn-google {"
              "color: white !important;"
              " background-color: #ea4335;"
              "}"
          ".btn-facebook {"
            "color: white !important;"
            "background-color: #0080bb;"
            "}"
          ".styleLogo{"
              "width:25rem;"
          "}"
          ".styleLogoTablesa"
          "{"
              "max-width: 14%;"
          "}"
          ".margin-img"
          "{"
              "text-align: end;"
           "}"
          
          "</style>"
          "<head>"
                "<link rel='stylesheet' href='style1'/>"
                "<link rel='stylesheet' href='style2'/>"
                "<link rel='stylesheet' href='style3'/>"
                "<title>CONFIGURACI&Oacute;N</title>"
              "</head>"
          "<body>"
            "<div class='container'>"
              "<div class='row'>"
                "<div class='col-sm-9 col-md-12 col-lg-5 mx-auto'>"
                  "<div class='card border-0 shadow rounded-3 my-5'>"
                    "<div class='card-body p-4 p-sm-5'>"
                      "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N DE RED WIFI</h5>"
                      "<h5 style='text-align:center' class='text-success mt-4'>"; 
                  String formulario2 = 
                      "</h5>"
                      "<div class='col-md-12 mt-4 margin-img'>"
                      "<small>Powered by </small><img src='logo_tablesa' class='styleLogoTablesa'>"
                    "</div>"
                  "</div>"
                "</div>"
              "</div>"
            "</div>"
          "</div>"
        "</body>"
      "</html>";
   request->send(200, "text/html", formulario+"Conexi&oacute;n exitosa, el LED debe estar en color verde, por favor verifique, ya puede puede cerrar esta ventana."+formulario2); 
    }
    else{
     Serial.println("WiFi Connect Failed!");
     delay(100);
     String formulario =                                        //Construcción del formulario HTML de respuesta en cadena de texto
         "<!DOCTYPE HTML>"
          "<html>"
          "<style>"
              "body {"
            "background: #0080bb;"
            "background: linear-gradient(to right, #0080bb, #33AEFF);"
          "}"
          "@media (min-width: 1400px)"
          ".container, .container-lg, .container-md, .container-sm, .container-xl, .container-xxl" 
          "{"
            "zoom: 0;"
           "}"
           "@media (min-width: 768px)"
          ".container, .container-md, .container-sm" 
          "{"
            "zoom: 2;"
          "}"
          ".btn-login {"
            "  font-size: 0.9rem;"
            " letter-spacing: 0.05rem;"
            " padding: 0.75rem 1rem;"
            "}"
            ".btn-google {"
              "color: white !important;"
              " background-color: #ea4335;"
              "}"
          ".btn-facebook {"
            "color: white !important;"
            "background-color: #0080bb;"
            "}"
          ".styleLogo{"
              "width:25rem;"
          "}"
          ".styleLogoTablesa"
          "{"
              "max-width: 14%;"
          "}"
          ".margin-img"
          "{"
              "text-align: end;"
           "}"
          
          "</style>"
          "<head>"
                "<link rel='stylesheet' href='style1'/>"
                "<link rel='stylesheet' href='style2'/>"
                "<link rel='stylesheet' href='style3'/>"
                "<title>CONFIGURACI&Oacute;N</title>"
              "</head>"
          "<body>"
            "<div class='container'>"
              "<div class='row'>"
                "<div class='col-sm-9 col-md-12 col-lg-5 mx-auto'>"
                  "<div class='card border-0 shadow rounded-3 my-5'>"
                    "<div class='card-body p-4 p-sm-5'>"
                      "<h5 class='card-title text-center mb-5 fw-light fs-5'>CONFIGURACI&Oacute;N DE RED WIFI</h5>"
                      "<form  action='/connection'>"
                    "<div class='form-floating mb-3'>"
                    "<input name='input1' type='text' class='form-control' id='floatingInput'  placeholder='Nombre red' required='true'>"
                    "<label for='floatingInput'>'Escriba el nombre de la red wifi'</label>"
                  "</div>"
                  "<div class='form-floating mb-3'>"
                    "<input name='input2' type='password' class='form-control' id='floatingPassword' placeholder='Password' required='true'>"
                    "<label for='floatingPassword'>Constrase&ntilde;a</label>"
                  "</div>" 
                    "<div>"
                    "<label for='mac'>"
                    "<strong>MAC: </strong>"
                    + WiFi.macAddress() + 
                    "</label>"
                    "</div>"
                  "<div class='d-grid mt-3'>"
                    "<button class='btn btn-primary btn-login text-uppercase fw-bold' type='submit'>Conectar</button>"
                  "</div>"
                "</form>"
                "<h5 style='text-align:center' class='text-danger mt-4'>"; 
                String formulario2 = 
                "</h5>"
                "<div class='col-md-12 mt-4 margin-img'>"
                   "<small>Powered by </small><img src='logo_tablesa' class='styleLogoTablesa'>"
                "</div>"
              "</div>"
            "</div>"
          "</div>"
        "</div>"
      "</div>"
    "</body>"
    "</html>";
    inputMessage1 = "";
    inputMessage2 = "";
   request->send(404, "text/html", formulario+"Error al conectar con red wifi, intente nuevamente"+formulario2); 
   }
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {}
