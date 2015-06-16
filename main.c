//******************************************************************************************************************//
// The project is used as an assembler compiler. 								    //
// Inputs: list of files names without it's extantions.								    //
// Outputs: 3 files for each input file:									    //
//    ext file: Contain all the external variables of the program, encoded to octal base.			    //
//    ent file: Contain all the program symbols and labels, encoded to octal base.				    //
//    ob file: Contain the program code , encoded to octal base.						    //
// How it works ? 												    //
//   The program pass over the code 2 times:									    //
//     The first read is initialize all the arrays and summarize the counters.					    //
//     The second read is encoding the commnands and the vaiables to machine launuage and pack it as octal base.    //
//******************************************************************************************************************//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "second.h"
#include <math.h>

/* Base 10====>6 */
 long DecToBase6Convertor(int decimalNumber)
{
int result = 0, temp = 1;

while (decimalNumber!= 0)
{
result += (decimalNumber % 6) * temp;
decimalNumber /= 6;
temp *= 10;
}

return result;
}


  /* Base 2===>6  */
 unsigned long BinToBase6Convertor(int binaryNumber[])
{
   int i;
   int temp=0;
   unsigned long r;
   int  w;
for(i = 0; i < 19; i++)
{
   w=pow(2,18-i);
   temp=temp+(binaryNumber[i]*w);
 }
    r=DecToBase6Convertor(temp);
    return r;
}
 
      /*   Base    8====>6        */
int* Base8ToBase6(int octal[])
{
   int temp=0;;
   int i;
   int* array; 
   array=(int*)malloc(10*sizeof(int));
   int r;
   int w=0;
   for(i = 0; i < 10; i++)
   {
     w= pow(8,9-i);
     temp=temp+(octal[i]*w);  /* temp now is the Number at 10 Base */
   }
  
   r=DecToBase6Convertor(temp);
  
   
   for(i = 9; i >= 0; i--)
   {
       array[i]=r%10;
       r /= 10; 
   }
   return array;

}

	 




