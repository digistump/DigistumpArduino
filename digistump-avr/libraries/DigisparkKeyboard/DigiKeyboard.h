/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 * Modified for Digispark by Digistump
 * Added full Keyboard usage values  by Danjovic, February 2016
 * Report Buffer extended up to 6 keytrokes simultaneous by Danjovic, March 2016  
 */
#ifndef __DigiKeyboard_h__
#define __DigiKeyboard_h__

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/delay.h>
#include <string.h>

#include "usbdrv.h"
#include "scancode-ascii-table.h"

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;


#define BUFFER_SIZE 2 // Minimum of 2: 1 for modifiers + 1 for keystroke


static uchar    idleRate;           // in 4 ms units


/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = { /* USB report descriptor */
<<<<<<< HEAD
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop) 
  0x09, 0x06,                    // USAGE (Keyboard) 
  0xa1, 0x01,                    // COLLECTION (Application) 
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard) 
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl) 
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI) 
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0) 
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1) 
  0x75, 0x01,                    //   REPORT_SIZE (1) 
  0x95, 0x08,                    //   REPORT_COUNT (8) 
  0x81, 0x02,                    //   INPUT (Data,Var,Abs) 
  0x95, 0x06,                    //   REPORT_COUNT (6 simultaneous keystrokes) 
  0x75, 0x08,                    //   REPORT_SIZE (8) 
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101) 
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated)) 
  0x29, 0xE7,                    //   USAGE_MAXIMUM (Right GUI) 
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs) 
  0xc0                           // END_COLLECTION 
=======
  0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                    // USAGE (Keyboard)
  0xa1, 0x01,                    // COLLECTION (Application)
  0x05, 0x07,                    //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                    //   REPORT_SIZE (1)
  0x95, 0x08,                    //   REPORT_COUNT (8)
  0x81, 0x02,                    //   INPUT (Data,Var,Abs)
  0x95, 0x01,           //   REPORT_COUNT (simultaneous keystrokes)
  0x75, 0x08,                    //   REPORT_SIZE (8)
  0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
  0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)
  0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
  0xc0                           // END_COLLECTION
>>>>>>> origin/master
};



/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
#define MOD_CONTROL_LEFT    (1<<0)
#define MOD_SHIFT_LEFT      (1<<1)
#define MOD_ALT_LEFT        (1<<2)
#define MOD_GUI_LEFT        (1<<3)
#define MOD_CONTROL_RIGHT   (1<<4)
#define MOD_SHIFT_RIGHT     (1<<5)
#define MOD_ALT_RIGHT       (1<<6)
#define MOD_GUI_RIGHT       (1<<7)

