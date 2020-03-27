#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 256

int main() {
        FILE *fptr1, *fptr2;
	char fname[MAX];
        char temp[] = "temp.txt";
	char * line;
        char * in; 
	char * aux; 
	char * copy;
	line = (char *)malloc(MAX);
	in = (char *)malloc(MAX);
	copy = (char *)malloc(MAX);
	aux = (char *)malloc(MAX);
	
	printf("\n\n Delete a specific line from a file :\n");
	printf("-----------------------------------------\n"); 
	printf(" Input the file name to be opened : ");
	
	scanf("%s",fname);		
        fptr1 = fopen(fname, "r");
        
	if (!fptr1) {
        	printf(" File not found or unable to open the input file!!\n");
        	return -1;
        }

	fptr2 = fopen(temp, "w"); // open the temporary file in write mode 
        if (!fptr2) {
        	printf("Unable to open a temporary file to write!!\n");
        	fclose(fptr1);
        	return -1;
        }

        printf(" Input the string to seek: ");
        scanf("%s", in);
        int round = 0;
	// copy all contents to the temporary file except the specific line
        while (!feof(fptr1)) {
        	strcpy(line, "\0");
        	fgets(line, MAX, fptr1);
        	if (!feof(fptr1)) {
			round = 0;
			copy = line;
            		while(!('/'==*copy)){
				//printf("Round: %d\n", round);
				strncat(aux, copy, 1);
				/*printf("*line: %c\n", *copy);
				printf("aux: %s\n", aux);
				printf("--------------");*/
				copy++;
				round++;
			}
			
			if(0!=strcmp(aux, in)){
				//printf("Copying line..\n");
				fprintf(fptr2, "%s", line);
			}
			memset(aux,0,sizeof(aux));
		}
        }
        
	fclose(fptr1);
        fclose(fptr2);
        remove(fname);  	// remove the original file 
        rename(temp, fname); 	// rename the temporary file to original name
}

