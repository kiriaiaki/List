#include "List.h"

int main ()
{
    FILE* file_html = fopen ("Debag.html", "w");
    fprintf (file_html, "<pre>\n\n");
    fprintf (file_html, "+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n\n");

    list_k List = {};
    if (List_Ctor (&List, file_html) == There_Are_Errors)
    {
        printf ("\033[31m!!!EEERRRRRROOORRR!!!\033[0m\n");
        return 0;
    }

    // List_Insert_After (10, 0, &List, file_html);
    // List_Insert_After (20, 1, &List, file_html);
    // List_Insert_After (30, 2, &List, file_html);
    // List_Insert_After (25, 2, &List, file_html);
    // List_Insert_After (5, 0, &List, file_html);
    // List_Insert_After (15, 1, &List, file_html);
    // List_Delete (2, &List, file_html);
    // List_Delete (3, &List, file_html);
    // List_Insert_After (2007, 4, &List, file_html);
    // List_Insert_Before (1111, 3, &List, file_html);

    List_Insert_After (10, 0, &List, file_html);
    List_Insert_After (20, 1, &List, file_html);
    List_Insert_After (30, 2, &List, file_html);
    List_Insert_After (40, 3, &List, file_html);
    List.Array_Prev[2] = 77;
    List.Array_Next[3] = 700;
    List_Insert_After (50, 4, &List, file_html);

    List_Push_Front   (5, &List, file_html);
    List_Push_Back    (55, &List, file_html);


    // List_Insert_After (60, 5, &List, file_html);
    // List_Insert_After (70, 6, &List, file_html);
    // List_Insert_After (80, 7, &List, file_html);
    // List_Insert_After (90, 8, &List, file_html);
    // List_Insert_After (100, 9, &List, file_html);
    // List_Push_Front   (5, &List, file_html);
    // List_Push_Back    (110, &List, file_html);

    fclose (file_html);
    List_Dtor (&List);
    return 0;
}