<<<<<<< HEAD
#define KEY_NOKEY             0x00     // Reserved (no event indicated)
#define KEY_ERR_ROLLOVER      0x01     // Keyboard ErrorRollOver
#define KEY_POSTFAIL          0x02     // Keyboard POSTFail
#define KEY_ERR_UNDEFINED     0x03     // Keyboard ErrorUndefined
#define KEY_A                 0x04     // Keyboard a and A
#define KEY_B                 0x05     // Keyboard b and B
#define KEY_C                 0x06     // Keyboard c and C
#define KEY_D                 0x07     // Keyboard d and D
#define KEY_E                 0x08     // Keyboard e and E
#define KEY_F                 0x09     // Keyboard f and F
#define KEY_G                 0x0A     // Keyboard g and G
#define KEY_H                 0x0B     // Keyboard h and H
#define KEY_I                 0x0C     // Keyboard i and I
#define KEY_J                 0x0D     // Keyboard j and J
#define KEY_K                 0x0E     // Keyboard k and K
#define KEY_L                 0x0F     // Keyboard l and L
#define KEY_M                 0x10     // Keyboard m and M
#define KEY_N                 0x11     // Keyboard n and N
#define KEY_O                 0x12     // Keyboard o and O
#define KEY_P                 0x13     // Keyboard p and P
#define KEY_Q                 0x14     // Keyboard q and Q
#define KEY_R                 0x15     // Keyboard r and R
#define KEY_S                 0x16     // Keyboard s and S
#define KEY_T                 0x17     // Keyboard t and T
#define KEY_U                 0x18     // Keyboard u and U
#define KEY_V                 0x19     // Keyboard v and V
#define KEY_W                 0x1A     // Keyboard w and W
#define KEY_X                 0x1B     // Keyboard x and X
#define KEY_Y                 0x1C     // Keyboard y and Y
#define KEY_Z                 0x1D     // Keyboard z and Z
#define KEY_1                 0x1E     // Keyboard 1 and !
#define KEY_2                 0x1F     // Keyboard 2 and
#define KEY_3                 0x20     // Keyboard 3 and #
#define KEY_4                 0x21     // Keyboard 4 and $
#define KEY_5                 0x22     // Keyboard 5 and %
#define KEY_6                 0x23     // Keyboard 6 and ^
#define KEY_7                 0x24     // Keyboard 7 and &
#define KEY_8                 0x25     // Keyboard 8 and *
#define KEY_9                 0x26     // Keyboard 9 and (
#define KEY_0                 0x27     // Keyboard 0 and )
#define KEY_ENTER             0x28     // Keyboard Return (ENTER)
#define KEY_ESCAPE            0x29     // Keyboard ESCAPE
#define KEY_DELETE            0x2A     // Keyboard DELETE (Backspace)
#define KEY_TAB               0x2B     // Keyboard Tab
#define KEY_SPACE             0x2C     // Keyboard Spacebar
#define KEY_MINUS             0x2D     // Keyboard - and (underscore)
#define KEY_EQUAL             0x2E     // Keyboard = and +
#define KEY_L_BRACKET         0x2F     // Keyboard [ and {
#define KEY_R_BRACKET         0x30     // Keyboard ] and }
#define KEY_BACKSLASH         0x31     // Keyboard \ and |
#define KEY_HASH              0x32     // Keyboard Non-US # and ~
#define KEY_SEMICOLON         0x33     // Keyboard ; and :
#define KEY_APOSTROPHE        0x34     // Keyboard ‘ and “
#define KEY_GRAVE             0x35     // Keyboard Grave Accent and Tilde
#define KEY_COMMA             0x36     // Keyboard , and <
#define KEY_DOT               0x37     // Keyboard . and >
#define KEY_SLASH             0x38     // Keyboard / and ?
#define KEY_CAPS_LOCK         0x39     // Keyboard Caps Lock
#define KEY_F1                0x3A     // Keyboard F1
#define KEY_F2                0x3B     // Keyboard F2
#define KEY_F3                0x3C     // Keyboard F3
#define KEY_F4                0x3D     // Keyboard F4
#define KEY_F5                0x3E     // Keyboard F5
#define KEY_F6                0x3F     // Keyboard F6
#define KEY_F7                0x40     // Keyboard F7
#define KEY_F8                0x41     // Keyboard F8
#define KEY_F9                0x42     // Keyboard F9
#define KEY_F10               0x43     // Keyboard F10
#define KEY_F11               0x44     // Keyboard F11
#define KEY_F12               0x45     // Keyboard F12
#define KEY_PRTSCN            0x46     // Keyboard PrintScreen
#define KEY_SCR_LOCK          0x47     // Keyboard Scroll Lock
#define KEY_PAUSE             0x48     // Keyboard Pause
#define KEY_INSERT            0x49     // Keyboard Insert
#define KEY_HOME              0x4A     // Keyboard Home
#define KEY_PAGE_UP           0x4B     // Keyboard PageUp
#define KEY_DELETE            0x4C     // Keyboard Delete Forward
#define KEY_END               0x4D     // Keyboard End
#define KEY_PAGE_DOWN         0x4E     // Keyboard PageDown
#define KEY_ARROW_RIGHT       0x4F     // Keyboard RightArrow
#define KEY_ARROW_LEFT        0x50     // Keyboard LeftArrow
#define KEY_ARROW_DOWN        0x51     // Keyboard DownArrow
#define KEY_ARROW_UP          0x52     // Keyboard UpArrow

#define KEY_RIGHT_ARROW       0x4F     // Keyboard RightArrow  -> Alternative arrow keys names
#define KEY_LEFT_ARROW        0x50     // Keyboard LeftArrow
#define KEY_DOWN_ARROW        0x51     // Keyboard DownArrow
#define KEY_UP_ARROW          0x52     // Keyboard UpArrow

