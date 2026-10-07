#define _POSIX_C_SOURCE 200809L
#include "main.h"

char *builtin_commands[] = {
    "echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs",
    "let", "eval", "set", "unset", "export", "declare", "typeset",
    "readonly", "getopts", "source", "exit", "exec", "shopt", "caller",
    "true", "type", "hash", "bind", "help", NULL
};

static void print_prompt(void)
{
    char cwd[1024];

    if (getcwd(cwd, sizeof(cwd)) != NULL)
        printf(ANSI_COLOR_GREEN "minishell:%s$ " ANSI_COLOR_RESET, cwd);
    else
        printf("minishell$ ");

    fflush(stdout);
}

static int is_builtin(const char *command)
{
    for (int i = 0; builtin_commands[i] != NULL; i++)
        if (strcmp(command, builtin_commands[i]) == 0)
            return 1;
    return 0;
}

static void builtin_help(void)
{
    printf("Built-in commands:\n");
    for (int i = 0; builtin_commands[i] != NULL; i++)
        printf("  %s\n", builtin_commands[i]);
}

static int run_builtin(char **argv)
{
    if (strcmp(argv[0], "exit") == 0)
        exit(0);

    if (strcmp(argv[0], "cd") == 0) {
        const char *path = argv[1] ? argv[1] : getenv("HOME");
        if (chdir(path) != 0)
            perror("cd");
        return 0;
    }

    if (strcmp(argv[0], "pwd") == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)
            printf("%s\n", cwd);
        else
            perror("pwd");
        return 0;
    }

    if (strcmp(argv[0], "echo") == 0) {
        for (int i = 1; argv[i] != NULL; i++) {
            if (i > 1) putchar(' ');
            fputs(argv[i], stdout);
        }
        putchar('\n');
        return 0;
    }

    if (strcmp(argv[0], "help") == 0) {
        builtin_help();
        return 0;
    }

    if (strcmp(argv[0], "true") == 0)
        return 0;

    if (strcmp(argv[0], "false") == 0)
        return 1;

    if (strcmp(argv[0], "export") == 0) {
        if (!argv[1]) {
            fprintf(stderr, "usage: export NAME=VALUE\n");
            return 1;
        }
        char *eq = strchr(argv[1], '=');
        if (!eq) {
            fprintf(stderr, "usage: export NAME=VALUE\n");
            return 1;
        }
        *eq = '\0';
        if (setenv(argv[1], eq + 1, 1) != 0)
            perror("export");
        return 0;
    }

    if (strcmp(argv[0], "unset") == 0) {
        if (!argv[1]) {
            fprintf(stderr, "usage: unset NAME\n");
            return 1;
        }
        if (unsetenv(argv[1]) != 0)
            perror("unset");
        return 0;
    }

    if (strcmp(argv[0], "type") == 0) {
        if (!argv[1]) {
            fprintf(stderr, "usage: type COMMAND\n");
            return 1;
        }
        if (is_builtin(argv[1])) {
            printf("%s is a shell builtin\n", argv[1]);
            return 0;
        }
        return 1;
    }

    if (strcmp(argv[0], "printf") == 0) {
        if (argv[1])
            printf("%s", argv[1]);
        for (int i = 2; argv[i]; i++)
            printf(" %s", argv[i]);
        return 0;
    }

    fprintf(stderr, "%s: builtin recognized but not implemented in this version\n", argv[0]);
    return 1;
}

static int parse_line(char *line, char **argv, int max_args)
{
    int argc = 0;
    char *token = strtok(line, " \t");

    while (token != NULL && argc < max_args - 1) {
        argv[argc++] = token;
        token = strtok(NULL, " \t");
    }

    argv[argc] = NULL;
    return argc;
}

static void execute_external(char **argv)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return;
    }

    if (pid == 0) {
        signal(SIGINT, SIG_DFL);
        execvp(argv[0], argv);
        fprintf(stderr, "%s: command not found or could not execute\n", argv[0]);
        _exit(127);
    }

    int status;
    if (waitpid(pid, &status, 0) < 0)
        perror("waitpid");
}

void signal_handler(int sig_num)
{
    (void)sig_num;
    write(STDOUT_FILENO, "\n", 1);
}

int main(void)
{
    char line[MAX_INPUT];
    char *argv[MAX_ARGS];

    signal(SIGINT, signal_handler);
    signal(SIGQUIT, SIG_IGN);

    while (1) {
        print_prompt();

        if (fgets(line, sizeof(line), stdin) == NULL) {
            putchar('\n');
            break;
        }

        line[strcspn(line, "\n")] = '\0';

        if (line[0] == '\0')
            continue;

        int argc = parse_line(line, argv, MAX_ARGS);
        if (argc == 0)
            continue;

        if (is_builtin(argv[0]))
            run_builtin(argv);
        else
            execute_external(argv);
    }

    return 0;
}
