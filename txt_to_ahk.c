#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Extracts the name of the .txt file so the program can create a .ahk file with the same name
void extractFilename(const char *filename, char *result) {
    int i = 0;
    while (i < strlen(filename) && filename[i] != '.') {
        result[i] = filename[i];
        i++;
    }
    result[i] = '\0';
}

int main(int argc, char *argv[]) {

    // FIRST CASE: no .txt provided
    if (argc == 1) {
        printf("Enter text:\n");

        // Creates a array of strings (message) and stores each line on it until it reaches an only \n line
        char message[1000][1000];
        int count = 0;
        fgets(message[0], sizeof(message[0]), stdin);
        while (strcmp(message[count], "\n") != 0) {
            count++;
            fgets(message[count], sizeof(message[count]), stdin);
        }

        // Tries to open a file to store the result
        FILE *file;
        file = fopen("result.ahk", "w");
        if (file == NULL) {
            printf("Unable to create the file.\n");
            return 1;
        }

        // Stores the result and close the file
        fprintf(file, "#NoEnv\nSendMode Input\nSetWorkingDir %%A_ScriptDir%%\n\nF2::\n");
        for (int i = 0; i < count; i++) {
            fprintf(file, "SendInput, ");
            fprintf(file, message[i]);
            fprintf(file, "Sleep, 150\nSend {Enter}\nSleep, 150\n");
        }
        fprintf(file, "return\n\nEsc::ExitApp");
        fclose(file);

        printf("File created (result.ahk)\n");
        return 0;
    }

    // SECOND CASE: a .txt is provided
    else {
        // Opens the .txt provided 
        FILE *fileRead;
        fileRead = fopen(argv[1], "r");

        // Store the contents of the .txt file on an array of strings
        char message[1000][1000];
        int count = 0;
        fgets(message[0], sizeof(message[0]), fileRead);
        while (!feof(fileRead)) {
            count++;
            fgets(message[count], sizeof(message[count]), fileRead);
        }
        strcat(message[count], "\n");
        fclose(fileRead);

        // Extracts the name of the file on the variable filename
        char *filename = (char *)malloc(strlen(argv[1]) + 1);
        extractFilename(argv[1], filename);
        strcat(filename, ".ahk");

        // Opens the file
        FILE *fileWrite;
        fileWrite = fopen(filename, "w");
        free(filename);

        // Stores the result
        fprintf(fileWrite, "#NoEnv\nSendMode Input\nSetWorkingDir %%A_ScriptDir%%\n\nF2::\n");
        for (int i = 0; i <= count; i++) {
            fprintf(fileWrite, "SendInput, ");
            // If there is an empty line it put the `n character, which is a break line
            if (strcmp(message[i], "\n") == 0){
                fprintf(fileWrite, "`n\n");
            }
            else{
                fprintf(fileWrite, message[i]);
            }
            fprintf(fileWrite, "Sleep, 150\nSend {Enter}\nSleep, 150\n");
        }
        fprintf(fileWrite, "return\n\nEsc::ExitApp");
        fclose(fileWrite);

        return 0;
    }
}