#define KEY_NUM_LOCK          0x53     // Keypad Num Lock and Clear
#define KEY_KPAD_SLASH        0x54     // Keypad /
#define KEY_KPAD_TIMES        0x55     // Keypad *
#define KEY_KPAD_MINUS        0x56     // Keypad -
#define KEY_KPAD_PLUS         0x57     // Keypad +
#define KEY_KPAD_ENTER        0x58     // Keypad ENTER
#define KEY_KPAD_1            0x59     // Keypad 1 and End
#define KEY_KPAD_2            0x5A     // Keypad 2 and Down Arrow
#define KEY_KPAD_3            0x5B     // Keypad 3 and PageDn
#define KEY_KPAD_4            0x5C     // Keypad 4 and Left Arrow
#define KEY_KPAD_5            0x5D     // Keypad 5
#define KEY_KPAD_6            0x5E     // Keypad 6 and Right Arrow
#define KEY_KPAD_7            0x5F     // Keypad 7 and Home
#define KEY_KPAD_8            0x60     // Keypad 8 and Up Arrow
#define KEY_KPAD_9            0x61     // Keypad 9 and PageUp
#define KEY_KPAD_0            0x62     // Keypad 0 and Insert
#define KEY_KPAD_DOT          0x63     // Keypad . and Delete
#define KEY_INTL_BACKSLASH    0x64     // Keyboard Non-US \ and |
#define KEY_APPLICATION       0x65     // Keyboard Application
#define KEY_POWER             0x66     // Keyboard Power
#define KEY_KPAD_EQUAL        0x67     // Keypad =
#define KEY_F13               0x68     // Keyboard F13
#define KEY_F14               0x69     // Keyboard F14
#define KEY_F15               0x6A     // Keyboard F15
#define KEY_F16               0x6B     // Keyboard F16
#define KEY_F17               0x6C     // Keyboard F17
#define KEY_F18               0x6D     // Keyboard F18
#define KEY_F19               0x6E     // Keyboard F19
#define KEY_F20               0x6F     // Keyboard F20
#define KEY_F21               0x70     // Keyboard F21
#define KEY_F22               0x71     // Keyboard F22
#define KEY_F23               0x72     // Keyboard F23
#define KEY_F24               0x73     // Keyboard F24
#define KEY_EXECUTE           0x74     // Keyboard Execute
#define KEY_HELP              0x75     // Keyboard Help
#define KEY_MENU              0x76     // Keyboard Menu
#define KEY_SELECT            0x77     // Keyboard Select
#define KEY_STOP              0x78     // Keyboard Stop
#define KEY_AGAIN             0x79     // Keyboard Again
#define KEY_UNDO              0x7A     // Keyboard Undo
#define KEY_CUT               0x7B     // Keyboard Cut
#define KEY_COPY              0x7C     // Keyboard Copy
#define KEY_PASTE             0x7D     // Keyboard Paste
#define KEY_FIND              0x7E     // Keyboard Find
#define KEY_MUTE              0x7F     // Keyboard Mute
#define KEY_VOLUME_UP         0x80     // Keyboard Volume Up
#define KEY_VOLUME_DOWN       0x81     // Keyboard Volume Down
#define KEY_LOCK_CAPS_LOCK    0x82     // Keyboard Locking Caps Lock
#define KEY_LOCK_NUM_LOCK     0x83     // Keyboard Locking Num Lock
#define KEY_LOCK_SCR_LOCK     0x84     // Keyboard Locking Scroll Lock
#define KEY_KPAD_COMMA        0x85     // Keypad Comma
#define KEY_EQUAL_SIGN        0x86     // Keypad Equal Sign
#define KEY_INTL1             0x87     // Keyboard International1
#define KEY_INTL2             0x88     // Keyboard International2
#define KEY_INTL3             0x89     // Keyboard International3
#define KEY_INTL4             0x8A     // Keyboard International4
#define KEY_INTL5             0x8B     // Keyboard International5
#define KEY_INTL6             0x8C     // Keyboard International6
#define KEY_INTL7             0x8D     // Keyboard International7
#define KEY_INTL8             0x8E     // Keyboard International8
#define KEY_INTL9             0x8F     // Keyboard International9
#define KEY_LANG1             0x90     // Keyboard LANG1
#define KEY_LANG2             0x91     // Keyboard LANG2
#define KEY_LANG3             0x92     // Keyboard LANG3
#define KEY_LANG4             0x93     // Keyboard LANG4
#define KEY_LANG5             0x94     // Keyboard LANG5
#define KEY_LANG6             0x95     // Keyboard LANG6
#define KEY_LANG7             0x96     // Keyboard LANG7
#define KEY_LANG8             0x97     // Keyboard LANG8
#define KEY_LANG9             0x98     // Keyboard LANG9
#define KEY_ALT_ERASE         0x99     // Keyboard Alternate Erase
#define KEY_SYSREQ            0x9A     // Keyboard SysReq/Attention
#define KEY_CANCEL            0x9B     // Keyboard Cancel
#define KEY_CLEAR             0x9C     // Keyboard Clear
#define KEY_PRIOR             0x9D     // Keyboard Prior
#define KEY_RETURN            0x9E     // Keyboard Return
#define KEY_SEPARATOR         0x9F     // Keyboard Separator
#define KEY_OUT               0xA0     // Keyboard Out
#define KEY_OPER              0xA1     // Keyboard Oper
#define KEY_CLEAR             0xA2     // Keyboard Clear/Again
#define KEY_CRSEL             0xA3     // Keyboard CrSel/Props
#define KEY_EXSEL             0xA4     // Keyboard ExSel
                                       // Keys 0xA5 to 0xAF reserved
