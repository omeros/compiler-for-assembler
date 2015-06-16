//**********************************************************************************//
// This header file is used to Implementat all the main functions that being used   //
// in the different other files.						    //
//**********************************************************************************//

#include "defenitions.h"

// this function get a pointer to a string and skipping it's first spaces.
void SkipSpacesInLine(char** line)
{
	int i=0;
	int len= strlen(*line);

	while(*line && i<len && *(*line+i) != '\0' && *(*line+i)==' ')
		i++;

	*line=*line+i;	
}


// Count the chars in line
int CharsInLine(char* line)
{
	int i=0,charsCount=0;

	while(line[i]!='\"')
		i++;
	i++;
	while(line[i]!='\"') 
	{
		i++;
		charsCount++;
	}

	return ++charsCount;
}

// Count the number of variables in line
int DataCounter(char* line)
{
	int i=0,dataCount=0;

	while(i<strlen(line))
	{
		while(line[i]!=',' && line[i]!=' ' && i<strlen(line))
		{
			i++;
		}
		while(line[i]!=',' && i<strlen(line))
			i++;
		dataCount++;
		i++;
	}

	return dataCount;
}



// the function get the command string and returns it's 
// index in opcode array. if not exist returns -1.
int GetCommandBin(char* commandName)
{
	int i;

	for(i=0;i<COMMANDS_AMOUNT;i++)
		if(!strcmp(commandsArr[i],commandName))
			return i;

	return -1;
}

// get register and return the number of the register
// if its not register return -1
int GetRegisterIndex(char* registerName)
{
	int i;

	for(i=0;i<REGISTERS_AMOUNT;i++)
		if(!strcmp(registersArr[i],registerName))
		{
			if (i>7)
				i-=8;
			return i;
		}
	return -1;
}



// The procedure get line and return the 
// number of lines at the ob file for the current command that contains one operand
int OneOperandHandler(char* line)
{
	int numberOfLines=1;
	char operand[MAX_STRING_LEN];

	if(!line) 
	{
		errorFlag=ERROR_ON;
	}

	sscanf(line,"%s",operand);

	if(GetRegisterIndex(operand) == -1)
		numberOfLines++;

	if (IsContainIndex(line) != -1)
	{
		numberOfLines++;
	}

	return numberOfLines;
}

// The procedure get line and return the 
// number of lines at the ob file for the current command that contains two operands
int TwoOperandsHandler(char* line)
{
	int i=0;
	int numberOfLines=1;
	char operand[50];

	while(i<strlen(line) && line[i]!=' ' && line[i]!=',')
		i++;

	strncpy(operand,line,i);
	operand[i]='\0';
	if(GetRegisterIndex(operand) == -1)
		numberOfLines++;

	if(strlen(line) == i) 
	{
		errorFlag=ERROR_ON;
		printf("Error- The opcode is illegal, expecting for 2 operands.\n");
	}
	
	while(i<strlen(line) && line[i] != ',')
	{
		if(line[i] != ' ') 
		{
			errorFlag=ERROR_ON;
			printf("Error- Illegal command, expecting to have a comma that separated the operands.\n");
		}
		i++;
	}
	
	if(strlen(line) == i) 
	{
		errorFlag=ERROR_ON;
		printf("Error- The opcode is illegal, expecting for 2 operands.\n");
	}

	SkipSpacesInLine(&line);
	
	if(!line) 
	{
		errorFlag=ERROR_ON;
		printf("Error- The opcode is illegal, expecting for 2 operands.\n");
	}

	sscanf(line+i+1,"%s",operand);
	if(GetRegisterIndex(operand) == -1)
		numberOfLines++;

	if (IsContainIndex(line) != -1)
	{
		numberOfLines++;
	}

	return numberOfLines;
}


// The method function returns the number of lines to decode for the current command 
int LinesAllocator(char* line)
{
	char temp[20];
	int opcode = 0;

	SkipSpacesInLine(&line);
	sscanf(line,"%s",temp);

	line+=strlen(temp);
	SkipSpacesInLine(&line);

	opcode = GetCommandBin(temp);

	if((opcode>=0 && opcode<=3) || opcode==6)
		return TwoOperandsHandler(line);
	else if((opcode==4 || opcode==5) || opcode>=7 && opcode<=13)
		return OneOperandHandler(line);
	else if(opcode==14 || opcode==15)
		return 1;

	errorFlag=ERROR_ON;
	return -1;

}



//Gets: string with operand
//Return:1 if the string operand is index or -1 if he isn't
int IsContainIndex(char* operand)
{
	if(!strrchr(operand, ')'))
		return -1;
	else if(!strrchr(operand, ')'))
		return -1;

	return 1;
}



// this functions convert a decimal number to octal.
long DecToOctalConvertor(int decimalNumber)
{
	int result = 0, temp = 1;

	while (decimalNumber!= 0)
	{
		result += (decimalNumber % 8) * temp;
		decimalNumber /= 8;
		temp *= 10;
	}

	return result;
}
