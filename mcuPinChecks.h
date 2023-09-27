/**
 * \file mcuPinChecks.h
 * \author Tim Robbins
 *
 * \brief Macros and such for pin related utilities
 */
#ifndef MCUPINCHECKS_H_
#define MCUPINCHECKS_H_	1


#ifndef MCUPINS_H_
#  error "Include mcuPins.h instead of this file."
#endif



#if defined(__GNUC__) || defined(GCC)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif




#if !defined(__PIN_UTIL_PREFIX_READ_REG) && !defined(__PIN_UTIL_PREFIX_DIR_REG) && !defined(__PIN_UTIL_PREFIX_WRITE_REG)

#ifdef __AVR

#include <avr/io.h>


#ifndef INPUT
///Value for input
#define INPUT				0x0
#endif

#ifndef OUTPUT
///Value for output
#define OUTPUT				0x1
#endif


#if defined(VPORTA) || defined(VPORTB) || defined(VPORTC) || defined(VPORTD) || defined(VPORTE) || defined(VPORTF)

#define __PIN_UTIL_PREFIX_VPORT_REG			VPORT
#define __PIN_UTIL_PREFIX_PORT_REG			PORT

///Helper for tokenizing
#define __VPIN_UTIL_TOKENIZE_REG_NAME(reg, t)	_TOKENIZE(__PIN_UTIL_PREFIX_VPORT_REG , _TOKENIZE(t, reg))

///Helper for tokenizing
#define __PORT_DIRECT_PIN_UTIL_TOKENIZE_REG_NAME(reg, t)	_TOKENIZE(reg, t)

///Helper for tokenizing
#define __PIN_UTIL_TOKENIZE_REG_NAME(reg, t)	_TOKENIZE3( VPORT, t, reg)


#if defined(PINS_USE_STRUCTS)
#define __PIN_UTIL_PREFIX_WRITE_REG         .OUT
#define __PIN_UTIL_PREFIX_READ_REG          .IN
#define __PIN_UTIL_PREFIX_DIR_REG           .DIR
#else
#define __PIN_UTIL_PREFIX_READ_REG          _IN
#define __PIN_UTIL_PREFIX_DIR_REG           _DIR
#define __PIN_UTIL_PREFIX_WRITE_REG         _OUT
#endif
#else
#define __PIN_UTIL_PREFIX_READ_REG          PIN
#define __PIN_UTIL_PREFIX_DIR_REG           DDR
#define __PIN_UTIL_PREFIX_WRITE_REG         PORT
#endif
#elif defined(__XC)
#include <xc.h>        /* XC8 General Include File */
#define __PIN_UTIL_PREFIX_READ_REG			PORT
#define __PIN_UTIL_PREFIX_DIR_REG			TRIS
#define __PIN_UTIL_PREFIX_WRITE_REG			LAT


#ifndef INPUT
///Value for input
#define INPUT				0x1
#endif

#ifndef OUTPUT
///Value for output
#define OUTPUT				0x0
#endif

#elif defined(HI_TECH_C)
#include <htc.h>       /* HiTech General Include File */
#define __PIN_UTIL_PREFIX_READ_REG			PORT
#define __PIN_UTIL_PREFIX_DIR_REG			TRIS
#define __PIN_UTIL_PREFIX_WRITE_REG			LAT


#ifndef INPUT
///Value for input
#define INPUT				0x1
#endif

#ifndef OUTPUT
///Value for output
#define OUTPUT				0x0
#endif

#elif defined(__18CXX)
#include <p18cxxx.h>   /* C18 General Include File */
#define __PIN_UTIL_PREFIX_READ_REG			PORT
#define __PIN_UTIL_PREFIX_DIR_REG			TRIS
#define __PIN_UTIL_PREFIX_WRITE_REG			LAT

#ifndef INPUT
///Value for input
#define INPUT				0x1
#endif

#ifndef OUTPUT
///Value for output
#define OUTPUT				0x0
#endif

#elif (defined __XC8)
#include <xc.h>            /* XC8 General Include File */
#define __PIN_UTIL_PREFIX_READ_REG			PORT
#define __PIN_UTIL_PREFIX_DIR_REG			TRIS
#define __PIN_UTIL_PREFIX_WRITE_REG			LAT

