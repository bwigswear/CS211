#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int data;
	struct node *left;
	struct node *right;
}node;

//Declare root of bst and size variable
node *first = NULL;
int size = 0;

int search(node *root, int input){

	//If bst is empty return a failed search
	if(size == 0){printf("absent\n");}

	if(root == NULL){return 0;}

	//If query is found, return a successful search
	if(input == root->data){return 1;}

	//If query is not equal to current node's value, move to next
	if(input < root->data){
		return search(root->left, input);
	} else if(input > root->data){
		return search(root->right, input);
	}

	return 0;
}

node* max(node *root){

	//If there is no root to the right of the current node, return the current node's value

	if(root->right == NULL){return root;} else {return max(root->right);}
}

node* insert(node *root, int input){

	//If the current node is empty, allocate a new node and return it
	if(root == NULL){
		node *new = (node*)malloc(sizeof(node));
		new->data = input;
		new->left = NULL;
		new->right = NULL;
		size++;
		printf("inserted\n");
		return new;
	}

	/*Traverse the bst until the appropriate location for a new node is found,
	 if the input value is already present do not insert a new node*/
	if(input < root->data){

		root->left = insert(root->left, input);

	} else if(input > root->data){

		root->right = insert(root->right, input);

	} else if(input == root->data){

		printf("not inserted\n");
		return root;

	}
	return root;
}

node* delete(node *root, int input){

	//If the bst is empty, state that the query is absent
	if(size == 0){

		printf("absent\n");

		return NULL;
	}

	if(root == NULL){printf("absent\n"); return NULL;}

	//Search the bst for the query
	if(input < root->data){

		root->left = delete(root->left, input);

	} else if(input > root->data){

		root->right = delete(root->right, input);

	} else if(input == root->data){

		if(root->left == NULL && root->right == NULL){

			/*If the query is found in a node with no children,
			delete the node and reduce the size*/
			free(root);
			printf("deleted\n");
			size--;
			return NULL;

		} else if(root->left == NULL){

			/*If the query is found in a node with only a right child,
			delete the node and replace it with the right child*/
			node *temp = root->right;
			free(root);
			printf("deleted\n");
			size--;
			return temp;

		} else if(root->right == NULL){

			/*If the query is found in a node with only a left child,
			delete the node and replace it with the left child*/
			node *temp = root->left;
			free(root);
			printf("deleted\n");
			size--;
			return temp;

		} else {

			/*If the query is found in a node with two children, replace the
			node with the largest value node in the left branch*/
			node *temp = max(root->left);
			root->data = temp->data;
			root->left = delete(root->left, root->data);

		}

	}

	return root;
}

void print(node *input){

	//If the bst is empty, print nothing
	if(size == 0){return;}

	//Traverse the bst and print each entry
	printf("(");

	if(input->left != NULL){

		print(input->left);

	}

	printf("%d", input->data);

	if(input->right != NULL){

		print(input->right);

	}

	printf(")");

	return;
}

int main(int argc, char **argv){

	//Variables for input command and input value
	char command;
	int input;

	/*Wait for a relevant input command and input value.
	If an appropriate command character is received, run the
	associated function*/

	while(1){

		//If there is no input received, exit the loop
		if(scanf("%c", &command) < 1){break;}

		if(command == 'i'){

			//Insert a node in the tree with the inputed value

			if(scanf("%d", &input) < 1){break;}

			if(size == 0){

				node *temp = NULL;
				first = temp;
				first = insert(first, input);

			} else {

				insert(first, input); 

			}

		} else if(command == 'd'){

			//Attempt to delete a node in the bst with the inputed value

			if(scanf("%d", &input) < 1){break;}

			first = delete(first, input); 

		} else if(command == 's'){

			//Search for a node with the inputed value

			if(scanf("%d", &input) < 1){break;}


			if(size == 0){printf("absent\n"); continue;}

			int a = search(first, input);

			if(a){printf("present\n");}else{printf("absent\n");}

		}else if(command == 'p'){print(first); printf("\n");}

	}

}
