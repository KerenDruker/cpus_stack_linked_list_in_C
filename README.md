# CPU Inventory Management System

This C program manages an inventory system for CPUs, allowing for operations like adding, renaming, and sorting CPU types in a linked list. The program reads an initial list of CPUs from a file, processes commands from an actions file, and updates the list accordingly. The final sorted list is saved to an output file.

## Features

- **Initialize CPU List**: Reads an initial list of CPU names and quantities from a file and adds them to a linked list.
- **Rename CPUs**: Changes the name of an existing CPU in the list.
- **Return from Customer**: Increases the quantity of an existing CPU or adds a new CPU if it’s not already in the list.
- **Handle Fatal Malfunctions**: Decreases the quantity of a CPU or removes it if the quantity falls to zero.
- **Finalize and Sort**: Sorts the CPU list alphabetically and writes the updated list to an output file.

## File Structure

- **CPU Inventory File**: Contains initial CPU data with each line formatted as `CPU_NAME$$$ COPIES`.
- **Actions File**: Contains commands to manipulate the CPU list, including `Initialize`, `Rename`, `Returned_from_customer`, `Fatal_malfunction`, and `Finalize`.
- **Updated File**: Stores the final sorted list of CPUs after processing all commands.

## Usage

Compile and run the program, passing the file paths as arguments for the CPU inventory, actions, and output files:

```bash
gcc -o cpus cpus.c
./cpus cpus.txt actions.txt updated_cpus.txt
```

## Functions

- **`CPU* create_cpu(char *name, int copies, CPU *head)`**: Allocates memory for a new CPU structure, initializes it with a name and quantity, and returns a pointer to it.
- **`CPU* add(CPU *head, char *name, int copies)`**: Adds a CPU to the linked list in sorted order.
- **`void change_name(CPU *head, char curr_name[], char new_name[])`**: Renames an existing CPU in the list.
- **`CPU* return_from_customer(CPU *head, char name[], int plus_copies)`**: Increases the quantity of a CPU or adds it if it doesn't exist in the list.
- **`void destroy(CPU *head, char name[], int minus_copies)`**: Decreases the quantity of a CPU or sets it to zero if the quantity becomes insufficient.
- **`CPU* bubble_sort(CPU *head)`**: Sorts the linked list alphabetically by CPU name.
- **`void print_cpu_to_file(CPU *cpu, FILE *fPtrUpdate)`**: Writes CPU data to the output file in the required format.

## Example Files

- **cpus.txt**: Example of a CPU inventory file.
  ```
  Intel_i7$$$ 5
  AMD_Ryzen5$$$ 3
  ```
  
- **actions.txt**: Example of an actions file.
  ```
  Initialize
  Rename Intel_i7$$$ Intel_i9
  Returned_from_customer AMD_Ryzen5$$$ 2
  Fatal_malfunction Intel_i9$$$ 1
  Finalize
  ```

## Error Handling

- The program checks for file errors and reports any issues with memory allocation or file access.
- **Memory Management**: A custom `free_list` function ensures proper memory deallocation for the linked list.

