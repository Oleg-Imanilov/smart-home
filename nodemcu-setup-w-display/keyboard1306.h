#ifndef __KEYBOARD1306_H__
#define __KEYBOARD1306_H__

#include "SSD1306.h"

#define KEY_ABC 1
#define KEY_abc 2
#define KEY_123 3

const char BACKSPACE_8_9[] PROGMEM = {0x00, 0xC0, 0xF0, 0xFC, 0xFF, 0xFF, 0xFC, 0xF0, 0xC0};
const char SPACE_8_9[] PROGMEM =     {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x82, 0x82, 0xFE};
const char ENTER_8_9[] PROGMEM =     {0x00, 0x00, 0xC0, 0xC8, 0xCC, 0xFE, 0xFE, 0x0C, 0x08};
const char SHIFT_8_9[] PROGMEM =     {0x18, 0x24, 0x42, 0x81, 0x81, 0xE7, 0x24, 0x24, 0x3C};
const char NUM_8_9[] PROGMEM =       {0x02, 0x03, 0x62, 0x92, 0x82, 0x47, 0x20, 0x10, 0xF0};
const char ABC_8_9[] PROGMEM =       {0x0C, 0x2A, 0x29, 0x2F, 0x69, 0xA9, 0xA0, 0xA0, 0x60};
const char SYMB_8_9[] PROGMEM =      {0x06, 0x09, 0x08, 0x04, 0x22, 0x22, 0xF8, 0x22, 0x20};

const char KEY_ABC_MAP[] =  {'\10', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O',   'P', '\12', '\13',
                             '\10', ' ', 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K',   'L', '\12', '\13',
                             '\10', ' ', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ' ', '\14', '\11', '\15'
                            };

const char KEY_abc_MAP[] =  {'\10', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o',   'p', '\12', '\13',
                             '\10', ' ', 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k',   'l', '\12', '\13',
                             '\10', ' ', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ' ', '\14', '\11', '\15'
                            };

const char KEY_123_MAP[] =  {'\10', '1', '2', '3', '4', '5', '6', '7', '8', '9',  '0', '\12', '\13',
                             '\10', '!', '@', '#', '$', '%', '^', '&', '*', '(',  ')', '\12', '\13',
                             '\10', '-', '_', '+', '=', ',', '.', '/', '?', '~', '\"', '\11', '\15'
                            };

class Keyboard1306 {
  private:
    SSD1306  *display;
    int keyMode = KEY_ABC;
    String value = "Hello world";
    int curr_key = 0;
    const char *str = KEY_ABC_MAP;
  public:
    Keyboard1306 (SSD1306  *_display) {
      display = _display;
    }

    void setValue(String &val) {
      value = val;
    }
    String getValue(){
      return value;
    }  
    void draw() {
      display->clear();
      display->setTextAlignment(TEXT_ALIGN_CENTER);
      display->drawString(63, 0, value);
      display->drawRect(0, 0, 128, 15);
      display->setTextAlignment(TEXT_ALIGN_LEFT);
      char k[] = " ";
      int ix = 0;
      int x = 2; int y = 20;
      while (str[ix] != '\15') {
        switch (str[ix]) {
          case '\10':
            display->drawXbm(x, y, 8, 9, SHIFT_8_9);
            break;
          case '\11':
            display->drawXbm(x, y, 8, 9, ENTER_8_9);
            break;
          case '\12':
            display->drawXbm(x, y, 8, 9, BACKSPACE_8_9);
            break;
          case '\13': // New line
            x = -8;
            y += 15;
            break;
          case '\14':
            display->drawXbm(x, y, 8, 9, SPACE_8_9);
            break;
          case ' ':
            // skip
            break;
          default:
            k[0] = str[ix];
            display->drawString(x, y, k);
        }
        if (ix == curr_key) {
          display->drawRect(x - 2, y-1, 12, 15);
        }
        ix++;
        x += 10;
      }
      display->display();
    }

    void addChar(char ch) {
      value += ch;
    }
    
    void backSpace() {
      if(value.length()>0) {
        value = value.substring(0, value.length()-1);
      }
    }
    
    void finishEditing() {
    
    }

    void rotate(int diff) {
      Serial.print(curr_key);
      Serial.print("  =>  ");
      do {
       curr_key += diff;
       if(curr_key<0) {
          curr_key = 38;
       }
       if(curr_key > 38) {
          curr_key = 0;
       }
      } while (str[curr_key]==' '||str[curr_key]=='\13'||str[curr_key]=='\15');
      Serial.print(curr_key);
    }

    bool click() {
      switch (str[curr_key]) {
        case '\10': // SHIFT
          // Rotate keyboards
          str = str == KEY_ABC_MAP ? KEY_abc_MAP : str == KEY_abc_MAP ? KEY_123_MAP : KEY_ABC_MAP;
          break;
        case '\11': // Enter
          return false;
          break;
        case '\12':
          backSpace();
          break;
        case '\13':
        case ' ':
          break;

        case '\14':
          addChar(' ');
          break;

        case '\15':
          // Can't be here
          break;
        default:
          addChar(str[curr_key]);
      }
      return true;
    }
};

#endif
