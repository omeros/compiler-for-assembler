//**********************************************************************************//
// This header file is used to Implementat the second read function and all the     //
// related functions that are used in it's function.		         	    //
//**********************************************************************************//
#include "first.h"

// this function convert a number from binary to decimal.
int BinToDecimalConvertor(int binaryNumber)
{
	int result = 0, temp = 1;

	while (binaryNumber != 0)
	{
		result += (binaryNumber % 2) * temp;
		binaryNumber /= 2;
		temp *= 10;
	}

	return result;
}

//return the name of the entry file
char* GetEntFileName(char* fileName)
{
	char* newName=(char*)malloc(sizeof(char)*(strlen(fileName)+4));
	char fileExtantion[5]=".ent";

	strcpy(newName,fileName);
	strcat(newName,fileExtantion);
	return newName;
}

//return the name of the extern file
char* GetExtFileName(char* fileName)
{
	char* newName=(char*)malloc(sizeof(char)*(strlen(fileName)+4));
	char fileExtantion[5]=".ext";

	strcpy(newName,fileName);
	strcat(newName,fileExtantion);
	return newName;
}


// The procedure doing the decode process for the current operand
void GetOperandBinCode(char* operand, char* Parameters, int ic, char** symbolArr,char** externArr,char** entryArr,unsigned long* binarCode,char* 
	conectCode,char** dataArr,FILE* extFile, int lineBinaryBits[20], int firstOperand, int ICCount, int* externCount,char** ext1,int* ext2,int g)
{
	char* offset=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char* variable=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	int destinationRegister = 0, destinationMiun = 0, sourceRegister = 0, sourceMiun = 0, firstNumber = 0, i, isPointer=0, j,mul;
	unsigned long lNumber;
	int destinationBinaryRegister, destinationBinaryMiun,sourceBinaryRegister, sourceBinaryMiun, index;
        

	if(IsContainIndex(operand) != -1)
	{
		variable = GetVariable(operand);
		offset = GetParameterIndex(operand);
		VariableHandler(variable, binarCode, dataArr, externArr, ic, externCount, ICCount, conectCode);
		VariableHandler(offset, binarCode, dataArr, externArr, ic, externCount, ICCount, conectCode);

		if (firstOperand == 1)
		{
			sourceMiun = 2;

			if(GetRegisterIndex(offset) != -1)
				sourceRegister = GetRegisterIndex(offset);
		}
		else
		{
			destinationMiun = 2;

			if(GetRegisterIndex(offset) != -1)
				destinationRegister = GetRegisterIndex(offset);	
		}
	}
	else
	{
		// Decode the variable of the operand 
		VariableHandler(operand, binarCode, dataArr, externArr, ic, externCount, ICCount, conectCode);
	}

	// Checks if there is an immidiate miun
	if(*operand=='#')
	{
		sourceMiun = 0;		
		
		mul=1;
		operand++;
		i=0;
		if(*operand=='-')
		{
			mul=-1;
			operand++;
		}

		while(operand[i]!=' ' && operand[i]!='\0')
			firstNumber = firstNumber*10+(*(operand+i++)-'0');	
		firstNumber *= mul;
		
		if (firstNumber < 0)
		{
			firstNumber = -firstNumber;
			lNumber = BinaryNegativeValuesHandler(firstNumber);
		}
		else
		{
			lNumber = DecToOctalConvertor(firstNumber);
		}

		binarCode[ic+(*externCount)] = lNumber;
		(*externCount)++;

		conectCode[ic+*externCount]='a';
	}

	else if(GetSymbolIndex(operand,symbolArr,dataArr)==-1 && GetRegisterIndex(operand)==-1 && IsExtern(operand,externArr)==-1 && 
	  (IsContainIndex(operand) == -1))
	{
		printf("Error- Illegal operand: %s.\n",operand);
		exit(1);
	}

	else if(GetSymbolIndex(operand,symbolArr,dataArr)!=-1)
	{
		if(!isPointer)
		{
			conectCode[ic+*externCount]='r';
		}
	}

	else if(GetRegisterIndex(operand)!=-1)
	{
		if(firstOperand == 1)
		{
			sourceRegister = GetRegisterIndex(operand);
			sourceMiun = 3;
		}
		else
		{
			destinationRegister = GetRegisterIndex(operand);
			destinationMiun = 3;
		}
	}

	else if(IsExtern(operand,externArr)!=-1)                 
	{
		conectCode[ic+*externCount]='e';
	        strcpy(ext1[g],operand);
                ext2[g]=DecToOctalConvertor(ADDRESS_TO_BEGIN+ic+*externCount - 1);
                             
                        	
        }

	if(GetOperandIndex(operand,externArr,dataArr) != -1)
	{
			if (firstOperand == 1)
				sourceMiun = 1;
			else
				destinationMiun = 1;
	}

	destinationBinaryRegister = BinToDecimalConvertor(destinationRegister);
	destinationBinaryMiun = BinToDecimalConvertor(destinationMiun);
	sourceBinaryRegister = BinToDecimalConvertor(sourceRegister);
	sourceBinaryMiun = BinToDecimalConvertor(sourceMiun);
	index = 2;

	while(destinationBinaryRegister)
	{
		lineBinaryBits[index] = destinationBinaryRegister % 10;
		index++;
		destinationBinaryRegister /= 10;
	}

	index = 5;

	while(destinationBinaryMiun)
	{
		lineBinaryBits[index] = destinationBinaryMiun % 10;
		index++;
		destinationBinaryMiun /= 10;
	}

	index = 7;

	while(sourceBinaryRegister)
	{
		lineBinaryBits[index] = sourceBinaryRegister % 10;
		index++;
		sourceBinaryRegister /= 10;
	}

	index = 10;

	while(sourceBinaryMiun)
	{
		lineBinaryBits[index] = sourceBinaryMiun % 10;
		index++;
		sourceBinaryMiun /= 10;
	}
}


