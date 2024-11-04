#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <ctype.h> 
#include <stdlib.h>
#include <time.h>
#define NAME_LENGTH 200
# define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

// define struct
typedef struct cpu
{
	char name[NAME_LENGTH];
	int copies;
	struct cpu *next;
}CPU;

//delete list - free all cpus
//input arguments: pointer to cpu - head of list.
void free_list(CPU* head)
{
	CPU* temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;
		free(temp);
	}
}

// A function that allocates and initializes a cpu.
// input arguments: string name of cpu, int - number of copies
// result argument: pointer to cpu.
CPU* create_cpu(char *name[], int copies,CPU *head)
{
	CPU *cpu = (CPU*)malloc(sizeof(CPU));
	if (cpu == NULL)
	{
		printf("Error: memory allocation failed\n");
		free_list(head);
		return 1;
	}

	strcpy(cpu->name, name);
	cpu->copies = copies;
	cpu->next = NULL;

}

//add cpu to sorted list
//input arguments - 
//arg1 - pointer to head of list
//arg2 - string name of cpu
//arg3 - int number of copies of cpu
//result arguments - pointer to cpu - head of list  
CPU* add(CPU *head, char *name[], int copies)
{
	CPU *iter, *prev = NULL;
	CPU *new_cpu = create_cpu(name, copies, head);
	if (head == NULL)
		return new_cpu;
	if (strcmp(new_cpu->name, head->name)<0)
	{
		new_cpu->next = head;
		return new_cpu;
	}
	iter = head;
	while (iter != NULL && (strcmp(new_cpu->name, head->name) > 0))
	{
		prev = iter;
		iter = iter->next;
	}
	prev->next = new_cpu;
	new_cpu->next = iter;

	return head;
}
//print cpu to update file
// input arguments- pointer to cpu , pointer to file. 
void print_cpu_to_file(CPU *cpu, FILE *fPtrUpdate)
{
	int i = 0;
	while (cpu->name[i] != '\0')
	{
		fprintf(fPtrUpdate, "%c", cpu->name[i]);
		i++;
	}
	fprintf(fPtrUpdate, "$$$ ");
	if (cpu->next!=NULL) 
	{
		fprintf(fPtrUpdate, "%d\n", cpu->copies);
	}
	else
		fprintf(fPtrUpdate, "%d", cpu->copies);
}

//chages a certin cpu name
// input arguments -
// arg1- pointer to head of list
// arg2- string current name of cpu
// arg3- string new name of cpu.
void change_name(CPU *head, char curr_name[], char new_name[])
{
	CPU *temp = head;
	

	while (temp != NULL && strcmp(temp->name,curr_name)!=0)
	{
		temp = temp->next;
	}
	if (temp != NULL)
	{
		strcpy(temp->name, new_name);
	}
}
// return cpu, if the cpu is in list add number to copies , else add cpu to list.
// input arguments- 
// arg1 - pointer head of list
// arg2 - string name of cpu.
// agr3- int number of returned copies
// result arguments - pointer to head of list
CPU* return_from_costomer(CPU *head, char name[], int plus_copies)
{
	CPU *temp = head;
	
	while (!(temp == NULL) && strcmp(temp->name, name) != 0)
	{
		temp = temp->next;
	}
	if (temp != NULL)
	{
		temp->copies += plus_copies;
		return head;
	}
	if (temp == NULL)
	{
		head = add(head ,name, plus_copies);
		return head;
	}
}

//destroy copies if there is a fatal malfanction
//input arguments - 
//arg1 - pointer head of list
//arg2 - string name of cpu
//arg3 - int number of copies to destroy.
void destroy(CPU *head, char name[], int minus_copies)
{
	CPU *temp = head;
	if (temp == NULL)
	{
		return;
	}

	while (temp != NULL && (strcmp(temp->name, name) != 0))
	{
		temp = temp->next;
	}
	if (temp != NULL)
	{
		if (temp->copies <= minus_copies)
		{
			temp->copies = 0;
		}

		if (temp->copies > minus_copies)
		{
			temp->copies = temp->copies-minus_copies;
		}
	}
}

//swap 2 cpus
//input arguments - 
//arg1- pointer head of list
//arg2- pointer to one cpu before
//arg3- pointer to first cpu to swap
//arg4- pointer to second cpu to swap
//result- pointer to head of list
CPU *swap(CPU *head, CPU* prev,CPU *a, CPU *b)
{
	CPU *temp;
	if (prev == NULL)
	{
		temp = b->next;
		head = b;
		b->next = a;
		a->next = temp;
		return head;
	}
	else
	{
		temp = b->next;
		prev->next = b;
		b->next = a;
		a->next = temp;
		return head;
	}
}

