#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int data;
	struct node *next;
}node;

node *first = NULL;
int size = 0;

void insert(int input){

	//Allocate new node and populate it with input data
	node *new = (node*)malloc(sizeof(node));
	new->data = input;

	//If linked list is empty, set the new node as the head and return
	if(size == 0){first = new; size++; return;}

	//If linked list is not empty, create a temporary node to traverse the linked list
	node *temp = first;

	/*If the new input is already present in the list, free the temporary node and return.
	Otherwise, traverse the list until the input data is greater than the current node and
	and less than the next, then insert and increase the size*/
	if(new->data == temp->data){free(new); return;}

	if(new->data < temp->data){new->next = temp; first = new; size++; return;}

	for(int i = 0; i < size - 1; i++){

		if(new->data == temp->data || new->data == temp->next->data){

			free(new);
			return;

		}

		if(new->data > temp->data && new->data < temp->next->data){

			new->next = temp->next;
			temp->next = new;
			size++;
			return;

		}

		temp = temp->next;

	}

	//If the input is the greatest in the list, add the node to the end of the list and increase the size
	temp->next = new;
	size++;

	return;
}

void delete(int input){

	//If the list is empty, return
	if(size == 0){

		return;

	}

	//Create a temporary node to traverse the linked list
	node *temp = first;

	//Traverse the list and if the input is found, delete the node and reduce the size
	if(temp->data == input){first = temp->next; free(temp); size--; return;}

	for(int i = 0; i < size - 1; i++){

		if(temp->next->data == input){

			node *freenode = temp->next;
			temp->next = temp->next->next;
			size--;
			free(freenode);
			return;

		}

		temp = temp->next;

	}

	return;

}

int main(int argc, char** argv){

	char command;
	int input;

	while(1){

		//Listen for a command and input value
		if(scanf("%c%d", &command, &input) < 1){break;};

		//If a valid command is read, call the associated function
		if(command == 'd'){

			delete(input);
			printf("%d :", size);
			node *traverse = first;

			for(int i = 0; i < size; i++){

				printf(" %d", traverse->data);
				traverse = traverse->next;

			}

			printf("\n");

		} else if(command == 'i'){

			insert(input);
			printf("%d :", size);
			node *traverse = first;

			for(int j = 0; j < size; j++){

				printf(" %d", traverse->data);
				traverse = traverse->next;

			}

			printf("\n");

		}

	}

	return 0;
}