// The procedure doing the second read that decode all the commands and operands to "assembly"
int SecondRead(FILE* asFile,char** symbolArr,char** dataArr,char** externArr,char** entryArr,unsigned long* binarCode,char* conectCode,char* fileName,int* rept,char** ext1,int* ext2,char** ent1,int* ent2)
{
	char* tempStr=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	int lineBinaryBits[20];
	char* ParametersCode=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char* temp=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char* operand=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char* line=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char* s=(char*)malloc(MAX_STRING_LEN*sizeof(char));
	FILE *externFile, *entryFile;
	int* externCount = (int*)malloc(sizeof(int));
	int ic=0,dc=IC,i,numOfLines,isEntry=0,isComb=0,opCode=0,isType=0, j,k,repeating = 0,len,number,sig, binaryComb, binaryOpCode, index;
	long command;
	int f=0;
  	int g=0;

	// create file for extern, if failed- print error.
	if(!(externFile=fopen(GetExtFileName(fileName),"w")))
	{
		printf("Error- the file %s cannot be open.\n",GetExtFileName(fileName));
		return -1;
	}
	
	// create file for entry, if failed- print error.
	if(!(entryFile=fopen(GetEntFileName(fileName),"w")))
	{
		printf("Error- the file %s cannot be open.\n",GetEntFileName(fileName));
		return -1;
	}

	// get line by line from the file.
	while(fgets(line,MAX_STRING_LEN,asFile))
	{
		*externCount = 1;
		isComb = 0;
		isType = 0;
		
		// initialize lineBinaryBits array to 0.
		for(i = 0;i<20;i++)
			lineBinaryBits[i] = 0;
		
		isEntry=0;

		//skip the line's spaces.
		SkipSpacesInLine(&line);
		
		//get the first word from line into tempStr.
		sscanf(line,"%s",tempStr);

		// skip entries, extrens and ;.
		if(strcmp(tempStr,".entry")==0  ||strcmp(tempStr,".extern")==0 || *tempStr==';')
			continue;

		// check if it's symbol
		len=strlen(tempStr);
		if(tempStr[len-1]==':')
		{
			// put the symbol name in s.
			strncpy(s,tempStr,len-1);
			s[len-1]='\0';

			// check if s is really an entry.
			if(IsEntry(s,entryArr)==1)
				isEntry=1;
			// go to next word and skip spaces.
			line+=strlen(tempStr);
			SkipSpacesInLine(&line);

			// get the next word from line into tempStr.
			sscanf(line,"%s",tempStr);
		}

		// check if the word is string.
		if(!strcmp(tempStr,".string"))
		{
			// go to the next word and skip spaces.
			line+=strlen(tempStr);
			SkipSpacesInLine(&line);
/*Entry*/
			// get the next word from line into operand.
			sscanf(line,"%s",operand);
			
			//skip the first " of the string, and move until the end of the string(the next ").
			operand++;
			while(*operand!='\"')
				binarCode[dc++]=*(operand++);

			binarCode[dc++]=0;

			// if isEntry is marked, write it in the entry file       
			// converted from decimal to 8 base.
			if(isEntry==1 )
			{	
				 strcpy(ent1[f],s);
                                 ent2[f]=DecToOctalConvertor(ADDRESS_TO_BEGIN+dc);
                                 f++;
                         }         
                             
                                         
			continue;
		}
		// if the word is data , 
		else if(!strcmp(tempStr,".data"))
		{
			sig=1;
			// move to the next word of line.
			line+=strlen(tempStr);

			while(line && *line!='\n')
			{
				SkipSpacesInLine(&line);

				number=0;

				if(*line=='-')
				{
					sig=-1;
					line++;
				}
				
				// check if isDigit, summerize as decimal digits to the correct number.
				while(isdigit(*line))
					number=number*10+(*line++)-'0';

				binarCode[dc]=number;
				dc++;

				while(*line!=',' && *line!='\n' && line)
					line++;

			}

			// if isEntry is marked, write it in the entries file in the correct place.
			// converted from decimal to base 8.
			if(isEntry==1)				
                         {       
				     strcpy(ent1[f],s);
                                     ent2[f]=DecToOctalConvertor(ADDRESS_TO_BEGIN+dc-1);
                                     f++;
                                       
                            }          
			continue;
		}
		
		// if is entry, write it in the entry file in the correct place.
		if(isEntry==1)
		{
          	                strcpy(ent1[f],s);
                                ent2[f]=DecToOctalConvertor(ADDRESS_TO_BEGIN+ic);
                                f++;
                         
                }         
                                    

		temp = strcpy(temp,tempStr);
		j = 0;
		i = 0;

		while(temp[i] != ' ' && temp[i] != '\0')
		{
			if (temp[i] == '/')
			{
				ParametersCode[j] = temp[i+1];
				ParametersCode[j+1] =  + '\0';
				j++;
				tempStr[i] = '\0';	
				line[i] = ' ';
				tempStr[i+1] = '\0';
				line[i+1] = ' ';
				i++;
			}
			else if (temp[i] == ',')
			{

				repeating = temp[i+1]-'0';
				line[i] = ' ';
				line[i+1] = ' ';
				i++;
			}

			i++;
		}


		// check the necessary number of lines.
		numOfLines=LinesAllocator(line);
		
		// put 'a' in the correct place.
		conectCode[ic]='a';

		// get the next word, skip spaces.
		line+=strlen(tempStr);
		SkipSpacesInLine(&line);

		// if isType bit is 1 , 
		if(ParametersCode[0] == '1')
		{
			isType = 1;
			//manipulate isComb to the correct binary value for isComb.
			isComb = (ParametersCode[1] - '0') * 2 + (ParametersCode[2] - '0');
		}

		//get the opcode index in the opcodes array.
		opCode = GetCommandBin(tempStr);

		//get the binary value for bin and opcode.
		binaryComb = BinToDecimalConvertor(isComb); 
		binaryOpCode = BinToDecimalConvertor(opCode); 

		index = 0;

		// the next while loops are encoding the bits of the command
		// and put each bit in its place of the binary command.
		while(binaryComb)
		{
			lineBinaryBits[index] = binaryComb % 10;
			index++;
			binaryComb /= 10;
		}
		index = 12;
		while(binaryOpCode)
		{
			lineBinaryBits[index] = binaryOpCode % 10;
			index++;
			binaryOpCode /= 10;
		}
		lineBinaryBits[16] = isType;
		

		// handling the command that gets 2 operands.
		if((GetCommandBin(tempStr)>=0 && GetCommandBin(tempStr)<=3) || GetCommandBin(tempStr) == 6)
		{
			i=0;
			while(line[i]!=' ' && line[i]!=',')
				i++;

			strncpy(operand,line,i);
			operand[i]='\0';
			//handle the bits of the second operand of the command binary code for lineBinaryBits.
			GetOperandBinCode(operand,ParametersCode,
			ic,symbolArr,externArr,entryArr,binarCode,conectCode,dataArr,externFile, lineBinaryBits, 1, IC, externCount,ext1,ext2,g);
                        g++;
			//move to the next value to get.
			line+=i;
			i=0;
			while(line[i]!=',')
				i++;

			line+=i+1;
			
			// get the next value into operand.
			sscanf(line,"%s",operand);
			
			//handle the bits of the first operand to complete the binary code of the command.
			GetOperandBinCode(operand,ParametersCode,
			ic,symbolArr,externArr,entryArr,binarCode,conectCode,dataArr,externFile, lineBinaryBits,0, IC, externCount,ext1,ext2,g);
			g++;
			// convert the completed lineBinaryBits to base 8 binary code.
			// and put it in the right place of the binarCode.
			command = BinToOctalConvertor(lineBinaryBits);
			binarCode[ic] = command;
		        rept[ic] = repeating;
                       
                 }

		// if the command gets only 1 operand.
		else if(GetCommandBin(tempStr)==4 || GetCommandBin(tempStr)==5 || (GetCommandBin(tempStr)>=7 && GetCommandBin(tempStr)<=13))
		{
			//get the operand from line into operand.
			sscanf(line,"%s",operand);
			
			//handle the bits of the operand of the command
			GetOperandBinCode(operand,ParametersCode,
			ic,symbolArr,externArr,entryArr,binarCode,conectCode,dataArr,externFile, lineBinaryBits, 0, IC, externCount,ext1,ext2,g);
			g++;
			
			// convert the completed lineBinaryBits to base 8 binary code.
			// and put it in the right place of the binarCode.
			command = BinToOctalConvertor(lineBinaryBits);
			binarCode[ic] = command;
			rept[ic] = repeating;	
                        
		}
		// if the command does not gets operands at all.
		else 
		{
			binarCode[ic] = GetCommandBin(tempStr);
			binarCode[ic]<<=12;
			binarCode[ic] = DecToOctalConvertor(binarCode[ic]);
                        rept[ic] = repeating;		
		        
                }

		//increase the numOfLines to the next place to handle the next commands.
		ic+=numOfLines;
	}


	//close the file.
	fclose(externFile);

	return 1;
}


