# HalBuilder
A builder application and header utilities for building and implementing AVR, PIC, etc MCU pin and hal utilities, helpers, and more

Perfect for both abstracting away gpio functionalities AND for doing so by only COPYING, PASTING, and doing minor editing FROM DATASHEETS

This is an easy to use solution for having a generic HAL(Hardware Abstraction Layer) for GPIO and is the basis of much of my professional work.

Combining this with my (not uploaded) other work, this can be used to create enough abstraction that you can easily create drivers and swap your chipsets.

Why not use something like arduino or something like ST's or Microchips HAL? Honestly, I found this to be easier to use, more reliable, and faster, especially for rapid prototyping.
Much easier to understand and create as well (but that may just be the case since I created this system).

By defining a projects pins as either something predefined, as a class/struct, or by simply using ```#define CONTROL_PIN B,2 //PORTB, PIN 2``` has made my life easier.

This is something you can also expand upon for more periphreal functionality, something I've done with PWM, ADC, a few more definitions and functions, and some copy/paste from a datasheet.

To use, create a text file(or other file) and copy and paste pins from your mcu's data sheet. Super easy. Follow the examples! 
Launch the gui(build it yourself, .net winforms) or terminal app for the pin builder. Afterwards, include your pin file and mcuPins.h in your project.

Quick Example for setting up the parse document, including this in the file will cause these affects

```
<<[IGNORE_LINE:@@@:]>>               Lines begining with @@@ will be ignored
<<[REMOVE:(:):/:*:]>>                Removes these characters when generating
@@@<<[REPLACE:PCINT;TO;PC_INT_:]>>   Reformats these pins when building BUT WAIT, IT STARTS WITH THE IGNORE SEQUENCE!
<<[PIN:PA#A:PB#B:PC#C:PD#D:]>>       Defines standard formatting for pins and their final outputs


<<[ADD_TOP_NEXT:#if defined(__AVR_ATmega1284__) || defined(__AVR_ATmega1284P__):]>> This will add the phrase to the top but at the end of the header, in this case an ifdef gaurd for the atmega controller
<<[ADD_END_PRE:#endif:]>>                                                           This will add the phrase to the bottom of the page for the begining footer, in this case an endif for our ifdef gaurd!

```

### Scroll down to view information on usage!

<hr>

# IMPORTANT INFO FOR BUILDING .NET WINFORMS APP

If this process is too much, I could eventually provide a full download but I probably wont so you can just change the source code if you want
Optionally, if not using windows especially, you can call the python pin builder file with the commands listed.


To build and run:

Since I've only included the source code, once you have the your .net use the command

```
dotnet new winforms -n HalBuilder
```

Then you must add the provided c# source files <h2> MAKING SURE YOU REPLACE THE Program.cs WITH THE ONE PROVIDED </h2>


From here, you can use these build commands

```

For my full build I used
dotnet build --sc -c Release

But you can also just run
dotnet build -c Release

```

Now you can either use MOVE the provided py folder fully or...

Within your Release folder, create a folder name 'py' 

Copy the python source file into this folder

In this folder, we also want to create a virtual enviroment

Example:
```
py -m venv ./venv
```

Your final python path should be ..../py/venv/Scripts/python.exe.

At this point, running the .exe for the GUI app should run just fine

<hr>

<br>

In the GUI app, you will be able to select the configuration files, the main definitions files, the output file, and you can create configuration files with the drop down.

<br>

Check builder.py for class usage or...

<br>

Commands for builder.py:

<br>

```
-f, --file:          Sets the file to read from
-o, --out:           Sets the file to ouput to
-a, --author:        Sets the file author name
-s, --start:         The lowest possible starting pin number(defaults to 0)
-e, --end:           The highest possible ending pin number(defaults to 7)
-h, --help:          Show the help menu
-c, --cmdHelp        Shows the possible commands in the definition file
-i, --config         Adds a path for a command configuration file to scan additional commands from
```


<hr>


Commands for creating your definition files:

<br>


