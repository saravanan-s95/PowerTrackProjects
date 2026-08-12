/* 
 * File:   main.h
 * Author: Dell
 *
 * Created on 6 August, 2026, 10:44 AM
 */

#ifndef MAIN_H
#define MAIN_H

/* Header Files */
#include <xc.h>
#include <stdio.h>
#include <stdint.h>

/* Configuration Bits */
#pragma config FOSC = HS
#pragma config WDTE = OFF
#pragma config PWRTE = OFF
#pragma config BOREN = ON
#pragma config LVP = OFF
#pragma config CPD = OFF
#pragma config WRT = OFF
#pragma config CP = OFF

/* Crystal Frequency */
#define _XTAL_FREQ 20000000

#endif