//bobble sort
//input argument - pointer to head of list
//result argument - pointer head of list
CPU* bubble_sort(CPU *head)
{
	CPU *iter = head;
	CPU *last = NULL;
	CPU *prev = NULL;
	while (iter != last)
	{		
			while (iter->next != last)
			{
				if (strcmp(iter->name, iter->next->name) > 0)
				{
					head = swap(head, prev, iter, iter->next);
					if (prev != NULL)
						prev = prev->next;
					else
						prev = head;
				}
				else
				{
					prev = iter;
					iter = iter->next;
				}
			}

			last = iter;
			iter = head;
			prev = NULL;
	}
	return head;
}


int main(int argc, char* argv[])
{
	FILE *fPtrAction = NULL;
	FILE *fPtrCpus = NULL;
	FILE *fPtrUpdate = NULL;
	char action[400] = { 0 };
	char Initialize[] = "Initialize\n";
	char Finalize[] = "Finalize";
	char Rename[] = "Rename ";
	char Returned[] = "Returned_from_customer ";
	char Fatal_malfunction[] = "Fatal_malfunction ";
	char op[400] = { 0 };
	CPU *head_cpus = NULL;
	char cpu_line[400] = { 0 };
	char name[400] = { 0 };
	char tav = 0;
	int i = 0;
	int copies = 0;
	char curr_cpu_name[NAME_LENGTH] = { 0 };
	char new_cpu_name[NAME_LENGTH] = { 0 };
	int plus_copies;
	int minus_copies;
	int j = 0;
	
	fPtrAction = fopen(argv[2], "r");
	if (fPtrAction == NULL)
	{
		printf("Error: opening actions failed\n");
		return 1;
	}
	do
	{
		fgets(action, 400, fPtrAction);

		//first action initialize cpu list
		if (strcmp(action, Initialize) == 0)
		{
			fPtrCpus = fopen(argv[1], "r");
			if (fPtrCpus == NULL)
			{
				printf("Error: opening cpus failed\n");
				return 1;
			}
			
			while (!feof(fPtrCpus))
			{
				fgets(cpu_line, 400, fPtrCpus);
				for (i = 0;cpu_line[i]!='\0' && cpu_line[i] != '$'; i++)
				{
					name[i] = cpu_line[i];
				}
				name[i] = '\0';

				if (name[0] == '\0')
				{
					continue;
				}

				copies = 0;
				while (cpu_line[i] != '\0')
				{
					if (cpu_line[i] >= '0' && cpu_line[i] <= '9')
						copies = copies * 10 + (cpu_line[i] - '0');
					i++;
				}

				head_cpus = add(head_cpus, name, copies);

			} 
			fclose(fPtrCpus);
		}

		// last action print cpus to updated file.
		if (strcmp(action, Finalize) == 0)
		{

			fPtrUpdate = fopen(argv[3], "w");
			if (fPtrUpdate == NULL)
			{
				printf("Error: opening update_cpus failed\n");
				free_list(head_cpus);
				return 1;
			}

			head_cpus = bubble_sort(head_cpus);

			for (CPU *curr = head_cpus; curr != NULL; curr = curr->next)
			{
				print_cpu_to_file(curr, fPtrUpdate);

			}
			free_list(head_cpus);
			fclose(fPtrUpdate);
		}

		for (i = 0; action[i] != '$' & action[i]!=0; i++)
		{
			op[i] = action[i];
		}
		op[i] = '\0';
		
		// rename a cpu
		if (strcmp(op, Rename) == 0)
		{
			for (i = i + 4, j = 0; action[i] != '$'; i++,j++)
			{
				curr_cpu_name[j] = action[i];
			}
			curr_cpu_name[j] = '\0';
			for (j=0,i = i + 4; action[i] != '\0' && action [i]!='\n' ; i++,j++)
			{
				new_cpu_name[j] = action[i];
			}
			new_cpu_name[j] = ' ';
			new_cpu_name[j+1] = '\0';
			change_name(head_cpus, curr_cpu_name, new_cpu_name);
		}
		// returned
		if (strcmp(op, Returned) == 0)
		{
			for (j=0,i = i + 4; action[i] != '$'; i++,j++)
			{
				curr_cpu_name[j] = action[i];
			}
			curr_cpu_name[j] =  '\0';

			plus_copies = 0;
			while (action[i] != '\0')
			{
				if (action[i] >= '0' && action[i] <= '9')
					plus_copies = plus_copies * 10 + (action[i] - '0');
				i++;
			}
			head_cpus = return_from_costomer(head_cpus, curr_cpu_name, plus_copies);
		}
		//fatal malfanction
		if (strcmp(op, Fatal_malfunction) == 0)
		{
			for (j=0,i = i + 4; action[i] != '$'; i++,j++)
			{
				curr_cpu_name[j] = action[i];
			}
			curr_cpu_name[j] = '\0';

			minus_copies = 0;
			while (action[i] != '\0')
			{
				if (action[i] >= '0' && action[i] <= '9')
					minus_copies = minus_copies * 10 + (action[i] - '0');
				i++;
			}
			destroy(head_cpus, curr_cpu_name, minus_copies);
		}
	}
	while (!feof(fPtrAction));
	fclose(fPtrAction);

}
