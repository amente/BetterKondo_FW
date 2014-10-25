/*
 * servos.h
 *
 *  Created on: Sep 7, 2014
 *      Author: Amente
 */
#ifndef SERVOS_H_
#define SERVOS_H_

#include <stdint.h>

#define SERVO_MIN_POS 175    // Based on servo minimum duty period, which is 1ms
#define SERVO_CENTER_POS 375  // servo center duty period is 1.5ms
#define SERVO_MAX_POS 575  // servo max duty period is 2ms

#define PWM_GEN_MODE_A 0xC8
#define PWM_GEN_MODE_B 0xC08
#define PWM_FREQ 50

#define CH1_HOME    375
#define CH2_HOME    560
#define CH3_HOME    575
#define CH4_HOME    375
#define CH5_HOME    375
#define CH6_HOME    375
#define CH7_HOME    375
#define CH8_HOME    375
#define CH9_HOME    375
#define CH10_HOME    375
#define CH11_HOME    500
#define CH12_HOME    375
#define CH13_HOME    375
#define CH14_HOME    190
#define CH15_HOME    390
#define CH16_HOME    375

void servos_init(void);
void servos_doPosDegree(uint8_t posDegree[16]);
void servos_enable(uint8_t channel);
void servos_disable(uint8_t channel);
void servos_setPos(uint8_t channel,uint16_t value);
void servos_setDegree(uint8_t channel,float value);
void servos_setDegreeSmooth(uint8_t channel,float value,float increment);


#endif /* SERVOS_H_ */