int List_Ctor (list_k* const List, FILE* file_html)
{
    List->Size = 0;
    List->Capacity = 10;
    List->Array_Value = (int*) calloc (List->Capacity, sizeof (int));
    if (List->Array_Value == NULL)
    {
        printf ("Error allocation memory for array value in List_Ctor!\n");
        return There_Are_Errors;
    }
    List->Array_Value[0] = Canary;

    List->Array_Next = (size_t*) calloc (List->Capacity, sizeof (size_t));
    if (List->Array_Next == NULL)
    {
        printf ("Error allocation memory for array next in List_Ctor!\n");
        return There_Are_Errors;
    }

    for (size_t i = 1; i < List->Capacity - 1; i++)
    {
        List->Array_Next[i] = i + 1;
    }
    List->Array_Next[List->Capacity - 1] = 0;

    List->Array_Prev = (ssize_t*) calloc (List->Capacity, sizeof (ssize_t));
    if (List->Array_Prev == NULL)
    {
        printf ("Error allocation memory for array prev in List_Ctor!\n");
        return There_Are_Errors;
    }

    for (size_t i = 1; i < List->Capacity; i++)
    {
        List->Array_Prev[i] = -1;
    }

    List->Free = 1;

    fprintf (file_html, "<u>Dump after function creator</u>\n\n");
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Dtor (list_k* const List)
{
    List->Size = 0;
    List->Capacity = 0;

    free (List->Array_Value);

    free (List->Array_Next);

    free (List->Array_Prev);

    List->Free = 0;

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

    if (List->Array_Value == NULL)
    {
        return Not_Array_Data;
    }

    if (List->Array_Next == NULL)
    {
        return Not_Array_Next;
    }

    if (List->Array_Prev == NULL)
    {
        return Not_Array_Prev;
    }

    if (List->Size >= List->Capacity)
    {
        return Bad_Size;
    }

    if (List->Array_Value[0] != Canary)
    {
        return Bad_Canary;
    }

    if (List->Free == 0 || List->Free >= List->Capacity)
    {
        return Bad_Free;
    }

    if (List->Array_Prev[0] >= ssize_t (List->Capacity))
    {
        return Bad_Head;
    }

    if (List->Array_Next[0] >= List->Capacity)
    {
        return Bad_Tail;
    }

    size_t Counter_Free_Element = 0;
    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] == -1)
        {
            Counter_Free_Element++;
        }
    }
    if (Counter_Free_Element != (List->Capacity - 1) - List->Size)
    {
        return Bad_Count_Free;
    }

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] >= ssize_t (List->Capacity))
        {
            return Impossible_Prev;
        }

        else if (List->Array_Next[i] >= List->Capacity)
        {
            return Impossible_Next;
        }
    }

    for (size_t i = 0; i < List->Capacity; i++)
    {
        for (size_t j = i + 1; j < List->Capacity; j++)
        {
            if (List->Array_Prev[i] == List->Array_Prev[j] && List->Array_Prev[j] != -1)
            {
                return Repeat_Prev;
            }

            else if (List->Array_Next[i] == List->Array_Next[j])
            {
                if (List->Array_Next[i] == 0 && (List->Array_Prev[i] == - 1 || List->Array_Prev[j] == - 1))
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
        if (ssize_t (i) != List->Array_Prev[List->Array_Next[i]])
        {
            if (List->Array_Prev[i] == -1 && List->Array_Prev[List->Array_Next[i]] == -1)
            {
                continue;
            }

            else if (List->Array_Prev[i] == -1 && List->Array_Next[i] == 0)
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
    for (size_t i = List->Free; List->Array_Next[i] != 0; i = List->Array_Next[i])
    {
        Len_Free_Cycle++;
    }
    if (Len_Free_Cycle != (List->Capacity - 1) - List->Size - 1)
    {
        return Bad_Len_Free_Cycle;
    }

    size_t Len_Next_Cycle = 0;
    for (size_t i = 0; List->Array_Next[i] != 0; i = List->Array_Next[i])
    {
        Len_Next_Cycle++;
    }
    if (Len_Next_Cycle != List->Size)
    {
        return Bad_Len_Next_Cycle;
    }

    size_t Len_Prev_Cycle = 0;
    for (ssize_t i = 0; List->Array_Prev[i] != 0; i = List->Array_Prev[i])
    {
        Len_Prev_Cycle++;
    }
    if (Len_Prev_Cycle != List->Size)
    {
        return Bad_Len_Prev_Cycle;
    }

    return Not_Error_List;
}


int List_Dump_In_Html (const list_k* const List, FILE* file_html)
{
    char Name_File[25] = "File_txt/File_";
    Naming_File (Name_File);

    FILE* file = fopen (Name_File, "w");

    Dump_For_Graph (List, file);

    fclose (file);

    char Command[64] = "dot ";
    Naming_Command_Dot (Command, Name_File);

    system (Command);

    Dump_For_Html (List, file_html, Name_File);

    return 0;
}

int Naming_File (char* const Name_File)
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

int Dump_For_Graph (const list_k* const List, FILE* const file)
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

    fprintf (file, "digraph G\n"
                   "{\n"
                   "    rankdir = LR;\n"
                   "    node[shape = \"doubleoctagon\", style = \"filled\", fillcolor = \"red\"];\n"
                   "    edge[color = \"red\", style = \"bold\"];\n");
    fprintf (file, "\n");

    fprintf (file, "    %d [shape = Mrecord, label = \"index = %d | value = %d | {head = %zd | tail = %zu}\", style = \"filled\", fillcolor = \"lightblue\"];\n", 0, 0, List->Array_Value[0], List->Array_Prev[0], List->Array_Next[0]);

    for (size_t i = 1; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] == -1)
        {
            fprintf (file, "    %zu [shape = Mrecord, label = \"index = %zu | value = %d | {prev = %zd | next = %zu}\", style = \"filled\", fillcolor = \"lightgrey\"];\n", i, i, List->Array_Value[i], List->Array_Prev[i], List->Array_Next[i]);
        }

        else
        {
            fprintf (file, "    %zu [shape = Mrecord, label = \"index = %zu | value = %d | {prev = %zd | next = %zu}\", style = \"filled\", fillcolor = \"lightblue\"];\n", i, i, List->Array_Value[i], List->Array_Prev[i], List->Array_Next[i]);
        }
    }
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity - 1; i++)
    {
        fprintf (file, "    %zu->%zu[color = \"white\", weight = 100];\n", i, i + 1);
    }
    fprintf (file, "\n");

    fprintf (file, "    head [shape = invhouse, label = \"head = %zd\", style = \"filled\", fillcolor = \"goldenrod1\"];\n", List->Array_Prev[0]);
    fprintf (file, "    tail [shape = invhouse, label = \"tail = %zu\", style = \"filled\", fillcolor = \"pink2\"];\n", List->Array_Next[0]);
    if (List->Free != 0)
    {
        fprintf (file, "    free [shape = invhouse, label = \"first free = %zu\", style = \"filled\", fillcolor = \"green\"];\n", List->Free);
    }
    else
    {
        fprintf (file, "    free [shape = Mrecord, label = \"not free\", style = \"filled\", fillcolor = \"orange\"];\n");
    }
    fprintf (file, "\n");

    fprintf (file, "    {rank = same; head; %zd};\n", List->Array_Prev[0]);
    fprintf (file, "    {rank = same; tail; %zu};\n", List->Array_Next[0]);
    fprintf (file, "    {rank = same; free; %zu};\n", List->Free);
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity ; i++)
    {
        if (List->Array_Next[i] < List->Capacity)
        {
            if (List->Array_Prev[i] == -1 && List->Array_Prev[List->Array_Next[i]] == -1)
            {
                if (List->Array_Next[i] != Repeat_Free)
                {
                    fprintf (file, "    %zu->%zu[color = \"green\"];\n", i, List->Array_Next[i]);
                }

                else
                {
                    fprintf (file, "    %zu->%zu[label = \"Повторные стрелки\"];\n", i, List->Array_Next[i]);
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
                    fprintf (file, "    %zu->%zu[color = \"pink2:goldenrod1\", dir = both];\n", i, List->Array_Next[i]);
                }

                else
                {
                    fprintf (file, "    %zu->%zu[label = \"Неправильный цикл\"];\n", i, List->Array_Next[i]);
                }
            }
        }

        else
        {
            fprintf (file, "    %zu->%zu[label = \"Невозможное значение\"];\n", i, List->Array_Next[i]);
        }
    }
    fprintf (file, "\n");

    for (size_t i = 0; i < List->Capacity; i++)
    {
        if (List->Array_Prev[i] != -1)
        {
            if (size_t (List->Array_Prev[i]) < List->Capacity)
            {
                if (List->Array_Next[List->Array_Prev[i]] != i)
                {
                    fprintf (file, "    %zu->%zd[label = \"Неправильный цикл\"];\n", i, List->Array_Prev[i]);
                }
            }
            else
            {
                fprintf (file, "    %zu->%zd[label = \"Невозможное значение\"];\n", i, List->Array_Prev[i]);
            }
        }
    }

    fprintf (file, "}\n");
    return 0;
}

