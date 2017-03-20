ESP8266WebServer web_server(80);

void webServerSendFile(String filename) {
  File f = SPIFFS.open("/" + filename, "r");
  String data = f.readString();

  web_server.send(200, "text/html", data);
}

void handleRoot() {
  webServerSendFile("index.html");
}

//void set_angle(float target_angle) {
//
//  nana.st();
//
//  float start_angle = getHeading();
//
//  unsigned long start = millis();
//
//  if (((int)(start_angle - target_angle + 360) % 360) > 180) {
//    nana.turnaround_left();
//    while (((int)(start_angle - target_angle + 360) % 360) <= ((int)(getHeading() - target_angle + 360) % 360) && millis() - start <= 3000) {
//      delay(3);
//    }
//  } else {
//    nana.turnaround_right();
//    while (((int)(start_angle - target_angle + 360) % 360) >= ((int)(getHeading() - target_angle + 360) % 360) && millis() - start <= 3000) {
//      delay(3);
//    }
//  }
//
//  nana.st();
//}

long dist_front() {
  long duration;
  int dist;


  digitalWrite(sonar_front_trig, LOW);
  delayMicroseconds(2);
  digitalWrite(sonar_front_trig, HIGH);
  delayMicroseconds(5);
  digitalWrite(sonar_front_trig, LOW);

  duration = pulseIn(sonar_front_echo, HIGH);
  dist = duration / 58.138;

  return dist;
}

float select_course() {

  nana.drive(0, 0);

///

  nana.drive(0, 0);
}

void a() {
  nana.setDriveMode(1);
}

void left() {


  nana.drive(-4095, 4095);

  web_server.send(200, "text/html", "ok");
}

void right() {


  nana.drive(4095, -4095);

  web_server.send(200, "text/html", "ok");
}

void up() {

  nana.drive(4095, 4095);

  web_server.send(200, "text/html", "ok");
}

void down() {


  nana.drive(-4095, -4095);

  web_server.send(200, "text/html", "ok");
}

void space() {
  nana.drive(0, 0);
  nana.moving_mode = nana.WAITING;

  web_server.send(200, "text/html", "ok");
}

void cleanEnable() {
  nana.clean(true);
  web_server.send(200, "text/html", "ok");
}

void cleanDisable() {
  nana.clean(false);
  web_server.send(200, "text/html", "ok");
}

void web_setup() {
  web_server.on("/", handleRoot);

  web_server.on("/32", space);

  web_server.on("/37", left);
  web_server.on("/38", up);
  web_server.on("/39", right);
  web_server.on("/40", down);

  web_server.on("/65", a);

  web_server.on("/68", cleanDisable);

  web_server.on("/69", cleanEnable);


  web_server.begin();
}
