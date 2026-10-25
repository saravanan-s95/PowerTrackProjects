#include "stack.h"

int main()
{
	int option, data, peek;

	/* Creating stack object */
	Stack stack;

	cout << "1. Push\n2. Pop\n3. Display Stack\n4. Peek(Element at Top)\n5. Exit\nEnter choice: ";

	while(1)
	{
		/*ask user options*/
		cin >> option;

		switch(option)
		{
			case 1:

				/* To Push the element */
				cout << "Enter the element you have to insert at the first : ";
				cin >> data;
				if(stack.Push(data) == FAILURE)
				{
					cout << "INFO : Push operation Failure\n";
				}
				break;
			case 2:

				/* To Pop the top of the value in the stack */
				if (stack.Pop() == FAILURE)
				{
					cout << "INFO : Stack is empty\n";
				}
				else
				{
				    cout << "INFO : Pop operation is successfull\n";
				}
				break;
			case 3:
				/* Printing or Dispaying the elements */
				stack.Peep();
				break;
			case 4:
				/* To find the Peek element in the stack */
				if((peek = stack.Peek()) == FAILURE)
				{
					cout << "INFO : Stack is empty\n";
				}
				else
				{
					cout << "INFO : Peek element is " << peek << endl;
				}
				break;
			case 5:
				return SUCCESS;

			default:
				cout << "Enter proper choice !!\n";
				break;
		}

	}

	return SUCCESS;
}
