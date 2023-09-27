/**
 * \file mcuPins.h
 * \author Tim Robbins
 *
 * \brief Macros, Helpers, and pin related utilities
 */
#ifndef MCUPINS_H_
#define MCUPINS_H_	1


///Macro for general "no pin functionality"
#define _NO_PIN_FUNC    -1


#include "mcuPinChecks.h"
#include "mcuPinUtils.h"


#ifndef GPIO_PIN_REG_TYPE
#define GPIO_PIN_REG_TYPE uint8_t
#endif

#ifndef GPIO_PIN_NUM_TYPE
#define GPIO_PIN_NUM_TYPE uint8_t
#endif

typedef GPIO_PIN_REG_TYPE PinRegister_t;
typedef GPIO_PIN_REG_TYPE PinRegisterData_t;
typedef GPIO_PIN_NUM_TYPE PinOffset_t;


typedef struct GpioPin_t
{
	PinOffset_t pin;
	volatile PinRegister_t* read;
	volatile PinRegister_t* write;
	volatile PinRegister_t* dir;
	void* connectedPeriphreals;
}
GpioPin_t;



#define PIN_TO_GPIO_TYPE(pL, pN) { ((pN), &(_GET_READ_REG(pL)), &(_GET_OUTPUT_REG(pL)), &(_GET_DIR_REG(pL))) }

#ifdef _cplusplus


#define PIN_TO_GPIO_CLASS(pL, pN)	((pN), &(_GET_READ_REG(pL)), &(_GET_OUTPUT_REG(pL)), &(_GET_DIR_REG(pL)))

class GpioPin
{
	
	protected:
	volatile PinRegister_t* read;
	volatile PinRegister_t* write;
	volatile PinRegister_t* dir;
	
	public:
	PinOffset_t pin;
	
	
	GpioPin(PinOffset_t pinPosition,
	volatile PinRegister_t* readRegister,
	volatile PinRegister_t* writeRegister,
	volatile PinRegister_t* directionControlRegister
	)
	{
		pin = pinPosition;
		read = readRegister;
		write = writeRegister;
		dir = directionControlRegister;
	}
	
	
	GpioPin(PinOffset_t pinPosition, const GpioPin otherPin)
	{
		pin = pinPosition;
		read = otherPin.read;
		write = otherPin.write;
		dir = otherPin.dir;
	}
	
	~GpioPin()
	{
		pin = 0;
		read = 0;
		write = 0;
		dir = 0;
	}
	
	inline const PinRegisterData_t ReadFull()
	{
		return *read;
	}
	
	inline PinOffset_t Read()
	{
		return readBit(*read,pin);
	}
	
	inline PinOffset_t GetMask()
	{
		return (1 << pin);
	}
	
	
	inline void Write(bool setHigh)
	{
		writeBit(*write,pin,setHigh);
	}
	
	inline void Write(bool setHigh, bool setInput)
	{
		writeBit(*write,pin,setHigh);
		writeBit(*dir, pin, ((setInput == true) ? INPUT : OUTPUT));
	}
	
	
	inline void WriteDirection(bool setInput)
	{
		writeBit(*dir, pin, ((setInput == true) ? INPUT : OUTPUT));
	}
	
};



#else



#endif



#endif /* MCUPINS_H_ */