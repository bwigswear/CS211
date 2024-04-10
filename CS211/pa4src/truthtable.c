
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Enumerate gate types so that type checking is easier
typedef enum  {AND, OR, NAND, NOR, XOR, NOT, PASS, DECODER, MULTIPLEXER } kind_t;

//Variable struct to be stored in a variable array once the inputs and outputs are read
typedef struct variable {
	char name[17];
	int enumeration;
	int value;
}variable;


/*Logic gate struct that contains a link to the next gate so that the circuit
can be processed sequentially and memory can be dynamically allocated for gate variables*/

typedef struct gate {
	int size;
	int temp;
	int* params;
	kind_t kind;
	struct gate *next;
}gate;


/*Temporary variable struct that contains a link to another temporary variable
so that memory can be dynamically allocated for temporary variables*/

typedef struct tempvariable {
	char name[17];
	int enumeration;
	int value;
	struct tempvariable *next;
}tempvariable;

//Store number of input and output variables
int inputnum;
int outputnum;

int tempnum = 0;

//Int arrays to enumerate input and output variables
int* inputvariables;
int* outputvariables;

//Array of variables to store the given list of input and output variables
variable* allvariables;

//Header of temporary variable linked list to store temporary variables as they appear when the circuit is being read
tempvariable* firsttemp;

//Header of logic gate linked list to store the logic gates sequentially
gate* firstgate;

//Variable to store the number of current gates
int numgates = 0;

//Function prototype because I wrote callgate after runcircuit for whatever reason
void callgate(gate *currentgate);

//Retrieves the current value of a given variable given a gate and the corresponding variable parameter of the gate
int getValue(gate *query, int parameter){

	//Stores the enumeration of the variable at the given gate and location
	int findparam = query->params[parameter];

	//Check to see whether the variable is temporary or not
	if(findparam >= 0){

		//If the variable is not a temporary one, then just retrieve its current value
		return allvariables[query->params[parameter]].value;

	} else {

		//If the variable is temporary, traverse the temporary variable linked list in order to find its current value
		tempvariable* find = firsttemp;

		for(int i = 0; i < tempnum; i++){

			if(find->enumeration == findparam){return find->value;}
			find = find->next;

		}
	}

	return 0;
}

//Sets the current value of a variable given a gate, the corresponding variable parameter of the gate, and a new value
void setValue(gate *query, int parameter, int newvalue){

	//Stores the enumeration of the variable at the given gate and location
	int param = query->params[parameter];

	//Check to see whether the variable is temporary or not
	if(param >= 0){

		//If the variable is not a temporary one, then just change its current value
		allvariables[query->params[parameter]].value = newvalue;

	} else {

		//If the variable is a temporary one, traverse the temporary variable linked list in order to change its current value
		tempvariable* find = firsttemp;

		for(int i = 0; i < tempnum; i++){

			if(find->enumeration == param){find->value = newvalue; break;}
			find = find->next;

		}
	}

	return;
}

/*Compares an inputted variable name to the list of current variables to find its current enumeration,
if the variable is a temporary variable and is not already stored, allocates space and stores it. Also
accounts for any constant 0 or 1 by storing them as temporary variables with their associated value*/
int getEnum(char* varname){

	//Searches input and output variables
	for(int i = 0; i < inputnum + outputnum; i++){

		if(strcmp(allvariables[i].name, varname) == 0){return i;}

	}

	//If there are no temporary variables, set query variable as the first temporary variable
	if(tempnum == 0){

		firsttemp = malloc(sizeof(tempvariable));
		strcpy(firsttemp->name, varname);
		firsttemp->enumeration = -1;
		tempnum++;

		//If a constant 0 or 1 is read, store them as a temporary variable with their respective value
		if(strcmp(varname, "0") == 0){firsttemp->value = 0;}else if(strcmp(varname, "1") == 0){firsttemp->value = 1;}

		return -1;
	} else {

		//If there already exists a temporary variable, searches the current temporary variables for the enumeration
		tempvariable* find = firsttemp;

		for(int i = 0; i < tempnum; i++){

			if(strcmp(find->name, varname) == 0){return find->enumeration;}
			if(i != tempnum - 1){find = find->next;}

		}

		//If the temporary variable is not already stored, create a new temporary variable and store it
		tempvariable* new = malloc(sizeof(tempvariable));
		strcpy(new->name, varname);
		tempnum++;
		new->enumeration = -1 * tempnum;
		if(strcmp(varname, "0") == 0){new->value = 0;}else if(strcmp(varname, "1") == 0){new->value = 1;}
		find->next = new;

		return new->enumeration;

	}

	return -10000;
}

