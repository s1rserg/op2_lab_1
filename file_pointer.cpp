#include "file_pointer.h"
namespace fp {

    void menu() {
        char *fileName = new char[MAX];
        char choice;
        bool to_continue = true;
        while (to_continue) {
            cout << "Do you want to create New file or open an Existing (N/E):_";
            cin >> choice;
            cin.ignore();
            if (choice == 'n' || choice == 'N') {
                strcpy(fileName, "input.txt");
                to_continue = false;
            } else if (choice == 'e' || choice == 'E') {
                cout << "Enter the name of the file with extension:_";
                cin.getline(fileName, MAX);
                outputFile(fileName);
                to_continue = false;
            } else{
                cout << "You should enter only E or N. Try again" << endl;
            }
        }
        inputFile(fileName);
        processFile(fileName);
        delete[] fileName;
    }


    void inputFile(char *fileN) {
        char choice; // чи продовжувати ввід
        char *line = new char[MAX]; // рядок, який вводить користувач

        FILE *file = fopen(fileN, "a");
        if (!file) {
            cout << "File opening error " << fileN << endl;
            return;
        }

        do {
            cout << "Enter line:_";
            cin.getline(line, MAX);
            fprintf(file, "%s\n", line);
            cout << "Do you want to continue printing (Y/N):_";
            cin >> choice;
            cin.ignore();
        } while (choice != 'N' && choice != 'n');

        fclose(file);
    }

    void outputFile(char *fileN) {
        FILE *file = fopen(fileN, "r");
        if (!file) {
            cout << "File opening error " << fileN << endl;
            return;
        }
        cout << fileN << " contents:" << endl;
        char ch; // символ з файла
        while ((ch = fgetc(file)) != EOF) {
            cout << ch;
        }
        cout << endl;
        fclose(file);
    }

    void processFile(char *fileN) {
        FILE *file1 = fopen(fileN, "r");
        if (!file1) {
            cout << "File opening error " << fileN << endl;
            return;
        }
        FILE *file2 = fopen("input2.txt", "a+");
        if (!file2) {
            cout << "File opening error " << "input2.txt" << endl;
            return;
        }
        char *line = new char[MAX + 1]; // рядок з файлу
        char *copy_line = new char[MAX + 1]; //копія рядка з файлу

        while (fgets(line, MAX, file1)) {
            int len_long_seq = 0; // довжина найдовшої послідовності цифр
            int len_curr_seq = 0; // довжина поточної послідовності цифр
            int is_add_count = 0;// лічильник найдовших послідовностей
            int n = 47; // ascii код цифри послідовності
            char *seq = new char[MAX + 1];; //поточна послідовність
            seq[0] = '\0';

            strcpy(copy_line, line);
            if (copy_line[strlen(copy_line) - 1] == '\n' || copy_line[strlen(copy_line) - 1] == ' ') {
                copy_line[strlen(copy_line) - 1] = '\0';
            }
            char **words = new char *[MAX]; // масив для зберігання слів рядка
            int count = 0; // кількість слів у рядку

            char *word = strtok(line, " ,.");
            while (word != NULL) {
                char *newWord = new char[strlen(word) + 1];
                strcpy(newWord, word);
                words[count++] = newWord;
                word = strtok(NULL, " ,.");
            }
            char *cur_word = words[0];

            char **words_with_digits = new char *[count];
            char **digits_sequences = new char *[count];
            for (int i = 0; i < count; i++) { // по словам рядка
                for (int j = 0; j <= strlen(words[i]); j++) { // по символам слова
                    if ((!isdigit(words[i][j]) || words[i][j] <= n)) { // якщо послідовність перервалась
                        if (len_curr_seq >= len_long_seq &&
                            len_curr_seq != 0) { //якщо перервана послідовність була довшою за найдовшу
                            if (is_add_count > 0 && len_curr_seq > len_long_seq) {
                                for (int c = 0; c < is_add_count; c++) {
                                    words_with_digits[c] = NULL;
                                    digits_sequences[c] = NULL;
                                }
                                is_add_count = 0;
                            }
                            words_with_digits[is_add_count] = strdup(cur_word);
                            digits_sequences[is_add_count] = strdup(seq);
                            is_add_count++;
                            len_long_seq = len_curr_seq;
                        }
                        memset(seq, 0, len_curr_seq);
                        len_curr_seq = 0;
                        if (words[i][j] <= n && words[i][j] != '\0') {
                            len_curr_seq++;
                            n = words[i][j];
                            strncat(seq, &words[i][j], 1);
                        }
                    } else {
                        len_curr_seq++;
                        n = words[i][j];
                        strncat(seq, &words[i][j], 1);
                    }
                    cur_word = words[i];
                }// кінець по символам слова
                n = 47;
            }// кінець по словам рядка
            moveSubstringToEnd(copy_line, words_with_digits, is_add_count);
            swapSubstrings(copy_line, digits_sequences, is_add_count);
            fprintf(file2, "%s\n", copy_line);

            delete[] seq;
            delete[] cur_word;
            for (int i = 0; i < is_add_count; i++) {
                delete[] words_with_digits[i];
                delete[] digits_sequences[i];
            }
            delete[] words_with_digits;
            delete[] digits_sequences;
            delete[] words;

        }// кінець проходження по рядкам файла
        delete[] line;
        delete[] copy_line;

        fclose(file1);
        fclose(file2);
        remove(fileN);
        rename("input2.txt", fileN);

        outputFile(fileN);
        char *fileName = new char[11];
        strcpy(fileName, "output.txt");
        outputFile(fileName);
        delete[] fileName;
    }

    void swapSubstrings(char *str, char *substrings[], int numSubstrings) {
        FILE *file = fopen("output.txt", "a");
        if (!file) {
            cout << "File opening error " << "output.txt" << endl;
            return;
        }

        int len = strlen(str);
        int *marked = new int[len];
        memset(marked, 0, len * sizeof(int));
        for (int i = 0; i < numSubstrings; i++) {
            const char *substr = substrings[i];
            int sublen = strlen(substr);
            const char *p = strstr(str, substr);
            while (p != nullptr) {
                int pos = p - str;
                for (int j = pos; j < pos + sublen; j++) {
                    marked[j] = 1;
                }
                p = strstr(p + sublen, substr);
            }
        }
        for (int i = 0; i < len; i++) {
            if (marked[i] == 1) {
                fprintf(file, "*");
            } else {
                fprintf(file, "%c", str[i]);
            }
        }
        fprintf(file, "\n");
        delete[] marked;
        fclose(file);
    }

    void moveSubstringToEnd(char *str, char *substrings[], int numSubstrings) {
        for (int i = 0; i < numSubstrings; i++) {
            if(substrings[i][strlen(substrings[i]) - 1] == '\n'){
                substrings[i][strlen(substrings[i]) - 1] = '\0';
            }
            int subStrLen = strlen(substrings[i]) + 1;
            if (str[strlen(str) - 1] == '\n' || str[strlen(str) - 1] == ' ') {
                str[strlen(str) - 1] = '\0';
            }
            int strLen = strlen(str);
            char *found = strstr(str, substrings[i]);

            if (found != NULL) {
                int startIndex = found - str;
                int endIndex = startIndex + subStrLen - 1;

                str[strLen] = ' ';
                str[strLen + 1] = '\0';

                char temp[subStrLen + 1];
                strncpy(temp, str + startIndex, subStrLen);
                temp[subStrLen] = '\0';
                strncpy(str + startIndex, str + endIndex + 1, strLen - endIndex);
                strncpy(str + strLen - subStrLen + 1, temp, subStrLen);
            }
        }
    }
}