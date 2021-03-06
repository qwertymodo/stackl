#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "machine_def.h"
#include "io_handler.h"
#include "vmem.h"

#define DEFAULT_MEMORY_SIZE  500000

static int8_t *Memory;
static int  Memory_Size = DEFAULT_MEMORY_SIZE;
static FILE *Mem_Log = NULL;
static int Log_Enabled = 0;
//***************************************
static void write_log(int32_t address, int32_t value, const char *label)
{
    if (Log_Enabled)
    {
        if (Mem_Log == NULL) Mem_Log = fopen("mem.log", "w");
        fprintf(Mem_Log, "%5d %8d %08X %s\n", address, value, value, label);
    }
}
//***************************************
void VM_Enable_Log(int enabled)
{
    Log_Enabled = enabled;
}
//***************************************
int Mem_Size()
{
    return Memory_Size;
}
//***************************************
static int validate_address(Machine_State *regs, int32_t address, int is_char)
{
    if (address < 0)
    {
        Machine_Check(MC_ILLEGAL_ADDR, "Invalid address at %d\n", address);
        return -1;
    }

    if ( (address & 0x0003) && !is_char)
    {
        Machine_Check(MC_ILLEGAL_ADDR, "misaligned address %d\n", address);
        return -1;
    }

    if (regs != NULL && (regs->FLAG & FL_USER_MODE))
    {
        int size = 1;
        if (!is_char) size = WORD_SIZE;
        if (address < 0 || (address+size) > regs->LP - regs->BP)
        {
            Machine_Check(MC_ILLEGAL_ADDR,
                    "address %d out of bounds\n", address);
            return -1;
        }

        address = regs->BP + address;
    }

    return address;
}
//***************************************
int32_t Get_Word(Machine_State *cpu, int32_t address)
{
    int32_t value;

    address = validate_address(cpu, address, 0);
    if (address < 0) return 0;

    if (address < Memory_Size)
        value = *(int32_t *)&Memory[address];
    else
        value = IO_Get_Word(address);

    write_log(address, value, "Get_Word");
    return value;
}
//***************************************
void Set_Word(Machine_State *cpu, int32_t address, int32_t value)
{
    address = validate_address(cpu, address, 0);
    if (address < 0) return;

    write_log(address, value, "Set_Word");
    if (address < Memory_Size)
        *(int32_t *)&Memory[address] = value;
    else
        IO_Set_Word(address, value);
}
//***************************************
int32_t Get_Byte(Machine_State *cpu, int32_t address)
{
    int32_t value;

    address = validate_address(cpu, address, 1);
    if (address < 0) return 0;

    if (address < Memory_Size)
        value = Memory[address];
    else
        value = IO_Get_Byte(address);

    write_log(address, value, "Get_Byte");
    return value;
}
//***************************************
void Set_Byte(Machine_State *cpu, int32_t address, int32_t value)
{
    address = validate_address(cpu, address, 1);
    if (address < 0) return;

    write_log(address, value, "Set_Byte");
    if (address < Memory_Size)
        Memory[address] = value;
    else
        IO_Set_Byte(address, value);
}
//***************************************
void *Get_Addr(Machine_State *cpu, int32_t address)
{
    //int value = Get_Word(address);
    address = validate_address(cpu, address, 1);
    if (address < 0 || address >= Memory_Size) 
    {
        Machine_Check(MC_ILLEGAL_ADDR, "address %d out of bounds\n", address);
        return NULL;
    }

    write_log(address, 0, "Get_Addr");
    return &Memory[address];
}
//***************************************
void Init_Memory(int mem_size)
{
    if (mem_size <= 0) mem_size = DEFAULT_MEMORY_SIZE;
    Memory_Size = mem_size;
    Memory = (int8_t *)malloc(Memory_Size);
    memset(Memory, 0x79, Memory_Size);
}
//***************************************
int32_t Abs_Get_Word(int32_t address)
{
    return Get_Word(NULL, address);
}
//***************************************
void Abs_Set_Word(int32_t address, int32_t value)
{
    return Set_Word(NULL, address, value);
}
//***************************************
int32_t Abs_Get_Byte(int32_t address)
{
    return Get_Byte(NULL, address);
}
//***************************************
void Abs_Set_Byte(int32_t address, int32_t value)
{
    return Set_Byte(NULL, address, value);
}
//***************************************
void *Abs_Get_Addr(int32_t address)
{
    return Get_Addr(NULL, address);
}
//***************************************
int VM_Check_Addr(Machine_State *regs, int32_t address, int is_byte)
{
    if ((regs->FLAG & (FL_USER_MODE|FL_VMEM) ) == (FL_USER_MODE|FL_VMEM))
    {
        if (regs->VMEM <= 0) 
        {
            Machine_Check(MC_ILLEGAL_ADDR, "VMEM reg invalid while in VM mode");
            return 0;
        }
    }

    return 1;
}

