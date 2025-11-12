#include <stdio.h>
#include <assert.h>
#include <string.h>

struct node_k
{
    int Value;
    node_k* Next;
    node_k* Prev;
};

struct list_k
{
    size_t Capacity;
    size_t Size;
    node_k* Array;
    node_k* Free;
};

enum Name_List_Error
{
    Not_Error_List     =  0,
    Not_Struct_List    =  1,
    Bad_Capacity       =  2,
    Not_Array          =  3,
    Bad_Size           =  4,
    Bad_Canary         =  5,
    Bad_Free           =  6,
    Bad_Head           =  7,
    Bad_Tail           =  8,
    Bad_Count_Free     =  9,
    Impossible_Prev    = 10,
    Impossible_Next    = 11,
    Repeat_Prev        = 12,
    Repeat_Next        = 13,
    Bad_Cycle          = 14,
    Bad_Len_Free_Cycle = 15,
    Bad_Len_Next_Cycle = 16,
    Bad_Len_Prev_Cycle = 17,
    Quantity_Error     = 18
};

const char* Array_Name_List_Error[Quantity_Error] = {
    "NOT ERROR LIST",
    "!!!NOT STRUCT LIST!!!",
    "!!!BAD CAPACITY!!!",
    "!!!NOT ARRAY DATA!!!",
    "!!!BAD SIZE!!!",
    "!!!BAD CANARY!!!",
    "!!!BAD FREE!!!",
    "!!!BAD HEAD!!!",
    "!!!BAD TAIL!!!",
    "!!!BAD COUNT FREE!!!",
    "!!!IMPOSSIBLE PREV!!!",
    "!!!IMPOSSIBLE NEXT!!!",
    "!!!REPEAT PREV!!!",
    "!!!REPEAT NEXT!!!",
    "!!!BAD CYCLE!!!",
    "!!!BAD LEN FREE CYCLE!!!",
    "!!!BAD LEN NEXT CYCLE!!!",
    "!!!BAD LEN PREV CYCLE!!!"
};

const int There_Are_Errors = -2902;
const int Canary = 2008;
const char Name_Log[] = "Log_File.html";

int List_Ctor  (list_k* const List);
int List_Dtor  (list_k* const List);
int List_Error (const list_k* const List);

int List_Dump          (const list_k* const List, const char* const Name_Function);
int Naming_File        (char* const Name_File);
int Naming_Command_Dot (char* const Name_Command, char* const Name_File);
int Dump_For_Graph     (const list_k* const List, FILE* const file);
int Dump_For_Html      (const list_k* const List, const char* const Name_File, const char* const Name_Function);

int List_Insert_After  (const int Value, const int Index, list_k* const List);
int List_Insert_Before (const int Value, const int Index, list_k* const List);
int List_Push_Front    (const int Value, list_k* const List);
int List_Push_Back     (const int Value, list_k* const List);
int List_Delete        (const int Index, list_k* const List);
int List_Reallocation  (list_k* const List);

int Insert_After (const int Value, const int Index, list_k* const List);
int Insert_Before (const int Value, const int Index, list_k* const List);

int Start_Logfile          ();
int Print_Separator_In_Log (const size_t Len_Separator, FILE* file_html);
int Reverse_Str            (char* const Str);
char* itoa_k               (int Number, char* const Str);

