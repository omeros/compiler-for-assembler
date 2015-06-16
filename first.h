//**********************************************************************************//
// This header file is used to Implementat the first read function.		    //
//**********************************************************************************//
#include "general.h"

// Pass all over the assembler program and count the variables and the commands
void FirstRead(FILE* asFile,char** symbolArr,char** dataArr,char** externArr,char** entryArr)
{
	char tempString[MAX_STRING_LEN],symbol[MAX_STRING_LEN];
	char* line=(char*)malloc(MAX_STRING_LEN*sizeof(char));
	char* temp=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	int ic=0,dc=0,externIndex=0,entryIndex=0,i=0;

	rewind(asFile);
	
	// get line by line all over the file.
	while (fgets(line,MAX_STRING_LEN,asFile) != NULL)	
	{
		SkipSpacesInLine(&line);

		if(*line==';' || !line)
			continue;

		// get the first word from the line into str.
		sscanf(line,"%s",tempString);	

		// if its extern , put it in the right place inside externArr array.
		if(!strcmp(tempString,".extern"))
		{
			line+=strlen(tempString);
			SkipSpacesInLine(&line);
			sscanf(line,"%s",externArr[externIndex]);
			externIndex++;
			continue;
		}
		
		// if its entry, put it in the right place inside entryArr array.
		else if(!strcmp(tempString,".entry"))
		{
			line+=strlen(tempString);
			SkipSpacesInLine(&line);
			sscanf(line,"%s",entryArr[entryIndex]);
			entryIndex++;
			continue;
		}
				
		// if its a simbol , put it in the simbols array.
		if(tempString[strlen(tempString)-1]==':')
		{
			i=0;

			line+=strlen(tempString);

			symbolFlag=SYMBOL_ON;
			while(i<strlen(tempString)-1)
			{
				symbol[i]=tempString[i];
				i++;
			}

			symbol[i]='\0';

			// put the next word from line in temp.
			temp = strcpy(temp,line);
			
			i = 0;
			// change the / char to spaces.
			while(temp[i])
			{
				if (temp[i] == '/')
				{
					line[i] = ' ';
					line[i+1] = ' ';
					i++;
				}
				i++;
			}

			SkipSpacesInLine(&line);
			// put the next word from line into tempString.
			sscanf(line,"%s",tempString);
		}
		else 
		{
			symbolFlag=SYMBOL_OFF;
		}

		// if data
		if(!strcmp(tempString,".data")) 
		{
			if(symbolFlag==SYMBOL_ON)
				strcpy(*(dataArr+dc),symbol);

			line+=strlen(tempString);
			SkipSpacesInLine(&line);
			dc+=DataCounter(line);
		}
			
		// if string
		else if(!strcmp(tempString,".string")) 
		{
			if(symbolFlag==SYMBOL_ON)
				strcpy(*(dataArr+dc),symbol);
			line+=strlen(tempString);
			SkipSpacesInLine(&line);
			dc+=CharsInLine(line);
		}
		
		// if opcode
		else if(GetCommandBin(tempString)!=-1)
		{
			if(symbolFlag==SYMBOL_ON)
				strcpy(*(symbolArr+ic),symbol);
			ic+=LinesAllocator(line);
		}

	}
}
