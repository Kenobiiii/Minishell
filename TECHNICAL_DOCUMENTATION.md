# 📚 Documentación Técnica - Minishell

## 📋 Índice

1. [Visión General](#-visión-general)
2. [Arquitectura del Sistema](#-arquitectura-del-sistema)
3. [PARTE 1: PARSEO](#-parte-1-parseo)
4. [PARTE 2: EJECUCIÓN](#-parte-2-ejecución)
5. [Gestión de Memoria](#-gestión-de-memoria)
6. [Manejo de Señales](#-manejo-de-señales)
7. [Casos de Uso Complejos](#-casos-de-uso-complejos)

---

## 🎯 Visión General

Esta minishell es una implementación completa de un intérprete de comandos que reproduce las funcionalidades básicas de bash. El proyecto se divide en dos grandes módulos:

- **PARSEO**: Análisis léxico y sintáctico de comandos
- **EJECUCIÓN**: Interpretación y ejecución del AST generado

### ⚡ Flujo Principal

```
Input → Tokenización → AST → Ejecución → Output
```

### 🏗️ Estructuras de Datos Principales

```c
// Tipos de nodos en el AST
typedef enum e_type {
    CMD,           // Comando
    REDIRECT_OUT,  // Redirección > 
    REDIRECT_IN,   // Redirección <
    REDIN2,        // Heredoc <<
    REDOUT2,       // Append >>
    PIPE           // Pipe |
} t_type;

// Nodo del AST
typedef struct s_ast {
    t_type          type;     // Tipo de nodo
    char            *value;   // Valor (comando/archivo)
    char            **args;   // Argumentos del comando
    struct s_ast    *left;    // Hijo izquierdo
    struct s_ast    *right;   // Hijo derecho
} t_ast;

// Token durante el parseo
typedef struct s_tokens {
    t_type              type;   // Tipo de token
    char                *value; // Valor del token
    struct s_tokens     *next;  // Siguiente token
} t_tokens;

// Estructura principal de datos
typedef struct s_data {
    char        *line;              // Línea de comando leída
    char        *prompt;            // Prompt mostrado
    char        *cwd;              // Directorio actual
    int         exit;              // Flag de salida
    int         wstatus;           // Status de último comando
    t_env       *env;              // Variables de entorno
    t_tokens    *tokens;           // Lista de tokens
    t_ast       *ast;              // Árbol sintáctico abstracto
    int         input_redir_fd;    // FD de redirección entrada
    int         output_redir_fd;   // FD de redirección salida
    int         heredoc_pipe_fd;   // FD de pipe heredoc
} t_data;
```

---

## 🏛️ Arquitectura del Sistema

### 📁 Organización de Archivos

```
src/
├── main.c                    # Función principal y bucle de shell
├── main_utils.c             # Utilidades principales
├── main_checks.c            # Validaciones de entrada
├── minishell.h              # Definiciones y prototipos
├── parse/                   # MÓDULO DE PARSEO
│   ├── ft_tokens.c          # Tokenización
│   ├── ft_ast.c             # Construcción AST
│   ├── ft_ast_utils.c       # Utilidades AST
│   ├── ft_ast_redirects.c   # Manejo redirecciones
│   ├── ft_ast_red_helpers.c # Helpers redirecciones
│   ├── ft_handle_quotes.c   # Procesamiento comillas
│   ├── ft_handle_dollar.c   # Expansión variables
│   └── ft_quotesplit.c      # Split con comillas
├── execution/               # MÓDULO DE EJECUCIÓN
│   ├── exec.c               # Ejecución principal
│   ├── exec_utils.c         # Utilidades ejecución
│   ├── redirections.c       # Gestión redirecciones
│   ├── pipe.c               # Gestión pipes
│   ├── heredoc.c            # Gestión heredocs
│   └── signals.c            # Manejo señales
├── builts_ins/              # COMANDOS BUILT-IN
│   ├── echo_builtin.c
│   ├── cd_builtin.c
│   ├── pwd_builtin.c
│   ├── export_builtin.c
│   ├── unset_builtin.c
│   ├── env_builtin.c
│   └── exit_builtin.c
└── init/                    # INICIALIZACIÓN
    ├── init.c               # Inicialización principal
    └── init_utils.c         # Utilidades inicialización
```

---

## 🔍 PARTE 1: PARSEO

El módulo de parseo convierte la entrada del usuario en un Árbol Sintáctico Abstracto (AST) que puede ser ejecutado.

### 🏁 1.1 Flujo de Parseo

```
Entrada → Validación → Tokenización → Construcción AST → Expansión Variables
```

#### **Función Principal: `line_syntax()`**

```c
int line_syntax(t_data *data)
{
    add_history(data->line);                    // Añadir al historial
    
    if (is_only_empty_var(data->line))         // Verificar variable vacía
        return (free_while(data));
    
    if (!check_line_errors(data))              // Validar sintaxis
        return (0);
    
    ft_tokens(data, data->line);               // TOKENIZAR
    data->ast = ft_build_ast(data, data->tokens); // CONSTRUIR AST
    
    if (data->ast == NULL)
        return (free_while(data));
    return (1);
}
```

### 🔤 1.2 Tokenización (`ft_tokens.c`)

La tokenización divide la línea de entrada en tokens individuales.

#### **Proceso de Tokenización**

1. **Split con awareness de comillas**: `ft_quotesplit()`
2. **Identificación de tipos**: `ft_types()`
3. **Creación de tokens**: `ft_new_token()`

```c
int ft_tokens(t_data *data, char *str)
{
    char        **array;
    t_tokens    *current;
    int         i;

    // 1. Split respetando comillas
    array = ft_quotesplit(str, ' ', data);
    
    // 2. Procesar comillas en cada elemento
    ft_process_all_quotes(data, array);
    
    // 3. Crear lista de tokens
    data->tokens = ft_new_token(array[0]);
    current = data->tokens;
    
    i = 1;
    while (array[i])
    {
        current->next = ft_new_token(array[i]);
        current = current->next;
        i++;
    }
    
    free_matrix(array);
    return (1);
}
```

#### **Identificación de Tipos de Token**

```c
int ft_types(char *value)
{
    if (ft_strncmp(value, "<<", 2) == 0)    return (REDIN2);     // Heredoc
    if (ft_strncmp(value, ">>", 2) == 0)    return (REDOUT2);    // Append
    if (ft_strncmp(value, "<", 1) == 0)     return (REDIRECT_IN); // Input
    if (ft_strncmp(value, ">", 1) == 0)     return (REDIRECT_OUT);// Output
    if (ft_strncmp(value, "|", 1) == 0)     return (PIPE);       // Pipe
    return (CMD);                                                // Comando
}
```

### 🌳 1.3 Construcción del AST (`ft_ast.c`)

El AST se construye procesando cada token de la lista.

#### **Función Principal: `ft_build_ast()`**

```c
t_ast *ft_build_ast(t_data *data, t_tokens *tokens)
{
    t_ast       *root;
    t_ast       *cmd;
    t_ast       *last_op;
    t_ast_args  args;

    // Inicializar variables del AST
    init_ast_vars(&root, &cmd, &last_op, &args);
    
    // Procesar cada token
    while (tokens && parse_success)
    {
        args.tokens = tokens;
        parse_success = process_token(data, &args);
        if (parse_success)
            tokens = tokens->next;
    }
    
    return (root);
}
```

#### **Procesamiento de Tokens**

```c
int process_token(t_data *data, t_ast_args *args)
{
    if (args->tokens->type == CMD)
    {
        // Manejar nodo comando
        ft_handle_command_node(args->root, args->cmd, 
                              args->last_op, args->tokens);
        return (1);
    }
    else
    {
        // Manejar nodo operador (pipe, redirección)
        return (ft_handle_operator_node(data, args));
    }
}
```

### 🎯 1.4 Manejo de Comandos

#### **Creación de Nodo Comando**

```c
void ft_handle_command_node(t_ast **root, t_ast **cmd,
                           t_ast **last_op, t_tokens *tokens)
{
    if (is_redirection_operator(last_op))
    {
        // Si hay operador de redirección pendiente
        handle_redirection(root, cmd, last_op, tokens);
    }
    else if (*cmd)
    {
        // Añadir como argumento a comando existente
        if ((*cmd)->type == CMD)
            ft_add_argument(*cmd, tokens->value);
    }
    else
    {
        // Crear nuevo comando
        handle_new_command(root, cmd, last_op, tokens);
    }
}
```

### 🔄 1.5 Manejo de Redirecciones (`ft_ast_red_helpers.c`)

Las redirecciones requieren tratamiento especial en el AST.

#### **Creación de Nodo de Redirección**

```c
t_ast *create_redirection_node(t_ast **cmd, t_ast **last_op,
                              t_tokens *tokens)
{
    t_ast *new_node;

    // Crear nodo para el archivo/comando
    new_node = ft_create_ast_node(CMD, tokens->value);
    
    if (is_heredoc_operator(last_op))
        handle_heredoc_special_case(cmd, last_op, new_node, tokens);
    else
        (*last_op)->right = new_node;  // Conectar a la derecha
    
    return (new_node);
}
```

#### **Finalización de AST de Redirecciones**

```c
void finalize_redirection_ast(t_ast **root, t_ast **cmd,
                             t_ast **last_op, t_finalize_params *params)
{
    // Conectar comando original si es necesario
    if (!(*last_op)->left && params->original_cmd)
        (*last_op)->left = params->original_cmd;
    
    // Establecer como root si es necesario
    if (!*root)
        *root = *last_op;
    
    // Preservar pipes en la cadena
    t_ast *pipe_to_preserve = NULL;
    t_ast *current = *root;
    
    while (current && current->right)
    {
        if (current->type == PIPE)
            pipe_to_preserve = current;
        else if (current->right->type == PIPE)
            pipe_to_preserve = current->right;
        current = current->right;
    }
    
    *last_op = pipe_to_preserve;
}
```

### 📝 1.6 Manejo de Comillas (`ft_handle_quotes.c`)

#### **Procesamiento de Comillas**

```c
void ft_process_all_quotes(t_data *data, char **matrix)
{
    int count_x = 0;
    
    // Contar elementos en la matriz
    while (matrix[count_x])
        count_x++;
    
    // Procesar comillas en cada elemento
    ft_find_quotes_in_line(data, matrix, count_x);
}
```

#### **Eliminación de Delimitadores**

```c
char *remove_quote_delimiters(char *line, char quote_char)
{
    int     content_len;
    char    *result;
    int     i, j;

    // Calcular longitud del contenido
    content_len = calculate_quote_content_length(line, quote_char);
    
    result = malloc(content_len + 1);
    
    // Copiar contenido sin comillas
    i = 1;  // Saltar primera comilla
    j = 0;
    while (line[i] && line[i] != quote_char)
    {
        result[j] = line[i];
        i++;
        j++;
    }
    result[j] = '\0';
    
    return (result);
}
```

### 💰 1.7 Expansión de Variables (`ft_handle_dollar.c`)

#### **Expansión de Variables Dollar**

```c
char *expand_dollar_variables(t_data *data, char *line)
{
    int     expanded_size;
    char    *result;
    int     i, j;

    // Calcular tamaño expandido
    expanded_size = calculate_expanded_size(data, line);
    result = malloc(expanded_size + 1);
    
    i = 0;
    j = 0;
    while (line[i])
    {
        if (line[i] == '$')
        {
            if (line[i + 1] == '?')
                handle_exit_status(data, result, &j);
            else
            {
                char *var = extract_var(line, &i);
                char *value = get_env_value(data, var);
                if (value)
                {
                    ft_strlcpy(result + j, value, ft_strlen(value) + 1);
                    j += ft_strlen(value);
                }
                free(var);
            }
        }
        else
            result[j++] = line[i];
        i++;
    }
    
    result[j] = '\0';
    return (result);
}
```

---

## ⚡ PARTE 2: EJECUCIÓN

El módulo de ejecución interpreta el AST generado y ejecuta los comandos.

### 🚀 2.1 Flujo de Ejecución

```
AST → Análisis Tipo → Configuración → Ejecución → Limpieza
```

#### **Función Principal: `exec_ast()`**

```c
void exec_ast(t_data *data, t_ast *node)
{
    if (node->type == CMD)
        exec_command_node(data, node);           // Ejecutar comando
    else if (node->type == REDIRECT_OUT)
        exec_redirect_out(data, node);           // Redirección salida
    else if (node->type == REDIRECT_IN)
        exec_redirect_in(data, node);            // Redirección entrada
    else if (node->type == REDIN2)
        exec_heredoc(data, node);                // Heredoc
    else if (node->type == REDOUT2)
        exec_redirect_append(data, node);        // Append
    else if (node->type == PIPE)
        exec_pipe(data, node);                   // Pipe
}
```

### 🎯 2.2 Ejecución de Comandos

#### **Comando Simple**

```c
static void exec_command_node(t_data *data, t_ast *node)
{
    t_ast *original_ast;

    // Expandir variables en tiempo de ejecución
    expand_command_variables(data, node);
    
    if (is_builtin_command(node->value))
    {
        // Ejecutar built-in
        original_ast = data->ast;
        data->ast = node;
        execute_builtin_with_redirections(data);
        data->ast = original_ast;
    }
    else
    {
        // Ejecutar comando externo
        exec_simple_cmd(data, node);
    }
}
```

#### **Comando Externo**

```c
void exec_simple_cmd(t_data *data, t_ast *node)
{
    char    *path;
    pid_t   pid;

    // Obtener path del comando
    path = get_cmd_path(data, node->value);
    
    pid = fork();
    if (pid == 0)
    {
        // Proceso hijo
        configure_signals(1);
        setup_child_redirections(data);
        
        if (!path)
            exit_minishell(data, node->value, 127);
            
        if (execve(path, node->args, 
                   (char **)list_to_array(data->env)) == -1)
        {
            free(path);
            exit_minishell(data, "Error in execve", 126);
        }
    }
    else if (pid > 0)
    {
        // Proceso padre
        close_redirection_fds(data);
        waitpid(pid, &data->wstatus, 0);
        analyse_status(data);
        free(path);
    }
}
```

### 🔄 2.3 Gestión de Pipes (`pipe.c`)

#### **Ejecución de Pipe**

```c
void exec_pipe(t_data *data, t_ast *node)
{
    int     pipefd[2];
    pid_t   pid1, pid2;

    if (pipe(pipefd) < 0)
    {
        perror("pipe");
        return;
    }

    // Proceso izquierdo (escritor)
    pid1 = fork();
    if (pid1 == 0)
    {
        close(pipefd[0]);                    // Cerrar lectura
        dup2(pipefd[1], STDOUT_FILENO);      // Redirigir stdout
        close(pipefd[1]);
        exec_ast(data, node->left);          // Ejecutar comando izquierdo
        exit(0);
    }

    // Proceso derecho (lector)
    pid2 = fork();
    if (pid2 == 0)
    {
        close(pipefd[1]);                    // Cerrar escritura
        dup2(pipefd[0], STDIN_FILENO);       // Redirigir stdin
        close(pipefd[0]);
        exec_ast(data, node->right);         // Ejecutar comando derecho
        exit(0);
    }

    // Proceso padre
    close(pipefd[0]);
    close(pipefd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, &data->wstatus, 0);
    analyse_status(data);
}
```

### 📄 2.4 Gestión de Redirecciones

#### **Redirección de Salida**

```c
void exec_redirect_out(t_data *data, t_ast *node)
{
    int fd;
    
    // Abrir archivo para escritura
    fd = open(node->right->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0)
    {
        perror("open");
        data->wstatus = 1;
        return;
    }
    
    // Configurar redirección
    data->output_redir_fd = fd;
    
    // Ejecutar comando izquierdo
    if (node->left)
        exec_ast(data, node->left);
        
    // Limpiar
    close(fd);
    data->output_redir_fd = -1;
}
```

#### **Redirección de Entrada**

```c
void exec_redirect_in(t_data *data, t_ast *node)
{
    int fd;
    
    // Abrir archivo para lectura
    fd = open(node->right->value, O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        data->wstatus = 1;
        return;
    }
    
    // Configurar redirección
    data->input_redir_fd = fd;
    
    // Ejecutar comando izquierdo
    if (node->left)
        exec_ast(data, node->left);
        
    // Limpiar
    close(fd);
    data->input_redir_fd = -1;
}
```

### 📋 2.5 Heredocs (`heredoc.c`)

#### **Ejecución de Heredoc**

```c
void exec_heredoc(t_data *data, t_ast *node)
{
    t_ast   *heredocs[10];
    int     count = 0;
    t_ast   *cmd_node;

    // Recopilar cadena de heredocs
    collect_heredoc_chain(node, heredocs, &count);
    
    // Procesar todos los heredocs
    process_heredoc_loop(data, heredocs, count);
    
    // Encontrar nodo comando para ejecutar
    cmd_node = node;
    while (cmd_node && cmd_node->type == REDIN2)
        cmd_node = cmd_node->left;
    
    if (cmd_node)
        exec_ast(data, cmd_node);
}
```

#### **Lectura de Heredoc**

```c
int read_heredoc_lines(int pipefd, char *delim)
{
    char *line;
    
    while (1)
    {
        line = readline("> ");
        if (!line)
            return (handle_eof_case(delim));
            
        if (g_shell_state == STATE_HEREDOC_INTERRUPTED)
        {
            free(line);
            return (-1);
        }
        
        if (ft_strncmp(line, delim, ft_strlen(delim)) == 0
            && ft_strlen(line) == ft_strlen(delim))
        {
            free(line);
            break;
        }
        
        if (write_heredoc_line(pipefd, line) == -1)
        {
            free(line);
            return (-1);
        }
        free(line);
    }
    
    return (0);
}
```

### 🔧 2.6 Built-ins

#### **Identificación y Ejecución**

```c
int is_builtin_command(char *cmd)
{
    if (ft_strncmp(cmd, "echo", 5) == 0)    return (1);
    if (ft_strncmp(cmd, "cd", 3) == 0)      return (1);
    if (ft_strncmp(cmd, "pwd", 4) == 0)     return (1);
    if (ft_strncmp(cmd, "export", 7) == 0)  return (1);
    if (ft_strncmp(cmd, "unset", 6) == 0)   return (1);
    if (ft_strncmp(cmd, "env", 4) == 0)     return (1);
    if (ft_strncmp(cmd, "exit", 5) == 0)    return (1);
    return (0);
}
```

#### **Ejemplo: Echo Built-in**

```c
int echo_builtin(t_data *data)
{
    int     i;
    int     newline;

    // Verificar flag -n
    i = 1;
    newline = 1;
    if (data->ast->args[1] && ft_strncmp(data->ast->args[1], "-n", 3) == 0)
    {
        newline = 0;
        i = 2;
    }

    // Imprimir argumentos
    while (data->ast->args[i])
    {
        printf("%s", data->ast->args[i]);
        if (data->ast->args[i + 1])
            printf(" ");
        i++;
    }

    if (newline)
        printf("\n");
        
    data->wstatus = 0;
    return (0);
}
```

---

## 🧠 Gestión de Memoria

### 🔄 Limpieza Automática

#### **Liberación Durante Ejecución**

```c
int free_while(t_data *data)
{
    if (data->line)
    {
        free(data->line);
        data->line = NULL;
    }
    if (data->tokens)
    {
        ft_free_tokens(data->tokens);
        data->tokens = NULL;
    }
    if (data->ast)
    {
        ft_free_ast(data->ast);
        data->ast = NULL;
    }
    close_redirection_fds(data);
    return (0);
}
```

#### **Liberación de AST**

```c
void ft_free_ast(t_ast *ast)
{
    if (!ast)
        return;
        
    if (ast->left)
        ft_free_ast(ast->left);     // Recursivo izquierda
    if (ast->right)
        ft_free_ast(ast->right);    // Recursivo derecha
        
    if (ast->value)
        free(ast->value);
    if (ast->args)
        free_matrix(ast->args);
        
    free(ast);
}
```

#### **Limpieza de Nodos No Utilizados**

```c
void cleanup_unused_node(t_ast *new_node, t_ast **last_op)
{
    if (new_node && !is_heredoc_operator(last_op)
        && (*last_op)->right != new_node)
    {
        if (new_node->value)
            free(new_node->value);
        if (new_node->args)
            free_matrix(new_node->args);
        free(new_node);
    }
}
```

---

## 📡 Manejo de Señales

### 🎯 Estados de Señales

```c
#define STATE_PROMPT_NORMAL         0   // En prompt normal
#define STATE_PROMPT_INTERRUPTED    1   // Prompt interrumpido
#define STATE_EXECUTING             2   // Ejecutando comando
#define STATE_EXECUTION_INTERRUPTED 3   // Ejecución interrumpida
#define STATE_HEREDOC_INTERRUPTED   4   // Heredoc interrumpido
```

### ⚡ Configuración de Señales

#### **Modo Prompt**

```c
void setup_signals(void)
{
    signal(SIGINT, handle_sigint);   // Ctrl+C
    signal(SIGQUIT, SIG_IGN);        // Ctrl+\ ignorado
}
```

#### **Modo Ejecución**

```c
void configure_signals(int mode)
{
    if (mode == 0)  // Modo ejecución
    {
        signal(SIGINT, SIG_DFL);     // Comportamiento por defecto
        signal(SIGQUIT, SIG_DFL);    // Comportamiento por defecto
    }
    else            // Modo hijo
    {
        signal(SIGINT, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
    }
}
```

#### **Manejador SIGINT**

```c
void handle_sigint(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    
    if (g_shell_state == STATE_PROMPT_NORMAL
        || g_shell_state == STATE_PROMPT_INTERRUPTED)
    {
        g_shell_state = STATE_PROMPT_INTERRUPTED;
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
    else if (g_shell_state == STATE_EXECUTING)
    {
        g_shell_state = STATE_EXECUTION_INTERRUPTED;
    }
}
```

---

## 🔥 Casos de Uso Complejos

### 💎 Ejemplo 1: Pipe con Redirecciones

**Comando**: `ls | grep .c > output.txt`

#### **AST Generado**:
```
        PIPE
       /    \
     CMD     REDIRECT_OUT
    (ls)     /          \
           CMD          CMD
        (grep .c)   (output.txt)
```

#### **Flujo de Ejecución**:
1. `exec_pipe()` detecta nodo PIPE
2. Fork proceso izquierdo: ejecuta `ls`
3. Fork proceso derecho: ejecuta redirección
4. `exec_redirect_out()` abre `output.txt`
5. Ejecuta `grep .c` con stdout redirigido

### 🎭 Ejemplo 2: Heredoc con Pipe

**Comando**: `cat << EOF | grep hello`

#### **AST Generado**:
```
        PIPE
       /    \
    REDIN2  CMD
    /    \  (grep hello)
  CMD    CMD
 (cat)  (EOF)
```

#### **Flujo de Ejecución**:
1. `exec_pipe()` detecta nodo PIPE
2. Proceso izquierdo: `exec_heredoc()`
3. Lee líneas hasta "EOF"
4. Crea pipe interno para heredoc
5. `cat` lee del pipe interno
6. Proceso derecho: `grep hello` lee del pipe principal

### 🌟 Ejemplo 3: Expansión de Variables

**Comando**: `echo "Hello $USER, you are in $PWD"`

#### **Proceso de Parseo**:
1. **Tokenización**: `["echo", "Hello $USER, you are in $PWD"]`
2. **Procesamiento comillas**: Detecta comillas dobles
3. **Expansión variables**: 
   - `$USER` → valor de variable USER
   - `$PWD` → directorio actual
4. **Resultado**: `echo "Hello john, you are in /home/john"`

### 🔧 Ejemplo 4: Múltiples Redirecciones

**Comando**: `echo "test" > file1.txt >> file2.txt`

#### **Procesamiento**:
1. Detecta múltiples redirecciones de salida
2. `process_multiple_output_redirections()` maneja la secuencia
3. Aplica la **última redirección válida** (append a file2.txt)
4. Ejecuta comando con stdout redirigido

---

## 🎯 Flujo Completo de Ejemplo

### Comando: `echo "Hello World" | wc -l > output.txt`

#### **1. Parseo**
```
Input: echo "Hello World" | wc -l > output.txt

Tokens: [echo] ["Hello World"] [|] [wc] [-l] [>] [output.txt]

AST:
              PIPE
             /    \
           CMD     REDIRECT_OUT
       (echo)      /          \
         |       CMD          CMD
    ["Hello      (wc)      (output.txt)
     World"]       |
                 [-l]
```

#### **2. Ejecución**
```
1. exec_ast() → detecta PIPE
2. exec_pipe():
   a. Fork proceso izquierdo:
      - exec_ast(echo_node)
      - exec_command_node() → es built-in
      - echo_builtin() → imprime "Hello World"
   
   b. Fork proceso derecho:
      - exec_ast(redirect_node)
      - exec_redirect_out():
        * Abre output.txt
        * exec_ast(wc_node)
        * exec_simple_cmd(wc -l)

3. Resultado: "1" escrito en output.txt
```

---

## 🏆 Puntos Clave de la Implementación

### ✅ **Fortalezas del Diseño**

1. **Modularidad**: Parseo y ejecución completamente separados
2. **AST Bien Estructurado**: Representa fielmente la sintaxis
3. **Gestión de Memoria**: Limpieza automática y manual
4. **Manejo de Señales**: Estados bien definidos
5. **Expansión Variables**: En tiempo de ejecución para máxima compatibilidad

### 🔍 **Aspectos Técnicos Destacados**

1. **Quote-Aware Splitting**: Respeta comillas durante tokenización
2. **Heredoc Chaining**: Maneja múltiples heredocs concatenados
3. **Redirection Finalization**: Preserva pipes en cadenas complejas
4. **Memory Cleanup**: Evita leaks con limpieza sistemática
5. **Signal State Management**: Estados globales para comportamiento correcto

### 🎯 **Casos Edge Manejados**

- Comillas anidadas y mixtas
- Variables vacías y no definidas
- Redirecciones múltiples
- Pipes con redirecciones
- Heredocs interrumpidos
- Comandos no encontrados
- Permisos de archivos

---

## 📚 Conclusión

Esta minishell implementa una arquitectura robusta que separa claramente el análisis sintáctico de la ejecución. El uso del AST como representación intermedia permite manejar comandos complejos de forma estructurada y mantenible.

La gestión cuidadosa de memoria, el manejo correcto de señales y la implementación completa de redirecciones y pipes hacen de esta minishell una implementación completa y funcional que reproduce fielmente el comportamiento de bash en la mayoría de casos de uso.

---

*Documentación generada para el proyecto Minishell - 42 School*  
*Autores: paromero & anggalle*  
*Fecha: Junio 2025*
