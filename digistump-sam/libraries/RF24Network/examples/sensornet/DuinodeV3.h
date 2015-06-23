#ifndef __DUINODE_V3_H__
#define __DUINODE_V3_H__

/**
 * @file DuinodeV3.h
 *
 * Contains hardware definitions for RF Duinode V3 (2V4)
 */

#define PINS_DEFINED 1 
#define __PLATFORM__ "RF Duinode V3/V4 (2V4)"

const int rf_irq = 0;

const int led_red = 3;
const int led_yellow = 4;
const int led_green = 5;
const int button_a = 6;

const int rf_ce = 8;
const int rf_csn = 7;

const int temp_pin = 2; // analog
const int voltage_pin = 3; // analog

// 1.1V internal reference after 1M/470k divider, in 8-bit fixed point
const unsigned voltage_reference = 0x371;

#endif // __DUINODE_V3_H__
// vim:cin:ai:sts=2 sw=2 ft=cpp