//Given a list of input variable values, runs the current global circuit using the given values as inputs
void runcircuit(int* inputs){

	//Sets the values of all current variables according to the input array
	for(int i = 0; i < inputnum; i++){

		allvariables[i].value = inputs[i];

	}

	//Creates a temporary variable to store the first gate variable
	gate* circuit = firstgate;

	//Traverses the circuit and calls each sequential gate to act upon the current variable values
	for(int i = 0; i < numgates; i++){callgate(circuit); circuit = circuit->next;}

	return;
}

//Function that reads a gate type and does the computation stored within the gate
void callgate(gate *currentgate){

	//Reads the gate type and then acts accordingly
	kind_t type = currentgate->kind;

	if(type == AND){

		//Set third parameter equal to product of first two
		int new = getValue(currentgate, 0) * getValue(currentgate, 1);
		setValue(currentgate, 2, new);

	}else if(type == OR){

		//Set third paramater equal to 1 if one parameter is equal to 1
		int new = getValue(currentgate, 0) || getValue(currentgate, 1);
		setValue(currentgate, 2, new);

	}else if(type == NAND){

		//Set third parameter equal to opposite of the product of the first two
		int new = getValue(currentgate, 0) * getValue(currentgate, 1);
		new = !new;
		setValue(currentgate, 2, new);

	}else if(type == NOR){

		//Set third parameter equal to 1 if both parameters are 0
		int new = getValue(currentgate, 0) || getValue(currentgate, 1);
		new = !new;
		setValue(currentgate, 2, new);

	}else if(type == XOR){

		//Set third paramater equal to 1 if only one parameter is 1
		int new = getValue(currentgate, 0) + getValue(currentgate, 1);
		if(new == 2){setValue(currentgate, 2, 0);}else{setValue(currentgate, 2, new);}

	}else if(type == NOT){

		//Set second parameter equal to opposite of first
		setValue(currentgate, 1, !getValue(currentgate, 0));

	}else if(type == PASS){

		setValue(currentgate, 1, getValue(currentgate, 0));

	}else if(type == DECODER){

		//Using the inputs, calculate which output is set to 1. Then, set all outputs to 0. Afterwards, set the desired output to 1.
		int size = currentgate->size;
		int total = getValue(currentgate, 0);
		int change = 0;
		for(int i = 1; i < size; i++){total = total << 1; change = getValue(currentgate, i); total += change;}
		for(int i = 0; i < (1 << size); i++){setValue(currentgate, size + i, 0);}
		setValue(currentgate, total + size, 1);

	}else if(type == MULTIPLEXER){

		//Using the selectors, calculate which input is chosen as the output and set the output accordingly
		int size = currentgate->size;
		int total = getValue(currentgate, 1 << size);
		int change = 0;
		for(int i = 1; i < size; i++){total = total << 1; change = getValue(currentgate, (1 << size) + i); total +=change;}
		setValue(currentgate, size + (1 << size), getValue(currentgate, total));

	}
	return;
}