#ifndef INPUT
///Value for input
#define INPUT				0x1
#endif

#ifndef OUTPUT
///Value for output
#define OUTPUT				0x0
#endif

#endif

#else
#warning "Pins will need custom definitions for OUTPUT, INPUT, __PIN_UTIL_PREFIX_READ_REG, __PIN_UTIL_PREFIX_DIR_REG, and __PIN_UTIL_PREFIX_WRITE_REG, and probably __PIN_UTIL_TOKENIZE_REG_NAME to use"
#endif




#pragma region PIN_CHECKS



#if defined(PORTA)
#define PIN_A0 A, 0
#define PIN_A1 A, 1
#define PIN_A2 A, 2
#define PIN_A3 A, 3
#define PIN_A4 A, 4
#define PIN_A5 A, 5
#define PIN_A6 A, 6
#define PIN_A7 A, 7
#else
#define PIN_A0  _NO_PIN_FUNC
#define PIN_A1  _NO_PIN_FUNC
#define PIN_A2  _NO_PIN_FUNC
#define PIN_A3  _NO_PIN_FUNC
#define PIN_A4  _NO_PIN_FUNC
#define PIN_A5  _NO_PIN_FUNC
#define PIN_A6  _NO_PIN_FUNC
#define PIN_A7  _NO_PIN_FUNC
#endif



#if defined(PORTB)
#define PIN_B0 B, 0
#define PIN_B1 B, 1
#define PIN_B2 B, 2
#define PIN_B3 B, 3
#define PIN_B4 B, 4
#define PIN_B5 B, 5
#define PIN_B6 B, 6
#define PIN_B7 B, 7
#else
#define PIN_B0 _NO_PIN_FUNC
#define PIN_B1 _NO_PIN_FUNC
#define PIN_B2 _NO_PIN_FUNC
#define PIN_B3 _NO_PIN_FUNC
#define PIN_B4 _NO_PIN_FUNC
#define PIN_B5 _NO_PIN_FUNC
#define PIN_B6 _NO_PIN_FUNC
#define PIN_B7 _NO_PIN_FUNC
#endif



#if defined(PORTC)
#define PIN_C0 C, 0
#define PIN_C1 C, 1
#define PIN_C2 C, 2
#define PIN_C3 C, 3
#define PIN_C4 C, 4
#define PIN_C5 C, 5
#define PIN_C6 C, 6
#define PIN_C7 C, 7
#else
#define PIN_C0 _NO_PIN_FUNC
#define PIN_C1 _NO_PIN_FUNC
#define PIN_C2 _NO_PIN_FUNC
#define PIN_C3 _NO_PIN_FUNC
#define PIN_C4 _NO_PIN_FUNC
#define PIN_C5 _NO_PIN_FUNC
#define PIN_C6 _NO_PIN_FUNC
#define PIN_C7 _NO_PIN_FUNC
#endif



#if defined(PORTD)
#define PIN_D0 D, 0
#define PIN_D1 D, 1
#define PIN_D2 D, 2
#define PIN_D3 D, 3
#define PIN_D4 D, 4
#define PIN_D5 D, 5
#define PIN_D6 D, 6
#define PIN_D7 D, 7
#else
#define PIN_D0 _NO_PIN_FUNC
#define PIN_D1 _NO_PIN_FUNC
#define PIN_D2 _NO_PIN_FUNC
#define PIN_D3 _NO_PIN_FUNC
#define PIN_D4 _NO_PIN_FUNC
#define PIN_D5 _NO_PIN_FUNC
#define PIN_D6 _NO_PIN_FUNC
#define PIN_D7 _NO_PIN_FUNC
#endif



#if defined(PORTE)
#define PIN_E0 E, 0
#define PIN_E1 E, 1
#define PIN_E2 E, 2
#define PIN_E3 E, 3
#define PIN_E4 E, 4
#define PIN_E5 E, 5
#define PIN_E6 E, 6
#define PIN_E7 E, 7
#else
#define PIN_E0 _NO_PIN_FUNC
#define PIN_E1 _NO_PIN_FUNC
#define PIN_E2 _NO_PIN_FUNC
#define PIN_E3 _NO_PIN_FUNC
#define PIN_E4 _NO_PIN_FUNC
#define PIN_E5 _NO_PIN_FUNC
#define PIN_E6 _NO_PIN_FUNC
#define PIN_E7 _NO_PIN_FUNC
#endif



