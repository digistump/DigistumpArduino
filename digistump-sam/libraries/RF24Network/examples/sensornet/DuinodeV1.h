#ifndef __DUINODE_V3_H__
#define __DUINODE_V3_H__

/**
 * @file DuinodeV1.h
 *
 * Contains hardware definitions for RF Duinode V1 (3V3)
 */

#define PINS_DEFINED 1 
#define __PLATFORM__ "RF Duinode V1 (3V3)"

const int rf_irq = 0;

const int led_red = 0;
const int led_yellow = 0;
const int led_green = 0;
const int button_a = 0;

const int rf_ce = 8;
const int rf_csn = 9;

const int temp_pin = 2; // analog
const int voltage_pin = 3; // analog

// 1.1V internal reference after 1M/470k divider, in 8-bit fixed point
const unsigned voltage_reference = 0x371;

#endif // __DUINODE_V3_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
