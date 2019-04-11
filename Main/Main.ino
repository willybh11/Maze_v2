
#include <time.h>

int pins[4] = {5, 6, 7, 8}; //NESW

int starts[3][2] = {
  {0, 9}, // marked 1
  {0, 0}, // marked 2
  {9, 0}  // marked 3
};

int row = 0;
int col = 0;

#define x_pin A2
#define y_pin A1
#define buzzerPin 13
#define togglePin 12
#define resetPin 10

bool grid[10][10][4] = { // true: connection | false: wall
  { // row 0
    {false, true, false, false}, // 0
    {false, true, false, true}, // 1
    {false, true, true, true},  // 2
    {false, true, false, true}, // 3
    {false, false, true, true}, // 4
    {false, true, false, false}, // 5
    {false, false, true, true}, // 6
    {false, true, false, false}, // 7
    {false, false, true, true}, // 8
    {false, false, true, false} // 9
  },
  { // row 1
    {false, false, true, false}, // 0
    {false, true, true, false}, // 1
    {true, true, false, true},  // 2
    {false, false, true, true}, // 3
    {true, true, true, false},  // 4
    {false, true, false, true}, // 5
    {true, true, false, true},  // 6
    {false, true, true, true},  // 7
    {true, true, false, true},  // 8
    {true, false, false, true}  // 9
  },
  { // row 2
    {true, false, true, false}, // 0
    {true, true, true, false},  // 1
    {false, false, true, true}, // 2
    {true, false, false, false}, // 3
    {true, true, true, false},  // 4
    {false, false, false, true}, // 5
    {false, false, true, false}, // 6
    {true, false, true, false}, // 7
    {false, true, true, false}, // 8
    {false, false, false, true} // 9
  },
  { // row 3
    {true, true, true, false},  // 0
    {true, false, false, true}, // 1
    {true, true, false, false}, // 2
    {false, false, true, true}, // 3
    {true, false, true, false}, // 4
    {false, true, false, false}, // 5
    {true, false, true, true},  // 6
    {true, false, true, false}, // 7
    {true, true, false, false}, // 8
    {false, false, true, true}  // 9
  },
  { // row 4
    {true, false, false, false}, // 0
    {false, true, true, false}, // 1
    {false, true, true, true},  // 2
    {true, false, false, true}, // 3
    {true, true, false, false}, // 4
    {false, false, true, true}, // 5
    {true, true, true, false},  // 6
    {true, true, false, true},  // 7
    {false, false, true, true}, // 8
    {true, false, true, false}  // 9
  },
  { // row 5
    {false, true, true, false}, // 0
    {true, false, true, true},  // 1
    {true, true, false, false}, // 2
    {false, true, true, true},  // 3
    {false, true, false, true}, // 4
    {true, false, true, true},  // 5
    {true, true, true, false},  // 6
    {false, false, true, true}, // 7
    {true, true, false, false}, // 8
    {true, false, true, true}   // 9
  },
  { // row 6
    {true, false, true, false}, // 0
    {true, false, false, false}, // 1
    {false, true, false, false}, // 2
    {true, false, false, true}, // 3
    {false, true, true, false}, // 4
    {true, false, false, true}, // 5
    {true, false, false, false}, // 6
    {true, false, true, false}, // 7
    {false, true, true, false}, // 8
    {true, false, false, true}  // 9
  },
  { // row 7
    {true, true, false, false}, // 0
    {false, true, true, true},  // 1
    {false, true, false, true}, // 2
    {false, false, false, true}, // 3
    {true, false, false, false}, // 4
    {false, true, true, false}, // 5
    {false, false, true, true}, // 6
    {true, false, false, false}, // 7
    {true, true, false, false}, // 8
    {false, false, true, true}  // 9
  },
  { // row 8
    {false, true, true, false}, // 0
    {true, false, false, true}, // 1
    {false, true, true, false}, // 2
    {false, true, false, true}, // 3
    {false, true, true, true},  // 4
    {true, false, true, true},  // 5
    {true, true, true, false},  // 6
    {false, true, false, true}, // 7
    {false, true, true, true},  // 8
    {true, false, false, true}, // 9
  },
  { // row 9
    {true, true, false, false}, // 0
    {false, true, false, true}, // 1
    {true, true, false, true},  // 2
    {false, false, false, true}, // 3
    {true, false, false, false}, // 4
    {true, false, false, false}, // 5
    {true, true, false, false}, // 6
    {false, false, false, true}, // 7
    {true, true, false, false}, // 8
    {false, false, false, true} // 9 FIN
  }
};

void setup() {
  Serial.begin(9600);
  col = sudoRand(10);
  row = sudoRand(10);

  Serial.print(row);
  Serial.print(' ');
  Serial.println(col);

  pinMode(2, OUTPUT);
  pinMode(togglePin, INPUT);
  pinMode(resetPin, INPUT);

  for (int pin : pins) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {

  if (digitalRead(resetPin) == LOW) {
    tone(buzzerPin, 450, 250);
    delay(250);
    tone(buzzerPin, 500, 500);
    resetBoard();
    while (digitalRead(resetPin) == LOW) {
      delay(10);
    }
  }

  updatePos();
  updatePins();

}

void updatePins() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(pins[i], !grid[row][col][i]);
  }
  digitalWrite(2, HIGH);
}

void printPos() {
  Serial.print(row);
  Serial.print(' ');
  Serial.print(col);
  Serial.print(" diff: ");
  Serial.print(digitalRead(togglePin));
  Serial.print(" reset: ");
  Serial.println(digitalRead(resetPin));
}

void updatePos() {

  if (analogRead(x_pin) > 900) {
    if (grid[row][col][1]) {
      tone(buzzerPin, 1000, 50);
      col++; // no wall east
    }
    else tone(buzzerPin, 50, 200);
    delay(200);
    printPos();
  } else if (analogRead(x_pin) < 100) {
    if (grid[row][col][3]) {
      tone(buzzerPin, 1000, 50);
      col--; // no wall west
    }
    else tone(buzzerPin, 50, 200);
    delay(200);
    printPos();
  } else if (analogRead(y_pin) > 900) {
    if (grid[row][col][2]) {
      tone(buzzerPin, 1000, 50);
      row++; // no wall south
    }
    else tone(buzzerPin, 50, 200);
    delay(200);
    printPos();
  } else if (analogRead(y_pin) < 100) {
    if (grid[row][col][0]) {
      tone(buzzerPin, 1000, 50);
      row--; // now all north
    }
    else tone(buzzerPin, 50, 200);
    delay(200);
    printPos();
  }
}

void resetBoard() {
  if (digitalRead(togglePin) == HIGH) {
    easyReset();
    Serial.println("reset: easy");
  } else {
    hardReset();
    Serial.println("reset: hard");
  }
}

void easyReset() {
  int startIndex = sudoRand(3);
  col = starts[startIndex][1];
  row = starts[startIndex][0];
}

void hardReset() {
  int quadrant = sudoRand(3); //zero quadrant is top left; one quadrant is top right; two quadrant is bottom left; three quadrant is bottom right
  if (quadrant == 0) {
    col = sudoRand(5);
    row = sudoRand(5);
  } else if (quadrant == 1) {
    col = sudoRand(5) + 5;
    row = sudoRand(5);
  } else {
    col = sudoRand(5);
    row = sudoRand(5) + 5;
  }
}

int sudoRand(int maximum) {
  Serial.print(millis());
  Serial.print(" --> ");
  delay(1);
  Serial.println((millis()) % maximum);
  return ((millis())) % maximum;
}

void resetNoise() {
  //maybe don't use this
  return;
}