// Gets:string and two array of string
// Return :-1 isn't symbol,else return he index in one of the arrays 
int GetSymbolIndex(char* symbol,char** symbolArr,char** dataArr)
{
	int i=0;

	for(i=0;i<IC;i++)
		if(!strcmp(symbol,symbolArr[i]))
			return i;
	for(i=0;i<DC;i++)
		if(!strcmp(symbol,dataArr[i]))
			return i+IC;
	return -1;

}
//Gets:string and two array of string
//Return :-1 isn't symbol,else return he index in one of the arrays 
int GetOperandIndex(char* parameter,char** externArr,char** dataArr)
{
	int i;

	for(i=0;i<numExtern;i++)
		if(!strcmp(externArr[i],parameter))
			return i;
	for(i=0;i<DC;i++)
		if(!strcmp(parameter,dataArr[i]))
			return i+IC;
	return -1;

}

//Gets:string and array of string
//Return:-1 if operand isn't extern 'if he is extern return the index in the array
int IsExtern(char* operand,char** externArr)
{
	int i;

	for(i=0;i<numExtern;i++)
		if(!strcmp(externArr[i],operand))
			return i;

	return -1;
}
 



// the function gets string and returns 1 if exist in entryArr and -1 otherwise.
int IsEntry(char* string,char** entryArr)
{
	int i=0;

	for(i=0;i<numEntry;i++)
		if(!strcmp(*(entryArr+i),string))
			return 1;

	return -1;
}