#define KEY_KPAD_00           0xB0     // Keypad 00
#define KEY_KPAD_000          0xB1     // Keypad 000
#define KEY_THOUSANDS_SEP     0xB2     // Thousands Separator
#define KEY_DECIMAL_SEP       0xB3     // Decimal Separator
#define KEY_CURRENCY          0xB4     // Currency Unit
#define KEY_SUB_CURRENCY      0xB5     // Currency Sub-unit
#define KEY_KPAD_LEFT_PAREN   0xB6     // Keypad (
#define KEY_KPAD_RIGHT_PAREN  0xB7     // Keypad )
#define KEY_KPAD_LEFT_BRACE   0xB8     // Keypad {
#define KEY_KPAD_RIGHT_BRACE  0xB9     // Keypad }
#define KEY_KPAD_TAB          0xBA     // Keypad Tab
#define KEY_KPAD_BACKSPACE    0xBB     // Keypad Backspace
#define KEY_KPAD_A            0xBC     // Keypad A
#define KEY_KPAD_B            0xBD     // Keypad B
#define KEY_KPAD_C            0xBE     // Keypad C
#define KEY_KPAD_D            0xBF     // Keypad D
#define KEY_KPAD_E            0xC0     // Keypad E
#define KEY_KPAD_F            0xC1     // Keypad F
#define KEY_KPAD_XOR          0xC2     // Keypad XOR
#define KEY_KPAD_CARET        0xC3     // Keypad ^
#define KEY_KPAD_PERCENT      0xC4     // Keypad %
#define KEY_KPAD_LESS_THAN    0xC5     // Keypad <
#define KEY_KPAD_GREAT_THAN   0xC6     // Keypad >
#define KEY_KPAD_AND          0xC7     // Keypad &
#define KEY_KPAD_DBL_AND      0xC8     // Keypad &&
#define KEY_KPAD_OR           0xC9     // Keypad |
#define KEY_KPAD_DBL_OR       0xCA     // Keypad ||
#define KEY_KPAD_COLON        0xCB     // Keypad :
#define KEY_KPAD_HASH         0xCC     // Keypad #
#define KEY_KPAD_SPACE        0xCD     // Keypad Space
#define KEY_KPAD_AT           0xCE     // Keypad @
#define KEY_KPAD_EXCLAMATION  0xCF     // Keypad !
#define KEY_KPAD_MEM_STORE    0xD0     // Keypad Memory Store
#define KEY_KPAD_MEM_RECALL   0xD1     // Keypad Memory Recall
#define KEY_KPAD_MEM_CLEAR    0xD2     // Keypad Memory Clear
#define KEY_KPAD_MEM_ADD      0xD3     // Keypad Memory Add
#define KEY_KPAD_MEM_SUB      0xD4     // Keypad Memory Subtract
#define KEY_KPAD_MEM_MULTIPLY 0xD5     // Keypad Memory Multiply
#define KEY_KPAD_MEM_DIVIDE   0xD6     // Keypad Memory Divide
#define KEY_PLUS_MINUS        0xD7     // Keypad +/-
#define KEY_CLEAR             0xD8     // Keypad Clear
#define KEY_CLEAR_ENTRY       0xD9     // Keypad Clear Entry
#define KEY_BINARY            0xDA     // Keypad Binary
#define KEY_OCTAL             0xDB     // Keypad Octal
#define KEY_DECIMAL           0xDC     // Keypad Decimal
#define KEY_HEXADECIMAL       0xDD     // Keypad Hexadecimal
                                       // Keys 0xDE to 0xDF reserved
#define KEY_L_CONTROL         0xE0     // Keyboard LeftControl
#define KEY_L_SHIFT           0xE1     // Keyboard LeftShift
#define KEY_L_ALT             0xE2     // Keyboard LeftAlt
#define KEY_L_GUI             0xE3     // Keyboard Left GUI
#define KEY_R_CONTROL         0xE4     // Keyboard RightControl
#define KEY_R_SHIFT           0xE5     // Keyboard RightShift
#define KEY_R_ALT             0xE6     // Keyboard RightAlt
#define KEY_R_GUI             0xE7     // Keyboard Right GUI

