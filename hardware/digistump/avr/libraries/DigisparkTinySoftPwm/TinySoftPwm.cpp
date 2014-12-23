// a Tiny optimized Software PWM Manager (all pins must be part of the same port)
// Only resources RAM/Program Memory of used pins are declared in the code at compilation time.
// based largely on Atmel's AVR136: Low-Jitter Multi-Channel Software PWM Application Note:
// http://www.atmel.com/dyn/resources/prod_documents/doc8020.pdf
// RC Navy 2013
// http://p.loussouarn.free.fr

#include <TinySoftPwm.h> 
 
#define TINY_SOFT_PWM_PORT			PORTB
#define TINY_SOFT_PWM_DDR 			DDRB

#define TINY_SOFT_PWM_CLEAR_PIN(RamIdx)	(PortPwmTo1 &= GET_INV_PIN_MSK(RamIdx))

#define TINY_SOFT_PWM_DECLARE_PIN(Px)		TINY_SOFT_PWM_DDR |= (1<<(Px)); PortPwmMask |= (1<<(Px))

#define GET_INV_PIN_MSK(RamIdx)		((uint8_t)pgm_read_byte(&RamIdxToInvPinMsk[(RamIdx)]))

uint8_t RamIdxToInvPinMsk[] PROGMEM ={
#if (TINY_SOFT_PWM_USES_P0 == 1)
				~(1<<0),
#endif  
#if (TINY_SOFT_PWM_USES_P1 == 1)
				~(1<<1),
#endif  
#if (TINY_SOFT_PWM_USES_P2 == 1)
				~(1<<2),
#endif  
#if (TINY_SOFT_PWM_USES_P3 == 1)
				~(1<<3),
#endif  
#if (TINY_SOFT_PWM_USES_P4 == 1)
				~(1<<4),
#endif  
#if (TINY_SOFT_PWM_USES_P5 == 1)
				~(1<<5),
#endif  
				};
static   uint8_t Compare[TINY_SOFT_PWM_CH_MAX];
volatile uint8_t PwmOrder[TINY_SOFT_PWM_CH_MAX];
static   uint8_t PortPwmMask=0;
volatile uint8_t PortPwmTo1=0x00;
volatile uint8_t PortPwmTo0=0xFF;
static   uint8_t _TickMax=255;

static uint8_t PwmToPwmMax(uint8_t Pwm);

void TinySoftPwm_begin(uint8_t TickMax, uint8_t PwmInit)
{
uint8_t oldSREG = SREG;
  cli();
  // set the direction of the used ports and update PortPwmMask
#if (TINY_SOFT_PWM_USES_P0 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB0);
#endif
#if (TINY_SOFT_PWM_USES_P1 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB1);
#endif
#if (TINY_SOFT_PWM_USES_P2 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB2);
#endif
#if (TINY_SOFT_PWM_USES_P3 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB3);
#endif
#if (TINY_SOFT_PWM_USES_P4 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB4);
#endif
#if (TINY_SOFT_PWM_USES_P5 == 1)
  TINY_SOFT_PWM_DECLARE_PIN(PB5);
#endif
  _TickMax=TickMax;
  PortPwmTo1=PortPwmMask;
  // initialise all channels
  for(uint8_t i=0 ; i<TINY_SOFT_PWM_CH_MAX ; i++)
  {
    Compare[i] = PwmToPwmMax(PwmInit);  // set default PWM values
    PwmOrder[i] = Compare[i]; // set default PWM values
  }
  SREG = oldSREG;
}

void TinySoftPwm_analogWrite(uint8_t PinIdx, uint8_t Pwm)
{
uint8_t RamIdx;

  for(RamIdx=0;RamIdx<TINY_SOFT_PWM_CH_MAX;RamIdx++)
  {
    if(GET_INV_PIN_MSK(RamIdx)==((1<<PinIdx)^0xFF)) break;
  }
  if(RamIdx<TINY_SOFT_PWM_CH_MAX)
  {
    PwmOrder[RamIdx] = PwmToPwmMax(Pwm);
  }
}

static uint8_t PwmToPwmMax(uint8_t Pwm)
{
  return(map(Pwm, 0, 255, 0, _TickMax));
}

void TinySoftPwm_process(void) 
{
static uint8_t OvfCount=0xFF;
 
  PortPwmTo0 = (~PortPwmTo1)^PortPwmMask;
  TINY_SOFT_PWM_PORT |= PortPwmTo1; // update ONLY used outputs to 1 without disturbing the others
  TINY_SOFT_PWM_PORT &= PortPwmTo0; // update ONLY used outputs to 0 without disturbing the others
  if(++OvfCount == _TickMax)
  { // increment modulo 256 counter and update
    // the compare values only when counter = 0.
    OvfCount=0;
#if (TINY_SOFT_PWM_CH_MAX >= 1)
    Compare[0] = PwmOrder[0]; // verbose code for speed
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 2)
    Compare[1] = PwmOrder[1];
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 3)
    Compare[2] = PwmOrder[2];
#endif   
#if (TINY_SOFT_PWM_CH_MAX >= 4)
    Compare[3] = PwmOrder[3];
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 5)
    Compare[4] = PwmOrder[4];
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 6)
    Compare[5] = PwmOrder[5];
#endif   
    PortPwmTo1 = PortPwmMask; // set all port used pins high
  }
  // clear port pin on compare match (executed on next interrupt)
#if (TINY_SOFT_PWM_CH_MAX >= 1)
  if(Compare[0] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(0);
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 2)
  if(Compare[1] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(1);
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 3)
  if(Compare[2] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(2);
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 4)
  if(Compare[3] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(3);
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 5)
  if(Compare[4] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(4);
#endif
#if (TINY_SOFT_PWM_CH_MAX >= 6)
  if(Compare[5] == OvfCount) TINY_SOFT_PWM_CLEAR_PIN(5);
#endif
}