#if defined(PORTF)
#define PIN_F0 F, 0
#define PIN_F1 F, 1
#define PIN_F2 F, 2
#define PIN_F3 F, 3
#define PIN_F4 F, 4
#define PIN_F5 F, 5
#define PIN_F6 F, 6
#define PIN_F7 F, 7
#else
#define PIN_F0 _NO_PIN_FUNC
#define PIN_F1 _NO_PIN_FUNC
#define PIN_F2 _NO_PIN_FUNC
#define PIN_F3 _NO_PIN_FUNC
#define PIN_F4 _NO_PIN_FUNC
#define PIN_F5 _NO_PIN_FUNC
#define PIN_F6 _NO_PIN_FUNC
#define PIN_F7 _NO_PIN_FUNC
#endif



#if defined(PORTG)
#define PIN_G0 G, 0
#define PIN_G1 G, 1
#define PIN_G2 G, 2
#define PIN_G3 G, 3
#define PIN_G4 G, 4
#define PIN_G5 G, 5
#define PIN_G6 G, 6
#define PIN_G7 G, 7
#else
#define PIN_G0 _NO_PIN_FUNC
#define PIN_G1 _NO_PIN_FUNC
#define PIN_G2 _NO_PIN_FUNC
#define PIN_G3 _NO_PIN_FUNC
#define PIN_G4 _NO_PIN_FUNC
#define PIN_G5 _NO_PIN_FUNC
#define PIN_G6 _NO_PIN_FUNC
#define PIN_G7 _NO_PIN_FUNC
#endif



#if defined(PORTH)
#define PIN_H0 H, 0
#define PIN_H1 H, 1
#define PIN_H2 H, 2
#define PIN_H3 H, 3
#define PIN_H4 H, 4
#define PIN_H5 H, 5
#define PIN_H6 H, 6
#define PIN_H7 H, 7
#else
#define PIN_H0 _NO_PIN_FUNC
#define PIN_H1 _NO_PIN_FUNC
#define PIN_H2 _NO_PIN_FUNC
#define PIN_H3 _NO_PIN_FUNC
#define PIN_H4 _NO_PIN_FUNC
#define PIN_H5 _NO_PIN_FUNC
#define PIN_H6 _NO_PIN_FUNC
#define PIN_H7 _NO_PIN_FUNC
#endif



#if defined(PORTI)
#define PIN_I0 I, 0
#define PIN_I1 I, 1
#define PIN_I2 I, 2
#define PIN_I3 I, 3
#define PIN_I4 I, 4
#define PIN_I5 I, 5
#define PIN_I6 I, 6
#define PIN_I7 I, 7
#else
#define PIN_I0 _NO_PIN_FUNC
#define PIN_I1 _NO_PIN_FUNC
#define PIN_I2 _NO_PIN_FUNC
#define PIN_I3 _NO_PIN_FUNC
#define PIN_I4 _NO_PIN_FUNC
#define PIN_I5 _NO_PIN_FUNC
#define PIN_I6 _NO_PIN_FUNC
#define PIN_I7 _NO_PIN_FUNC
#endif



#if defined(PORTJ)
#define PIN_J0 J, 0
#define PIN_J1 J, 1
#define PIN_J2 J, 2
#define PIN_J3 J, 3
#define PIN_J4 J, 4
#define PIN_J5 J, 5
#define PIN_J6 J, 6
#define PIN_J7 J, 7
#else
#define PIN_J0 _NO_PIN_FUNC
#define PIN_J1 _NO_PIN_FUNC
#define PIN_J2 _NO_PIN_FUNC
#define PIN_J3 _NO_PIN_FUNC
#define PIN_J4 _NO_PIN_FUNC
#define PIN_J5 _NO_PIN_FUNC
#define PIN_J6 _NO_PIN_FUNC
#define PIN_J7 _NO_PIN_FUNC
#endif




#pragma endregion








#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif


#endif /* MCUPINCHECKS_H_ */
