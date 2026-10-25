#include "stack.h"

int Stack :: Peep()
{
	/* Checking the list is empty or not */
	if(top == nullptr)
	{
		cout << "INFO : Stack is empty\n";
		return FAILURE;
	}

	/* Storing the value in top for traversing */
	Node* temp = top;	

	while(temp)		
	{
		/* Printing the value from stack */
		cout << temp->data << "-> ";

		/* Traversing to next node */
		temp = temp -> link;
	}

	cout << "NULL\n";
	return SUCCESS;
}