//Creates a new gate variable when given a gate type and if the gate is a decoder or multiplexer, a gate size
gate* newgate(kind_t type, int DorM){

	//If this is the first gate, allocate space and set it as the first gate variable
	//Given the gate type, the size is set and the parameter array is allocated
	if(numgates == 0){
		if(type >= 0 && type <= 4){

			firstgate = malloc(sizeof(gate));
			firstgate->size = 3;
			firstgate->params = malloc(3 * sizeof(int));

		} else if(type > 4 && type < 7){

			firstgate = malloc(sizeof(gate));
			firstgate->size = 2;
			firstgate->params = malloc(2 * sizeof(int));

		} else if(type == 7){

			firstgate = malloc(sizeof(gate));
			firstgate->size = DorM;
			firstgate->params = malloc((DorM + (1 << DorM)) * sizeof(int));

		} else{

			firstgate = malloc(sizeof(gate));
			firstgate->size = DorM;
			firstgate->params = malloc((DorM + 1 + (1 << DorM)) * sizeof(int));

		}

		firstgate->kind = type;
		numgates++;
		return firstgate;

	}else{

		//If this is not the first gate, traverse the already existing gates to find the end of the circuit
		gate* temp = firstgate;

		for(int i = 0; i < numgates - 1; i++){

			temp = temp->next;

		}

		//Create a new temporary gate to allocate space for the new gate and create it
		gate* temp2;

		if(type >= 0 && type <= 4){

			temp2 = malloc(sizeof(gate));
			temp2->size = 3;
			temp2->params = malloc(3 * sizeof(int));

		} else if(type > 4 && type < 7){

			temp2 = malloc(sizeof(gate));
			temp2->size = 2;
			temp2->params = malloc(2 * sizeof(int));

		} else if(type == 7){

			temp2 = malloc(sizeof(gate));
			temp2->size = DorM;
			temp2->params = malloc((DorM + (1 << DorM)) * sizeof(int));

		} else {

			temp2 = malloc(sizeof(gate));
			temp2->size = DorM;
			temp2->params = malloc((DorM + 1 + (1 << DorM)) * sizeof(int));

		}

		//Add the new gate to the end of the circuit
		temp2->kind = type;
		temp->next = temp2;
		numgates++;
		return temp2;

	}

}

