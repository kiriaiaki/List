#include "List_Classic.h"
//#define STOP_PROGRAMME // в случае выявления ошибок в списке программа останавливается

int main ()
{
    if (Start_Logfile () == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    list_k List = {};
    if (List_Ctor (&List) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }


    // if (List_Insert_After (10, 0, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Insert_After (20, 1, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Insert_After (30, 2, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Insert_After (40, 3, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // List.Array_Prev[2] = -7; // специальные ошибки
    // List.Array_Next[3] = 700;
    // if (List_Insert_After (50, 4, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Insert_Before (45, 5, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Push_Front (5, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Push_Back (55, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }
    // if (List_Delete (7, &List) == There_Are_Errors)
    // {
    //     printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
    //     return 0;
    // }

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

    if (List_Dump (List, "List_Ctor (List)") == There_Are_Errors)
    {
        printf ("Error in function List_Ctor (List)\n");
        return There_Are_Errors;
    }

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

// int List_Error (const list_k* const List)
// {
//     if (List == NULL)
//     {
//         return Not_Struct_List;
//     }
//
//     if (List->Capacity < 10)
//     {
//         return Bad_Capacity;
//     }
//
//     if (List->Array_Value == NULL)
//     {
//         return Not_Array_Data;
//     }
//
//     if (List->Array_Next == NULL)
//     {
//         return Not_Array_Next;
//     }
//
//     if (List->Array_Prev == NULL)
//     {
//         return Not_Array_Prev;
//     }
//
//     if (List->Size >= List->Capacity)
//     {
//         return Bad_Size;
//     }
//
//     if (List->Array_Value[0] != Canary)
//     {
//         return Bad_Canary;
//     }
//
//     if (List->Free >= List->Capacity)
//     {
//         return Bad_Free;
//     }
//
//     if (List->Array_Prev[0] >= ssize_t (List->Capacity) || List->Array_Prev[0] < -1)
//     {
//         return Bad_Head;
//     }
//
//     if (List->Array_Next[0] >= List->Capacity)
//     {
//         return Bad_Tail;
//     }
//
//     size_t Counter_Free_Element = 0;
//     for (size_t i = 0; i < List->Capacity; i++)
//     {
//         if (List->Array_Prev[i] == -1)
//         {
//             Counter_Free_Element++;
//         }
//     }
//     if (Counter_Free_Element != (List->Capacity - 1) - List->Size)
//     {
//         return Bad_Count_Free;
//     }
//
//     for (size_t i = 0; i < List->Capacity; i++)
//     {
//         if (List->Array_Prev[i] >= ssize_t (List->Capacity) || List->Array_Prev[i] < -1)
//         {
//             return Impossible_Prev;
//         }
//
//         else if (List->Array_Next[i] >= List->Capacity)
//         {
//             return Impossible_Next;
//         }
//     }
//
//     for (size_t i = 0; i < List->Capacity; i++)
//     {
//         for (size_t j = i + 1; j < List->Capacity; j++)
//         {
//             if (List->Array_Prev[i] == List->Array_Prev[j] && List->Array_Prev[j] != -1)
//             {
//                 return Repeat_Prev;
//             }
//
//             else if (List->Array_Next[i] == List->Array_Next[j])
//             {
//                 if (List->Array_Next[i] == 0 && (List->Array_Prev[i] == - 1 || List->Array_Prev[j] == - 1))
//                 {
//                     continue;
//                 }
//
//                 else
//                 {
//                     return Repeat_Next;
//                 }
//             }
//         }
//     }
//
//     for (size_t i = 0; i < List->Capacity; i++)
//     {
//         if (ssize_t (i) != List->Array_Prev[List->Array_Next[i]])
//         {
//             if (List->Array_Prev[i] == -1 && List->Array_Prev[List->Array_Next[i]] == -1)
//             {
//                 continue;
//             }
//
//             else if (List->Array_Prev[i] == -1 && List->Array_Next[i] == 0)
//             {
//                 continue;
//             }
//
//             else
//             {
//                 return Bad_Cycle;
//             }
//         }
//     }
//
//     size_t Len_Free_Cycle = 0;
//     if (List->Free != 0)
//     {
//         for (size_t i = List->Free; List->Array_Next[i] != 0; i = List->Array_Next[i])
//         {
//             Len_Free_Cycle++;
//         }
//         if (Len_Free_Cycle != (List->Capacity - 1) - List->Size - 1)
//         {
//             return Bad_Len_Free_Cycle;
//         }
//     }
//
//     size_t Len_Next_Cycle = 0;
//     for (size_t i = 0; List->Array_Next[i] != 0; i = List->Array_Next[i])
//     {
//         Len_Next_Cycle++;
//     }
//     if (Len_Next_Cycle != List->Size)
//     {
//         return Bad_Len_Next_Cycle;
//     }
//
//     size_t Len_Prev_Cycle = 0;
//     for (ssize_t i = 0; List->Array_Prev[i] != 0; i = List->Array_Prev[i])
//     {
//         Len_Prev_Cycle++;
//     }
//     if (Len_Prev_Cycle != List->Size)
//     {
//         return Bad_Len_Prev_Cycle;
//     }
//
//     return Not_Error_List;
// }


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
    size_t Repeat_Free = 0;
    for (size_t i = 0; i < List->Capacity; i++)
    {
        for (size_t j = i + 1; j < List->Capacity; j++)
        {
            if (List->Array_Next[i] == List->Array_Next[j])
            {
                if (List->Array_Next[i] != 0 || (List->Array_Prev[i] != - 1 && List->Array_Prev[j] != - 1))
                {
                    Repeat_Free = List->Array_Next[i];
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

    fprintf (file_graph, "    %d [shape = Mrecord, label = \"index = %d | value = %d | {head = %zd | tail = %zu}\", style = \"filled\", fillcolor = \"lightblue\"];\n", 0, 0, List->Array_Value[0], List->Array_Prev[0], List->Array_Next[0]);

    for (size_t i = 1; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] == -1)
        {
            fprintf (file_graph, "    %zu [shape = Mrecord, label = \"index = %zu | value = %d | {prev = %zd | next = %zu}\", style = \"filled\", fillcolor = \"lightgrey\"];\n", i, i, List->Array_Value[i], List->Array_Prev[i], List->Array_Next[i]);
        }

        else
        {
            fprintf (file_graph, "    %zu [shape = Mrecord, label = \"index = %zu | value = %d | {prev = %zd | next = %zu}\", style = \"filled\", fillcolor = \"lightblue\"];\n", i, i, List->Array_Value[i], List->Array_Prev[i], List->Array_Next[i]);
        }
    }
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity - 1; i++)
    {
        fprintf (file_graph, "    %zu->%zu[style = invis, weight = 500];\n", i, i + 1);
    }
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    head [shape = invhouse, label = \"head = %zd\", style = \"filled\", fillcolor = \"goldenrod1\"];\n", List->Array_Prev[0]);
    fprintf (file_graph, "    tail [shape = invhouse, label = \"tail = %zu\", style = \"filled\", fillcolor = \"pink2\"];\n", List->Array_Next[0]);
    if (List->Free != 0)
    {
        fprintf (file_graph, "    free [shape = invhouse, label = \"first free = %zu\", style = \"filled\", fillcolor = \"green\"];\n", List->Free);
    }
    else
    {
        fprintf (file_graph, "    free [shape = Mrecord, label = \"not free\", style = \"filled\", fillcolor = \"grey\"];\n");
    }
    fprintf (file_graph, "\n");

    fprintf (file_graph, "    {rank = same; head; %zd};\n", List->Array_Prev[0]);
    fprintf (file_graph, "    {rank = same; tail; %zu};\n", List->Array_Next[0]);
    fprintf (file_graph, "    {rank = same; free; %zu};\n", List->Free);
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity ; i++)
    {
        if (List->Array_Next[i] < List->Capacity)
        {
            if (List->Array_Prev[i] == -1 && List->Array_Prev[List->Array_Next[i]] == -1)
            {
                if (List->Array_Next[i] != Repeat_Free)
                {
                    fprintf (file_graph, "    %zu->%zu[color = \"green\"];\n", i, List->Array_Next[i]);
                }

                else
                {
                    fprintf (file_graph, "    %zu->%zu[label = \"Повторные стрелки\"];\n", i, List->Array_Next[i]);
                }
            }

            else if (List->Array_Prev[i] == -1 && List->Array_Next[i] == 0)
            {
                continue;
            }

            else
            {
                if (List->Array_Prev[List->Array_Next[i]] == ssize_t (i))
                {
                    fprintf (file_graph, "    %zu->%zu[color = \"pink2:goldenrod1\", dir = both];\n", i, List->Array_Next[i]);
                }

                else
                {
                    fprintf (file_graph, "    %zu->%zu[label = \"Неправильный цикл\"];\n", i, List->Array_Next[i]);
                }
            }
        }

        else
        {
            fprintf (file_graph, "    %zu->%zu[label = \"Невозможное значение Next\"];\n", i, List->Array_Next[i]);
        }
    }
    fprintf (file_graph, "\n");

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] != -1)
        {
            if (size_t (List->Array_Prev[i]) < List->Capacity)
            {
                if (List->Array_Next[List->Array_Prev[i]] != i)
                {
                    fprintf (file_graph, "    %zu->%zd[label = \"Неправильный цикл\"];\n", i, List->Array_Prev[i]);
                }
            }
            else
            {
                fprintf (file_graph, "    %zu->%zd[label = \"Невозможное значение Prev\"];\n", i, List->Array_Prev[i]);
            }
        }
    }

    fprintf (file_graph, "}\n");
    return 0;
}

