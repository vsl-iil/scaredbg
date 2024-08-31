#include <Windows.h>
#include <stdio.h>

char* SCARECROWS[] = {
    "windbg.exe",
    "ollydbg.exe",
    "x64dbg.exe",
    "x32dbg.exe",
};

size_t SCARECROWS_SIZE = sizeof(SCARECROWS)/sizeof(char*);

BOOL RunScarecrow(char* processName, char* programPath);

int main(int argc, char *argv[]) {
    if (argc > 1) {
        //
        // === START ===
        //
        if (!strcmp(argv[1], "start")) {
            if (argc < 3) {
                fprintf(stderr, "[-] You have not specified the scarecrow process to start.");
                return -1;
            } else {
                BOOL started = FALSE;
                for (int i = 0; i < SCARECROWS_SIZE; i++) {
                    if (!strcmp(argv[2], SCARECROWS[i])) {
                        started = RunScarecrow(argv[2], argv[0]);
                        break;
                    }
                }

                if (!started) {
                    fprintf(stderr, "[-] Please, specify a process from the following list: ");
                    for (int i = 0; i < SCARECROWS_SIZE; i++) {
                        fprintf(stderr, "%s; ", SCARECROWS[i]);
                    }
                    fprintf(stderr, "\n");
                    return -1;
                }
            }
        //
        // === KILLALL ===
        //
        // } else if (!strcmp(argv[1], "killall")) {
            // kill all dbg-processes
        //
        // HELP
        //
        } else if (!strcmp(argv[1], "help")) {
            printf("Usage:\n");
            printf("scaredbg.exe\n\twithout arguments - start all scarecrows.\n");
            printf("Arguments:\n");

            printf("\tstart <scarecrow> - start one of scarecrows. Possible values: ");
            for (int i = 0; i < SCARECROWS_SIZE; i++) {
                printf("%s; ", SCARECROWS[i]);
            }
            printf("\n");

            // printf("\tkillall - kill all scarecrows currently running\n");
            printf("\thelp - show this message\n");
        }

        return 0;
    }

    for (int i = 0; i < SCARECROWS_SIZE; i++) {
        char fpath[MAX_PATH];

        strcpy_s(fpath, sizeof(fpath), argv[0]);
        char* fname;
        char* context;
        for (char* p = strtok_s(fpath, "\\", &context); p != NULL; p = strtok_s(NULL, "\\", &context)) {
            fname = p;
        }

        if (!strcmp(SCARECROWS[i], fname)) {
            // бесконечный цикл
            // сюда можно поместить какую-нибудь затрачивающую ОЗУ и 
            // процессорное время логику для большей реалистичности
            while(1) { Sleep(10000); }
        } 
    }

    for (int i = 0; i < SCARECROWS_SIZE; i++) {
        RunScarecrow(SCARECROWS[i], argv[0]);
    }
}


BOOL RunScarecrow(char* processName, char* programPath) {
        printf("[+] Starting %s process...\n", processName);

        if (!CopyFile(programPath, processName, FALSE)) {
            fprintf(stderr, "[-] Unable to create executable \"%s\": %lu\n", processName, GetLastError());
            return FALSE;
        }

        STARTUPINFO si;
        PROCESS_INFORMATION pi;

        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));

        if (!CreateProcess(
                processName,
                NULL,
                NULL,
                NULL,
                FALSE,
                DETACHED_PROCESS,
                NULL,
                NULL, // TODO: подменять адрес запуска на C:\Program Files...
                &si,
                &pi
        )) {
            fprintf(stderr, "[-] Unable to start process %s: %lu\n", processName, GetLastError());
            return FALSE;
        }

        return TRUE;
}

// BOOL KillScarecrow(char* processName) {
//     HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, )
// }