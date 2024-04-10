#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(int argc, char**argv){
	//If there is no input string, print nothing and return
	if(strcmp(argv[1],"") == 0){return 0;}

	//Intilialize i and loud
	int i = 0;
	char loud = '\0';

	//Traverse input and capitalize any lower case characters
	while(argv[1][i]){
		loud = argv[1][i];
		if(islower(loud)){
			printf("%c", toupper(loud));
		} else {
			printf("%c", loud);
		}
		i++;
	}

	//Print exclamation points and return
	printf("!!\n");
	return 0;
}
