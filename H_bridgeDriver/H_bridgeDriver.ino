// 3 high, 1 high/PWM - rotation
// 2 high, 4 high/PWM - reverse
// 1 high, 4 high - brake

#define DEADZONE 10  // "мёртвая зона" потенциометра

// пины драйвера, P1 и P4 должны быть на PWM пинах! (3, 5, 6, 9, 10, 11 для NANO и UNO)
#define P1 3
#define P2 4
#define P3 5
#define P4 6

#define potPin 0      // сюда подключен потенциометр

boolean switch_flag;
int potent, duty;
int left_min, right_min;

void setup() {
  // все пины драйвера как выходы, и сразу выключаем
  pinMode(P1, OUTPUT);
  digitalWrite(P1, 0);
  pinMode(P2, OUTPUT);
  digitalWrite(P2, 0);
  pinMode(P3, OUTPUT);
  digitalWrite(P3, 0);
  pinMode(P4, OUTPUT);
  digitalWrite(P4, 0);

  left_min = 512 - DEADZONE;   // расчёт границ мёртвой зоны
  right_min = 512 + DEADZONE;  // расчёт границ мёртвой зоны
}

void loop() {
  potent = analogRead(potPin);                    // читаем с потенциометра

  if (potent > left_min && potent < right_min) {  // если мы в "мёртвой" зоне
    // вырубить все ключи
    digitalWrite(P1, 0);
    digitalWrite(P2, 0);
    digitalWrite(P3, 0);
    digitalWrite(P4, 0);
    switch_flag = 1;          // разрешить переключение
  } else if (potent > right_min) {                // если мы вышли из мёртвой зоны справа
    if (switch_flag) {        // если разрешено переключение
      switch_flag = 0;        // запретить переключение
      digitalWrite(P2, 0);    // вырубить Р канальный второго плеча
      digitalWrite(P4, 0);    // вырубить N канальный второго плеча
      delayMicroseconds(5);   // задержечка на переключение на всякий случай
      digitalWrite(P3, 1);    // врубить Р канальный первого плеча
    }
    // рассчитать скважность для N канального первого плеча
    duty = map(potent, right_min, 1023, 0, 255);
    analogWrite(P1, duty);    // ЖАРИТЬ ШИМ!

  } else if (potent < left_min) {                 // если мы вышли из мёртвой зоны слева
    if (switch_flag) {        // если разрешено переключение
      switch_flag = 0;        // запретить переключение
      digitalWrite(P3, 0);    // вырубить Р канальный первого плеча
      digitalWrite(P1, 0);    // вырубить N канальный первого плеча
      delayMicroseconds(5);   // задержечка на переключение на всякий случай
      digitalWrite(P2, 1);    // врубить Р канальный второго плеча
    }
    // рассчитать скважность для N канального второго плеча
    duty = map(potent, left_min, 0, 0, 255);
    analogWrite(P4, duty);    // ЖАРИТЬ ШИМ!
  }

}
