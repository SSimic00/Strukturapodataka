#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#define MAX_SIZE 50

struct Element;
typedef struct Element* Position;

typedef struct Element {
	double number;
	Position next;
} El;

Position CreateElement(double number);
int InsertAfter(Position P, Position newElement);
int Push(Position Head, double number);
int DeleteAfter(Position P);
int Pop(double* destination, Position Head);
int Operation(Position Head, char op);
int CalculatePostfixFromFile(char* filename);
int DeleteAll(Position Head);

int main(int argc, char** argv)
{
	char filename[MAX_SIZE] = { 0 };

	printf("Enter the file name:");
	scanf(" %s", filename);

	CalculatePostfixFromFile(filename);

	return EXIT_SUCCESS;
}

Position CreateElement(double number)
{
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(El));
	if (!newElement)
	{
		perror("Unable to allocate memory!\n");
		system("pause > nul");
		return NULL;
	}

	newElement->number = number;
	newElement->next = NULL;

	return newElement;
}

int InsertAfter(Position P, Position newElement)
{
	newElement->next = P->next;
	P->next = newElement;

	return EXIT_SUCCESS;
}

int Push(Position Head, double number)
{
	Position newElement = NULL;

	newElement = CreateElement(number);
	if (!newElement)
		return -1;

	InsertAfter(Head, newElement);
}

int DeleteAfter(Position P)
{
	Position temp = P->next;

	if (!temp)
		return EXIT_SUCCESS;

	P->next = temp->next;
	free(temp);

	return EXIT_SUCCESS;
}

int Pop(double* destination, Position Head)
{
	Position first = Head->next;

	if (!first)
	{
		perror("The postfix expression is not valid. Check the content of the file!\n");
		system("pause > nul");
		return -1;
	}

	*destination = first->number;

	DeleteAfter(Head); // remove the "book"

	return EXIT_SUCCESS;
}

int DeleteAll(Position Head)
{
	Position temp = Head;

	while (temp->next)
	{
		DeleteAfter(temp);
	}

	return EXIT_SUCCESS;
}

int Operation(Position Head, char op)
{
	double operand2 = 0.0;
	double operand1 = 0.0;
	double result = 0.0;
	int status1 = EXIT_SUCCESS;
	int status2 = EXIT_SUCCESS;

	status2 = Pop(&operand2, Head);
	if (status2 != EXIT_SUCCESS)
		return -1;

	status1 = Pop(&operand1, Head);
	if (status1 != EXIT_SUCCESS)
		return -2;

	switch (op)
	{
	case '+':
	{
		result = operand1 + operand2;
		printf("%lf %c %lf = %lf\n", operand1, op, operand2, result);
		break;
	}

	case '-':
	{
		result = operand1 - operand2;
		printf("%lf %c %lf = %lf\n", operand1, op, operand2, result);
		break;
	}

	case '*':
	{
		result = operand1 * operand2;
		printf("%lf %c %lf = %lf\n", operand1, op, operand2, result);
		break;
	}

	case '/':
	{
		if (operand2 == 0)
		{
			printf("Division by 0 is not possible!\n");
			return -3;
		}
		result = operand1 / operand2;
		printf("%lf %c %lf = %lf\n", operand1, op, operand2, result);
		break;
	}

	default:
	{
		printf("Operation not supported!\n");
		system("pause > nul");
		DeleteAll(Head);
		return -4;
	}
	}

	Push(Head, result);

	return EXIT_SUCCESS;
}

int CalculatePostfixFromFile(char* filename)
{
	FILE* fp = NULL;
	int fileLength = 0;
	char* buffer = NULL;
	char* currentBuffer = NULL;
	int nB = 0;
	char op = 0;
	double value = 0;
	int status = 0;
	int counter = 0;
	El Head = { .number = 0, .next = NULL };
	fp = fopen(filename, "rb");

	if (!fp)
	{
		perror("Unable to open the file!\n");
		system("pause > nul");
		return -1;
	}

	fseek(fp, 0, SEEK_END);
	fileLength = ftell(fp);

	buffer = (char*)malloc((fileLength + 1) * sizeof(char));

	if (!buffer)
	{
		perror("Unable to allocate memory!\n");
		system("pause > nul");
		return -1;
	}

	rewind(fp);

	fread(buffer, sizeof(char), fileLength, fp);
	printf("Postfix: \n");
	fclose(fp);

	currentBuffer = buffer;
	while (strlen(currentBuffer) > 0)
	{
		counter = sscanf(currentBuffer, " %lf %n", &value, &nB);

		if (counter == 1)
		{
			Push(&Head, value);
			currentBuffer += nB;
		}

		else
		{
			sscanf(currentBuffer, " %c %n", &op, &nB);
			status = Operation(&Head, op);

			if (status != EXIT_SUCCESS)
			{
				free(buffer);
				while (Head.next = NULL)
				{
					DeleteAfter(&Head);
				}
				return -1;
			}

			currentBuffer += nB;
		}
	}

	free(buffer);

	return EXIT_SUCCESS;
}
