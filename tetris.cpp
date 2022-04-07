#define VRX A0
#define VRY A1
#define SW 2


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
    void drawSquare(byte x, byte y);
    void clearSquare(byte x, byte y);
    void render() {
      m_display.display();
    }
    void clearScreen();
    void flashScreen();
    void drawPixel(byte x, byte y);
    void clearPixel(byte x, byte y);
    OledDisplay* getDisplay() {
      return this;
    }
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
  m_display.drawRect(3, 1, 122, 62, white);
  m_display.display();
}

void OledDisplay::drawSquare(byte x, byte y)
{
  byte newx = 4 + 6 * y;
  byte newy = 56 - 6 * x;
  m_display.fillRect(newx, newy, 6, 6, white);
}

void OledDisplay::clearSquare(byte x, byte y) {
  byte newx = 4 + 6 * y;
  byte newy = 56 - 6 * x;
  m_display.fillRect(newx, newy, 6, 6, black);
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

//Provided in case you really wanted to manually draw something, but if you want to do that
//  I reccomend that you just use Adafruit_SSD1306 and Adafruit_GFX by themselves
void OledDisplay::drawPixel(byte x, byte y)
{
  m_display.drawPixel(x, y, white);
  m_display.display();
}

//Provided in case you really wanted to manually draw something, but if you want to do that
//  I recommend that you just use Adafruit_SSD1306 and Adafruit_GFX by themselves
void OledDisplay::clearPixel(byte x, byte y)
{
  m_display.drawPixel(x, y, black);
  m_display.display();
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

    void renderToScreen() {
      for (int row = 0; row < 20; row++) {
        for (int col = 0; col < 10; col++) {
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

bool o_piece[16] = {0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0};

bool j_piece[16] = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0};

bool l_piece[16] = {0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0};

bool s_piece[16] = {0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0};

bool t_piece[16] = {0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0};

bool z_piece[16] = {0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};

class Tetromino {
  public:
    Tetromino(TetrisBoard* tb, byte x = 4, byte y = 0, byte orientation = 0) {
      m_type = random(0, 7);
      m_tb = tb;
      m_x = x;
      m_y = y;
      m_orientation = orientation;

      switch (m_type) {
        case 0:
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
      for (int row = 0; row < 4; row += 1) {
        for (int col = 0; col < 4; col += 1) {
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
      for (int row = 0; row < 4; row += 1) {
        for (int col = 3; col >= 0; col -= 1) {
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
      for (int col = 0; col < 4; col += 1) {
        for (int row = 3; row >= 0; row -= 1) {
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
      bool arr[16];
      arr[0] = arr[12];
    }
    
    byte getX() {
      return m_x;
    }
    byte getY() {
      return m_y;
    }

    bool getBlock(byte row, byte col) {
      if (row < 0 || row > 3 || col < 0 || col > 3) {
        return false;
      }
      return m_contents[4 * row + col];
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
    byte m_orientation;
    byte m_x;
    byte m_y;
    bool* m_contents;
    TetrisBoard* m_tb;
};


int getJoyStickInput() {
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
unsigned long current_time; // millis() function returns unsigned long
unsigned long tempo = 500;

void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(9600);
  display.drawBoard();
  delay(500);
  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT_PULLUP);

  display.render();

  current_time = millis();
  start_time = current_time;

}

int counter = 0;

//joystick controls
const int UP = 0;
const int DOWN = 1;
const int RIGHT = 2;
const int LEFT = 3;
const int PRESS = 4;

//tet pieces
const int I_PIECE = 0;
const int O_PIECE = 1;
const int J_PIECE = 2;
const int L_PIECE = 3;
const int S_PIECE = 4;
const int T_PIECE = 5;
const int Z_PIECE = 6;

Tetromino* tet = new Tetromino(&tb);

void loop() {

  while (tet->canMoveDown()) {
    current_time = millis();
    switch (getJoyStickInput()) {
      case UP:
        break;
      case DOWN:
        tet->shiftDown();
        delay(0);
        break;
      case LEFT:
        tet->shiftLeft();
        delay(40);
        break;
      case RIGHT:
        tet->shiftRight();
        delay(40);
        break;
    }

    if (current_time - start_time >= 500) {
      tet->shiftDown();
      start_time = current_time;
    }
  }

  delete tet;
  tet = new Tetromino(&tb);
  if (tet->canMoveDown() == false) {
    display.flashScreen();
    display.clearScreen();
    exit(0);
  }
}