int Dump_For_Html (const list_k* const List, FILE* const file, const char* const Name_File)
{
    int Number_Error = List_Error (List);
    if (Number_Error != 0)
    {
        fprintf (file, "                          <b>%s</b>\n", Array_Name_List_Error[Number_Error]);
    }

    fprintf (file, "SIZE = %zu\n", List->Size);
    fprintf (file, "CAPACITY = %zu\n", List->Capacity);
    fprintf (file, "HEAD = %zd\n", List->Array_Prev[0]);
    fprintf (file, "TAIL = %zu\n", List->Array_Next[0]);
    fprintf (file, "FREE = %zu\n", List->Free);
    fprintf (file, "\n");

    fprintf (file, "DATA:\n");
    size_t Len = List->Capacity;
    size_t Len_Line = List->Capacity;
    for (size_t i = 0; i < Len_Line; i++)
    {
        fprintf (file, "   %2zu  ", i);
    }
    fprintf (file, "\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4d ", List->Array_Value[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "NEXT:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4zu ", List->Array_Next[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "PREV:\n");
    for (size_t j = 0; j < Len_Line; j++)
    {
        fprintf (file, "+------");
    }
    fprintf (file, "+\n");
    for (size_t i = 0; i < Len_Line; i++)
    {
        if (i >= Len)
        {
            fprintf (file, "|      ");
        }
        else
        {
            fprintf (file, "| %4zd ", List->Array_Prev[i]);
        }


        if (i % Len_Line == Len_Line - 1)
        {
            fprintf (file, "|\n");

            for (size_t j = 0; j < Len_Line; j++)
            {
                fprintf (file, "+------");
            }

            fprintf (file, "+\n");
        }

    }
    fprintf (file, "\n");


    fprintf (file, "<img src = \"%s\">\n", Name_File);
    fprintf (file, "\n");

    fprintf (file, "+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n\n");

    return 0;
}


int List_Insert_After (const int Value, const int Index, list_k* const List, FILE* file_html)
{
    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    if (Index < 0 || Index >= int (List->Capacity) - 1 || List->Array_Prev[Index] == -1)
    {
        printf ("Wrong insert_after (%d)\n", Index);
        return There_Are_Errors;
    }

    size_t Insert_Position = List->Free;
    List->Free = List->Array_Next[List->Free];

    List->Array_Value[Insert_Position] = Value;
    List->Size++;

    List->Array_Next[Insert_Position] = List->Array_Next[Index];
    List->Array_Prev[Insert_Position] = Index;

    List->Array_Prev[List->Array_Next[Index]] = ssize_t (Insert_Position);
    List->Array_Next[Index] = Insert_Position;

    fprintf (file_html, "<u>Dump after function insert after (%d, %d)</u>\n\n", Value, Index);
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Insert_Before (const int Value, const int Index, list_k* const List, FILE* file_html)
{
    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    if (Index <= 0 || Index > int (List->Capacity) - 1 || List->Array_Prev[Index] == -1)
    {
        printf ("Wrong insert_before (%d)\n", Index);
        return There_Are_Errors;
    }

    size_t Insert_Position = List->Free;
    List->Free = List->Array_Next[List->Free];

    List->Array_Value[Insert_Position] = Value;
    List->Size++;

    List->Array_Next[Insert_Position] = size_t (Index);
    List->Array_Prev[Insert_Position] = List->Array_Prev[Index];

    List->Array_Next[List->Array_Prev[Index]] = Insert_Position;
    List->Array_Prev[Index] = ssize_t (Insert_Position);

    fprintf (file_html, "<u>Dump after function insert before (%d, %d)</u>\n\n", Value, Index);
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Push_Front (const int Value, list_k* const List, FILE* file_html)
{
    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    size_t Index = List->Array_Next[0];

    size_t Insert_Position = List->Free;
    List->Free = List->Array_Next[List->Free];

    List->Array_Value[Insert_Position] = Value;
    List->Size++;

    List->Array_Next[Insert_Position] = size_t (Index);
    List->Array_Prev[Insert_Position] = List->Array_Prev[Index];

    List->Array_Next[List->Array_Prev[Index]] = Insert_Position;
    List->Array_Prev[Index] = ssize_t (Insert_Position);

    fprintf (file_html, "<u>Dump after function push front (%d)</u>\n\n", Value);
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Push_Back (const int Value, list_k* const List, FILE* file_html)
{
    if (List_Reallocation (List) != 0)
    {
        return There_Are_Errors;
    }

    ssize_t Index = List->Array_Prev[0];

    size_t Insert_Position = List->Free;
    List->Free = List->Array_Next[List->Free];

    List->Array_Value[Insert_Position] = Value;
    List->Size++;

    List->Array_Next[Insert_Position] = List->Array_Next[Index];
    List->Array_Prev[Insert_Position] = Index;

    List->Array_Prev[List->Array_Next[Index]] = ssize_t (Insert_Position);
    List->Array_Next[Index] = Insert_Position;

    fprintf (file_html, "<u>Dump after function push back (%d)</u>\n\n", Value);
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Delete (const int Index, list_k* const List, FILE* file_html)
{
    if (Index <= 0 || Index >= int (List->Capacity) || List->Array_Prev[Index] == -1)
    {
        printf ("Wrong delete (%d)", Index);
        return There_Are_Errors;
    }

    List->Array_Value[Index] = 0;
    List->Size--;

    List->Array_Prev[List->Array_Next[Index]] = List->Array_Prev[Index];
    List->Array_Next[List->Array_Prev[Index]] = List->Array_Next[Index];

    List->Array_Next[Index] = List->Free;
    List->Free = size_t (Index);
    List->Array_Prev[Index] = -1;

    fprintf (file_html, "<u>Dump after function delete (%d)</u>\n\n", Index);
    List_Dump_In_Html (List, file_html);
    return 0;
}

int List_Reallocation (list_k* const List)
{
    if (List->Size + 1 == List->Capacity)
    {
        List->Capacity = List->Capacity * 2;

        List->Array_Value = (int*) realloc (List->Array_Value, List->Capacity * sizeof (int));
        if (List->Array_Value == NULL)
        {
            printf ("Error allocation memory for array value in List_Reallocation\n");
            return There_Are_Errors;
        }
        for (size_t i = List->Size + 1; i < List->Capacity; i++)
        {
            List->Array_Value[i] = 0;
        }


        List->Array_Next = (size_t*) realloc (List->Array_Next, List->Capacity * sizeof (size_t*));
        if (List->Array_Next == NULL)
        {
            printf ("Error allocation memory for array next in List_Reallocation!\n");
            return There_Are_Errors;
        }
        for (size_t i = List->Size + 1; i < List->Capacity - 1; i++)
        {
            List->Array_Next[i] = i + 1;
        }
        List->Array_Next[List->Capacity - 1] = 0;

        List->Array_Prev = (ssize_t*) realloc (List->Array_Prev, List->Capacity * sizeof (ssize_t*));
        if (List->Array_Prev == NULL)
        {
            printf ("Error allocation memory for array prev in List_Reallocation!\n");
            return There_Are_Errors;
        }
        for (size_t i = List->Size + 1; i < List->Capacity; i++)
        {
            List->Array_Prev[i] = -1;
        }

        List->Free = List->Size + 1;
    }

    return 0;
}


int Reverse_Str (char* const Str)
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

char* itoa_k (int Number, char* const Str)
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
