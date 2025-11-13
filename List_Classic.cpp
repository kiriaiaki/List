#include "List_Classic.h"
#define DEBUG // включает верификаторы и создание logfile
//#define STOP_PROGRAMME // в случае выявления ошибок в списке программа останавливается

int main ()
{
    #ifdef DEBUG
    if (Start_Logfile () == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    #endif // DEBUG

    list_k List = {};
    if (List_Ctor (&List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }


    if (List_Insert_After (10, &List.Array[0], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (20, &List.Array[1], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (30, &List.Array[2], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (40, &List.Array[3], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Insert_After (25, &List.Array[2], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Delete (&List.Array[5], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    // List.Array[2].Prev = List.Array - 100; // специальные ошибки
    // List.Array[3].Next = &List.Array[List.Capacity - 1] + 191;

    if (List_Insert_After (50, &List.Array[4], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Push_Front (5, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Push_Back (55, &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    if (List_Delete (&List.Array[7], &List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    #ifndef DEBUG
    if (Start_Logfile () == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }
    List_Dump (&List, "After all function");
    #endif // DEBUG

    List_Dtor (&List);
    return 0;
}

int List_Ctor  (list_k* const List)
{
    List->Size = 0;
    List->Capacity = 10;

    List->Array = (node_k*) calloc (List->Capacity, sizeof (node_k));
    if (List->Array == NULL)
    {
        printf ("Error allocation memory for array in List_Ctor!\n");
        return There_Are_Errors;
    }

    List->Array[0].Value = Canary;
    List->Array[0].Next = List->Array;
    List->Array[0].Prev = &List->Array[0];

    for (size_t i = 1; i < List->Capacity - 1; i++)
    {
        List->Array[i].Next = &List->Array[i + 1];
    }

    List->Array[List->Capacity - 1].Next = List->Array;

    for (size_t i = 1; i < List->Capacity; i++)
    {
        List->Array[i].Prev = NULL;
    }

    List->Free = &List->Array[1];

    #ifdef DEBUG
    if (List_Dump (List, "List_Ctor (List)") == There_Are_Errors)
    {
        printf ("Error in function List_Ctor (List)\n");
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Dtor  (list_k* const List)
{
    List->Size = 0;
    List->Capacity = 0;

    free (List->Array);

    List->Free = NULL;

    return 0;
}

int List_Error (const list_k* const List)
{
    if (List == NULL)
    {
        return Not_Struct_List;
    }

    if (List->Capacity < 10)
    {
        return Bad_Capacity;
    }

    if (List->Array == NULL)
    {
        return Not_Array;
    }

    if (List->Size >= List->Capacity)
    {
        return Bad_Size;
    }

    if (List->Array[0].Value != Canary)
    {
        return Bad_Canary;
    }

    if (List->Free >= &List->Array[List->Capacity] || List->Free < List->Array)
    {
        return Bad_Free;
    }

    if (List->Array[0].Prev >= &List->Array[List->Capacity] || List->Array[0].Prev < List->Array)
    {
        return Bad_Head;
    }

    if (List->Array[0].Next >= &List->Array[List->Capacity] || List->Array[0].Next < List->Array)
    {
        return Bad_Tail;
    }

    size_t Counter_Free_Element = 0;
    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array[i].Prev == NULL)
        {
            Counter_Free_Element++;
        }
    }
    if (Counter_Free_Element != (List->Capacity - 1) - List->Size)
    {
        return Bad_Count_Free;
    }

    for (size_t i = 1; i < List->Capacity; i++)
    {
        if ((List->Array[i].Prev >= &List->Array[List->Capacity] || List->Array[i].Prev < List->Array) && List->Array[i].Prev != NULL)
        {
            return Impossible_Prev;
        }

        else if (List->Array[i].Next >= &List->Array[List->Capacity] || List->Array[i].Next < List->Array)
        {
            return Impossible_Next;
        }
    }

    for (size_t i = 0; i < List->Capacity; i++)
    {
        for (size_t j = i + 1; j < List->Capacity; j++)
        {
            if (List->Array[i].Prev == List->Array[j].Prev && List->Array[j].Prev != NULL)
            {
                return Repeat_Prev;
            }

            else if (List->Array[i].Next == List->Array[j].Next)
            {
                if (List->Array[i].Next == List->Array && (List->Array[i].Prev == NULL || List->Array[j].Prev == NULL))
                {
                    continue;
                }

                else
                {
                    return Repeat_Next;
                }
            }
        }
    }

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (&List->Array[i] != List->Array[i].Next->Prev)
        {
            if (List->Array[i].Prev == NULL && List->Array[i].Next->Prev == NULL)
            {
                continue;
            }

            else if (List->Array[i].Prev == NULL && List->Array[i].Next == List->Array)
            {
                continue;
            }

            else
            {
                return Bad_Cycle;
            }
        }
    }

    size_t Len_Free_Cycle = 0;
    if (List->Free != List->Array)
    {
        for (node_k* i = List->Free; i != List->Array; i = &(*i->Next))
        {
            Len_Free_Cycle++;
        }
        if (Len_Free_Cycle != (List->Capacity - 1) - List->Size)
        {
            return Bad_Len_Free_Cycle;
        }
    }

    size_t Len_Next_Cycle = 0;
    for (node_k* i = List->Array; &(*i->Next) != List->Array; i = &(*i->Next))
    {
        Len_Next_Cycle++;
    }
    if (Len_Next_Cycle != List->Size)
    {
        return Bad_Len_Next_Cycle;
    }

    size_t Len_Prev_Cycle = 0;
    for (node_k* i = List->Array; &(*i->Prev) != List->Array; i = &(*i->Prev))
    {
        Len_Prev_Cycle++;
    }
    if (Len_Prev_Cycle != List->Size)
    {
        return Bad_Len_Prev_Cycle;
    }

    return Not_Error_List;
}


int List_Dump          (const list_k* const List, const char* const Name_Function)
{
    char Name_File_Graph[25] = "File_txt/File_";
    Naming_File (Name_File_Graph);

    FILE* file_graph = fopen (Name_File_Graph, "w");
    if (file_graph == NULL)
    {
        printf ("Error open file for graph\n");
        return There_Are_Errors;
    }

    Dump_For_Graph (List, file_graph);

    fclose (file_graph);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File_Graph);

    system (Command);

    if (Dump_For_Html (List, Name_File_Graph, Name_Function) != Not_Error_List)
    {
        return There_Are_Errors;
    }

    return 0;
}

int Naming_File        (char* const Name_File)
{
    static int Counter = 1;

    char Temp_Str[11];
    itoa_k (Counter, Temp_Str);

    strcat(Name_File, Temp_Str);

    strcat(Name_File, ".txt");

    Counter++;
    return 0;
}

int Naming_Command_Dot (char* const Name_Command, char* const Name_File)
{
    strcat(Name_Command, Name_File);
    strcat(Name_Command, " -T png -o ");

    Name_File[5] = 'p';
    Name_File[6] = 'n';
    Name_File[7] = 'g';
    size_t Len = strlen (Name_File);
    Name_File[Len - 3] = 'p';
    Name_File[Len - 2] = 'n';
    Name_File[Len - 1] = 'g';
    strcat(Name_Command, Name_File);

    return 0;
}

int Dump_For_Graph     (const list_k* const List, FILE* const file_graph)
{
    node_k* Repeat_Free = 0;
    for (size_t i = 0; i < List->Capacity; i++)
    {
        for (size_t j = i + 1; j < List->Capacity; j++)
        {
            if (List->Array[i].Next == List->Array[j].Next)
            {
                if (List->Array[i].Next != List->Array || (List->Array[i].Prev != NULL && List->Array[j].Prev != NULL))
                {
                    Repeat_Free = List->Array[i].Next;
                }
            }
        }
    }

    fprintf (file_graph, "digraph G\n"
                   "{\n"
                   "    rankdir = LR;\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    %d [shape = Mrecord, label = \"index = %d | ptr = %p | value = %d | {head = %p | tail = %p}\", style = \"filled\", fillcolor = \"lightblue\"];\n", 0, 0, List->Array , List->Array[0].Value, List->Array[0].Prev, List->Array[0].Next);

    for (size_t i = 1; i < List->Capacity; i++)
    {
        if (List->Array[i].Prev == NULL)
        {
            fprintf (file_graph, "    %zu [shape = Mrecord, label = \"index = %zu | ptr = %p | value = %d | {prev = %p | next = %p}\", style = \"filled\", fillcolor = \"lightgrey\"];\n", i, i, &List->Array[i], List->Array[i].Value, List->Array[i].Prev, List->Array[i].Next);
        }

        else
        {
            fprintf (file_graph, "    %zu [shape = Mrecord, label = \"index = %zu | ptr = %p | value = %d | {prev = %p | next = %p}\", style = \"filled\", fillcolor = \"lightblue\"];\n", i, i, &List->Array[i], List->Array[i].Value, List->Array[i].Prev, List->Array[i].Next);
        }
    }
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity - 1; i++)
    {
        fprintf (file_graph, "    %zu->%zu[style = invis, weight = 500];\n", i, i + 1);
    }
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    head [shape = invhouse, label = \"head = %p\", style = \"filled\", fillcolor = \"goldenrod1\"];\n", List->Array[0].Prev);
    fprintf (file_graph, "    tail [shape = invhouse, label = \"tail = %p\", style = \"filled\", fillcolor = \"pink2\"];\n", List->Array[0].Next);
    if (List->Free != List->Array)
    {
        fprintf (file_graph, "    free [shape = invhouse, label = \"first free = %p\", style = \"filled\", fillcolor = \"green\"];\n", List->Free);
    }
    else
    {
        fprintf (file_graph, "    free [shape = Mrecord, label = \"not free\", style = \"filled\", fillcolor = \"grey\"];\n");
    }
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    {rank = same; head; %d};\n", int (List->Array[0].Prev - List->Array));
    fprintf (file_graph, "    {rank = same; tail; %d};\n", int (List->Array[0].Next - List->Array));
    fprintf (file_graph, "    {rank = same; free; %d};\n", int (List->Free - List->Array));
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity ; i++)
    {
        if (List->Array[i].Next < &List->Array[List->Capacity] && List->Array[i].Next >= List->Array)
        {
            if (List->Array[i].Prev == NULL && List->Array[i].Next->Prev == NULL)
            {
                if (List->Array[i].Next != Repeat_Free)
                {
                    fprintf (file_graph, "    %zu->%d[color = \"green\"];\n", i, int (List->Array[i].Next - List->Array));
                }

                else
                {
                    fprintf (file_graph, "    %zu->%d[label = \"Повторные стрелки\"];\n", i, int (List->Array[i].Next - List->Array));
                }
            }

            else if (List->Array[i].Prev == NULL && List->Array[i].Next == List->Array)
            {
                continue;
            }

            else
            {
                if (List->Array[i].Next->Prev == &List->Array[i])
                {
                    fprintf (file_graph, "    %zu->%d[color = \"pink2:goldenrod1\", dir = both];\n", i, int (List->Array[i].Next - List->Array));
                }

                else
                {
                    fprintf (file_graph, "    %zu->%d[label = \"Неправильный цикл\"];\n", i, int (List->Array[i].Next - List->Array));
                }
            }
        }

        else
        {
            fprintf (file_graph, "    %zu->%d[label = \"Невозможное значение Next\"];\n", i, int (List->Array[i].Next - List->Array));
        }
    }
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array[i].Prev != NULL)
        {
            if (List->Array[i].Prev < &List->Array[List->Capacity] && List->Array[i].Prev >= List->Array)
            {
                if (List->Array[i].Prev->Next != &List->Array[i])
                {
                    fprintf (file_graph, "    %zu->%d[label = \"Неправильный цикл\"];\n", i, int (List->Array[i].Prev - List->Array));
                }
            }
            else
            {
                fprintf (file_graph, "    %zu->%d[label = \"Невозможное значение Prev\"];\n", i, int (List->Array[i].Prev - List->Array));
            }
        }
    }

    fprintf (file_graph, "}\n");
    return 0;
}

int Dump_For_Html      (const list_k* const List, const char* const Name_File_Graph, const char* const Name_Function)
{
    FILE* file = fopen (Name_Log, "a");
    if (file == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    int Number_Error = List_Error (List);
    if (Number_Error != 0)
    {
        fprintf (file, "                          <b>%s</b>\n", Array_Name_List_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", List->Size);
    fprintf (file, "CAPACITY = %zu\n", List->Capacity);
    fprintf (file, "HEAD = %p\n", List->Array[0].Prev);
    fprintf (file, "TAIL = %p\n", List->Array[0].Next);
    fprintf (file, "FREE = %p\n", List->Free);
    fprintf (file, "\n");

    fprintf (file, "DATA:\n");
    size_t Len = List->Capacity;
    size_t Len_Line = List->Capacity;

    for (size_t i = 0; i < Len_Line; i++)
    {
        fprintf (file, "        %2zu       ", i);
    }
    fprintf (file, "\n");

    for (size_t i = 0; i < Len_Line; i++)
    {
        fprintf (file, "  %14p ", &List->Array[i]);
    }
    fprintf (file, "\n");

    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+----------------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|                ");
        }
        else
        {
            fprintf (file, "|     %4d       ", List->Array[i].Value);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+----------------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "NEXT:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+----------------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|                ");
        }
        else
        {
            fprintf (file, "| %14p ", List->Array[i].Next);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+----------------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "PREV:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+----------------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|                ");
        }
        else
        {
            fprintf (file, "| %14p ", List->Array[i].Prev);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+----------------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "<img src = \"%s\">\n", Name_File_Graph);
    fprintf (file, "\n");

    Print_Separator_In_Log (600, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
    return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}


int List_Insert_After  (const int Value, node_k* const Node, list_k* const List)
{
    int Index = int (Node - List->Array);

    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Insert_After (%d, %p, List)", Value, Node);

    #ifdef DEBUG
    if (List_Error (List) != Not_Error_List)
    {
        List_Dump (List, Name_Func);
        printf ("Error start function %s\n", Name_Func);
        #ifdef STOP_PROGRAMME
        return There_Are_Errors;
        #endif // STOP_PROGRAMME
    }
    #endif // DEBUG

    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    if (Index < 0 || Index >= int (List->Capacity) - 1 || List->Array[Index].Prev == NULL)
    {
        printf ("Impossible %s\n", Name_Func);
        return There_Are_Errors;
    }

    Insert_After (Value, Index, List);

    #ifdef DEBUG
    if (List_Dump (List, Name_Func) == There_Are_Errors)
    {
        printf ("Error in function %s\n", Name_Func);
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Insert_Before (const int Value, node_k* const Node, list_k* const List)
{
    int Index = int (Node - List->Array);

    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Insert_Before (%d, %p, List)", Value, Node);

    #ifdef DEBUG
    if (List_Error (List) != Not_Error_List)
    {
        List_Dump (List, Name_Func);
        printf ("Error start function %s\n", Name_Func);
        #ifdef STOP_PROGRAMME
        return There_Are_Errors;
        #endif // STOP_PROGRAMME
    }
    #endif // DEBUG

    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    if (Index <= 0 || Index > int (List->Capacity) - 1 || List->Array[Index].Prev == NULL)
    {
        printf ("Impossible %s\n", Name_Func);
        return There_Are_Errors;
    }

    Insert_Before (Value, Index, List);

    #ifdef DEBUG
    if (List_Dump (List, Name_Func) == There_Are_Errors)
    {
        printf ("Error in function %s\n", Name_Func);
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Push_Front    (const int Value, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Push_Front (%d, List)", Value);

    #ifdef DEBUG
    if (List_Error (List) != Not_Error_List)
    {
        List_Dump (List, Name_Func);
        printf ("Error start function %s\n", Name_Func);
        #ifdef STOP_PROGRAMME
        return There_Are_Errors;
        #endif // STOP_PROGRAMME
    }
    #endif // DEBUG

    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    int Index = int (List->Array[0].Next - List->Array);

    Insert_Before (Value, Index, List);

    #ifdef DEBUG
    if (List_Dump (List, Name_Func) == There_Are_Errors)
    {
        printf ("Error in function %s\n", Name_Func);
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Push_Back     (const int Value, list_k* const List)
{
    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Push_Back (%d, List)", Value);

    #ifdef DEBUG
    if (List_Error (List) != Not_Error_List)
    {
        List_Dump (List, Name_Func);
        printf ("Error start function %s\n", Name_Func);
        #ifdef STOP_PROGRAMME
        return There_Are_Errors;
        #endif // STOP_PROGRAMME
    }
    #endif // DEBUG

    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    int Index = int (List->Array[0].Prev - List->Array);

    Insert_After (Value, Index, List);

    #ifdef DEBUG
    if (List_Dump (List, Name_Func) == There_Are_Errors)
    {
        printf ("Error in function %s\n", Name_Func);
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Delete        (node_k* const Node, list_k* const List)
{
    int Index = int (Node - List->Array);

    char Name_Func[52];
    snprintf (Name_Func, sizeof (Name_Func), "List_Delete (%p, List)", Node);

    #ifdef DEBUG
    if (List_Error (List) != Not_Error_List)
    {
        List_Dump (List, Name_Func);
        printf ("Error start function %s\n", Name_Func);
        #ifdef STOP_PROGRAMME
        return There_Are_Errors;
        #endif // STOP_PROGRAMME
    }
    #endif // DEBUG

    if (Index <= 0 || Index >= int (List->Capacity) || List->Array[Index].Prev == NULL)
    {
        printf ("Impossible %s\n", Name_Func);
        return There_Are_Errors;
    }

    List->Array[Index].Value = 0;
    List->Size--;

    List->Array[Index].Next->Prev = List->Array[Index].Prev;
    List->Array[Index].Prev->Next = List->Array[Index].Next;

    List->Array[Index].Next = List->Free;
    List->Free = &List->Array[Index];
    List->Array[Index].Prev = NULL;

    #ifdef DEBUG
    if (List_Dump (List, Name_Func) == There_Are_Errors)
    {
        printf ("Error in function %s\n", Name_Func);
        return There_Are_Errors;
    }
    #endif // DEBUG

    return 0;
}

int List_Reallocation  (list_k* const List)
{
    if (List->Size + 1 == List->Capacity)
    {
        size_t New_Capacity = List->Capacity * 2;
        node_k* Old_Array = List->Array;
        node_k* New_Array = (node_k*) calloc (New_Capacity, sizeof (node_k));
        if (New_Array == NULL)
        {
            printf ("Error allocation memory for array value in List_Reallocation\n");
            return There_Are_Errors;
        }

        for (size_t i = 0; i < List->Capacity; i++)
        {
            New_Array[i].Value = Old_Array[i].Value;
            New_Array[i].Next = &New_Array[Old_Array[i].Next - Old_Array];
            New_Array[i].Prev = &New_Array[Old_Array[i].Prev - Old_Array];
        }

        for (size_t i = List->Size + 1; i < New_Capacity; i++)
        {
            New_Array[i].Value = 0;
        }

        for (size_t i = List->Size + 1; i < New_Capacity - 1; i++)
        {
            New_Array[i].Next = &New_Array[i + 1];
        }
        New_Array[New_Capacity - 1].Next = New_Array;

        for (size_t i = List->Size + 1; i < New_Capacity; i++)
        {
            New_Array[i].Prev = NULL;
        }

        List->Free = &New_Array[List->Size + 1];

        List->Array = New_Array;
        List->Capacity = New_Capacity;

        free (Old_Array);

        List_Dump(List, "DEBUG");
    }

    return 0;
}


int Insert_After  (const int Value, const int Index, list_k* const List)
{
    node_k* New_Free = List->Free->Next;

    List->Free->Value = Value;
    List->Size++;

    List->Free->Next = List->Array[Index].Next;
    List->Free->Prev = &List->Array[Index];

    List->Array[Index].Next->Prev = List->Free;
    List->Array[Index].Next = List->Free;

    List->Free = New_Free;

    return 0;
}

int Insert_Before (const int Value, const int Index, list_k* const List)
{
    node_k* New_Free = List->Free->Next;

    List->Free->Value = Value;
    List->Size++;

    List->Free->Next = &List->Array[Index];
    List->Free->Prev = List->Array[Index].Prev;

    List->Array[Index].Prev->Next = List->Free;
    List->Array[Index].Prev = List->Free;

    List->Free = New_Free;

    return 0;
}


int Start_Logfile          ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (600, file_html);

    fclose (file_html);

    return 0;
}

int Print_Separator_In_Log (const size_t Len_Separator, FILE* file_html)
{
    for (size_t i = 0; i < Len_Separator / 2; i++)
    {
        fprintf (file_html, "+=");
    }
    fprintf (file_html, "\n\n");

    return 0;
}

int Reverse_Str            (char* const Str)
{
    size_t i = 0;
    size_t j = strlen (Str) - 1;

    while (i < j)
    {
        char Symbol = Str[i];
        Str[i] = Str[j];
        Str[j] = Symbol;
        i++;
        j--;
    }

    return 0;
}

char* itoa_k               (int Number, char* const Str)
{
    size_t i = 0;

    if (Number == 0)
    {
        Str[i] = '0';
        i++;

        Str[i] = '\0';

        return Str;
    }

    while (Number != 0)
    {
        int Rem = Number % 10;

        Str[i] = char (Rem) + '0';
        i++;

        Number = Number / 10;
    }

    Str[i] = '\0';

    Reverse_Str (Str);

    return Str;
}