=======
#define KEY_A       4
#define KEY_B       5
#define KEY_C       6
#define KEY_D       7
#define KEY_E       8
#define KEY_F       9
#define KEY_G       10
#define KEY_H       11
#define KEY_I       12
#define KEY_J       13
#define KEY_K       14
#define KEY_L       15
#define KEY_M       16
#define KEY_N       17
#define KEY_O       18
#define KEY_P       19
#define KEY_Q       20
#define KEY_R       21
#define KEY_S       22
#define KEY_T       23
#define KEY_U       24
#define KEY_V       25
#define KEY_W       26
#define KEY_X       27
#define KEY_Y       28
#define KEY_Z       29
#define KEY_1       30
#define KEY_2       31
#define KEY_3       32
#define KEY_4       33
#define KEY_5       34
#define KEY_6       35
#define KEY_7       36
#define KEY_8       37
#define KEY_9       38
#define KEY_0       39

#define KEY_ENTER   40

#define KEY_SPACE   44

#define KEY_F1      58
#define KEY_F2      59
#define KEY_F3      60
#define KEY_F4      61
#define KEY_F5      62
#define KEY_F6      63
#define KEY_F7      64
#define KEY_F8      65
#define KEY_F9      66
#define KEY_F10     67
#define KEY_F11     68
#define KEY_F12     69

#define KEY_ARROW_UP        82
#define KEY_ARROW_DOWN      81
#define KEY_ARROW_LEFT      80
#define KEY_ARROW_RIGHT     79
>>>>>>> origin/master

class DigiKeyboardDevice : public Print {
 public:
  DigiKeyboardDevice () {
    cli();
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();


    usbInit();

    sei();

    // TODO: Remove the next two lines once we fix
    //       missing first keystroke bug properly.
    memset(reportBuffer, 0, sizeof(reportBuffer));
    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }

  void update() {
    usbPoll();
  }

	// delay while updating until we are finished delaying
	void delay(long milli) {
		unsigned long last = millis();
	  while (milli > 0) {
	    unsigned long now = millis();
	    milli -= now - last;
	    last = now;
	    update();
	  }
	}

  //sendKeyStroke: sends a key press AND release
  void sendKeyStroke(byte keyStroke) {
    sendKeyStroke(keyStroke, 0);
  }

  //sendKeyStroke: sends a key press AND release with modifiers
  void sendKeyStroke(byte keyStroke, byte modifiers) {
	sendKeyPress(keyStroke, modifiers);
    // This stops endlessly repeating keystrokes:
	sendKeyPress(0,0);
  }

  //sendKeyPress: sends a key press only - no release
  //to release the key, send again with keyPress=0
  void sendKeyPress(byte keyPress) {
	sendKeyPress(keyPress, 0);
  }

  //sendKeyPress: sends a key press only, with modifiers - no release
  //to release the key, send again with keyPress=0
  void sendKeyPress(byte keyPress, byte modifiers) {
   	while (!usbInterruptIsReady()) {
      // Note: We wait until we can send keyPress
      //       so we know the previous keyPress was
      //       sent.
    	usbPoll();
    	_delay_ms(5);
    }

    memset(reportBuffer, 0, sizeof(reportBuffer));

    reportBuffer[0] = modifiers;
    reportBuffer[1] = keyPress;

    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }

  size_t write(uint8_t chr) {
    uint8_t data = pgm_read_byte_near(ascii_to_scan_code_table + (chr - 8));
    sendKeyStroke(data & 0b01111111, data >> 7 ? MOD_SHIFT_RIGHT : 0);
    return 1;
  }

  //private: TODO: Make friend?
 // maximum 6 keystrokes, defined in HID report
  uchar    reportBuffer[7];    // buffer for HID reports [ 1 modifier byte + (len-1) key strokes]
  using Print::write;
};

DigiKeyboardDevice DigiKeyboard = DigiKeyboardDevice();

#ifdef __cplusplus
extern "C"{
#endif
  // USB_PUBLIC uchar usbFunctionSetup
	uchar usbFunctionSetup(uchar data[8]) {
    usbRequest_t    *rq = (usbRequest_t *)((void *)data);

    usbMsgPtr = DigiKeyboard.reportBuffer; //
    if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
      /* class request type */

      if (rq->bRequest == USBRQ_HID_GET_REPORT) {
				/* wValue: ReportType (highbyte), ReportID (lowbyte) */

				/* we only have one report type, so don't look at wValue */
        // TODO: Ensure it's okay not to return anything here?
				return 0;

      } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
				//usbMsgPtr = &idleRate;
				//return 1;
				return 0;

      } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
				idleRate = rq->wValue.bytes[1];

      }
    } else {
      /* no vendor specific requests implemented */
    }

    return 0;
  }
#ifdef __cplusplus
} // extern "C"
#endif


#endif // __DigiKeyboard_h__
