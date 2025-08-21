#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int run_command(const char* cmd) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(NULL, (LPSTR)cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Falha ao executar: %s\n", cmd);
        return 0;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return 1;
}

void extract_braces(const char* src, char* dest) {
    const char* start = strchr(src, '{');
    const char* end = strchr(src, '}');
    if (start && end && end > start) {
        size_t len = end - start - 1;
        strncpy(dest, start + 1, len);
        dest[len] = '\0';
    } else {
        dest[0] = '\0';
    }
}

// Verifica se o arquivo termina com ".mak"
int has_mak_extension(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return 0;
    return _stricmp(ext, ".mak") == 0; // compara ignorando maiúsculas/minúsculas
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: grinnmake.exe <arquivo.grinn-make.mak>\n");
        return 1;
    }

    if (!has_mak_extension(argv[1])) {
        printf("Erro: só arquivos .mak podem ser lidos!\n");
        return 1;
    }

    FILE* file = fopen(argv[1], "r");
    if (!file) {
        printf("Não foi possível abrir %s\n", argv[1]);
        return 1;
    }

    char line[MAX_LINE];
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "USE CMD") != NULL) {
            char command[512] = "";
            char file_arg[512] = "";
            char special[512] = "";

            extract_braces(line, command);

            char* file_pos = strstr(line, "FILE");
            if (file_pos) extract_braces(file_pos, file_arg);

            char* special_pos = strstr(line, "SPECIAL ARGUMENTS");
            if (special_pos) extract_braces(special_pos, special);

            char final_cmd[1024];
            snprintf(final_cmd, sizeof(final_cmd), "%s %s %s", command, file_arg, special);

            printf("Executando: %s\n", final_cmd);
            run_command(final_cmd);
        }
    }

    fclose(file);
    return 0;
}
