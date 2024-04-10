#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char**argv){

	//Check if there is an input string
	if(argv[1] == NULL){
		return 0;
	}

	//Create char pointer to manipulate input string
	char *input = argv[1];

	//Grab length of input, create stack for pushing open grouping symbols
	int length = strlen(input);
	char *stack = malloc((length + 1) * sizeof(char));

	//Create counter for keeping track of stack size and intialize current char variable
	int stackcounter = 0;
	char current = '0';

	//Loop through the string
	for(int i = 0; i < length; i++){
		current = input[i];

		//Read in each character of the input, ignore if not a grouping symbol

		if(current == '[' || current == '{' || current == '('){
			stackcounter++;
			stack[stackcounter] = current;
			//If an open symbol is encountered, add it to the stack and increase the stack counter
		} else if(current == ']'){
			if(stack[stackcounter] == '['){
				stack[stackcounter] = '0';
				stackcounter--;
			} else {
				printf("%d: %c", i, current);
				free(stack);
				return EXIT_FAILURE;
			}
			//If a close symbol is encountered and its counterpart is on the top of the stack, decrement the stack counter

			//If the symbol's counterpart is not on the top of the stack, print the parameter that caused the miss and return a failed case
		} else if(current == '}'){
			if(stack[stackcounter] == '{'){
				stack[stackcounter] = '0';
				stackcounter--;
			} else {
				printf("%d: %c", i, current);
				free(stack);
				return EXIT_FAILURE;
			}
		} else if(current == ')'){
			if(stack[stackcounter] == '('){
				stack[stackcounter] = '0';
				stackcounter--;
			} else { 
				printf("%d: %c", i, current);
				free(stack);
				return EXIT_FAILURE;
			}
		}
	}

	//If the entire input has been traveresed without an improper closed grouping symbol, check the stack counter
	if(stackcounter == 0){
		free(stack);
		return EXIT_SUCCESS;
		//If the stack counter is 0, there are no unclosed grouping symbols and return a success
	} else {
		printf("open: ");
		for(int j = length; j != 0; j--){
			if(stack[j] == '0'){
				continue;
			} else {
				current = stack[j];
				if(current == '('){
					printf(")");
				} else if(current == '{') {
					printf("}");
				} else if(current == '['){
					printf("]");
				}
			}
		}
		free(stack);
		return EXIT_FAILURE;
		//If the stack counter is not zero, pop all characters on the stack and print them to show the unclosed grouping symbols and return a failed case
	}
}