int Dump_For_Html      (const list_k* const List, const char* const Name_File_Graph, const char* const Name_Function)
{
    printf ("%s\n", Name_File_Graph);

    FILE* file = fopen (Name_Log, "a");
    if (file == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file, "<u>Dump after function</u> %s\n\n", Name_Function);

    // int Number_Error = List_Error (List);
    // if (Number_Error != 0)
    // {
    //     fprintf (file, "                          <b>%s</b>\n", Array_Name_List_Error[Number_Error]);
    // }

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

    Print_Separator_In_Log (300, file);
    fclose (file);

    #ifdef STOP_PROGRAMME
    return Number_Error;
    #endif // STOP_PROGRAMME

    return 0;
}

//
// int List_Insert_After  (const int Value, const int Index, list_k* const List)
// {
//     char Name_Func[52];
//     snprintf (Name_Func, sizeof (Name_Func), "List_Insert_After (%d, %d, List)", Value, Index);
//
//     #ifdef STOP_PROGRAMME
//     if (List_Error (List) != Not_Error_List)
//     {
//         List_Dump (List, Name_Func);
//         printf ("Error start function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//     #endif // STOP_PROGRAMME
//
//     if (List_Reallocation (List) != 0)
//     {
//         return There_Are_Errors;
//     }
//
//     if (Index < 0 || Index >= int (List->Capacity) - 1 || List->Array_Prev[Index] == -1)
//     {
//         printf ("Impossible %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     size_t Insert_Position = List->Free;
//     List->Free = List->Array_Next[List->Free];
//
//     List->Array_Value[Insert_Position] = Value;
//     List->Size++;
//
//     List->Array_Next[Insert_Position] = List->Array_Next[Index];
//     List->Array_Prev[Insert_Position] = Index;
//
//     List->Array_Prev[List->Array_Next[Index]] = ssize_t (Insert_Position);
//     List->Array_Next[Index] = Insert_Position;
//
//     if (List_Dump (List, Name_Func) == There_Are_Errors)
//     {
//         printf ("Error in function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     return 0;
// }
//
// int List_Insert_Before (const int Value, const int Index, list_k* const List)
// {
//     char Name_Func[52];
//     snprintf (Name_Func, sizeof (Name_Func), "List_Insert_Before (%d, %d, List)", Value, Index);
//
//     #ifdef STOP_PROGRAMME
//     if (List_Error (List) != Not_Error_List)
//     {
//         List_Dump (List, Name_Func);
//         printf ("Error start function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//     #endif // STOP_PROGRAMME
//
//     if (List_Reallocation (List) != 0)
//     {
//         return There_Are_Errors;
//     }
//
//     if (Index <= 0 || Index > int (List->Capacity) - 1 || List->Array_Prev[Index] == -1)
//     {
//         printf ("Impossible %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     size_t Insert_Position = List->Free;
//     List->Free = List->Array_Next[List->Free];
//
//     List->Array_Value[Insert_Position] = Value;
//     List->Size++;
//
//     List->Array_Next[Insert_Position] = size_t (Index);
//     List->Array_Prev[Insert_Position] = List->Array_Prev[Index];
//
//     List->Array_Next[List->Array_Prev[Index]] = Insert_Position;
//     List->Array_Prev[Index] = ssize_t (Insert_Position);
//
//     if (List_Dump (List, Name_Func) == There_Are_Errors)
//     {
//         printf ("Error in function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     return 0;
// }
//
// int List_Push_Front    (const int Value, list_k* const List)
// {
//     char Name_Func[52];
//     snprintf (Name_Func, sizeof (Name_Func), "List_Push_Front (%d, List)", Value);
//
//     #ifdef STOP_PROGRAMME
//     if (List_Error (List) != Not_Error_List)
//     {
//         List_Dump (List, Name_Func);
//         printf ("Error start function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//     #endif // STOP_PROGRAMME
//
//     if (List_Reallocation (List) != 0)
//     {
//         return There_Are_Errors;
//     }
//
//     size_t Index = List->Array_Next[0];
//
//     size_t Insert_Position = List->Free;
//     List->Free = List->Array_Next[List->Free];
//
//     List->Array_Value[Insert_Position] = Value;
//     List->Size++;
//
//     List->Array_Next[Insert_Position] = size_t (Index);
//     List->Array_Prev[Insert_Position] = List->Array_Prev[Index];
//
//     List->Array_Next[List->Array_Prev[Index]] = Insert_Position;
//     List->Array_Prev[Index] = ssize_t (Insert_Position);
//
//     if (List_Dump (List, Name_Func) == There_Are_Errors)
//     {
//         printf ("Error in function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     return 0;
// }
//
// int List_Push_Back     (const int Value, list_k* const List)
// {
//     char Name_Func[52];
//     snprintf (Name_Func, sizeof (Name_Func), "List_Push_Back (%d, List)", Value);
//
//     #ifdef STOP_PROGRAMME
//     if (List_Error (List) != Not_Error_List)
//     {
//         List_Dump (List, Name_Func);
//         printf ("Error start function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//     #endif // STOP_PROGRAMME
//
//     if (List_Reallocation (List) != 0)
//     {
//         return There_Are_Errors;
//     }
//
//     ssize_t Index = List->Array_Prev[0];
//
//     size_t Insert_Position = List->Free;
//     List->Free = List->Array_Next[List->Free];
//
//     List->Array_Value[Insert_Position] = Value;
//     List->Size++;
//
//     List->Array_Next[Insert_Position] = List->Array_Next[Index];
//     List->Array_Prev[Insert_Position] = Index;
//
//     List->Array_Prev[List->Array_Next[Index]] = ssize_t (Insert_Position);
//     List->Array_Next[Index] = Insert_Position;
//
//     if (List_Dump (List, Name_Func) == There_Are_Errors)
//     {
//         printf ("Error in function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     return 0;
// }
//
// int List_Delete        (const int Index, list_k* const List)
// {
//     char Name_Func[52];
//     snprintf (Name_Func, sizeof (Name_Func), "List_Delete (%d, List)", Index);
//
//     #ifdef STOP_PROGRAMME
//     if (List_Error (List) != Not_Error_List)
//     {
//         List_Dump (List, Name_Func);
//         printf ("Error start function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//     #endif // STOP_PROGRAMME
//
//     if (Index <= 0 || Index >= int (List->Capacity) || List->Array_Prev[Index] == -1)
//     {
//         printf ("Impossible %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     List->Array_Value[Index] = 0;
//     List->Size--;
//
//     List->Array_Prev[List->Array_Next[Index]] = List->Array_Prev[Index];
//     List->Array_Next[List->Array_Prev[Index]] = List->Array_Next[Index];
//
//     List->Array_Next[Index] = List->Free;
//     List->Free = size_t (Index);
//     List->Array_Prev[Index] = -1;
//
//     if (List_Dump (List, Name_Func) == There_Are_Errors)
//     {
//         printf ("Error in function %s\n", Name_Func);
//         return There_Are_Errors;
//     }
//
//     return 0;
// }
//
// int List_Reallocation  (list_k* const List)
// {
//     if (List->Size + 1 == List->Capacity)
//     {
//         List->Capacity = List->Capacity * 2;
//
//         List->Array_Value = (int*) realloc (List->Array_Value, List->Capacity * sizeof (int));
//         if (List->Array_Value == NULL)
//         {
//             printf ("Error allocation memory for array value in List_Reallocation\n");
//             return There_Are_Errors;
//         }
//         for (size_t i = List->Size + 1; i < List->Capacity; i++)
//         {
//             List->Array_Value[i] = 0;
//         }
//
//
//         List->Array_Next = (size_t*) realloc (List->Array_Next, List->Capacity * sizeof (size_t*));
//         if (List->Array_Next == NULL)
//         {
//             printf ("Error allocation memory for array next in List_Reallocation!\n");
//             return There_Are_Errors;
//         }
//         for (size_t i = List->Size + 1; i < List->Capacity - 1; i++)
//         {
//             List->Array_Next[i] = i + 1;
//         }
//         List->Array_Next[List->Capacity - 1] = 0;
//
//         List->Array_Prev = (ssize_t*) realloc (List->Array_Prev, List->Capacity * sizeof (ssize_t*));
//         if (List->Array_Prev == NULL)
//         {
//             printf ("Error allocation memory for array prev in List_Reallocation!\n");
//             return There_Are_Errors;
//         }
//         for (size_t i = List->Size + 1; i < List->Capacity; i++)
//         {
//             List->Array_Prev[i] = -1;
//         }
//
//         List->Free = List->Size + 1;
//     }
//
//     return 0;
// }


int Start_Logfile          ()
{
    FILE* file_html = fopen (Name_Log, "w");
    if (file_html == NULL)
    {
        printf ("Error open logfile\n");
        return There_Are_Errors;
    }

    fprintf (file_html, "<pre>\n\n");

    Print_Separator_In_Log (300, file_html);

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
