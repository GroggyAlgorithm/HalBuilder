/**
 * \file mcuPinUtils.h
 * \author Tim Robbins
 *
 * \brief Macros and such for pin related utilities
 */
#ifndef MCUPINUTILS_H_
#define MCUPINUTILS_H_ 1


#ifndef MCUPINS_H_
#  error "Include mcuPins.h instead of this file."
#endif


#if defined(__GNUC__) || defined(GCC)

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#endif


#include "MacroVarArgBuilder.h"


#if !defined(_TOKENIZE)
///Helper macro for token pasting
#define _TOKENIZE(x,y)			x##y
#endif

#if !defined(_TOKENIZE3)
///Helper macro for token pasting
#define _TOKENIZE3(x,y,z)		x##y##z
#endif

#if !defined(_PIN_MAKER)
#define _PIN_MAKER(num)       _TOKENIZE(PIN_, num)
#endif



#pragma region MACRO_FUNCTIONS_AND_HELPERS



#ifndef readBit
///returns the values bit at the passed bit position
#define readBit(value, bit) (((value) >> (bit)) & 0x01)
#endif

#ifndef setBit
///Sets the values bit to high at the passed bit position
#define setBit(value, bit) ((value) |= (1UL << (bit)))
#endif

#ifndef clearBit
///Clears the values bit to low at the passed bit position
#define clearBit(value, bit) ((value) &= ~(1UL << (bit)))
#endif

#ifndef writeBit
///Sets the bit at value's bit position to the specified bit value. If bitValue > 0, it sets the bit, else it clears it
#define writeBit(value, bitPosition, bitValue) ((bitValue) ? setBit(value, bitPosition) : clearBit(value, bitPosition))
#endif



#if !defined(__PIN_UTIL_TOKENIZE_REG_NAME)

///Helper for tokenizing 
#define __PIN_UTIL_TOKENIZE_REG_NAME(reg, t)	_TOKENIZE(reg, t)

#endif




#if !defined(__PINS_NEED_CUSTOM_WRITE) || __PINS_NEED_CUSTOM_WRITE != 1

///Abstraction macro for variable argument pin high macros
#define _PIN_HIGHV2(pL, ...)		__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL) |= VFUNC(_BUILDVALV, __VA_ARGS__)

///Abstraction macro for variable argument pin low macros
#define _PIN_LOWV2(pL, ...)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL) &= ~VFUNC(_BUILDVALV, __VA_ARGS__)

///Abstraction macro for variable argument pin input/output macros
#define _PIN_DIR_HIGHV2(pL, ...)	__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) |= VFUNC(_BUILDVALV, __VA_ARGS__)

///Abstraction macro for variable argument pin input/output macros
#define _PIN_DIR_LOWV2(pL, ...)		__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) &= ~(VFUNC(_BUILDVALV, __VA_ARGS__))

///Abstraction macro for port high macros
#define _PORT_DIR_HIGH(pL)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) |= 0xFF

///Abstraction macro for port low macros
#define _PORT_DIR_LOW(pL)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) &= ~(0xFF)

///Abstraction macro for reading pin value macros
#define _READ_PIN(pL, pN)			readBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_READ_REG, pL), pN)

///Abstraction macro for reading port value macros
#define _READ_PORT(pL, ...)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_READ_REG, pL)


///Abstraction macro for getting read registers 
#define _GET_READ(pL, ...)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_READ_REG, pL)

///Abstraction macro for Getting a specific port from letter passed
#define _GET_PORT(pL, ...)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL)

///Abstraction macro for Getting a specific output register from letter passed
#define _GET_OUTPUT_REG(pL, ...)	__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL)

///Abstraction macro for getting a specific direction register from the letter passed
#define _GET_DIR(pL, ...)			__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL)

///Abstraction macro for getting a specific direction register from the letter passed
#define _GET_DIR_REG(pL, ...)		__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL)

///Abstraction macro for getting read registers
#define _GET_READ_REG(pL, ...)		__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_READ_REG, pL)

///Abstraction for setting a port high
#define _SET_PORT(pL, pV)							__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL) |= pV

///Abstraction for setting a port low
#define _CLEAR_PORT(pL, pV)							__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL) &= ~(pV)

///Abstraction for setting a ports direction high
#define _SET_PORT_DIR(pL, pV)						__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) |= pV

///Abstraction for setting a ports direction low
#define _CLEAR_PORT_DIR(pL, pV)						__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL) &= ~(pV)

///Abstraction for toggling a pin
#define _PIN_TOGGLE(pL, ...)                        __PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL) ^= VFUNC(_BUILDVALV, __VA_ARGS__)




#endif





///Abstractions for getting a pin number
#define _PIN_NUMBER(pL, pN)         pN


