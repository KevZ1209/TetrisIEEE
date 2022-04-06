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
        m_display = Adafruit_SSD1306(w,h,twi,rst_pin);
    }

    void OledDisplay::begin(uint8_t switchvcc,uint8_t i2caddr)
    {
        m_display.begin(switchvcc,i2caddr);
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
          for(byte i = 0; i < 8; i++){
              toggle = !toggle;
              m_display.invertDisplay(toggle);
              delay(200);
          }
    }

    //Provided in case you really wanted to manually draw something, but if you want to do that
    //  I reccomend that you just use Adafruit_SSD1306 and Adafruit_GFX by themselves
    void OledDisplay::drawPixel(byte x, byte y)
    {
        m_display.drawPixel(x,y,white);
        m_display.display();
    }
    
    //Provided in case you really wanted to manually draw something, but if you want to do that
    //  I recommend that you just use Adafruit_SSD1306 and Adafruit_GFX by themselves
    void OledDisplay::clearPixel(byte x, byte y)
    {
        m_display.drawPixel(x,y,black);
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
    void addBlock(byte row, byte col) {
      arr[row][col] = true;
    }
    bool getBlockAt(byte row, byte col) {
      if (row >= 10 || col >= 20 || row < 0 || col < 0) {
        return true;
      }
      return arr[row][col];
    }
    void removeBlock(byte row, byte col) {
      arr[row][col] = false;
    }
    void removeRowAndShiftDown(byte row) {
      for (int i = 0; i < 10; i++) {
        removeBlock(i, row);
      }
    }
    void renderToScreen() {
      for (int row = 0; row < 10; row++) {
        for (int col = 0; col < 20; col++) {
          if (arr[row][col]) {
            m_od->drawSquare(row, col);  
          }
          else {
            m_od->clearSquare(row, col);
          }
          
        }
      }
      m_od->render();
    }
private:
    bool arr[10][20];
    OledDisplay* m_od;
};


// 0, 1, 2, 3, 4, 5, 6
// i, o, j, l, s, t, z

bool i_piece[4][16] = {
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0}, 
  {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0}, 
  {0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0},
  {0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0}
};

bool o_piece[4][16] {
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0}
};

bool j_piece[4][16] = {
  {0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0},
  {0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},
  {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0}
};

bool l_piece[4][16] = {
  {0,0,0,0,1,1,1,0,0,0,1,0,0,0,0,0},
  {0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},
  {1,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,1,1,0,0,1,0,0,0,1,0,0,0,0,0,0}
};

bool s_piece[4][16] = {
  {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
  {0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0},
  {0,0,0,0,0,1,1,0,1,1,0,0,0,0,0,0},
  {0,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0}
};

bool t_piece[4][16] = {
  {0,0,0,0,1,1,1,0,0,1,0,0,0,0,0,0},
  {0,1,0,0,1,1,0,0,0,1,0,0,0,0,0,0},
  {0,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0},
  {0,1,0,0,0,1,1,0,0,1,0,0,0,0,0,0}
};

bool z_piece[4][16] = {
  {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0},
  {0,0,1,0,0,1,1,0,0,1,0,0,0,0,0,0}
};

class Tetromino {
public:
  Tetromino(int type, TetrisBoard* tb, byte x = 4, byte y = -1, byte orientation = 0) {
    m_type = type;
    m_tb = tb;
    m_x = x;
    m_y = y;
    m_orientation = orientation;

    switch (m_type) {
      case 0:
        m_y = -2;
        m_contents = i_piece[0];
        break;
      case 1:
        m_contents = o_piece[0];
        break;
      case 2:
        m_contents = j_piece[0];
        break;
      case 3:
        m_contents = l_piece[0];
        break;
      case 4:
        m_contents = s_piece[0];
        break;
      case 5:
        m_contents = t_piece[0];
        break;
      case 6:
        m_contents = z_piece[0];
        break;
    }
  }
  bool canMoveLeft() {
    byte index;
    for (byte col = 3; col >= 0; col--) {
      for (byte row = 0; row < 4; row++) {
        index = row*4 + col;
        if ((m_contents[index] && (m_tb->getBlockAt(row + m_x - 1, col + m_y) && !getBlock(row - 1, col))) || row + m_x <= 0) {
           return false;
        }
      }
    }
    return true;
  }
  bool canMoveRight() {
    byte index;
    for (byte col = 3; col >= 0; col--) {
      for (byte row = 0; row < 4; row++) {
        index = row*4 + col;
        if ((m_contents[index] && (m_tb->getBlockAt(row + m_x, col + m_y) && !getBlock(row, col + 1))) || row + m_x <= 0) {
           return false;
        }
      }
    }
    return true;
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
    return m_contents[4*row + col];
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
  int m_type;
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

  
  
}

int tempo = 500;
int counter = 0;

const int I_PIECE = 0;
const int O_PIECE = 1;
const int J_PIECE = 2;
const int L_PIECE = 3;
const int S_PIECE = 4;
const int T_PIECE = 5;
const int Z_PIECE = 6;

void loop() {
  // put your main code here, to run repeatedly:
//  for (int a = 0; a < 20; a++) {
//    for (int i = 0; i < 4; i++) {
//      display.drawSquare(a, i);
//    }
//    // display.drawBoard();
//    display.render();
//    delay(1000);
//    for (int i = 0; i < 4; i++) {
//      display.clearSquare(a, i);
//    }
//  }

//    for (byte i = 0; i < 5; i++) {
//      tb.addBlock(i, 1);
//      tb.renderToScreen();
//      tb.removeBlock(i, 1); 
//      delay(tempo); 
//    }
//    
//    delay(tempo);
//
//      if (counter >= 6) {
//        counter = 0;
//      }
//      Tetromino* test;
//      if (1 == 1) {
//        test = new Tetromino(I_PIECE, &tb);
//      }
//      else {
//        test = new Tetromino(J_PIECE, &tb);
//      }
//
//      if (1 < 1) {
//        counter = 1;
//      }
//      else {
//        counter = 4;
//      }
//
//      test->render();
//      delay(500);
//      Serial.print("Can it move left? ");
//      Serial.println(test->canMoveLeft());
//      Serial.print("Can it move right? ");
//      Serial.println(test->canMoveRight());
//      test->derender();
//
//      for (int i = 0; i < 20; i++) {
//        for (int j = 0; j < 10; j++) {
//          tb.addBlock(j, i);
//        }
//        
//      }
      
//      tb.removeBlock(1, 2);
//      Serial.println(tb.getBlockAt(10, 2));
//      Serial.println(tb.getBlockAt(1, 20));

      tb.addBlock(1, 2);
      delay(1000);
      tb.renderToScreen();

      
      
      
//      counter++;
}
