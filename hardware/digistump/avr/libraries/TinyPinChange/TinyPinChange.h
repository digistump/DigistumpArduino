#ifndef TINY_PIN_CHANGE_H
#define TINY_PIN_CHANGE_H 1

/*
* <TinyPinChange>, a library for Pin Change Interrupt by RC Navy (2012)
* Supported device ATmega238P (UNO), ATtiny84, ATtiny85
* 
* http://p.loussouarn.free.fr
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <inttypes.h>

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
/* ATtinyX5 */
#define PIN_CHG_PORT_NB			1
#define DigitalPinToPortIdx(p)	0
#define PC_PIN0					PINB
#define PC_PCMSK0				PCMSK
#else
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
/* ATtinyX4 */
#define PIN_CHG_PORT_NB			2
#define DigitalPinToPortIdx(p)  (((p) <= 7) ? (0) : (((p) <= 10) ? (1) : (0)))
#define PC_PIN0					PINA
#define PC_PCMSK0				PCMSK0
#define PC_PIN1					PINB
#define PC_PCMSK1				PCMSK1
#else
/* UNO */
#define PIN_CHG_PORT_NB			3
#define DigitalPinToPortIdx(p)  (((p) <= 7) ? (2) : (((p) <= 13) ? (0) : (((p) <= 21) ? (1) : (0))))
#define PC_PIN0					PINB
#define PC_PCMSK0				PCMSK0
#define PC_PIN1					PINC
#define PC_PCMSK1				PCMSK1
#define PC_PIN2					PIND
#define PC_PCMSK2				PCMSK2
#endif
#endif

void	TinyPinChange_Init(void);
int8_t  TinyPinChange_RegisterIsr(uint8_t Pin, void(*Isr)(void));
void	TinyPinChange_EnablePin(uint8_t Pin);
void	TinyPinChange_DisablePin(uint8_t Pin);
uint8_t TinyPinChange_GetPinEvent(uint8_t VirtualPortIdx);
uint8_t TinyPinChange_GetPinCurSt(uint8_t VirtualPortIdx);
#define TinyPinChange_PinToMsk(Pin) _BV(digitalPinToPCMSKbit(Pin))

/*******************************************************/
/* Application Programming Interface (API) en Francais */
/*******************************************************/

/*      Methodes en Francais                            English native methods */
#define TinyPinChange_EnregistreFonctionInterruption		TinyPinChange_RegisterIsr
#define TinyPinChange_ActiveBroche				TinyPinChange_EnablePin
#define TinyPinChange_DesactiveBroche				TinyPinChange_DisablePin
#define TinyPinChange_RetourneEvenemenPort			TinyPinChange_GetPinEvent
#define TinyPinChange_RetourneEtatCourantPort			TinyPinChange_GetPinCurSt
#define TinyPinChange_MasqueDeBroche				TinyPinChange_PinToMsk

#endif
