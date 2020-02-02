const int gate = 9;
const int knockSensor = 0;
const int programSwitch = 2;
const int lockMotor = 3;
const int redLED = 4;
const int greenLED = 5;

const int threshold = 150;
const int rejectValue = 25;
const int averageRejectValue = 15;
const int knockFadeTime = 150;
const int lockTurnTime = 650;

const int maximumKnocks = 10;
const int knockComplete = 1500;

int secretCode[maximumKnocks] = {100, 100};
int knockReadings[maximumKnocks];
int knockSensorValue = 0;
int programButtonPressed = false;

void setup() {
  pinMode(lockMotor, OUTPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(programSwitch, INPUT);
  digitalWrite(greenLED, HIGH);
  pinMode(gate, INPUT);
  Serial.begin(9600);
}

int starting = digitalRead(gate);

void loop() {

  knockSensorValue = analogRead(knockSensor);

  if (digitalRead(programSwitch) == HIGH) {
    programButtonPressed = true;
    digitalWrite(redLED, HIGH);
  } else {
    programButtonPressed = false;
    digitalWrite(redLED, LOW);
  }

  if (knockSensorValue >= threshold) {
    listenToSecretKnock();
  }

}


void listenToSecretKnock() {
  int i, currentKnockNumber, startTime, now;
  if (starting == HIGH) {
    i = 0;
    for (i = 0; i < maximumKnocks; i++) {
      knockReadings[i] = 0;
    }

    currentKnockNumber = 0;
    startTime = millis();
    now = millis();

    digitalWrite(greenLED, LOW);
    if (programButtonPressed == true) {
      digitalWrite(redLED, LOW);
    }
    delay(knockFadeTime);
    digitalWrite(greenLED, HIGH);
    if (programButtonPressed == true) {
      digitalWrite(redLED, HIGH);
    }
    do {
      knockSensorValue = analogRead(knockSensor);
      if (knockSensorValue >= threshold) {
        now = millis();
        knockReadings[currentKnockNumber] = now - startTime;
        currentKnockNumber ++;
        startTime = now;

        digitalWrite(greenLED, LOW);
        if (programButtonPressed == true) {
          digitalWrite(redLED, LOW);
        }
        delay(knockFadeTime);
        digitalWrite(greenLED, HIGH);
        if (programButtonPressed == true) {
          digitalWrite(redLED, HIGH);
        }
      }

      now = millis();


    } while ((now - startTime < knockComplete) && (currentKnockNumber < maximumKnocks));


    if (programButtonPressed == false) {
      if (validateKnock() == true) {
        triggerDoorUnlock();
      } else {

        digitalWrite(greenLED, LOW);
        for (i = 0; i < 4; i++) {
          digitalWrite(redLED, HIGH);
          delay(100);
          digitalWrite(redLED, LOW);
          delay(100);
          Serial.println('0');
          break;
        }
        digitalWrite(greenLED, HIGH);
      }
    } else {
      validateKnock();
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, HIGH);
      for (i = 0; i < 3; i++) {
        delay(100);
        digitalWrite(redLED, HIGH);
        digitalWrite(greenLED, LOW);
        delay(100);
        digitalWrite(redLED, LOW);
        digitalWrite(greenLED, HIGH);
      }
    }
  }
}

void triggerDoorUnlock() {

  int i = 0;

  // turn the motor on for a bit.
  digitalWrite(lockMotor, HIGH);
  digitalWrite(greenLED, HIGH);

  delay (lockTurnTime);

  digitalWrite(lockMotor, LOW);

  for (i = 0; i < 5; i++) {
    digitalWrite(greenLED, LOW);
    delay(100);
    digitalWrite(greenLED, HIGH);
    delay(100);
    digitalWrite(greenLED, LOW);
    delay(100);
    digitalWrite(greenLED, HIGH);
    delay(100);
    Serial.println('1');
    break;
  }

}

boolean validateKnock() {
  int i = 0;
  int currentKnockCount = 0;
  int secretKnockCount = 0;
  int maxKnockInterval = 0;
  for (i = 0; i < maximumKnocks; i++) {
    if (knockReadings[i] > 0) {
      currentKnockCount++;
    }
    if (secretCode[i] > 0) {
      secretKnockCount++;
    }

    if (knockReadings[i] > maxKnockInterval) {
      maxKnockInterval = knockReadings[i];
    }
  }


  if (programButtonPressed == true) {
    for (i = 0; i < maximumKnocks; i++) {
      secretCode[i] = map(knockReadings[i], 0, maxKnockInterval, 0, 100);
    }

    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
    delay(1000);
    digitalWrite(greenLED, HIGH);
    digitalWrite(redLED, HIGH);
    delay(50);
    for (i = 0; i < maximumKnocks ; i++) {
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, LOW);
      if (secretCode[i] > 0) {
        delay(map(secretCode[i], 0, 100, 0, maxKnockInterval));
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, HIGH);
      }
      delay(50);
    }
    return false;
  }


  if (currentKnockCount != secretKnockCount) {
    return false;
  }


  int totaltimeDifferences = 0;
  int timeDiff = 0;
  for (i = 0; i < maximumKnocks; i++) {
    knockReadings[i] = map(knockReadings[i], 0, maxKnockInterval, 0, 100);
    timeDiff = abs(knockReadings[i] - secretCode[i]);
    if (timeDiff > rejectValue) {
      return false;
    }
    totaltimeDifferences += timeDiff;
  }

  if (totaltimeDifferences / secretKnockCount > averageRejectValue) {
    return false;
  }

  return true;

}