int main(int argc, char** argv){

	//Open circuit details to be read
	FILE *readcircuit;
	readcircuit = fopen(argv[1], "r");

	//String to store type of current gate being read
	char gatetype[17];

	//Use gatetype string temporarily to read in input denotation
	fscanf(readcircuit, "%16s", gatetype);

	//Read in number of input variables and allocate space for input enumeration array
	fscanf(readcircuit, "%d", &inputnum);
	inputvariables = malloc(inputnum * sizeof(int));

	/*Create currentvariable to store the variable currently being read in and firsthalf to store
	input variable array, to be later consolidated along with output variables*/
	variable currentvariable;
	variable* firsthalf = malloc(inputnum * sizeof(variable));

	//Read in input variables and add them to firsthalf
	for(int i = 0; i < inputnum; i++){fscanf(readcircuit, "%s", currentvariable.name); currentvariable.enumeration = i; inputvariables[i] = i; firsthalf[i] = currentvariable;}

	//Use gatetype temporarily to read in output denotation 
	fscanf(readcircuit, "%16s", gatetype);

	//Read in number of outputs and allocate space for output variable enumeration array
	fscanf(readcircuit, "%d", &outputnum);
	outputvariables = malloc(outputnum * sizeof(int));

	//Now that both input and output quantities are known, allocate space for all variable array
	allvariables = malloc((inputnum + outputnum) * sizeof(variable));

	//Copy firsthalf to allvariables and free it
	for(int i = 0; i < inputnum; i++){allvariables[i] = firsthalf[i];}
	free(firsthalf);

	//Read in output variables and add them to the allvariables array
	for(int j = 0; j < outputnum; j++){fscanf(readcircuit, "%16s", currentvariable.name); currentvariable.enumeration = inputnum + j; outputvariables[j] = inputnum + j; allvariables[inputnum + j] = currentvariable;}

	//Read in each gate sequentially and store it in the circuit
	char currentvar[17];
	while(fscanf(readcircuit, "%16s", gatetype) == 1){

		//A variable to traverse the following loops
		int a = 0;

		//Gate variable to store the current gate
		gate* currentgate;

		//Read in the current gate and store it in currentgate
		//Given the gate type, read in the appropriate number of variables
		if(strcmp(gatetype,"AND") == 0){

			currentgate = newgate(AND, 0);
			for(a = 0; a < 3; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"OR") == 0){

			currentgate = newgate(OR, 0);
			for(a = 0; a < 3; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"NAND") == 0){

			currentgate = newgate(NAND, 0);
			for(a = 0; a < 3; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"NOR") == 0){

			currentgate = newgate(NOR, 0);
			for(a = 0; a < 3; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"XOR") == 0){

			currentgate = newgate(XOR, 0);
			for(a = 0; a < 3; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"NOT") == 0){

			currentgate = newgate(NOT, 0);
			for(a = 0; a < 2; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		}else if(strcmp(gatetype,"PASS") == 0){

			currentgate = newgate(PASS, 0);
			for(a = 0; a < 2; a++){fscanf(readcircuit, "%16s", currentvar); currentgate->params[a] = getEnum(currentvar);}

		} else if(strcmp(gatetype,"DECODER") == 0){

			//Given the number of inputs to the decoder, read in the appropriate number of variables

			/*If a given variable is a "Don't care" variable, set its enumeration to 1000, since this is outside the scope of the 
			assignment test cases (a better method could be written to handle larger test cases, such as a DNC flag in the gate struct)*/
			int DorM = 0;
			fscanf(readcircuit, "%d", &DorM);
			currentgate = newgate(DECODER, DorM);
			for(a = 0; a < DorM + (1 << DorM); a++){fscanf(readcircuit, "%16s", currentvar); if(strcmp(currentvar,"_") == 0){currentgate->params[a] = 1000;}else{currentgate->params[a] = getEnum(currentvar);}}

		} else if(strcmp(gatetype,"MULTIPLEXER") == 0){

			//Multiplexers are handled similarly to decoders
			int DorM = 0;
			fscanf(readcircuit, "%d", &DorM);
			currentgate = newgate(MULTIPLEXER, DorM);
			for(a = 0; a < DorM + (1 << DorM) + 1; a++){fscanf(readcircuit, "%16s", currentvar); if(strcmp(currentvar,"_") == 0){currentgate->params[a] = 1000;}else{currentgate->params[a] = getEnum(currentvar);}}

		} else {return -1;}

	}

	//After the circuit has been read, create an array of ints to store the input values to be tested
	int i = 0;
	int* test = malloc(inputnum * sizeof(int));

	//Populate the input test array with zeros and run the circuit using the inputs. Then, print out the output variable current values.
	for(i = 0; i < inputnum; i++){test[i] = 0; printf("%d ", 0);}
	runcircuit(test);
	printf("|");
	for(i = 0; i < outputnum; i++){printf(" %d", allvariables[inputnum + i].value);}
	printf("\n");

	//Test each possible input combination then print the outputs after each test
	int counter = 0;
	for(long a = 0; a < (1 << inputnum) - 1; a++){

		/*Logic: Change the first 0 input encountered and switch it to a 1, while counting the number of 1's
		passed over in the process. Then, change each 1 passed over back to a 0.*/
		for(int j = inputnum - 1; j > -1; j--){

			if(!test[j]){test[j] = 1; break;}
			counter++;

		}

		for(int j = 0; j < counter; j++){

			test[inputnum - 1 - j] = 0;

		}

		//Print input values, run the circuit, then print output variables
		for(int j = 0; j < inputnum; j++){printf("%d ", test[j]);}
		runcircuit(test);
		printf("|");
		for(int j = 0; j < outputnum; j++){printf(" %d", allvariables[inputnum + j].value);}
		printf("\n");
		counter = 0;
	}

	free(test);
	free(inputvariables);
	free(outputvariables);
	free(allvariables);
}
