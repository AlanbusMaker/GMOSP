#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

int run_command(const char* cmd) {
    int ret = system(cmd);
    if (ret != 0) {
        printf("Falha ao executar: %s\n", cmd);
        return 0;
    }
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

int has_mak_extension(const char* filename) {
    const char* ext = strrchr(filename, '.');
    if (!ext) return 0;
    return strcasecmp(ext, ".mak") == 0;
}

int main(int argc, char* argv[]) {
    char filename[512] = "";

    if (argc < 2) {
        strcpy(filename, "GrinnMake.mak");
    } else {
        strcpy(filename, argv[1]);
    }

    if (!has_mak_extension(filename)) {
        printf("Erro: só arquivos .mak podem ser lidos!\n");
        return 1;
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Não foi possível abrir %s\n", filename);
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

            printf("[GrinnMake] Executando: %s\n", final_cmd);
            run_command(final_cmd);
        }
    }

    fclose(file);
    return 0;
}
