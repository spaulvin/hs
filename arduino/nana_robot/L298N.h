//Правый двиг
int IN4 = 2;
int IN5 = 0;

//Левый двиг
int IN6 = 4;
int IN7 = 5;

int speed = 0;

int getSpeed() {
  return speed;
}

void right_fwd() {
  digitalWrite (IN4, HIGH);
  digitalWrite (IN5, LOW);
}

void right_stop() {
  digitalWrite (IN4, LOW);
  digitalWrite (IN5, LOW);
}

void right_back() {
  digitalWrite (IN4, LOW);
  digitalWrite (IN5, HIGH);
}

void left_fwd() {
  digitalWrite (IN6, HIGH);
  digitalWrite (IN7, LOW);
}

void left_stop() {
  digitalWrite (IN6, LOW);
  digitalWrite (IN7, LOW);
}

void left_back() {
  digitalWrite (IN6, LOW);
  digitalWrite (IN7, HIGH);
}

void fwd() {
  left_fwd();
  right_fwd();
  speed = 1;
}
void st() {
  left_stop();
  right_stop();
  speed = 0;
}

void backwards() {
  right_back();
  left_back();
  speed = -1;
}

void turnaround_right() {
  right_back();
  left_fwd();
  speed = 0;
}

void turnaround_left() {
  right_fwd();
  left_back();
  speed = 0;
}