//Gets: string with operand
//Return: The index of the operand
char *GetParameterIndex(char* string)
{
	char* offset=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	int i, j , k;

	if(IsContainIndex(string) != -1)
	{				
		i = 0;
		while(string[i] != '(')
			i++;
		
		if (string[i+1] == '@')
			i++;
		
		j = i;
		while(string[j] != ')')
			j++;
		
		k = 0;
		for(i=i+1;i<j;i++)
			offset[k++] = string[i];

		offset[k] = '\0';
	}

	return offset;
}

//Gets: string with operand
//Return: The variable of the operand
char *GetVariable(char* string)
{
	char* Variable=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	int i, j;

	if(IsContainIndex(string) != -1)
	{				
		i = 0;
		while(string[i] != '(')
			i++;

		for(j=0;j<i;j++)
			Variable[j] = string[j];

		Variable[j] = '\0';
	}

	return Variable;
}

// the function convert from binary base to octal base.
unsigned long BinToOctalConvertor(int binaryNumber[])
{
	int i, temp,dig=1;
	unsigned long result = 0;

	for(i = 0; i < 19; i=i+3)
	{
		if (i==18)
			temp = binaryNumber[i];
		else
			temp = binaryNumber[i+2]*4 + binaryNumber[i+1]*2 + binaryNumber[i];
		result = result + temp*dig;
		dig*=10;
	}

	return result;
}


