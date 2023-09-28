import datetime
import os
import sys
import getopt

# Default file text
FILE_NAME_TEXT = "/**\n* \\file {0}"
FILE_AUTHOR_TEXT = "\n* \\author {0}"
FILE_BRIEF_TXT = "\n* \\brief Auto Generated Definitions \n*{0}\n*/"
FILE_DATE_TEXT = "\n* \\date {0}"
FILE_AUTHOR_SPACE_REPLACE_SEQUENCE = "???***&&&$$$"

class Builder:
    def __init__(self, filePath:str, outputFile:str=None,
                 fileHeader:str=None, fileFooter:str=None,
                   outputFileType:str=".h", author:str="Auto Builder, Tim Robbins", definitionSyntax:str="#define", additionalBriefText:str="") -> None:
        self.filePath = filePath
        self.outputFile = outputFile
        self.fileHeader = fileHeader
        self.fileFooter = fileFooter
        self.outputFileType = outputFileType
        self.author = author
        self.definitionSyntax = definitionSyntax
        self.additionalBriefText = additionalBriefText
        self.rangeStart:int = None
        self.rangeEnd:int = None
        self.formatting = dict()
        pass

    def _ReadFile(self) -> list[str]:
        text:list[str] = []
        with open(self.filePath, "r") as file:
            text = file.readlines() 
        return text

    def _ReadFromFile(self, path:str) -> list[str]:
        text:list[str] = []
        if os.path.isfile(path):
            with open(path, "r") as file:
                text = file.readlines() 
        return text

    def CanCreateFile(self) -> bool:
        """
        Returns true is the objects file path if gucci, false if not
        """
        # If the defintions file is none or does not exist...
        if self.filePath is None or self.filePath.isspace() or os.path.isfile(self.filePath) == False:
            return False
        # else...
        else:
            return True
    
    def PrintHelpMessage(self) -> None:
        """
        Prints the objects help message
        """
        return
    
    def PrintCommands(self) -> None:
        """
        Prints the objects command options
        """
        return
    
    def _HandleCreateDefinitions(self) -> None:
        """
        Handles the creation of the definitions file for this object
        """
        return
    
    def CreateDefinitions(self, rangeStart:int=None, rangeEnd:int=None) -> None:
        """
        Creates the definitions file for this object
        """
        # If our defintions file is gucci...
        if not self.CanCreateFile():
            print("Defintions file inappropriate: " + self.filePath)
            self.PrintHelpMessage()
            sys.exit(1)
        
        # If the output file name is none...
        if self.outputFile == None:
            # Set to the defintions file name, removing the path section and file type
            self.outputFile = str(os.path.basename(self.filePath)).split('.')[0]
            outputName = self.outputFile
            self.outputFile = self.outputFile + self.outputFileType

        # else...
        else:
            outputName = os.path.basename(self.outputFile).split('.')[0]
            outputPath = self.outputFile.split(outputName)[0]
            print("\f\n\n")
            print(outputPath)
            print(outputName)
            # outputPath = os.path.dirname(self.outputFile)
            self.outputFile = outputPath + outputName + self.outputFileType
            # self.outputFile = os.path.join(outputPath, outputName) + self.outputFileType

        if FILE_AUTHOR_SPACE_REPLACE_SEQUENCE in self.author:
            self.author = self.author.replace(FILE_AUTHOR_SPACE_REPLACE_SEQUENCE, " ")

        self.fileHeader = FILE_NAME_TEXT.format(outputName) + FILE_AUTHOR_TEXT.format(self.author) + FILE_DATE_TEXT.format(str(datetime.datetime.now()) + FILE_BRIEF_TXT.format(self.additionalBriefText))

        self.fileFooter = "#endif"
        defGaurd = outputName.upper() + "_" + self.outputFileType.upper().replace(".","") + "_"
        self.fileHeader += "\n#ifndef "+ defGaurd
        self.fileHeader += "\n#define "+ defGaurd + " 1\n\n"

        print("\nCreating file\n{0}\n\nFrom definitions\n{1}\n".format(self.outputFile, self.filePath))
        self.rangeStart = rangeStart
        self.rangeEnd = rangeEnd
        self._HandleCreateDefinitions()
        return

    def _CreateNumericDefsInRange(self, definitions:list[str], finalFormatting:dict=None) -> list[(str,str)]:
        """
        Takes definitions and formats them into a collection of definitions from number start to number end \n

        Arguments:
        -----------------------------
        rangeStart - The starting value for the definition \n
        rangeEnd - The ending value for the definition \n

        Return:
        -----------------------------
        -> list[(str,str)] - The new list of definitions with the first string being the formatted definition name and the second being the formatted definitions \n
        """
        retDef:list[(str,str)] = []
        for de in definitions:
            defStr = de
            if(finalFormatting is not None):
                if finalFormatting.get(de):
                    defStr = finalFormatting[de]
            if self.rangeEnd and (int)(self.rangeEnd) > 0 and self.rangeStart and (int)(self.rangeStart) >= 0:
                for num in range((int)(self.rangeStart),(int)(self.rangeEnd)+1):
                    newDef = (de+str(num), defStr + ", " + str(num))
                    retDef.append(newDef)
            else:
                newDef = (de, defStr)
                retDef.append(newDef)
        return retDef

    def _AppendCommand(self, collection:list[str], cmd:list[str], appendNextLine:bool = False) -> list[str]:
        valueCollection = collection
        for i in range(1, len(cmd)):
            if cmd[i].endswith("]>>"):
                break
            if appendNextLine == True:
                valueCollection.append("\n")
            valueCollection.append(cmd[i])
        return valueCollection
    
    def _EditStringCmd(self, stringToEdit:str, cmd:list[str], isPreText:bool) -> str:
        newString = stringToEdit
        if isPreText == True:
            for j in range(1, len(cmd)):
                if cmd[j].endswith("]>>"):
                    break
                newString = cmd[j] + '\n' + newString
        else:
            for j in range(1, len(cmd)):
                if cmd[j].endswith("]>>"):
                    break
                newString += '\n' + cmd[j]    
        return newString

    def _ReplaceCommand(self, collection:list[(str,str)], cmd:list[str]) -> list[(str,str)]:
        replacementCollection = collection
        for j in range(1, len(cmd)):
            if cmd[j].endswith("]>>"):
                break
            splitted = cmd[j].split(";TO;")
            if len(splitted) >= 2:
                replacement = (splitted[0], splitted[1])
                replacementCollection.append(replacement)
        return replacementCollection
    
    def _CustomDefCommand(self, collection:list[(str,str,str)], cmd:list[str]) -> list[(str,str,str)]:
        newCollection = collection
        for j in range(1, len(cmd)):
            if cmd[j].endswith("]>>"):
                break
            splitted = cmd[j].split(";TO;")
            if len(splitted) >= 2:
                extremeSplit = splitted[1].split(";AS;")
                replacement = (splitted[0], extremeSplit[0], extremeSplit[1])
                newCollection.append(replacement)
        return newCollection



