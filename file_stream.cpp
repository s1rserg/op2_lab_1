#include "file_stream.h"
namespace fs {

    void menu() {
        string fileName;
        char choice;
        bool to_continue = true;
        while (to_continue) {
            cout << "Do you want to create New file or open an Existing (N/E):_";
            cin >> choice;
            cin.ignore();
            if (choice == 'n' || choice == 'N') {
                fileName = "input.txt";
                to_continue = false;
            } else if (choice == 'e' || choice == 'E') {
                cout << "Enter the name of the file with extension:_";
                getline(cin, fileName);
                outputFile(fileName);
                to_continue = false;
            } else cout << "You should enter only E or N. Try again" << endl;
        }
        inputFile(fileName);
        processFile(fileName);
    }

    void inputFile(string name) {
        char choice; // чи продовжувати ввід
        string data; // рядок, який вводить користувач

        ofstream file(name, ios::app);
        if (!file) {
            cout << "File opening error " << name << endl;
            return;
        }
        do {
            cout << "Enter line:_";
            getline(cin, data);
            file << data << endl;
            cout << "Do you want to continue printing (Y/N):_";
            cin >> choice;
            cin.ignore();
        } while (choice != 'N' && choice != 'n');
        file.close();
    }

    void outputFile(string name) {
        string line; // рядок з файлу

        ifstream file(name);
        if (!file) {
            cout << "File opening error " << name << endl;
            return;
        }
        cout << name << " contents:" << endl;
        while (getline(file, line)) {
            cout << line << endl;
        }
        cout << endl;
        file.close();
    }

    void processFile(string name) {
        ifstream file1(name);
        if (!file1) {
            cout << "File opening error " << name << endl;
            return;
        }
        ofstream file2("input2.txt", ios::app);
        if (!file2) {
            cout << "File opening error " << "input2.txt" << endl;
            return;
        }
        string line; // рядок з файлу

        while (getline(file1, line)) {
            int len_long_seq = 0; // довжина найдовшої послідовності цифр
            int len_curr_seq = 0; // довжина поточної послідовності цифр
            int is_add_count = 0;// лічильник найдовших послідовностей
            int n = 47; // ascii код цифри послідовності
            string seq; //поточна послідовність

            string copy_line = line; //копія рядка з файлу
            if (copy_line[copy_line.length() - 1] == '\n') {
                copy_line[copy_line.length() - 1] = '\0';
            }
            string words[MAX]; // масив для зберігання слів рядка
            int count = 0; // кількість слів у рядку
            string word; // слово для розділення
            for (char i: line) {
                if (i == ' ' || i == ',' || i == '.') {
                    if (!word.empty()) {
                        words[count] = word;
                        count++;
                        word = "";
                    }
                } else word += i;
            }
            if (!word.empty()) {
                words[count] = word;
                count++;
            }
            string cur_word = words[0]; // поточне слово

            string words_with_digits[count];
            string digits_sequences[count];
            for (int i = 0; i < count; i++) { // по словам рядка
                for (int j = 0; j <= words[i].length(); j++) { // по символам слова
                    if ((!isdigit(words[i][j]) || words[i][j] <= n)) { // якщо послідовність перервалась
                        if (len_curr_seq >= len_long_seq && len_curr_seq != 0) { //якщо перервана довша за найдовшу
                            if (is_add_count > 0 && len_curr_seq > len_long_seq) {
                                for (int c = 0; c < is_add_count; c++) {
                                    words_with_digits[c] = "";
                                    digits_sequences[c] = "";
                                }
                                is_add_count = 0;
                            }
                            words_with_digits[is_add_count] = cur_word;
                            digits_sequences[is_add_count] = seq;
                            is_add_count++;
                            len_long_seq = len_curr_seq;
                        }
                        seq.clear();
                        len_curr_seq = 0;
                        if (words[i][j] <= n && words[i][j] != '\0') {
                            len_curr_seq++;
                            n = words[i][j];
                            seq += words[i][j];
                        }
                    } else {
                        len_curr_seq++;
                        n = words[i][j];
                        seq += words[i][j];
                    }
                    cur_word = words[i];
                }// кінець по символам слова
                n = 47;
                len_curr_seq = 0;
            }// кінець по словам рядка
            moveSubstringToEnd(copy_line, words_with_digits, is_add_count);
            swapSubstrings(copy_line, digits_sequences, is_add_count);
            file2 << copy_line << endl;
        }// кінець проходження по рядкам файла
        file1.close();
        file2.close();
        remove(name.c_str());
        rename("input2.txt", name.c_str());
        outputFile(name);
        outputFile("output.txt");
    }

    void swapSubstrings(string &str, string substrings[], int numSubstrings) {
        ofstream file("output.txt", ios::app);
        if (!file) {
            cout << "File opening error " << "output.txt" << endl;
            return;
        }
        int strLen = str.length(); // довжина рядка
        int marked[strLen]; // масив для зберігання індексів
        for(int i = 0; i < strLen; i++){
            marked[i] = 0;
        }
        for (int i = 0; i < numSubstrings; i++) {
            string substr = substrings[i];
            int sublen = substr.length();
            int pos = str.find(substr);
            while (pos != string::npos) { // цикл по рядку
                for (int j = pos; j < pos + sublen; j++) {
                    marked[j] = 1;
                }
                pos = str.find(substr, pos + sublen);
            }
        }
        for (int i = 0; i < str.length(); i++) {
            if (marked[i] == 1) {
                file << "*";
            } else {
                file << str[i];
            }
        }
        file << endl;
        file.close();
    }

    void moveSubstringToEnd(string &str, string substrings[], int numSubstrings) {
        for (int i = 0; i < numSubstrings; i++) {
            int subStrLen = substrings[i].length() + 1; // довжина підрядка
            int strLen = str.length(); // довжина рядка
            int found = str.find(substrings[i]); // індекс початку підрядка в рядку

            if (found != string::npos) {
                int startIndex = found;

                if (str[strLen - 1] != ' ' && str[strLen - 1] != ',' && str[strLen - 1] != '.') {
                    str += ' ';
                }

                string temp = str.substr(startIndex, subStrLen);
                str.erase(startIndex, subStrLen);
                str += temp;
            }
        }
    }
}