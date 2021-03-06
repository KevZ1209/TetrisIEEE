#define VRX A0
#define VRY A1
#define SW 2
char score = 0;
int actualScore = 0;

// OLED DISPLAY CLASS

#include "Arduino.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

class OledDisplay
{
  public:
    OledDisplay(uint8_t w = 128, uint8_t h = 64, TwoWire *twi = &Wire, int8_t rst_pin = -1 );
    void begin(uint8_t switchvcc = SSD1306_SWITCHCAPVCC, uint8_t i2caddr = 0x3C);
    void drawBoard();
    void drawSquare(char x, char y);
    void clearSquare(char x, char y);
    void drawSmallSquare(char x, char y);
    void drawP(char x, char y) {
      drawSmallSquare(x, y);
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x + 3, y + 1);
      drawSmallSquare(x + 2, y + 2);
      drawSmallSquare(x + 1, y + 2);
      drawSmallSquare(x, y + 2);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x, y + 3);
      drawSmallSquare(x, y + 4);
    }
    void drawR(char x, char y) {
      drawSmallSquare(x, y);
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x + 3, y + 1);
      drawSmallSquare(x + 2, y + 2);
      drawSmallSquare(x + 1, y + 2);
      drawSmallSquare(x, y + 2);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x, y + 3);
      drawSmallSquare(x, y + 4);
      drawSmallSquare(x, y + 4);
      drawSmallSquare(x + 2, y + 3);
      drawSmallSquare(x + 3, y + 4);
    }
    void drawE(char x, char y) {
      drawSmallSquare(x, y);
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x + 3, y);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x, y + 2);
      drawSmallSquare(x + 1, y + 2);
      drawSmallSquare(x, y + 3);
      drawSmallSquare(x, y + 4);
      drawSmallSquare(x + 1, y + 4);
      drawSmallSquare(x + 2, y + 4);
      drawSmallSquare(x + 3, y + 4);
    }
    void drawS(char x, char y) {
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x + 3, y);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x + 1, y + 2);
      drawSmallSquare(x + 2, y + 2);
      drawSmallSquare(x + 3, y + 3);
      drawSmallSquare(x, y + 4);
      drawSmallSquare(x + 1, y + 4);
      drawSmallSquare(x + 2, y + 4);
    }

    void drawO(char x, char y) {
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x + 3, y + 1);
      drawSmallSquare(x, y + 2);
      drawSmallSquare(x + 3, y + 2);
      drawSmallSquare(x, y + 3);
      drawSmallSquare(x + 3, y + 3);
      drawSmallSquare(x + 1, y + 4);
      drawSmallSquare(x + 2, y + 4);
    }

    void drawA(char x, char y) {
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x, y + 1);
      drawSmallSquare(x + 3, y + 1);
      drawSmallSquare(x, y + 2);
      drawSmallSquare(x + 1, y + 2);
      drawSmallSquare(x + 2, y + 2);
      drawSmallSquare(x + 3, y + 2);
      drawSmallSquare(x, y + 3);
      drawSmallSquare(x + 3, y + 3);
      drawSmallSquare(x, y + 4);
      drawSmallSquare(x + 3, y + 4);
    }

    void drawT(char x, char y) {
      drawSmallSquare(x, y);
      drawSmallSquare(x + 1, y);
      drawSmallSquare(x + 2, y);
      drawSmallSquare(x + 3, y);
      drawSmallSquare(x + 4, y);
      drawSmallSquare(x + 2, y + 1);
      drawSmallSquare(x + 2, y + 2);
      drawSmallSquare(x + 2, y + 3);
      drawSmallSquare(x + 2, y + 4);
    }
    void render() {
      m_display.display();
    }
    void clearScreen();
    void flashScreen();
  private:
    Adafruit_SSD1306 m_display;
    static const uint16_t white = SSD1306_WHITE;
    static const uint16_t black = SSD1306_BLACK;


};

OledDisplay::OledDisplay(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin)
{
  m_display = Adafruit_SSD1306(w, h, twi, rst_pin);
}



