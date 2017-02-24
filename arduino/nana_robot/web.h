ESP8266WebServer web_server(80);

String state = "stopped";

void webServerSendFile(String filename) {
  File f = SPIFFS.open("/" + filename, "r");
  String data = f.readString();

  web_server.send(200, "text/html", data);
}

void handleRoot() {
  webServerSendFile("index.html");
}

void set_angle(float target_angle) {

  st();

  float start_angle = getHeading();

  unsigned long start = millis();

  if (((int)(start_angle - target_angle + 360) % 360) > 180) {
    turnaround_left();
    while (((int)(start_angle - target_angle + 360) % 360) <= ((int)(getHeading() - target_angle + 360) % 360) && millis() - start <= 3000) {
      delay(3);
    }
  } else {
    turnaround_right();
    while (((int)(start_angle - target_angle + 360) % 360) >= ((int)(getHeading() - target_angle + 360) % 360) && millis() - start <= 3000) {
      delay(3);
    }
  }

  st();
}

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

  st();

  float current = getHeading();
  int target = ((int)current + 45) % 360;

  set_angle(target);

  st();
}

void a() {
  state = "auto";
}

void left() {
  state = "manual";

  turnaround_left();

  web_server.send(200, "text/html", "ok");
}

void right() {
  state = "manual";

  turnaround_right();

  web_server.send(200, "text/html", "ok");
}

void up() {
  state = "manual";

  fwd();

  web_server.send(200, "text/html", "ok");
}

void down() {
  state = "manual";

  backwards();

  web_server.send(200, "text/html", "ok");
}

void space() {
  st();
  state = "stopped";
  
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

  web_server.begin();
}
