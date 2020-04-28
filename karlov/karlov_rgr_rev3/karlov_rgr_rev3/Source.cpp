#include <iostream>
#include <stdio.h>
#include <conio.h>

#pragma warning(disable:4996)

void is_opened_file(char* argv[]) {//checking that file is exist
    FILE* f;
    f = fopen(argv[1], "rt");
    if (!f) {
        printf("There are no such file");
        exit(0);
    }
    fclose(f);
}

char* file_to_char(char* argv[], int& size)
{
    //includes argv[], transmitted file to string

    size = 0;
    FILE* f;
    f = fopen(argv[1], "r");

    while (!feof(f)) {//checking how many chars in file
        getc(f);
        size++;
    }
    fclose(f);

    f = fopen(argv[1], "r");
    char* string = new char[size];
    for (int i = 0; i < size; ++i)
    {
        string[i] = 0;
    }

    fread(string, 1, size, f);

    fclose(f);

    return string;
}

void print_to_screen(char* string) {
    printf("%s", string);
}

size_t mystrlen(char* string) {//solve string lenght(i mean how many chars in it) by using pointers
    char* copy = string;
    while (*string)
    {
        ++string;
    }
    return string - copy;
}

void cleaning(char* string, int size) {
    for (int i = 0; i < size; i++) {
        string[i] = ' ';
    }
}

int which_is_bigger(int len_left, int len_right) {//>0 if len_right > len_left, <0 len_left>len_right, 0 len_left=len_right
    return len_left - len_right;
}

char* create_new_word(char* string, int start/*always after special symbols*/) {
    int size = 0;

    for (int i = start; string[i] != ' '; i++) {
        if (string[i] == '>')
            break;
        size++;
    }
    char* word = new char[size];

    cleaning(word, size);

    for (int i = 0; i < size; i++, start) {
        word[i] = string[start];
    }

    return word;
}

//int new_size_string(char* string, int buffer, int replace_size) {
//    char* word = nullptr;
//    int count = 0;
//    int word_size = 0;
//    for (int i = 0; i < buffer; i++) {
//        if (string[i] == '"') {
//            i++;
//            word = create_new_word(string, i);
//            if (mystrlen(word) == replace_size)
//                count++;
//        }
//    }
//
//    for (int i = 0; i < buffer; i++) {
//        if (string[i] == '"') {
//            i++;
//            word = create_new_word(string, i);
//            if (mystrlen(word) == replace_size)
//                word_size += mystrlen(word) * replace_size;
//        }
//    }
//    
//    int word_count_with_size = count * word_size;
//    int replace_count_with_size = count * replace_size;
//
//    int real = which_is_bigger(word_count_with_size, replace_count_with_size);
//
//    return buffer - real;
//}

int mystrcmp(const char* str1, const char* str2) {
    while (*str1 && *str2) {
        if (*str1 < *str2)
            return -1;
        if (*str1 > * str2)
            return 1;
        ++str1; ++str2;
    }

    return *str1 ? -1 : *str2 ? 1 : 0;
}

int new_size_string(char* string, int buffer, int replace_size, char* find, int find_size) {
    //нельзя сказать точный размер, точнее можно но немного сложно. так как значения атрибутов могут быть зарных размеров. Придется пересчитывать.
    //вижу вариант вычислить длину строки без атрибутов и потом просто добавить по кол-ву * размер
    int count_of_atributes = 0;
    char* word;
    bool flag = false;

    for (int i = 0; i < buffer; i++) {
        if (string[i] == '<') {
            i++;
            flag = false;
            word = create_new_word(string, i);
            for (int j = 0; j < find_size; j++) {
                if (word[j] != find[j])
                    flag = true;
            }
            if(flag == false)
            count_of_atributes++;
        }
    }

    for (int i = 0; i < buffer; i++) {
        if (string[i] == '<') {
            flag = false;
            word = create_new_word(string, i);
            for (int j = 0; j < find_size; j++) {
                if (word[j] != find[j])
                    flag = true;
            }
            if (flag == false) {
                for (int j = i; j < buffer; j++, i++) {
                    if (string[j] == '"') {
                        j++;
                        i++;
                        word = create_new_word(string, i);
                        buffer -= mystrlen(word);
                    }
                }
            }
        }
    }

    int contr_size = count_of_atributes * replace_size;

    return buffer + contr_size;
}

char* hide_and_seek(char* argv[]) {
    char* find = argv[2];
    char* replace = argv[3];

    int find_size = mystrlen(find);
    int replace_size = mystrlen(replace);

    int buffer = 0;
    char* string = file_to_char(argv, buffer);
    int atribute_size;
    int new_string_size = new_size_string(string, buffer, replace_size, find, find_size);

    char* new_string = new char[new_string_size];

    cleaning(new_string, new_string_size);

    for (int i = 0, j = 0; j < buffer; i++, j++) {
        new_string[i] = string[j];
        if (string[j] == '<') {
            i++;
            j++;
            new_string[i] = string[j];
            char* word = create_new_word(string, j);
            int word_size = mystrlen(word);
            bool flag = false;
            if (mystrcmp(word, find))
                flag = true;
            if (flag == false) {
                while (true) {
                    if(string[j] == '"')
                        break;
                    new_string[i] = string[j];
                    i++;
                    j++;
                }
                new_string[i] = string[j];
                i++;
                //j += replace_size;
                atribute_size = 0;
                for (int k = j; string[k] != '"'; k++)
                    atribute_size++;
                j += atribute_size;
                for (int k = 0; k < replace_size; k++, i++) {
                    new_string[i] = replace[k];
                }
                delete[] word;
            }
        }
    }
    return new_string;
}

int main(int argc, char* argv[]) {
    if (argc == 4) {
        int buff = 0;
        int size = 0;
        char* old_string = file_to_char(argv, size);
        is_opened_file(argv);
        char* new_string = hide_and_seek(argv);

        printf("\n===================================\nOld string\n===================================\n");
        print_to_screen(old_string);

        printf("\n===================================\nNew string\n===================================\n");
        print_to_screen(new_string);
    }
    else {
        printf("Invalid params");
        exit(0);
    }

    printf("Enter any button to exit the program");
    getch();
    return 0;
}