void OledDisplay::begin(uint8_t switchvcc, uint8_t i2caddr)
{
  m_display.begin(switchvcc, i2caddr);
  m_display.clearDisplay();
  m_display.setTextColor(white);
  m_display.setTextSize(2);
}

void OledDisplay::drawBoard()
{
  m_display.drawRect(3, 1, 123, 63, white);
  for (byte row = 1; row < 20; row++) {
    for (byte col = 1; col < 10; col++) {
      if (row % 2 == 0 && col % 2 == 0) {
        m_display.drawPixel(row * 6 + 4, col * 6 + 2, white);
      }

    }
  }
  m_display.display();
}

void OledDisplay::drawSquare(char x, char y)
{
  char newx = 4 + 6 * y;
  char newy = 56 - 6 * x;
  m_display.fillRect(newx + 1, newy + 1, 5, 5, white);
}

void OledDisplay::clearSquare(char x, char y) {
  char newx = 4 + 6 * y;
  char newy = 56 - 6 * x;
  m_display.fillRect(newx + 1, newy + 1, 5, 5, black);
}

void OledDisplay::drawSmallSquare(char x, char y) {
  char newx = 4 + 2 * y;
  char newy = 56 - 2 * x;
  m_display.fillRect(newx, newy, 2, 2, white);
}

//Clears the entire screen
void OledDisplay::clearScreen()
{
  m_display.clearDisplay();
  m_display.display();
}

//Makes the screen flash by rapidly inverting colors. Good effect to add to a Game Over.
void OledDisplay::flashScreen()
{
  bool toggle = false;
  for (byte i = 0; i < 8; i++) {
    toggle = !toggle;
    m_display.invertDisplay(toggle);
    delay(200);
  }
}

// THE TetrisBoard
class TetrisBoard {
  public:
    TetrisBoard(OledDisplay* od) {
      m_od = od;
    }
    TetrisBoard* getBoard() {
      return this;
    }
    void addBlock(byte x, byte y) {
      arr[y][x] = true;
    }
    bool getBlockAt(byte x, byte y) {
      if (x >= 10 || y >= 20 || x < 0 || y < 0) {
        return true;
      }
      return arr[y][x];
    }
    void removeBlock(int x, int y) {
      arr[y][x] = false;
    }

    bool isFilledRow(char row) {
      for (char i = 0; i < 10; i++) {
        if (arr[row][i] == false) {
          return false;
        }
      }
      return true;
    }

    void clearRows() {
      for (char row = 0; row < 20; row++) {
        while (isFilledRow(row)) {
          for (char r = row; r >= 1; r--) {
            for (char c = 0; c < 10; c++) {
              arr[r][c] = arr[r - 1][c];
            }
          }
          score++;
          actualScore++;
        }
      }
      renderToScreen();
    }

    void renderToScreen() {
      for (byte row = 0; row < 20; row++) {
        for (byte col = 0; col < 10; col++) {
          if (arr[row][col]) {
            m_od->drawSquare(col, row);
          }
          else {
            m_od->clearSquare(col, row);
          }

        }
      }
      m_od->render();
    }
  private:
    bool arr[20][10];
    OledDisplay* m_od;
};


// 0, 1, 2, 3, 4, 5, 6
// i, o, j, l, s, t, z

bool i_piece[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0};

bool o_piece[16] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0};