class HalBuilder(Builder):

    def __init__(self, filePath: str, outputFile: str = None, configFilePath:str = None , fileHeader: str = None, fileFooter: str = None, outputFileType: str = ".h", author: str = "Auto Hal Builder, Tim Robbins", definitionSyntax: str = "#define", additionalBriefText: str = "") -> None:
        super().__init__(filePath, outputFile, fileHeader, fileFooter, outputFileType, author, definitionSyntax, additionalBriefText)
        self.removeValues:list[str] = []
        self.replaceValues:list[(str,str)] = []
        self.prefixes:list[str] = []
        self.suffixes:list[str] = []
        self.ignoreIndicators:list[str] = []
        self.orderedPrefix = None
        self.orderedSuffix = None
        self.customSections:list[str] = []
        self.customDef:list[(str,str,str)] = []
        self.peripheral:list[(str,str,str)] = []
        self.configFiles:list[str] = []
        if configFilePath != None:
            self.configFiles.append(configFilePath)
        pass



    def cmdCreate(self,argv):
        try:
            opts,args=getopt.getopt(argv, 'hcf:o:a:s:e:i:',['help','cmdHelp','file=','out=', 'author=', 'start=','end=', 'config='])
        except:
            self.PrintHelpMessage()
            sys.exit(2)
        for opt, arg in opts:
            if(opt == '-h' or opt == '--help'):
                self.PrintHelpMessage()
                sys.exit(0)
            elif(opt == '-i' or opt == '--config'):
                self.configFiles.append(arg)
            elif(opt == '-c' or opt == '--cmdHelp'):
                self.PrintCommands()
                sys.exit(0)
            elif(opt == '-s' or opt == '--start'):
                self.rangeStart = int(arg)
            elif(opt == '-e' or opt == '--end'):
                self.rangeEnd = int(arg)
            elif(opt == '-f' or opt =='--file'):
                self.filePath = arg
            elif(opt == '-o' or opt == '--out'):
                self.outputFile=arg
            elif(opt == '-a' or opt == '--author'):
                self.author=arg
            else:
                print('{0} : {1} : Command not recognized'.format(opt,arg))
                self.PrintHelpMessage()
                sys.exit(2)
        if os.path.exists(self.filePath):
            self.CreateDefinitions(self.rangeStart,self.rangeEnd)
        else:
            print("Could not find file: " + self.filePath)
            sys.exit(1)



    def _ScanForCommands(self, text:list[str]) -> list[str]:
        """
        Scans the passed text for known commands
        """
        savedI:int = 0
        definitionLines:list[str] = []
        for i in range(0, len(text)):
            text[i] = text[i].strip()
            text[i] = text[i].strip('\n')
            text[i] = text[i].strip('\r')
            if text[i].startswith("<<["):
                cmd =  text[i].split(":")
                if "REMOVE" in cmd[0]:
                    self.removeValues = self._AppendCommand(self.removeValues,cmd,False)
                elif "CUSTOM_DEF" in cmd[0]:
                    self.customDef = self._CustomDefCommand(self.customDef, cmd)
                elif "PERIPHERAL_DEF" in cmd[0]:
                    self.peripheral = self._CustomDefCommand(self.peripheral, cmd)
                elif "DEFS" in cmd[0]:
                    for j in range(1, len(cmd)):
                        if cmd[j].endswith("]>>"):
                            break
                        newc = cmd[j].split("#")[0]
                        newL = cmd[j].split("#")[1]
                        if newL and not newL.isspace() and newL is not newc:
                            self.formatting[newc] = newL
                        definitionLines.append(newc)
                elif "CUSTOM_SECTION" in cmd[0]:
                    savedI = i
                    for j in range(i+1, len(text)):
                        if "]>>" in text[j]:
                            break
                        self.customSections.append(text[j])
                        savedI+=1
                elif "ORDERED_PREFIX" in cmd[0]:
                    self.orderedPrefix = cmd[1]
                elif "ORDERED_SUFFIX" in cmd[0]:
                    self.orderedSuffix = cmd[1]
                elif "DEF_RANGE_START" in cmd[0]:
                    if self.rangeStart == None:
                        self.rangeStart = cmd[1]
                elif "DEF_RANGE_END" in cmd[0]:
                    if self.rangeEnd == None:
                        self.rangeEnd = cmd[1]
                elif "DEF_SYNTAX" in cmd[0]:
                    self.definitionSyntax = cmd[1]
                elif "PREFIX" in cmd[0]:
                    self.prefixes = self._AppendCommand(self.prefixes,cmd,False)
                elif "SUFFIX" in cmd[0]:
                    self.suffixes = self._AppendCommand(self.suffixes,cmd,False)
                elif "REPLACE" in cmd[0]:
                    self.replaceValues = self._ReplaceCommand(self.replaceValues,cmd)
                elif "IGNORE_LINE" in cmd[0]:
                    self.ignoreIndicators = self._AppendCommand(self.ignoreIndicators,cmd)
                elif "ADD_TOP_NEXT" in cmd[0]:
                    self.fileHeader = self._EditStringCmd(self.fileHeader,cmd,False)
                elif "ADD_END_NEXT" in cmd[0]:
                    self.fileFooter = self._EditStringCmd(self.fileFooter,cmd,False)
                elif "ADD_TOP_PRE" in cmd[0]:
                    self.fileHeader = self._EditStringCmd(self.fileHeader,cmd,True)
                elif "ADD_END_PRE" in cmd[0]:
                    self.fileFooter = self._EditStringCmd(self.fileFooter,cmd,True)
        return definitionLines
    
    
    
    def _HandleCreateDefinitions(self):
        fileText = self._ReadFile()
        reformattedText:list[str] = []
        if len(fileText) > 0:
            defLines = self._ScanForCommands(fileText)
            if len(self.configFiles) > 0:
                for file in self.configFiles:
                    print("Using Config File: " + file + "...")
                    configText = self._ReadFromFile(file)
                    configCommands = self._ScanForCommands(configText)
                    for cmd in configCommands:
                        defLines.append(cmd)
            if len(defLines) > 0:
                print("\nCreating Definitions...")
                baseDefs = self._CreateNumericDefsInRange(defLines, self.formatting)
                defIndex = 0
                if self.fileHeader:
                    reformattedText.append(self.fileHeader)
                reformattedText.append("\n\n")
                usedCustomDef = []
                for i in range(0, len(fileText)):
                    for ig in self.ignoreIndicators:
                        if fileText[i].lstrip().startswith(ig):
                            fileText[i] = " "
                            break
                    if not fileText[i].startswith("<<[") and not fileText[i].endswith("]>>") and not fileText[i].isspace() and fileText[i]:
                        foundDef = False
                        currentLine: str = fileText[i]
                        for remover in self.removeValues:
                            currentLine = currentLine.replace(remover, " " )
                        for v in self.replaceValues:
                            if v[0] in currentLine:
                                currentLine = currentLine.replace(v[0],v[1])
                        for cust in self.customDef:
                            if cust[0] in currentLine:
                                if ";SYNTAX;" in cust[1]:
                                    custLine = ((cust[1]).ljust(40) + cust[2])
                                    custLine = custLine.replace(";SYNTAX;","")
                                else:
                                    custLine = ((self.definitionSyntax + " " + cust[1]).ljust(40) + cust[2])
                                reformattedText.append(custLine + "\n")
                                currentLine = currentLine.replace(v[0],v[1])
                        for p in baseDefs:
                            pCount = 0
                            if p[0] in currentLine:
                                currentLine = currentLine.strip(p[0])
                                currentLine = currentLine.strip()
                                for periph in self.peripheral:
                                    if ";SYNTAX;" in periph[1]:
                                        custLine = ((periph[1]).ljust(40) + periph[2] + "\n")
                                        custLine = custLine.replace(";SYNTAX;","")
                                    else:
                                        custLine = ((self.definitionSyntax + " " + periph[1]).ljust(40) + periph[2] + "\n")
                                    if periph[0] in currentLine and custLine not in reformattedText:
                                        reformattedText.append(custLine )
                                        if ";SYNTAX;" in periph[1]:
                                            continue
                                        for prefix in self.prefixes:
                                            reformattedText.append(((self.definitionSyntax + " " + prefix + p[0] + "_PERIPHERAL_"+ str(pCount)).ljust(40) + periph[2]) + "\n")
                                            for suffix in self.suffixes:
                                                if prefix or suffix:
                                                    reformattedText.append(((self.definitionSyntax + " "  + prefix + periph[1] + suffix ).ljust(40) + p[1]) + "\n")
                                                else:
                                                    reformattedText.append(((self.definitionSyntax + " PERIPHERAL_" + prefix + periph[1] + suffix ).ljust(40) + p[1]) + "\n")
                                        pCount = pCount + 1
                                    
                                foundDef = True
                                lines = currentLine.split(" ")
                                for prefix in self.prefixes:
                                    for suffix in self.suffixes:
                                        namedDef = ((self.definitionSyntax + " " + prefix + p[0] + suffix).ljust(40) + p[1])
                                        if namedDef not in reformattedText:
                                            reformattedText.append(namedDef + "\n")
                                        if self.rangeEnd is not None and (int)(self.rangeEnd) > 0:
                                            indexDef = self.definitionSyntax + " "
                                            if self.orderedPrefix and not self.orderedPrefix.isspace():
                                                indexDef = str(self.definitionSyntax + " " + self.orderedPrefix + str(defIndex) + self.orderedSuffix + " ").ljust(40) + p[1]
                                            if indexDef not in reformattedText:
                                                reformattedText.append(indexDef + "\n")
                                defIndex += 1
                                for li in range(0, len(lines)):
                                    line = lines[li]
                                    if line.strip() and not line.isspace():
                                        for prefix in self.prefixes:
                                            for suffix in self.suffixes:
                                                newLine = self.definitionSyntax + " " + prefix
                                                newLine += line.strip()
                                                newLine += suffix
                                                newLine = newLine.ljust(40) + p[1]
                                                if newLine not in reformattedText:
                                                    reformattedText.append(newLine + "\n")
                                reformattedText.append("\n")

                            if foundDef:
                                break
                reformattedText.append("\n")

                for section in self.customSections:
                    reformattedText.append(section)
                reformattedText.append("\n\n")

                
                if self.rangeEnd and (int)(self.rangeEnd) > 0 and self.rangeStart and (int)(self.rangeStart) >= 0:
                    reformattedText += "\n\n//Pin Utils\n\n#define ORDERED_PREFIX " + self.orderedPrefix + "\n"
                    reformattedText += "#define ORDERED_SUFFIX " + self.orderedSuffix + "\n"
                    reformattedText += "#define _HAL_MAKER_TOKENIZER(a,b) a##b \n"
                    reformattedText += "#define _HAL_MAKER_TOKENIZER3(a,b,c) a##b##c \n"
                    reformattedText += "#define _HAL_PIN_V_3(a,b,c) _HAL_MAKER_TOKENIZER3(a, b, c) \n"
                    reformattedText += "#define _HAL_PIN(num)       _HAL_PIN_V_3(ORDERED_PREFIX, num, ORDERED_SUFFIX) \n"
                    reformattedText += "\n///Tokenizes a pin number into the appropriate ordered pin format. Example: Passing in 0 would create PIN_0 if the prefix is \"PIN_\" and the suffix is blank, which should ultimately be defined as PORT_LETTER, BIT_POSITION\n"
                    reformattedText += "#define PIN(...)            _HAL_PIN(__VA_ARGS__) \n\n\n"
                    
                

                if self.fileFooter is not None:
                    reformattedText.append(self.fileFooter)
                if self.outputFile is None:
                    for line in reformattedText:
                        print(line)
                else:
                    splitFilePath = os.path.split(self.outputFile)
                    if self.outputFile == None or os.path.isabs(self.outputFile) == False or os.path.exists(self.outputFile) == False:
                        if os.path.isabs(splitFilePath[0]) == True and os.path.exists(splitFilePath[0]) == True:
                            self.outputFile = splitFilePath[0] + "/" + splitFilePath[1]
                            outputPath = self.outputFile
                        else:
                            outputPath = os.path.join(os.path.dirname(sys.argv[0]) , self.outputFile)
                    else:
                        if os.path.isabs(splitFilePath[0]) == True and os.path.exists(splitFilePath[0]) == True:
                            self.outputFile = splitFilePath[0] + "/" + splitFilePath[1]
                        outputPath = self.outputFile

                    try:
                        print("\nSaving file...\n" + outputPath)
                        with open(outputPath, "w") as file:
                            file.writelines(reformattedText)
                        print("\nFile Saved: \n" + outputPath)
                    except:
                        print("\nSave failed: " + outputPath)
                        sys.exit(3)
        else:
            print("\nFailed!")
        return
    
     
    def PrintHelpMessage(self) -> None:
        """
        Prints the objects help message
        """
        print('\n\HAL BUILDER')
        print('------------------------------------------------------------------------')
        print("\nCreates a file for HAL naming and helping")
        print('\n-f, --file:          Sets the file to read from')
        print('\n-o, --out:           Sets the file to ouput to')
        print('\n-a, --author:        Sets the file author name')
        print('\n-s, --start:         The lowest possible starting range number(defaults to 0)')
        print('\n-e, --end:           The highest possible ending range number(defaults to 7)')
        print('\n-h, --help:          Show this help menu')
        print('\n-c, --cmdHelp        Shows the possible commands in the definition file')
        print('\n-i, --config         Adds a path for a command configuration file to scan additional commands from')
        print('\nYou can also pass variables from another module\nExample: ')
        print('cmds = [\'-f\',\'Yeller.txt\',\'-o\',\'Feller.h\']')
        print('cmdCreate(cmds)')
        print('\n------------------------------------------------------------------------\n\n')

    
    def PrintCommands(self) -> None:
        """
        Prints the objects command options
        """
        print('\n\HAL BUILDER FILE COMMANDS')
        print('\nAll commands should be formatted as <<[0:1:2]>>. 0 would be the command, 1 would be the definition, and 2 would be left blank, indicating the command is over\n')
        print('------------------------------------------------------------------------')
        print('\n<<[CMD:VALUES:HERE:]>>                   Place your commands in these, always ending with : and starting with the command')
        print('\n:                                        Seperator for commands')
        print('\n;TO;                                     Special Seperator conversion commands')
        print('\n;AS;                                     Special Seperator conversion commands')
        print('\n;SYNTAX;                                 Special Seperator indicating individual syntax\n')
        print('\n<<[DEF_SYNTAX::]>>                       Declares the definition (ie #define or other) for the definitions')
        print('\n<<[IGNORE_LINE::]>>                      Defines an ignore line sequence')
        print('\n\tExample                                <<[IGNORE_LINE:@@@:]>>')
        print('\n<<[REMOVE:::]>>                          Removes the values specified')
        print('\n\tExample Remove AF and *:               <<[REMOVE:AF:*:]>>')
        print('\n<<[REPLACE:;TO;:]>>                      Replaces the values specified. The replacement is specified with ;TO;')
        print('\n\tExample Replacing AF with DAF:         <<[REPLACE:AF;TO;DAF:]>>')
        print('\n<<[DEFS:INDICATORS#FORMAT:]>>            Defines the indication for definitions on that line, using # to seperate from the final format(if different)')
        print('\n\tExample                                <<[PIN:PA#A:PB#B:PC#C:PD#D:PE#E:]>>')
        print('\n<<[ADD_TOP_NEXT::]>>                     Adds the values to the end of the header')
        print('\n<<[ADD_END_NEXT::]>>                     Adds the values to the end of the footer')
        print('\n<<[ADD_TOP_PRE::]>>                      Adds the values to the beginning of the header')
        print('\n<<[ADD_END_PRE::]>>                      Adds the values to the beginning of the footer')
        print('\n<<[CUSTOM_DEF:X;TO;Y;AS;Z:]>>            States a custom definition converters.')
        print('\n\tNote: for custom syntax include ;SYNTAX; at the end of ;TO;')
        print('\n\tExample                                <<[CUSTOM_DEF:X;TO;;SYNTAX; uint8_t Y = ;AS;Z;:]>>')
        print('\n<<[PERIPHERAL_DEF:X;TO;Y;AS;Z:]>>        States a custom definition periphreal converters.')
        print('\n\tNote: for custom syntax include ;SYNTAX; at the end of ;TO;')
        print('\n\tExample                                <<[PERIPHERAL_DEF:X;TO;;SYNTAX; uint8_t Y = ;AS;Z;:]>>')
        print('\n<<[CUSTOM_SECTION::]>>                   Sets an area that will be created as a custom section with very little formatting, being as close to typed as possible')
        print('\n<<[ORDERED_PREFIX::]>>                   Sets the prefix for values in specified ranges')
        print('\n<<[ORDERED_SUFFIX::]>>                   Sets the suffix for values in specified ranges')
        print('\n<<[DEF_RANGE_START::]>>                  Sets the starting value of a definition range')
        print('\n<<[DEF_RANGE_END::]>>                    Sets the ending value of a definition range')
        print('\n<<[PREFIX::]>>                           A collection of prefixes to create')
        print('\n<<[SUFFIX::]>>                           A collection of suffixs to create')
        print('\n------------------------------------------------------------------------\n\n')

    def CreatePeriphrealFile(self, name:str):
        return



if __name__ == '__main__':
    
    newBuilder = HalBuilder("")
    newBuilder.cmdCreate(sys.argv[1:])
    sys.exit(0)