#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv){

	int dimension;

	//Open matrix text file input and store matrix dimension
	FILE* matrix = fopen(argv[1], "r");
	fscanf(matrix, "%d", &dimension);

	//Malloc the matrices used for matrix multiplication
	int *original = malloc(dimension * dimension * sizeof(int));
	int *product = malloc(dimension * dimension * sizeof(int));
	int *dynamic = malloc(dimension * dimension * sizeof(int));

	//Read in matrix to be raised to a certain power
	int temp = 0;
	for(int c = 0; c < dimension; c++){

		for(int d = 0; d < dimension; d++){
			fscanf(matrix, "%d", &temp);
			original[c * dimension + d] = temp;
			product[c * dimension + d] = temp;
		}
	}

	//Store the number of times the matrix is to be multiplied into itself
	int repetition;
	fscanf(matrix, "%d", &repetition);

	//If the exponent is zero, print the identity matrix and return
	if(repetition == 0){
		if(dimension == 1){printf("1\n");}else{
		for(int q = 0; q < dimension; q++){
			for(int w = 0; w < dimension; w++){
				if(w == q){
					printf("1");
				} else {
					printf("0");
				}
				if(w == dimension - 1 && w != dimension * dimension - 1){
					printf("\n");
				} else {printf(" ");}
			}
		}
		}
		fclose(matrix);
		free(product);
		free(original);
		free(dynamic);
		return 0;
	}

	/*In order to square a matrix, each entry has to be calculated one by one. To obtain each
	entry of the squared matrix, multiply each respective entry in the corresponding row of the
	original matrix by the entry in the corresponding column and add them together*/

	for(int p = 1; p < repetition; p++){//Loop for each power of the exponent

		for(int i = 0; i < dimension; i++){//Traverse each row

			for(int j = 0; j < dimension; j++){//Traverse each column

				//Create variables to store and calculate each matrix parameter
				int entry = 0;
				int tempcalc = 0;

				for(int a = 0; a < dimension; a++){
					tempcalc = product[i * dimension + a] * original[a * dimension + j];
					entry += tempcalc;
				}

				//Store each product matrix paramater into dynamic so that it can be used to find the next product
				dynamic[i * dimension + j] = entry;
			}
		}

		//Set the product matrix equal to the current dynamic matrix so that dynamic is free to be used in the next calculation
		for(int t = 0; t < dimension; t++){
			for(int z = 0; z < dimension; z++){
				product[t * dimension + z] = dynamic[t * dimension + z];
			}
		}
	}

	//Print the resulting matrix
	for(int r = 0; r < dimension * dimension; r++){
		if(r % dimension == dimension - 1){printf("%d\n", product[r]); continue;}
		printf("%d ", product[r]);
	}

	fclose(matrix);
	free(product);
	free(original);
	free(dynamic);
	return 0;
}