///Variable argument count macros for getting the pin number from pin utilities. Useful for creating arrays for port positions
#define _PIN_NUMBER_V2(pL1, pN1)                                                                         pN1
#define _PIN_NUMBER_V4(pL1, pN1, pL2, pN2)                                                               pN1, pN2
#define _PIN_NUMBER_V6(pL1, pN1, pL2, pN2, pL3, pN3)                                                     pN1, pN2, pN3
#define _PIN_NUMBER_V8(pL1, pN1, pL2, pN2, pL3, pN3, pL4, pN4)                                           pN1, pN2, pN3, pN4
#define _PIN_NUMBER_V10(pL1, pN1, pL2, pN2, pL3, pN3, pL4, pN4, pL5, pN5)                                pN1, pN2, pN3, pN4, pN5
#define _PIN_NUMBER_V12(pL1, pN1, pL2, pN2, pL3, pN3, pL4, pN4, pL5, pN5, pL6, pN6)                      pN1, pN2, pN3, pN4, pN5, pN6
#define _PIN_NUMBER_V14(pL1, pN1, pL2, pN2, pL3, pN3, pL4, pN4, pL5, pN5, pL6, pN6, pL7, pN7)            pN1, pN2, pN3, pN4, pN5, pN6, pN7
#define _PIN_NUMBER_V16(pL1, pN1, pL2, pN2, pL3, pN3, pL4, pN4, pL5, pN5, pL6, pN6, pL7, pN7, pL8, pN8)  pN1, pN2, pN3, pN4, pN5, pN6, pN7, pN8




#if INPUT == 0x1

///Helper for variable argument pin input macros
#define _PIN_INPUT(pL, ...)		    				_PIN_DIR_HIGHV2(pL, __VA_ARGS__)

///Helper for variable argument pin output macros
#define _PIN_OUTPUT(pL, ...)	    				_PIN_DIR_LOWV2(pL, __VA_ARGS__)


///Helper for port input macros
#define _PORT_INPUT(pL)		        				_PORT_DIR_HIGH(pL)

///Helper for port output macros
#define _PORT_OUTPUT(pL)	        				_PORT_DIR_LOW(pL)


#else

///Helper for variable argument pin macros
#define _PIN_INPUT(pL, ...)							_PIN_DIR_LOWV2(pL, __VA_ARGS__)

///Helper for variable argument pin macros
#define _PIN_OUTPUT(pL, ...)						_PIN_DIR_HIGHV2(pL, __VA_ARGS__)

///Helper for port macros
#define _PORT_INPUT(pL)								_PORT_DIR_LOW(pL)

///Helper for port macros
#define _PORT_OUTPUT(pL)							_PORT_DIR_HIGH(pL)



#endif


///Abstraction Macro for setting a pin high
#define _PIN_HIGH(pL, ...)							_PIN_HIGHV2(pL, __VA_ARGS__)

///Abstraction Macro for setting a pin low
#define _PIN_LOW(pL, ...)							_PIN_LOWV2(pL, __VA_ARGS__)

///Macro for getting a direction register from the literal letter passed(Example: GET_DIRECTION(B) = 0; Sets direction register for port B to 0. Do not pass quotes, only the letter)
#define GET_DIRECTION(...)							_GET_DIR(__VA_ARGS__)

///Macro for getting a port from the literal letter passed(Example: GET_PORT(B) = 0; Sets port B to 0. Do not pass quotes, only the letter)
#define GET_PORT(...)								_GET_PORT(__VA_ARGS__)

///Gets the pin number from predefined pin macros and discards the port letter
#define PIN_NUMBER(...)								_PIN_NUMBER(__VA_ARGS__)

///Gets the pin number from predefined pin macros and discards the port letter
#define PIN_GET_POSITIONS(...)						VFUNC(_PIN_NUMBER_V, __VA_ARGS__)

///Macro for setting a variable amount of pins to input. First argument must be the port letter and any others are the pins to set.
#define PIN_INPUT(...)								_PIN_INPUT(__VA_ARGS__)

///Macro for setting a variable amount of pins to output. First argument must be the port letter and any others are the pins to set.
#define PIN_OUTPUT(...)								_PIN_OUTPUT(__VA_ARGS__)

///Macro for setting a variable amount of pins to high. First argument must be the port letter and any others are the pins to set.
#define PIN_HIGH(...)								_PIN_HIGH(__VA_ARGS__)

///Macro for setting a variable amount of pins to low. First argument must be the port letter and any others are the pins to set.
#define PIN_LOW(...)								_PIN_LOW(__VA_ARGS__)

///Macro for setting a port to input
#define PORT_INPUT_MODE(pL)							_PORT_INPUT(pL)

