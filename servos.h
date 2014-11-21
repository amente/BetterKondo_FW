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

void servos_init(void);
void servos_doPosDegree(uint8_t posDegree[16]);
void servos_enable(uint8_t channel);
void servos_disable(uint8_t channel);
void servos_setPos(uint8_t channel,uint16_t value);
void servos_setDegree(uint8_t channel, uint8_t deg);
void servos_setDegreeSmooth(uint8_t channel,float value,float increment);


#endif /* SERVOS_H_ */
