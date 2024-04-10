#include <stdio.h>
#include <stdlib.h>

void printmatrix(double*, int, int);

double* multiply(double* product, double* matrix1, int rows1, int cols1, double* matrix2, int rows2, int cols2){
	//Receive target matrix, along with two matrices to be multiplied and their parameters

	//Traverse target matrix rows and columns in order to set product matrix parameters
	for(int i = 0; i < rows1; i++){
		for(int j = 0; j < cols2; j++){

			//Create entry variable to hold current param value and tempcalc to add each subsequent product to entry variable
			double entry = 0;
			double tempcalc = 0;

			//Find product of corresponding entries in multiplicand matrix and multiplier matrix and add them to entry's current value
			for(int k  = 0; k < cols1; k++){
				tempcalc = matrix1[i * cols1 + k] * matrix2[cols2 * k + j];
				entry += tempcalc;
			}

			//Set target matrix parameter to equal calculated value
			product[i * cols2 + j] = entry;
		}
	}
	return product;
}

double* transpose(double* target, double* matrix, int rows, int cols){
	//Receive target matrix, along with matrix to be transposed and its parameters

	//Traverse transpose matrix columns and rows, setting each parameter equal to its corresponding non-transpose parameter
	for(int i = 0; i < cols; i++){
		for(int j = 0; j < rows; j++){
			target[i * rows + j] = matrix[j * cols + i];
		}
	}
	return target;
}

double* inverse(double* target, double* matrix, int dimension){
	//Recieve target matrix, along with matrix to be inverted and its square dimension

	//Malloc space for a left-hand matrix and right-hand matrix in order to perform the Gauss/Jordan matrix inverse method
	double *leftcalc = malloc(dimension * dimension * sizeof(double));
	double *rightcalc = malloc(dimension * dimension * sizeof(double));

	//F is a variable used to temporarily store matrix parameters and use them as divisors
	double f = 0;

	//Set leftcalc equal to the original matrix and rightcalc equal to the identity matrix
	for(int i = 0; i < dimension; i++){
		for(int j = 0; j < dimension; j++){
			leftcalc[i * dimension + j] = matrix[i * dimension + j];
			if(i == j){rightcalc[i * dimension + j] = 1;}else{rightcalc[i * dimension +j] = 0;}
		}
	}

	for(int k = 0; k < dimension; k++){
		//Set f = to parameter (k,k) of left matrix and use it as a divisor for the corresponding row of both the left and right matrices
		f = leftcalc[k * dimension + k];
		for(int l = 0; l < dimension; l++){
			leftcalc[k * dimension + l] = leftcalc[k * dimension + l] / f;
			rightcalc[k * dimension + l] = rightcalc[k * dimension + l] / f;
		}
		for(int m = k + 1; m < dimension; m++){
			f = leftcalc[m * dimension + k];
			for(int n = 0; n < dimension; n++){
				leftcalc[m * dimension + n] = leftcalc[m * dimension + n] - (f * leftcalc[k * dimension + n]);
				rightcalc[m * dimension + n] = rightcalc[m * dimension + n] - (f * rightcalc[k * dimension + n]);
			}
		}

	}

	for(int p = dimension - 1; p >= 0; p--){
		for(int q = p - 1; q >= 0; q--){
			f = leftcalc[q * dimension + p];
			for(int r = 0; r < dimension; r++){
				leftcalc[q * dimension + r] = leftcalc[q * dimension + r] - (f * leftcalc[p * dimension + r]);
				rightcalc[q * dimension + r] = rightcalc[q * dimension + r] - (f * rightcalc[p * dimension + r]);
			}
		}
	}

	//Since target matrix is allocated in caller function, set target equal to rightcalc and free left and right matrices
	for(int s = 0; s < dimension * dimension; s++){
		target[s] = rightcalc[s];
	}
	free(leftcalc); free(rightcalc);

	return target;
}

void printmatrix(double* input, int rows, int cols){
	//Traverse target matrix and print each parameter. If last parameter in a row, print new line
	for(int i = 0; i < rows * cols; i++){
		if(i % cols == cols - 1){printf("%lf\n", input[i]); continue;}
		printf("%lf ", input[i]);
	}
	return;
}

int main(int argc, char** argv){

	//Open train file and read in train string
	FILE* housedata = fopen(argv[1], "r");
	char firstline[6];
	fscanf(housedata, "%5s", firstline);

	//Read house and attribute numbers
	int numattributes;
	int numhouses;
	fscanf(housedata, "%d", &numattributes);
	fscanf(housedata, "%d", &numhouses);

	//Allocate space for attributes, prices, and weights matrices and populate them
	double* attributes = malloc(numhouses * (numattributes + 1) * sizeof(double));
	double* prices = malloc(numhouses * sizeof(double));
	double* weight = malloc((numattributes + 1) * sizeof(double));
	double current = 0;

	for(int i = 0; i < numhouses; i++){

		attributes[i * (numattributes + 1)] = 1;

		for(int j = 1; j < numattributes + 1; j++){

			fscanf(housedata, "%lf", &current);
			attributes[i * (numattributes + 1) + j] = current;

		}

		fscanf(housedata, "%lf", &current);
		prices[i] = current;

	}

	//Calculate weight matrix using attributes and prices

	double* xtranspose = malloc(numhouses * (numattributes + 1) * sizeof(double));
	transpose(xtranspose, attributes, numhouses, numattributes + 1);

	double* xproduct = malloc(numhouses * numhouses * sizeof(double));
	multiply(xproduct, xtranspose, numattributes + 1, numhouses, attributes, numhouses, numattributes + 1);

	double* xinverse = malloc(numhouses * numhouses * sizeof(double));
	inverse(xinverse, xproduct, numattributes + 1);

	double* xcalc = malloc(numhouses * numhouses * sizeof(double));

	multiply(xcalc, xinverse, numattributes + 1, numattributes + 1, xtranspose, numattributes + 1, numhouses);

	multiply(weight, xcalc, numattributes + 1, numhouses, prices, numhouses, 1);

	free(xtranspose); free(xproduct); free(xinverse); free(xcalc);

	//Weight array now contains weights needed to calculate a house price given its attributes

	//Close train text file and open file containing new house data
	FILE* newhouses = fopen(argv[2], "r");
	fscanf(newhouses, "%4s", firstline);

	//Read house and attribute numbers and check if the attribute numbers align between train file and new file
	int numattributes2;
	int numhouses2;
	fscanf(newhouses, "%d", &numattributes2);
	fscanf(newhouses, "%d", &numhouses2);
	if(numattributes != numattributes2){printf("error"); return 0;}

	//Allocate space for new house and price matrices and populate them
	double* attributes2 = malloc(numhouses2 * (numattributes2 + 1) * sizeof(double));
	double* prices2 = malloc(numhouses2 * sizeof(double));

	for(int a = 0; a < numhouses2; a++){

		attributes2[a * (numattributes2 + 1)] = 1;

		for(int b = 1; b < numattributes2 + 1; b++){

			fscanf(newhouses, "%lf", &current);
			attributes2[a * (numattributes2 + 1) + b] = current;

		}

	}

	//Calculate new house prices using new house attributes and weight matrices
	multiply(prices2, attributes2, numhouses2, numattributes2 + 1, weight, numattributes + 1, 1);

	//Since prices2 matrix contains the calculated prices, print it
	for(int c = 0; c < numhouses2; c++){printf("%.0f\n", prices2[c]);}


	free(attributes);
	free(prices); free(weight); free(attributes2); free(prices2);
	fclose(housedata);
	fclose(newhouses);
	return EXIT_SUCCESS;

}