bool j_piece[16] = {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool l_piece[16] = {0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool s_piece[16] = {0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool t_piece[16] = {0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};

bool z_piece[16] = {1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0};




class Tetromino {
  public:
    Tetromino(TetrisBoard* tb, long type, char x = 4, char y = 0) {
      m_type = type;

      m_tb = tb;
      m_x = x;
      m_y = y;

      switch (m_type) {
        case 0:
          m_y = -2;
          m_contents = i_piece;
          break;
        case 1:
          m_contents = o_piece;
          break;
        case 2:
          m_contents = j_piece;
          break;
        case 3:
          m_contents = l_piece;
          break;
        case 4:
          m_contents = s_piece;
          break;
        case 5:
          m_contents = t_piece;
          break;
        case 6:
          m_contents = z_piece;
          break;
      }
    }

    bool canMoveLeft() {
      // check row by row
      for (char row = 0; row < 4; row += 1) {
        for (char col = 0; col < 4; col += 1) {
          if (m_contents[4 * row + col]) {
            if (m_tb->getBlockAt(col + m_x - 1, row + m_y)) {
              return false;
            }
            break;
          }
        }
      }
      return true;
    }

    bool canMoveRight() {
      // check row by row
      for (char row = 0; row < 4; row += 1) {
        for (char col = 3; col >= 0; col -= 1) {
          if (m_contents[4 * row + col]) {
            if (m_tb->getBlockAt(col + m_x + 1, row + m_y)) {
              return false;
            }
            break;
          }
        }
      }
      return true;
    }

    bool canMoveDown() {
      for (char col = 0; col < 4; col += 1) {
        for (char row = 3; row >= 0; row -= 1) {
          if (m_contents[4 * row + col]) {
            if (m_tb->getBlockAt(col + m_x, row + m_y + 1)) {
              return false;
            }
            break;
          }
        }
      }
      return true;
    }

    void shiftRight() {
      if (canMoveRight()) {
        derender();
        m_x++;
        render();
      }
    }

    void shiftLeft() {
      if (canMoveLeft()) {
        derender();
        m_x--;
        render();
      }
    }

    void shiftDown() {
      if (canMoveDown()) {
        derender();
        m_y++;
        render();
      }
    }

    void rotateRight() {
      derender();

      if (m_type >= 2 && m_x >= 0) {

        if ((m_contents[8] && m_tb->getBlockAt(m_x, m_y))
            || (m_contents[4] && m_tb->getBlockAt(m_x + 1, m_y))
            || (m_contents[0] && m_tb->getBlockAt(m_x + 2, m_y))
            || (m_contents[9] && m_tb->getBlockAt(m_x, m_y + 1))
            || (m_contents[5] && m_tb->getBlockAt(m_x + 1, m_y + 1))
            || (m_contents[1] && m_tb->getBlockAt(m_x + 2, m_y + 1))
            || (m_contents[10] && m_tb->getBlockAt(m_x, m_y + 2))
            || (m_contents[6] && m_tb->getBlockAt(m_x + 1, m_y + 2))
            || (m_contents[2] && m_tb->getBlockAt(m_x + 2, m_y + 2))) {
          render();
          return;
        }

        m_contents[3] = m_contents[0];
        m_contents[0] = m_contents[8];
        m_contents[7] = m_contents[2];
        m_contents[2] = m_contents[3];
        m_contents[3] = m_contents[10];
        m_contents[10] = m_contents[7];
        m_contents[7] = m_contents[8];
        m_contents[8] = m_contents[3];
        m_contents[3] = m_contents[1];
        m_contents[1] = m_contents[4];
        m_contents[7] = m_contents[6];
        m_contents[6] = m_contents[3];
        m_contents[3] = m_contents[9];
        m_contents[9] = m_contents[7];
        m_contents[4] = m_contents[3];
        m_contents[3] = 0;
        m_contents[7] = 0;
      }
      else if (m_type == 0 && m_x >= 0 && m_x <= 6) {

        if ((m_contents[8] && m_tb->getBlockAt(m_x + 2, m_y))
            || (m_contents[9] && m_tb->getBlockAt(m_x + 2, m_y + 1))
            || (m_contents[10] && m_tb->getBlockAt(m_x + 2, m_y + 2))
            || (m_contents[11] && m_tb->getBlockAt(m_x + 2, m_y + 3))
            || (m_contents[2] && m_tb->getBlockAt(m_x, m_y + 2))
            || (m_contents[6] && m_tb->getBlockAt(m_x + 1, m_y + 2))
            || (m_contents[14] && m_tb->getBlockAt(m_x + 3, m_y + 2))) {
          render();
          return;
        }


        m_contents[0] = m_contents[2];
        m_contents[2] = m_contents[8];
        m_contents[8] = m_contents[0];
        m_contents[0] = m_contents[9];
        m_contents[9] = m_contents[6];
        m_contents[6] = m_contents[0];
        m_contents[0] = m_contents[14];
        m_contents[14] = m_contents[11];
        m_contents[11] = m_contents[0];
        m_contents[0] = 0;
      }
      render();
    }

    // renders contents to screen
    void render() {
      for (int i = 0; i < 16; i++) {
        if (m_contents[i] == true) {
          m_tb->addBlock(i % 4 + m_x, i / 4 + m_y);
        }
      }
      m_tb->renderToScreen();
    }

    // clears contents from screen
    void derender() {
      for (int i = 0; i < 16; i++) {
        if (m_contents[i] == true) {
          m_tb->removeBlock(i % 4 + m_x, i / 4 + m_y);
        }
      }
      m_tb->renderToScreen();
    }
  private:
    long m_type;
    char m_x;
    char m_y;
    bool* m_contents;
    TetrisBoard* m_tb;
};


char getJoyStickInput() {
  int vrxValue = analogRead(VRX);
  int vryValue = analogRead(VRY);
  int swValue = digitalRead(SW);
  if (vrxValue <= 100) {
    // UP
    return 0;
  }
  if (vrxValue >= 923) {
    // DOWN
    return 1;
  }
  if (vryValue <= 100) {
    // Right
    return 2;
  }
  if (vryValue >= 923) {
    // Left
    return 3;
  }
  if (swValue == LOW) {
    // Button Pressed
    return 4;
  }

  // No joystick action
  return -1;
}

// ACTUAL CODE HERE
OledDisplay display;
TetrisBoard tb = TetrisBoard(&display);

unsigned long start_time;
// millis() function returns unsigned long
int tempo = 500;

void setup() {
  // put your setup code here, to run once:
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  display.begin();
  start_time = millis();
  
  randomSeed(analogRead(A2));
  
  display.render();
  while (getJoyStickInput() != 4) {
    if (millis() - start_time >= 500) {
      display.drawP(0, 15);
      display.drawR(5, 15);
      display.drawE(10, 15);
      display.drawS(15, 15);
      display.drawS(20, 15);
      display.render();
    }

    if (millis() - start_time >= 1000) {
      display.drawT(0, 21);
      display.drawO(5, 21);
      display.render();
    }

    if (millis() - start_time >= 1500) {
      display.drawS(0, 27);
      display.drawT(5, 27);
      display.drawA(10, 27);
      display.drawR(15, 27);
      display.drawT(20, 27);
      display.render();
    }
    
    if (millis() - start_time >= 3000) {
      display.clearScreen();
      start_time = millis();
    }
    
  }
  display.clearScreen();

  // get a random number so that we don't start with the same piece
  random(0, 7);
  
  Serial.begin(9600);
  display.drawBoard();
  delay(500);

  display.render();



}

//tet piece

Tetromino* tet = nullptr;

void loop() {

  tet = new Tetromino(&tb, random(0, 7));
  if (tet->canMoveDown() == false) {
    display.flashScreen();
    display.clearScreen();
    Serial.print(actualScore);
    Serial.print('\n');
    for (int c = 0; c < actualScore; c++) {
      delay(200);
      digitalWrite(3, HIGH);
      delay(200);
      digitalWrite(3, LOW);
    }
    exit(0);
  }
  while (tet->canMoveDown()) {
    if (millis() - start_time >= tempo) {
      tet->shiftDown();
      start_time = millis();
    }

    switch (getJoyStickInput()) {
      case 0:
        break;
      case 1:
        tet->shiftDown();
        delay(0);
        break;
      case 3:
        tet->shiftLeft();
        delay(50);
        break;
      case 2:
        tet->shiftRight();
        delay(50);
        break;
      case 4:
        tet->rotateRight();
        delay(150);
        break;
    }

    if (score >= 5) {
      score -= 5;
      if (tempo <= 100) {

      }
      else {
        tempo -= 50;
      }
    }

  }

  tb.clearRows();

  delete tet;

}