///Macro for setting a pin to output
#define PORT_OUTPUT_MODE(pL)						_PORT_OUTPUT(pL)

///Abstraction macro for writing a value a ports direction
#define _WRITE_PORT_DIR(pL, pD)						if(pD == INPUT || pD == FULL_INPUT)	PORT_INPUT_MODE(pL); else PORT_OUTPUT_MODE(pL)

///Abstraction macro for writing a value to a pin
#define _WRITE_PINV3(pL, pN, pV)					writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL), pN, pV)

///Abstraction macro for writing a value to a pin and its direction
#define _WRITE_PINV4(pL, pN, pV, pD)				writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL), pN, pV); writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL), pN, pD)

///Abstraction macro for writing a value to a pins direction
#define _WRITE_DIR(pL, pN, pD)						writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL), pN, pD)

///Abstraction macro for writing a value a ports direction
#define _WRITE_PORT_DIR(pL, pD)						if(pD == INPUT || pD == FULL_INPUT)	PORT_INPUT_MODE(pL); else PORT_OUTPUT_MODE(pL)



///Gets the value of the port. Takes the literal letter of the port and discards all other variables
#define PORT_READ(...)								_READ_PORT(__VA_ARGS__)

///Sets the bits(|=) of the port to the value passed
#define PORT_SET(port_letter, val)					_SET_PORT(port_letter, val)

///Clears the bit(&=~) of the port
#define PORT_CLEAR(port_letter, val)				_CLEAR_PORT(port_letter, val)

///Sets the direction(|=) of the port passed
#define PORT_SET_MODE(port_letter, val)				_SET_PORT_DIR(port_letter, val)

///Clears the direction(&=~) of the port passed
#define PORT_CLEAR_MODE(port_letter, val)			_CLEAR_PORT_DIR(port_letter, val)


///Writes a pins value. Takes the text letter for the port(PORTA would be A)
///the pin number, and the value for the pin and optionally the value for the direction
#define PIN_WRITE(...)								VFUNC(_WRITE_PINV, __VA_ARGS__)

///Writes a pins direction. Takes the text letter for the port(PORTA would be A)
///the pin number, and the value for the direction
#define PIN_MODE(port_letter, pin_num, pin_direction)	_WRITE_DIR(port_letter, pin_num, pin_direction)

///Reads the value of the pin passed. Takes the literal letter(PORTA would be A) and the pins position
#define PIN_READ(...)			                _READ_PIN(__VA_ARGS__)

///Toggles a pin. Takes the literal letter(PORTA would be A) and the position of the pin
#define PIN_TOGGLE(...)			                _PIN_TOGGLE(__VA_ARGS__)

///Sets a Ports direction to the direction passed. Takes the literal letter(PORTA would be A)
#define PORT_MODE(port_letter, mode)			_WRITE_PORT_DIR(port_letter, mode)

///Writes a port to the value passed Takes the literal letter(PORTA would be A) and the value.
#define PORT_WRITE(port_letter, val)			GET_PORT(port_letter) = val

///Tokenizes a pin number into the appropriate ordered pin format. Example: Passing in 0 would create PIN_0 if the prefix is "PIN_" and the suffix is blank, which should ultimately be defined as PORT_LETTER, BIT_POSITION
#define PIN(...)            					_PIN_MAKER(__VA_ARGS__) 

#if defined(__AVR)
//From microchip
/*/ should activate the pull up for power saving
		but a bit costly to do it here */
#define DISABLE_PIN(pL, pin)	*((uint8_t *)&__PORT_DIRECT_PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_PORT_REG, pL) + 0x10 + pin) |= 1 << PORT_PULLUPEN_bp

#define PIN_CONTROL(port, pin)	*((uint8_t *)&port + 0x10 + pin)


#if defined(PORT_ISC_gm)
	__attribute__((always_inline)) inline void CONFIGURE_PIN_INPUT_SENSE(volatile uint8_t *port, const uint8_t pin, const uint8_t isc)
	{
		PIN_CONTROL(port, pin) = (PIN_CONTROL(port, pin) & ~PORT_ISC_gm) | isc;
	}
#endif

#ifdef PORT_INVEN_bm
	__attribute__((always_inline)) inline void CONFIGURE_PIN_INVERTED(volatile uint8_t *port, const uint8_t pin, const bool isInverted)
	{
		PIN_CONTROL(port, pin) = (isInverted) ? (PIN_CONTROL(port, pin) | PORT_INVEN_bm) : (PIN_CONTROL(port, pin) | ~PORT_INVEN_bm);
	}

#endif


#endif

#pragma endregion


#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif



#endif /* MCUPINUTILS_H_ */