// The procedure doing decode to the variable of the operand
void VariableHandler(char* variable, unsigned long* binarCode, char** dataArr, char** externArr, int ic, int* externCount, int ICCount, char* conectCode)
{
	int j, mul = 1, i = 0, number= 0;
	int IsData = 0;	
	unsigned long lNumber;

	for (j = 0; j < DC; j++)
	{
		if (!strcmp(dataArr[j], variable))
		{
			binarCode[ic+(*externCount)] = DecToOctalConvertor(ICCount+j+ADDRESS_TO_BEGIN);
					
			IsData = 1;
			conectCode[ic+(*externCount)]='r';
			(*externCount)++;
		}
	}

	if (IsExtern(variable,externArr)!=-1)
	{
		binarCode[ic+(*externCount)] = 0;
		
		conectCode[ic+(*externCount)]='e';
			(*externCount)++;
	}
	else if ((IsData != 1) && (*variable != '#'))
	{
		if (GetRegisterIndex(variable) == -1)
		{
			if(*variable=='-')
			{
				mul=-1;
				variable++;
			}

			while(variable[i]!=' ' && variable[i]!='\0')
				number = number*10+(*(variable+i++)-'0');	
			number *= mul;

			if (number < 0)
			{
				number = -number;
				lNumber = BinaryNegativeValuesHandler(number);
			}
			else
				lNumber = DecToOctalConvertor(number);

			binarCode[ic+(*externCount)] = lNumber;
			conectCode[ic+(*externCount)]='a';
			(*externCount)++;
		}
	}
}

// The procedure change negative number in decimal base to binary base
unsigned long BinaryNegativeValuesHandler(int number)
{
	int binaryNumber, i, isFirst = 1;
	int x[20];
	unsigned long lNumber;

	for(i = 0; i < 20; i++)
		x[i] = 0;

	binaryNumber = BinToDecimalConvertor(number);
	i = 0;

	while (binaryNumber)
	{
		x[i] = binaryNumber % 10;
		binaryNumber /= 10;
		i++;
	}

	for (i = 0; i < 20; i++)
	{
		if (x[i] == 1)
		{
			if (isFirst == 1)
				isFirst = 0;
			else
				x[i] = 0;
		}
		else
		{
			if (isFirst == 0)
				x[i] = 1;
		}
	}
	lNumber = BinToOctalConvertor(x);

	return (lNumber);
}