int main(int argc,char* argv[])
{
	FILE *asFile,*obFile,*zip1, *zip2;
	int i=1, j,argCounter=1;
	char** symbolArr;
	char** dataArr;
	char** externArr;
	char** entryArr;
	unsigned long *binarCode;
	unsigned long CurrBinarCode = 0;
	char* conectCode;
	int binaryOutput[10];
        int* rept;
	int tp;      
        int m=0;
	int q=0;
        int t; 
        int p;
	int l;
        int IC2=0;
        int* dist;        
	unsigned long* binarCodeStretch; 
	char* conectCodeStretch;
        int v=0;     
        int* indexGuard;    
        int d;
        int u;
        long y=0; 
        char** ext1;
	int* ext2;
        char** ent1;
        int* ent2;
        int* B6;

// check if there are not enough arguments.
	if(argc<2)
	{
		// print error.
		printf("Error- Illegal arguments, expected to at least one argument.\n");
		exit(1);
	}
	
	// Pas over all arguments.
	while(argCounter<argc)
	{
		// check if there is such file to open.
		if (!(asFile=fopen(NewFileNameToOpen(argv[argCounter]),"r")))
		{
			// print error.
			printf("Error- the file %s cannot be open.\n",NewFileNameToOpen((argv[argCounter])));
			i++;
			continue;
		}

		// Allocate memory to all variables
		MemoryAllocator(asFile);

		// if during the allocate memory the error flag has been marked , exit the program.
		if(errorFlag==ERROR_ON)
			exit(1);
		symbolArr=(char**)malloc(IC*sizeof(char*));
		for(i=0;i<IC;i++)
			*(symbolArr+i)=(char*)malloc(MAX_STRING_LEN*sizeof(char));

		dataArr=(char**)malloc(DC*sizeof(char*));
		for(i=0;i<DC;i++)
			dataArr[i]=(char*)malloc(MAX_STRING_LEN*sizeof(char));

		externArr=(char**)malloc(numExtern*sizeof(char*));
		for(i=0;i<numExtern;i++)
			externArr[i]=(char*)malloc(MAX_STRING_LEN*sizeof(char));

		entryArr=(char**)malloc(numEntry*sizeof(char*));
		for(i=0;i<numEntry;i++)
			entryArr[i]=(char*)malloc(MAX_STRING_LEN*sizeof(char));

		binarCode=(unsigned long*)malloc((IC+DC)*sizeof(unsigned long));
                rept=(int*)malloc((IC*3)*sizeof(int));    // char array to count the repeated prints
                memset(rept,0,sizeof(int)*(IC+DC));    // initialize to 0
		
		ext1=(char*)malloc((IC*3)*sizeof(char));   // array of string for the externfile
                for(i=0;i<(IC*3);i++)
			ext1[i]=(char*)malloc(MAX_STRING_LEN*sizeof(char));
               
                ext2=(int*)malloc((IC*3)*sizeof(int));    // array of adresses for the externfile
               
                ent1=(char*)malloc((IC*3)*sizeof(char));  // array of string for the entryfile
                for(i=0;i<(IC*3);i++)
			ent1[i]=(char*)malloc(MAX_STRING_LEN*sizeof(char));
               
                 ent2=(int*)malloc((IC*3)*sizeof(int));    //array of adresses for the entryfile
                // initialize the array to 0.
		for (i = 0; i < IC + DC; i++)
			binarCode[i] = 0;

		conectCode=(char*)malloc(IC*sizeof(char));

		// go the the beginning of the file.
		rewind(asFile);

		// Pass all over the assembler program and count the variables and the commands
		FirstRead(asFile,symbolArr,dataArr,externArr,entryArr);
		
		// go the the beginning of the file.
		rewind(asFile);

		if(SecondRead(asFile,symbolArr,dataArr,externArr,entryArr,binarCode,conectCode,argv[argCounter],rept,ext1,ext2,ent1,ent2)==-1)
			continue;

		fclose(asFile);

		if (!(obFile=fopen(GetObjFileName(argv[argCounter]),"w")))
		{
			printf("Error- the file %s cannot be open.\n",GetObjFileName((argv[argCounter])));
			
continue;
		}


		// Write all the data to the OB file
		// counting the new IC after streaching the output   
		for(i=0;i<IC;i++) 
		{
                  IC2++;
		          if(rept[i]>0)
                          {
                            p=i;                            
			    l=i;
			    tp=rept[i];
                          }
                          if(rept[i]==0 && rept[i+1]>0 && tp>1)             
                         {	
                           	         
                             for(t=0;t<tp-1;t++)
 		             {
		                p=l;
                                for(;p<i+1 ;p++)
                                {
                                    IC2++;
                                 }
                         }   }
                }

               IC2=IC2+rept[IC-1]-1;
               binarCodeStretch=(unsigned long*)malloc((IC2+1)*sizeof(unsigned long*));     //stretching the array  
               conectCodeStretch=(char*)malloc((IC2+1)*sizeof(char));     //stretching the array   
               dist=(int*)malloc((IC2+1)*sizeof(int));    // the distance measuring array
               indexGuard=(int*)malloc((IC2+1)*sizeof(int));
               memset(binarCodeStretch,0,sizeof(unsigned long)*(IC2+1));
               memset(conectCodeStretch,0,sizeof(char)*(IC2+1));
               memset(dist,0,sizeof(int)*(IC2+1));
               indexGuard[0]=0;
            p=0;
	    l=0; 
            tp=0;
                 // streaching the final arrays of output for the reapitting codes 
                for(i=0;i<IC;i++) 
		{
                          indexGuard[v]=i;
		          if(i>0)
                          {
                             dist[v]=dist[v-1];
                          }
                          if(rept[i]>0)
                          {
                            p=i;                            
			    l=i;
			    tp=rept[i];
                          }
                              binarCodeStretch[v]=binarCode[i];
                              conectCodeStretch[v]=conectCode[i];                             
                              v++;
                              if(rept[i]==0 && rept[i+1]>0 && tp>1)             
                             {	
                           	         
                                for(t=0;t<tp-1;t++)
 		                {
		                  p=l;
                                  for(;p<i+1 ;p++)
                                  {
                                    indexGuard[v]=p; 
                                    binarCodeStretch[v]=binarCode[p];
                                    conectCodeStretch[v]=conectCode[p];
                                    dist[v]=dist[v-1];
                                    dist[v]++;
                                    v++;
                                  }
                                }
                             }
                    
                          if(rept[i]>0 && rept[i+1]>0)       //if one line of code
                         {                             
                         
				for( p=1;p<rept[i];p++) 
                               {
                                   indexGuard[v]=i;
                                   binarCodeStretch[v]=binarCode[i];
                                   conectCodeStretch[v]=conectCode[i];
                                   dist[v]=dist[v-1];
                                   dist[v]++;
                                   v++;
                               }                           
                         }
                         if(rept[i]>0 && i==IC-1)          // last code repeat codding   
                         {
                              for( p=1;p<rept[i];p++) 
                              {
                                  indexGuard[v]=i;
                                  binarCodeStretch[v]=binarCode[i];
                                  conectCodeStretch[v]=conectCode[i];
                                  dist[v]=dist[v-1];
                                  dist[v]++;
                                  v++;
                              }
                         }

               }
                // chang all the  adresses relativly to the repeating number at the code line
		for(i=0;i<IC;i++)                 
                {
                   if(conectCodeStretch[i]=='r')
                   {
                     d=Base8ToDec(binarCodeStretch[i]);
                     d=d-100;
                     u=searchIndex(indexGuard,d,IC2);
                     u=u+100;
                     y=DecToOctalConvertor(u);
                     binarCodeStretch[i]=y;
                   }
                }
          
           
                //ptints all the adresses to the "FileName.ext" 
                char* dip=GetExtFileName(argv[1]);             // dip="FileName.ext"
		zip1=fopen(dip,"w");
                int bal= fseek(zip1,0,SEEK_SET);
            
                 for(i=1;i<IC;i++)
		{
                    if(ext2[i]!=0)
                   {
                       d=Base8ToDec(ext2[i]);
                       d=d-100;
                       u=searchIndex(indexGuard,d,IC2);
                       u=u+100;
                       y=DecToBase6Convertor(u);
                       ext2[i]=y;
                       fprintf(zip1,"%s \t %d\n",ext1[i],ext2[i]);
		    }
                 }

            
                // ptints all the adresses to the "FileName.ent" 
                char* dig=GetEntFileName(argv[1]);               // dig="FileName.ent"		 
                zip2=fopen(dig,"w"); 
                int bat= fseek(zip2,0,SEEK_SET);             
                
                 for(i=0;i<IC;i++)
		{
                    if( ent2[i]!=0)
                    { 
		       d=Base8ToDec(ent2[i]);
                       d=d-100;
                       u=searchIndex(indexGuard,d,IC2);
                       u=u+100;
                       y=DecToBase6Convertor(u);
                       ent2[i]=y;
		       fprintf(zip2,"%s \t %d\n",ent1[i],ent2[i]);
		    }
		}


		
		// Write all the data to the OB file

                fprintf(obFile,"\t %d \t %d \n ",DecToBase6Convertor(IC2),DecToBase6Convertor(DC));
		for(i=0;i<IC2;i++)
		{
			CurrBinarCode = binarCodeStretch[i];
			for (j = 9; j >= 0; j--)
			{
				binaryOutput[j] = 0;
				if (CurrBinarCode != 0)
					binaryOutput[j] = CurrBinarCode % 10;

				CurrBinarCode /= 10;
				
			}

			fprintf(obFile,"%d \t", DecToBase6Convertor(ADDRESS_TO_BEGIN+m));
			m++;
                        B6=Base8ToBase6(binaryOutput);
			for (j = 0; j < 10; j++)
			{
				fprintf(obFile, "%d", B6[j]);
			}

                
			fprintf(obFile, "\t %c\n", conectCodeStretch[i]);
		}

       
                
		for(i=IC;i<IC+DC;i++)
		{
			CurrBinarCode = DecToBase6Convertor(binarCode[i]);
			for (j = 9; j >= 0; j--)
			{
				binaryOutput[j] = 0;
				if (CurrBinarCode != 0)
					binaryOutput[j] = CurrBinarCode % 10;

				CurrBinarCode /= 10;				
			}

			fprintf(obFile,"%d \t", DecToBase6Convertor(ADDRESS_TO_BEGIN+m));
                         m++;
			B6=Base8ToBase6(binaryOutput);
                        for (j = 0; j < 10; j++)
				fprintf(obFile, "%d", B6[j]);

			fprintf(obFile, "\n");
		}

		fclose(obFile);
		fclose(zip1);
		fclose(zip2);
		free(symbolArr);
		free(dataArr);
		free(externArr);
		free(entryArr);
		free(binarCode);
		free(conectCode);

		
		argCounter++;
	}

	return 1;

}


