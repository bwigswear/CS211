#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char**argv){

	//If input string is empty, return 0
	if(argv[1] == NULL){return 0;}

	//Store input and its length
	char *input = argv[1];
	int length = strlen(input);

	//If the string is one character, print string and return
	if(length == 1){printf("%s\n", input); return 0;}

	//Allocate two arrays to store output string in case a character appears double digits times
	//Multiply size by 2 for worst-case scenario
	char *output = malloc(length * sizeof(char) * 2);
	int *output2 = malloc(length * sizeof(int) * 2);

	//Create variables to traverse input string
	int i;
	int outputcounter = 0;
	char current = '\0';
	int counter = 0;

	for(i = 0; i < length + 1; i++){
		//If an input character is a digit, print nothing and return 0
		if(isdigit(input[i])){printf("error\n"); free(output); free(output2); return 0;}

		//Check each input character to see if it matches the last
		if(input[i] == current){
			//If current character matches the last, increase the character counter
			counter++;
		} else {

			if(i == 0){
				/*Since first character will not be equal to current's intialized value,
				check to see if the loop is on its first iteration*/
				counter = 1;
				current = input[i];
				continue;
			}

			if(outputcounter != 0){
				/*If the output string has not been started, output index will begin at 0.
				Otherwise, increase the current index by 1*/
				outputcounter++;
			}

			if(outputcounter + 2 > length){
				//Add check to see if the output string is going to be longer than the input string
				printf("%s\n", input);
				free(output);
				free(output2);
				return 0;
			}

			/*After a new character has been found and gone through the checks, add the previous character
			and its amount to the output arrays*/
			output[outputcounter] = current;
			outputcounter++;
			output2[outputcounter] = counter;
			counter = 1;
		}

		current = input[i];
	}

	//Ensure last character and quantity are included in print loop
	if(outputcounter != 0){outputcounter++;}
	output[outputcounter] = current;

	//Print output string
	for(int j = 0; j < outputcounter; j++){
		if(j % 2 == 0){
			printf("%c", output[j]);
		} else {
			printf("%d", output2[j]);
		}
	}

	printf("\n");

	free(output2);
	free(output);
	return 0;
}
