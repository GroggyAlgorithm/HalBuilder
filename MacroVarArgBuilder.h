/**
* \file MacroVarArgBuilder.h
* \author Tim Robbins
* \brief Variable length and optional arguments/parameters with the help of macros. \n
* Links: https://stackoverflow.com/questions/2124339/c-preprocessor-va-args-number-of-arguments,  https://groups.google.com/g/comp.std.c/c/d-6Mj5Lko_s?pli=1 \n
*
* To use this, you must name all of what you'll be calling, whether that's a macro or function, the same name BUT ending with the number of args that one will have. Afterwards, you'll declare a macro that will call the argument builder. \n
*
* ex. A macro to write pins as high or low and optionally set to either input or output \n
*
* #define writeBit(value, bitPosition, bitValue)	((bitValue) ? (value |= (1 << bitPosition)) : (value &= ~(1 << bitPosition))) \n
* #define _WRITE_PINV3(pL, pN, pV)					writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL), pN, pV) \n
* #define _WRITE_PINV4(pL, pN, pV, pD)				writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_WRITE_REG, pL), pN, pV); writeBit(__PIN_UTIL_TOKENIZE_REG_NAME(__PIN_UTIL_PREFIX_DIR_REG, pL), pN, pD) \n
* #define PIN_WRITE(...)							VFUNC(_WRITE_PINV, __VA_ARGS__) \n
* 
*			Walk through. \n
*  \n
*			This works by adding using the amount of arguments passed as padding, allowing us to call a set up numbers that causes N to land on the DIFFERENCE between the padding and the amount of arguments \n
*  \n
*			Pass the function name and the variable arguments \n
*			#define VAR_ARG_TEST(...)	VFUNC(_BUILD_MASK_V, __VA_ARGS__) \n
*  \n
*  \n
*			VAR_ARG_TEST(A, B, C)	-> VFUNC(_BUILD_MASK_V, A, B, C) \n
*			VFUNC					-> _VFUNC(_BUILD_MASK_V, __NUM_ARG__(A, B, C) ) (A, B, C) \n
*  \n
*			Using *** to represent the contents of __RSEQ_N that were not shown \n
*			__NUM_ARG__				-> __NUM_ARG_I_(A, B, C,63,62,***, 1, 0) \n
*			__NUM_ARG_I_			-> __ARG_NUM(A, B, C, 63,62,***, 1, 0) \n
*  \n
*  \n
*			__ARG_NUM				-> \n
*  \n
*			A, B, C, 63, 62, 61, 60, 59, 58, 57, \n
*			56, 55, 54, 53, 52, 51, 50, 49, 48, 47, \n
*			46, 45, 44, 43, 42, 41, 40, 39, 38, 37, \n
*			36, 35, 34, 33, 32, 31, 30, 29, 28, 27, \n
*			26, 25, 24, 23, 22, 21, 20, 19, 18, 17, \n
*			16, 15, 14, 13, 12, 11, 10, 9, 8, 7, \n
*			6, 5, 4, !!!N = 3!!!,2,1,0) N \n
*  \n
*  \n
*  \n
*			_VFUNC					-> _VFUNC_(_BUILD_MASK_V, n) \n
*			_VFUNC_					-> _BUILD_MASK_V##3 = _BUILD_MASK_V3 since the arguments used as padding caused N(in __ARG_NUM) to fall on the amount of arguments using __RSEQ_N \n
*  
*/
#ifndef MACROVARARGBUILDER_H_
#define MACROVARARGBUILDER_H_	1


///Calls num arg i to get the correct count of arguments by filling out any remaining unused spaced in the argument list count by passing the arguments followed by the contents of rseq_n allowing 'N' to fall on the amount of real arguments passed
#define __NUM_ARG__(...)  __NUM_ARG_I_(__VA_ARGS__,__RSEQ_N())

///Calls the arg nums
#define __NUM_ARG_I_(...) __ARG_NUM(__VA_ARGS__)

///Argument numbers, passing the real arguments as
#define __ARG_NUM(						 \
_1, _2, _3, _4, _5, _6, _7, _8, _9,_10,  \
_11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
_21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
_31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
_41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
_51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
_61,_62,_63,N,...) N

///Reversed sequential numbers for possible macro argument lengths
#define __RSEQ_N()			   \
63,62,61,60,                   \
59,58,57,56,55,54,53,52,51,50, \
49,48,47,46,45,44,43,42,41,40, \
39,38,37,36,35,34,33,32,31,30, \
29,28,27,26,25,24,23,22,21,20, \
19,18,17,16,15,14,13,12,11,10, \
9,8,7,6,5,4,3,2,1,0




///Tokenizer for the base function name and the amount of arguments passed
#define _VFUNC_(name, n)		name##n

///Tokenizer caller
#define _VFUNC(name, n)			_VFUNC_(name, n)

///Macro to call different macros based on variable count and names ending with the amount of variables passed
#define VFUNC(_func, ...)		_VFUNC(_func, __NUM_ARG__(__VA_ARGS__)) (__VA_ARGS__)




///Macros for helping to build bit masks and values
#define _BUILD_MASK_V1(b0)										(1 << b0)
#define _BUILD_MASK_V2(b0, b1)									(1 << b0 | 1 << b1)
#define _BUILD_MASK_V3(b0, b1, b2)								(1 << b0 | 1 << b1 | 1 << b2)
#define _BUILD_MASK_V4(b0, b1, b2, b3)							(1 << b0 | 1 << b1 | 1 << b2 | 1 << b3)
#define _BUILD_MASK_V5(b0, b1, b2, b3, b4)						(1 << b0 | 1 << b1 | 1 << b2 | 1 << b3 | 1 << b4)
#define _BUILD_MASK_V6(b0, b1, b2, b3, b4, b5)					(1 << b0 | 1 << b1 | 1 << b2 | 1 << b3 | 1 << b4 | 1 << b5)
#define _BUILD_MASK_V7(b0, b1, b2, b3, b4, b5, b6)				(1 << b0 | 1 << b1 | 1 << b2 | 1 << b3 | 1 << b4 | 1 << b5 | 1 << b6)
#define _BUILD_MASK_V8(b0, b1, b2, b3, b4, b5, b6, b7)			(1 << b0 | 1 << b1 | 1 << b2 | 1 << b3 | 1 << b4 | 1 << b5 | 1 << b6 | 1 << b7)

///Macro for helping to build bit masks and values
#define BUILD_BIT_MASK(...)										VFUNC(_BUILD_MASK_V, __VA_ARGS__)



#endif /* MACROVARARGBUILDER_H_ */