// this function update the global counters of the possible commands and data.
void  MemoryAllocator(FILE* asFile)
{
	int symbolFlag=SYMBOL_OFF, i = 0, x, numberOfLines;
	int errorFlag=ERROR_OFF;
	char* line = (char*)malloc (MAX_STRING_LEN * sizeof(char));
	char* temp=(char*)malloc(sizeof(char)*MAX_STRING_LEN);
	char str[MAX_STRING_LEN];
	DC=IC=0;
	
	// pass over all lines in file.
	while ((fgets(line,MAX_STRING_LEN,asFile)))
	{
		symbolFlag=SYMBOL_OFF;
		
		SkipSpacesInLine(&line);
	
		// if end of line, get the next line
		if(*line==';' || !line )
			continue;

		temp = strcpy(temp,line);
	
		i = 0;

		// go over the chars to change the / to space.
		while(temp[i] != '\0')
		{
			if (temp[i] == '/')
			{
				line[i] = ' ';
				line[i+1] = ' ';
				i++;
			}
			// Remove the double bit.
			else if ((temp[i] == ',') && ((temp[i+1] == '0')||(temp[i+1]=='1')||(temp[i+1]=='2')||(temp[i+1]=='3')))
			{
				line[i] = ' ';
				line[i+1] = ' ';
				i+=2;
			}
			i++;
		}

		//taking the first word into str.
		sscanf(line,"%s",str);

		// if extern , count it and get the next line.
		if(!strcmp(str,".extern"))
		{
			numExtern++;
			continue;
		}

		// if entry , count it and get the next line.
		else if(!strcmp(str,".entry"))
		{
			numEntry++;
			continue;
		}
		
		// if simbol, mark the flag and skip the first word.
		if((str[strlen(str)-1])==':')
		{
			symbolFlag=SYMBOL_ON;

			line+=strlen(str);

			SkipSpacesInLine(&line);

			sscanf(line,"%s",str);		
		}
		else 
			// not a symbol , dont mark the flag.
			symbolFlag=SYMBOL_OFF;

		// if data , encrease DC with the number of variables.
		if(!strcmp(str,".data")) 
		{
			line+=strlen(str);
			SkipSpacesInLine(&line);
			DC+=DataCounter(line);
		}
			
		// if string , get the number of chars and encrease DC.
		else if(!strcmp(str,".string")) 
		{
			line+=strlen(str);
			SkipSpacesInLine(&line);
			DC+=CharsInLine(line);
		}

		// if its a valid opcode , encrease IC with the number of lines it needed.
		else if(GetCommandBin(str)!=-1)
		{
			numberOfLines = LinesAllocator(line);

			// if the command is valid, encrease IC.
			if (numberOfLines != -1 ) 
			{
				IC+=LinesAllocator(line);
			} 
			else 
			{
				printf("Error- Illegal opcode: %s.\n",str);
				errorFlag=ERROR_ON;
				continue;
			}
		}

	}
}

// Get the input file name
char* NewFileNameToOpen(char* fileName)
{
	char* newName=(char*)malloc(sizeof(char)*(strlen(fileName)+3));
	char fileExtantion[4]=".as";

	strcpy(newName,fileName);
	strcat(newName,fileExtantion);
	return newName;

}



// Get the ob file name
char* GetObjFileName(char* fileName)
{
	char* newName=(char*)malloc(sizeof(char)*(strlen(fileName)+3));
	char fileExtantion[4]=".ob";

	strcpy(newName,fileName);
	strcat(newName,fileExtantion);
	return newName;

}
  
// searching for a number in not sorted array
int searchIndex(int* array,int d,int x)
{ 
   
   int i;
   //int k=(sizeof(array))/sizeof(array[0]);
   for ( i=0;i<x;i++)
   {
     if(array[i]==d)
     { 
        return i;
     }
   }
    
    return d-IC+x;
             
}



 /*  8====>10 */
int Base8ToDec(int octal)
{
   int i;
   int decimal=0;
   int p=0;
   for(i = 0; octal!=0; i++)
  {
    p=pow(8,i);
    decimal=decimal+(octal % 10)*p;
    octal=octal/10;
    
  }
  return decimal;
}










  
  