```

<<[CMD:VALUES:HERE:]>>                   Place your commands in these, always ending with : and starting with the command

:                                        Seperator for commands

;TO;                                     Special Seperator conversion commands

;AS;                                     Special Seperator conversion commands

;SYNTAX;                                 Special Seperator indicating individual syntax\n

<<[DEF_SYNTAX::]>>                       Declares the definition (ie #define or other) for the definitions

<<[IGNORE_LINE::]>>                      Defines an ignore line sequence
Example                                <<[IGNORE_LINE:@@@:]>>

<<[REMOVE:::]>>                          Removes the values specified
Example Remove AF and *:               <<[REMOVE:AF:*:]>>

<<[REPLACE:;TO;:]>>                      Replaces the values specified. The replacement is specified with ;TO;
Example Replacing AF with DAF:         <<[REPLACE:AF;TO;DAF:]>>

<<[DEFS:INDICATORS#FORMAT:]>>            Defines the indication for definitions on that line, using # to seperate from the final format(if different)
Example                                <<[PIN:PA#A:PB#B:PC#C:PD#D:PE#E:]>>

<<[ADD_TOP_NEXT::]>>                     Adds the values to the end of the header

<<[ADD_END_NEXT::]>>                     Adds the values to the end of the footer

<<[ADD_TOP_PRE::]>>                      Adds the values to the beginning of the header

<<[ADD_END_PRE::]>>                      Adds the values to the beginning of the footer

<<[CUSTOM_DEF:X;TO;Y;AS;Z:]>>            States a custom definition converters.
Note: for custom syntax include ;SYNTAX; at the end of ;TO;
Example                                <<[CUSTOM_DEF:X;TO;;SYNTAX; uint8_t Y = ;AS;Z;:]>>

<<[PERIPHERAL_DEF:X;TO;Y;AS;Z:]>>        States a custom definition periphreal converters.
Note: for custom syntax include ;SYNTAX; at the end of ;TO;
Example                                <<[PERIPHERAL_DEF:X;TO;;SYNTAX; uint8_t Y = ;AS;Z;:]>>

<<[CUSTOM_SECTION::]>>                   Sets an area that will be created as a custom section with very little formatting, being as close to typed as possible

<<[ORDERED_PREFIX::]>>                   Sets the prefix for values in specified ranges

<<[ORDERED_SUFFIX::]>>                   Sets the suffix for values in specified ranges

<<[DEF_RANGE_START::]>>                  Sets the starting value of a definition range

<<[DEF_RANGE_END::]>>                    Sets the ending value of a definition range

<<[PREFIX::]>>                           A collection of prefixes to create

<<[SUFFIX::]>>                           A collection of suffixs to create


```



<hr>





## Using in C/C++


When your using the pin files, include your generate pin file and address any concerns that could be brought up in the warnings menu, like requiring you create your own definitions because you forgot something or you're using something other than AVR or PIC.


From here, we have many wonderful options

You can create a struct from the pin, or a class, if you want to address them directly by using the things in mcuPins.h

Example:

```

Assuming a pin was created called OCR1A_PIN

GpioPin_t MyOcr1aPin = PIN_TO_GPIO_TYPE(OCR1A_PIN);

GpioPin Ocr1aPinClass(PIN_TO_GPIO_CLASS(OCR1A_PIN));


```




### Short Description of Pin Macros and uses, any that are unmentioned, you can look in the header yourself or just generate doxygen comments


From mcuPinUtils.h

```
PIN_NUMBER and _PIN_NUMBER              Will return the pin number of the pin passed. Use PIN_GET_POSITIONS if you're passing more than 1 pin

using _GET_PORT, _PIN_NUMBER, _GET_DIR, _GET_READ, _GET_OUTPUT_REG, _GET_DIR, _GET_DIR_REG, _GET_READ_REG Will ALL return their connected registers


GET_DIRECTION                           Will return the direction register of the pin passed
GET_PORT                                Will return the output register of the pin passed
PORT_READ                               Will return the read register (also is the value) of the pin passed

PIN_INPUT                               Will set the passed pin to input
PIN_OUTPUT                              Will set the passed pin to output
PIN_HIGH                                Will set the passed pin high
PIN_LOW                                 Will set the passed pin low
PIN_TOGGLE                              Toggles the output state of the pin passed


PIN_MODE                                Use this to write values to the direction register

PIN_WRITE                               Use this to write values to the output register AND optionally set the direction register. Must be in the following defined order
EXAMPLE:  PIN_WRITE(PIN,OUTPUT_VALUE) or PIN_WRITE(PIN,OUTPUT_VALUE, DIRECTION VALUE)

PIN_READ                                Reads the bit value of the pin passed

PORT_MODE                               Allows you to write the whole direction register
PORT_WRITE                              Allows you to write to the whole output regster



CONFIGURE_PIN_INVERTED, CONFIGURE_PIN_INPUT_SENSE, PIN_CONTROL, DISABLE_PIN can only be used on appropriate devices.

```



<hr>


### Additional Information

The macros INPUT and OUTPUT are defined for AVR and PIC, INPUT being 0 and OUTPUT being 1 for AVR, and flipped for PIC

```
INPUT  
OUTPUT
```



<hr>





