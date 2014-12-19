#ifndef TinySoftPwm_h
#define TinySoftPwm_h

// a Tiny optimized Software PWM Manager (all pins must be part of the same port)
// Only resources RAM/Program Memory of used pins are declared in the code at compilation time.
// based largely on Atmel's AVR136: Low-Jitter Multi-Channel Software PWM Application Note:
// http://www.atmel.com/dyn/resources/prod_documents/doc8020.pdf
// RC Navy 2013
// http://p.loussouarn.free.fr

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

/*************************************************/
/* Define here the PIN to use with Tiny Soft PWM */
/*       Unused Pin(s) SHALL be commented        */
/*************************************************/
//#define TINY_SOFT_PWM_USES_P0
#define TINY_SOFT_PWM_USES_P1
#define TINY_SOFT_PWM_USES_P2
//#define TINY_SOFT_PWM_USES_P3 /* /!\ used for USB on DigiSpark: do not use it for PWM if DigiUSB is also used /!\ */
//#define TINY_SOFT_PWM_USES_P4 /* /!\ used for USB on DigiSpark: do not use it for PWM if DigiUSB is also used /!\ */
#define TINY_SOFT_PWM_USES_P5





/*******************************************************************/
/* Do NOT modify below: it's used to optimize RAM and Program size */
/*******************************************************************/
#ifdef TINY_SOFT_PWM_USES_P0
#undef TINY_SOFT_PWM_USES_P0
#define TINY_SOFT_PWM_USES_P0	1
#else
#define TINY_SOFT_PWM_USES_P0	0
#endif

#ifdef TINY_SOFT_PWM_USES_P1
#undef TINY_SOFT_PWM_USES_P1
#define TINY_SOFT_PWM_USES_P1	1
#else
#define TINY_SOFT_PWM_USES_P1	0
#endif

#ifdef TINY_SOFT_PWM_USES_P2
#undef TINY_SOFT_PWM_USES_P2
#define TINY_SOFT_PWM_USES_P2	1
#else
#define TINY_SOFT_PWM_USES_P2	0
#endif

#ifdef TINY_SOFT_PWM_USES_P3
#undef TINY_SOFT_PWM_USES_P3
#define TINY_SOFT_PWM_USES_P3	1
#else
#define TINY_SOFT_PWM_USES_P3	0
#endif

#ifdef TINY_SOFT_PWM_USES_P4
#undef TINY_SOFT_PWM_USES_P4
#define TINY_SOFT_PWM_USES_P4	1
#else
#define TINY_SOFT_PWM_USES_P4	0
#endif

#ifdef TINY_SOFT_PWM_USES_P5
#undef TINY_SOFT_PWM_USES_P5
#define TINY_SOFT_PWM_USES_P5	1
#else
#define TINY_SOFT_PWM_USES_P5	0
#endif

#define TINY_SOFT_PWM_CH_MAX	(TINY_SOFT_PWM_USES_P0 + TINY_SOFT_PWM_USES_P1 + TINY_SOFT_PWM_USES_P2 + TINY_SOFT_PWM_USES_P3 + TINY_SOFT_PWM_USES_P4 + TINY_SOFT_PWM_USES_P5)


void TinySoftPwm_begin(uint8_t TickMax, uint8_t PwmInit);
void TinySoftPwm_analogWrite(uint8_t PinIdx, uint8_t Pwm);
void TinySoftPwm_process(void);

#endif
