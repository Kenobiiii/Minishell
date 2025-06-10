# 🌳 Guía Visual del Recorrido AST - Minishell

## 📖 Introducción

Este documento presenta de manera gráfica cómo el **Árbol Sintáctico Abstracto (AST)** recorre y ejecuta diferentes tipos de comandos en minishell. Cada ejemplo muestra:

1. **Comando de entrada**
2. **Estructura del AST generado**
3. **Flujo de ejecución paso a paso**
4. **Llamadas a funciones principales**

---

## 🔧 Tipos de Nodos AST

```c
typedef enum e_type {
    CMD,           // Comando simple
    REDIRECT_OUT,  // Redirección de salida >
    REDIRECT_IN,   // Redirección de entrada <
    REDIN2,        // Here-document <<
    REDOUT2,       // Append >>
    PIPE           // Pipe |
} t_type;

typedef struct s_ast {
    t_type          type;     // Tipo de nodo
    char            *value;   // Valor del comando/operador
    char            **args;   // Argumentos (solo para CMD)
    struct s_ast    *left;    // Hijo izquierdo
    struct s_ast    *right;   // Hijo derecho
} t_ast;
```

---

## 🏗️ Proceso de Construcción del AST

### 📋 Variables de Estado durante la Construcción

Durante la construcción del AST, se mantienen **3 punteros principales**:

```c
t_ast *root;     // Raíz del AST (nodo principal)
t_ast *cmd;      // Comando actual en construcción
t_ast *last_op;  // Último operador procesado
```

### 🔄 Flujo de Construcción: `ft_build_ast()`

#### Paso 1: Inicialización
```c
t_ast *root = NULL;      // Sin raíz inicialmente
t_ast *cmd = NULL;       // Sin comando actual
t_ast *last_op = NULL;   // Sin operador previo
```

#### Paso 2: Procesamiento Token por Token
```c
while (tokens && parse_success) {
    if (token->type == CMD)
        ft_handle_command_node();  // Procesar comando
    else
        ft_handle_operator_node();  // Procesar operador
    tokens = tokens->next;
}
```

---

## 🎯 Asignación Detallada por Tipo de Token

### 1️⃣ **Token CMD (Comando)**

#### Función: `ft_handle_command_node()`

##### Caso A: **Primer Comando**
```c
Entrada: tokens = ["echo", "hello", "|", "cat"]
Estado inicial: root=NULL, cmd=NULL, last_op=NULL

Token: "echo"
```

**Proceso de asignación:**
```c
1. handle_new_command() se ejecuta porque cmd == NULL
2. new_cmd = ft_create_ast_node(CMD, "echo")
3. new_cmd->args = ["echo", NULL]
4. cmd = new_cmd
5. root = cmd  // Primer nodo se convierte en raíz

Estado: root=echo_node, cmd=echo_node, last_op=NULL
```

##### Caso B: **Argumento de Comando Existente**
```c
Token: "hello" (continuando del ejemplo anterior)
```

**Proceso de asignación:**
```c
1. cmd != NULL y cmd->type == CMD
2. ft_add_argument(cmd, "hello")
3. cmd->args = ["echo", "hello", NULL]

Estado: root=echo_node, cmd=echo_node, last_op=NULL
```

##### Caso C: **Comando tras Operador de Redirección**
```c
Ejemplo: echo hello > output.txt
Token: "output.txt" (tras haber procesado ">")
```

**Proceso de asignación:**
```c
1. is_redirection_operator(last_op) == true
2. handle_redirection() se ejecuta
3. Se crea nodo CMD para "output.txt"
4. last_op->right = nuevo_nodo_archivo

Estado: last_op apunta al nodo de redirección
```

---

### 2️⃣ **Token OPERADOR (Pipe, Redirecciones)**

#### Función: `ft_handle_operator_node()`

##### Caso A: **Primer Pipe**
```c
Ejemplo: echo hello | cat
Estado antes: root=echo_node, cmd=echo_node, last_op=NULL
Token: "|"
```

**Proceso de asignación:**
```c
1. new_op = ft_create_ast_node(PIPE, "|")
2. handle_new_node() se ejecuta:
   a. new_op->left = cmd  // echo_node va a la izquierda
   b. root = new_op       // pipe se convierte en nueva raíz
   c. last_op = new_op    // pipe es el último operador
   d. cmd = NULL          // resetear para siguiente comando

Estado final: root=pipe_node, cmd=NULL, last_op=pipe_node
```

**Estructura resultante:**
```
    PIPE (root)
   /
echo_node
```

##### Caso B: **Pipe Anidado (Múltiples Pipes)**
```c
Ejemplo: echo hello | grep h | wc -l
Estado antes: Primer pipe ya procesado, grep ya asignado
Token: "|" (segundo pipe)
```

**Proceso de asignación:**
```c
1. new_op = ft_create_ast_node(PIPE, "|")
2. connect_operator_to_ast():
   a. new_op->left = cmd          // grep_node va a la izquierda
   b. last_op->right = new_op     // conectar al pipe anterior
   c. last_op = new_op            // actualizar último operador
   d. cmd = NULL                  // resetear

Estado final: Pipe anidado creado
```

**Estructura resultante:**
```
        PIPE (root)
       /    \
   echo     PIPE (new_op)
           /
        grep
```

##### Caso C: **Redirección de Salida**
```c
Ejemplo: echo hello > output.txt
Estado antes: root=echo_node, cmd=echo_node, last_op=NULL
Token: ">"
```

**Proceso de asignación:**
```c
1. new_op = ft_create_ast_node(REDIRECT_OUT, ">")
2. connect_operator_to_ast():
   a. new_op->left = cmd     // echo_node va a la izquierda
   b. root = new_op          // redirección se convierte en raíz
   c. last_op = new_op       // redirección es último operador
   d. cmd permanece activo   // para redirecciones

Estado final: root=redirect_node, cmd=echo_node, last_op=redirect_node
```

**Estructura resultante:**
```
  REDIRECT_OUT (root)
 /
echo_node
```

---

## 🔗 Algoritmos de Conexión Específicos

### 🔄 **connect_operator_to_ast()** - Conexión de Operadores

Esta función es **crucial** para mantener la precedencia correcta:

```c
void connect_operator_to_ast(t_ast **root, t_ast **cmd, 
                           t_ast **last_op, t_ast *new_op) {
    new_op->left = *cmd;        // Comando actual va a la izquierda
    
    if (*last_op)               // Si hay operador previo
        (*last_op)->right = new_op;  // Conectar como hijo derecho
    else
        *root = new_op;         // Primer operador se convierte en raíz
    
    *last_op = new_op;          // Actualizar último operador
    
    if (!is_redirection_operator(last_op))
        *cmd = NULL;            // Resetear cmd para pipes
}
```

### ⚙️ **handle_new_node()** - Manejo de Nuevos Operadores

```c
void handle_new_node(t_ast **root, t_ast **cmd, 
                    t_ast **last_op, t_ast *new_op) {
    if (*cmd) {
        // Hay comando actual - usar conexión normal
        connect_operator_to_ast(root, cmd, last_op, new_op);
    } else {
        // No hay comando - el operador toma todo el árbol izquierdo
        new_op->left = *root;
        *root = new_op;
        *last_op = new_op;
    }
}
```

---

## 📊 Ejemplo Completo de Construcción Paso a Paso

### Comando: `echo "hello" | grep h > output.txt`

#### **Token 1: "echo"**
```c
Estado inicial: root=NULL, cmd=NULL, last_op=NULL
Acción: handle_new_command()
```
```
Estado: root=echo, cmd=echo, last_op=NULL
Árbol:
  echo
```

#### **Token 2: "hello"**
```c
Estado: root=echo, cmd=echo, last_op=NULL
Acción: ft_add_argument(cmd, "hello")
```
```
Estado: root=echo, cmd=echo, last_op=NULL
Árbol:
  echo ["hello"]
```

#### **Token 3: "|" (PIPE)**
```c
Estado: root=echo, cmd=echo, last_op=NULL
Acción: handle_new_node() → connect_operator_to_ast()
```
```
Estado: root=pipe, cmd=NULL, last_op=pipe
Árbol:
     PIPE
    /
  echo
  ["hello"]
```

#### **Token 4: "grep"**
```c
Estado: root=pipe, cmd=NULL, last_op=pipe
Acción: handle_new_command()
```
```
Estado: root=pipe, cmd=grep, last_op=pipe
Árbol:
     PIPE
    /    \
  echo   grep
  ["hello"]
```

#### **Token 5: "h"**
```c
Estado: root=pipe, cmd=grep, last_op=pipe
Acción: ft_add_argument(cmd, "h")
```
```
Estado: root=pipe, cmd=grep, last_op=pipe
Árbol:
     PIPE
    /    \
  echo   grep
  ["hello"] ["h"]
```

#### **Token 6: ">" (REDIRECT_OUT)**
```c
Estado: root=pipe, cmd=grep, last_op=pipe
Acción: handle_new_node() → connect_operator_to_ast()
```
```
Estado: root=pipe, cmd=grep, last_op=redirect
Árbol:
     PIPE
    /         \
  echo    REDIRECT_OUT
  ["hello"]   /
           grep
           ["h"]
```

#### **Token 7: "output.txt"**
```c
Estado: root=pipe, cmd=grep, last_op=redirect
Acción: handle_redirection() para archivo de destino
```
```
Estado final: root=pipe, cmd=grep, last_op=redirect
Árbol final:
        PIPE
       /         \
    echo      REDIRECT_OUT
   ["hello"]    /        \
             grep      output.txt
             ["h"]
```

---

## 🧩 Casos Especiales de Asignación

### 🔀 **Precedencia de Operadores**

Los **pipes tienen menor precedencia** que las redirecciones:

```bash
echo hello > file.txt | cat
```

**Construcción:**
1. `echo hello` → nodo comando
2. `>` → nodo redirección (toma echo como izquierdo)
3. `file.txt` → se asigna como derecho de redirección
4. `|` → nodo pipe (toma TODO el nodo redirección como izquierdo)

**Resultado:**
```
    PIPE
   /    \
REDIRECT_OUT  cat
 /        \
echo    file.txt
```

### 🔄 **Redirecciones Múltiples**

```bash
echo hello > file1.txt >> file2.txt
```

**Proceso de asignación:**
```c
1. echo hello → comando base
2. > file1.txt → primera redirección
3. >> file2.txt → segunda redirección (anida la primera)
```

**Estructura resultante:**
```
    REDOUT2
   /        \
REDIRECT_OUT file2.txt
 /        \
echo    file1.txt
```

### 📋 **Here-documents**

```bash
cat << EOF
```

**Asignación especial:**
```c
1. handle_heredoc_special_case() se ejecuta
2. Delimitador "EOF" se asigna como right
3. Comando "cat" se asigna como left
```

---

## ⚡ Reglas de Asignación y Optimización

### 🎯 **Reglas de Precedencia**

El AST respeta las siguientes reglas de precedencia (de mayor a menor):

1. **Comandos y Argumentos** - Máxima precedencia
2. **Redirecciones** (`<`, `>`, `>>`, `<<`) - Alta precedencia
3. **Pipes** (`|`) - Baja precedencia

### 🔄 **Algoritmo de Inserción por Precedencia**

```c
// Pseudocódigo del algoritmo de asignación
if (token == CMD) {
    if (last_op is redirection) {
        // Comando va como archivo/destino de redirección
        assign_as_redirection_target();
    } else if (current_cmd exists) {
        // Comando va como argumento
        add_argument_to_current_cmd();
    } else {
        // Nuevo comando base
        create_new_command();
    }
} else if (token == REDIRECTION) {
    // Redirección toma precedencia sobre pipes
    create_redirection_node();
    connect_higher_precedence();
} else if (token == PIPE) {
    // Pipe tiene menor precedencia - toma todo como izquierdo
    create_pipe_node();
    connect_lower_precedence();
}
```

### 🧩 **Estrategia de Conexión Inteligente**

#### **Para Redirecciones:**
```c
void assign_redirection(t_ast **root, t_ast *redirect_node) {
    redirect_node->left = current_command;
    
    if (last_operator_is_pipe) {
        // Redirección se conecta al pipe
        last_pipe->right = redirect_node;
    } else {
        // Redirección se convierte en nueva raíz
        *root = redirect_node;
    }
}
```

#### **Para Pipes:**
```c
void assign_pipe(t_ast **root, t_ast *pipe_node) {
    pipe_node->left = *root;  // Pipe toma TODO el árbol izquierdo
    *root = pipe_node;        // Pipe se convierte en nueva raíz
}
```

### 🔀 **Manejo de Ambigüedades**

#### **Caso: Múltiples Redirecciones del Mismo Tipo**
```bash
echo hello > file1.txt > file2.txt
```

**Estrategia de Resolución:**
1. Primera redirección se procesa normalmente
2. Segunda redirección **anida** la primera
3. Solo la **última es efectiva** en ejecución

#### **Caso: Redirecciones Mezcladas**
```bash
echo hello > out.txt < in.txt >> append.txt
```

**Algoritmo de Anidación:**
```c
foreach (redirection_token) {
    new_redirect = create_redirection_node();
    
    if (last_redirect_exists) {
        new_redirect->left = last_redirect;  // Anidar anterior
    } else {
        new_redirect->left = base_command;   // Primer redirección
    }
    
    last_redirect = new_redirect;
}
```

---

## 🎛️ Estados de la Máquina de AST

Durante la construcción, el parser mantiene **estados internos**:

### 📊 **Estados Principales**

```c
typedef enum {
    STATE_EXPECTING_COMMAND,     // Esperando comando inicial
    STATE_BUILDING_COMMAND,      // Construyendo comando + argumentos
    STATE_EXPECTING_OPERATOR,    // Esperando operador tras comando
    STATE_EXPECTING_REDIRECT_TARGET,  // Esperando archivo tras >/<
    STATE_BUILDING_PIPE         // Construyendo cadena de pipes
} ast_parser_state_t;
```

### 🔄 **Transiciones de Estado**

```
EXPECTING_COMMAND
    │
    │ (CMD token)
    ▼
BUILDING_COMMAND ←─────┐
    │                  │
    │ (CMD token)      │ (CMD token)
    └──────────────────┘
    │
    │ (OPERATOR token)
    ▼
EXPECTING_OPERATOR
    │
    │ (REDIRECTION)     │ (PIPE)
    ▼                   ▼
EXPECTING_REDIRECT → BUILDING_PIPE
TARGET
```

### ⚙️ **Acciones por Estado**

```c
switch (current_state) {
    case STATE_EXPECTING_COMMAND:
        create_new_command_node();
        state = STATE_BUILDING_COMMAND;
        break;
        
    case STATE_BUILDING_COMMAND:
        if (token_is_command())
            add_argument();
        else if (token_is_operator()) {
            finalize_command();
            state = STATE_EXPECTING_OPERATOR;
        }
        break;
        
    case STATE_EXPECTING_OPERATOR:
        create_operator_node();
        connect_to_ast();
        if (token_is_redirection())
            state = STATE_EXPECTING_REDIRECT_TARGET;
        else
            state = STATE_EXPECTING_COMMAND;
        break;
}
```

---

## 🔍 Debugging: Visualización del Proceso

### 📋 **Tabla de Trazado de Construcción**

Para el comando: `echo "test" | grep test > result.txt`

| Token | Tipo | Estado Antes | Acción | Estado Después | Estructura AST |
|-------|------|--------------|--------|----------------|----------------|
| `echo` | CMD | root=NULL | `create_new_command()` | root=echo, cmd=echo | `echo` |
| `"test"` | CMD | root=echo | `add_argument()` | root=echo, cmd=echo | `echo["test"]` |
| `\|` | PIPE | root=echo | `create_pipe()` | root=pipe, cmd=NULL | `PIPE/echo` |
| `grep` | CMD | root=pipe | `create_new_command()` | root=pipe, cmd=grep | `PIPE/echo,grep` |
| `test` | CMD | root=pipe | `add_argument()` | root=pipe, cmd=grep | `PIPE/echo,grep["test"]` |
| `>` | REDIR | root=pipe | `create_redirect()` | root=pipe, last_op=redir | `PIPE/echo,REDIR/grep` |
| `result.txt` | CMD | root=pipe | `assign_redirect_target()` | Completo | `PIPE/echo,REDIR/grep,result.txt` |

### 🎯 **Puntos de Control de Integridad**

Durante la construcción, se verifican estas invariantes:

1. **Root siempre apunta al nodo de mayor precedencia**
2. **cmd apunta al comando en construcción activa**  
3. **last_op apunta al último operador para conexiones**
4. **Cada nodo tiene máximo 2 hijos (left, right)**
5. **Los comandos CMD solo pueden tener args, no hijos**

---

## 1️⃣ Comando Simple

### Ejemplo: `echo "Hello World"`

#### 🌳 Estructura del AST:
```
    CMD
   /   \
"echo" ["Hello World"]
```

#### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, node)
   └─ node->type == CMD
   └─ exec_command_node(data, node)
      └─ expand_command_variables(data, node)
      └─ is_builtin_command("echo") == true
      └─ execute_builtin_with_redirections(data)
         └─ echo_builtin(data)
            └─ Imprime: "Hello World"
```

#### 📋 Funciones Principales:
- `exec_ast()` → `exec_command_node()` → `echo_builtin()`

---

## 2️⃣ Comando Compuesto (Pipe)

### Ejemplo: `echo "Hello" | grep "Hello" | wc -l`

#### 🌳 Estructura del AST:
```
                PIPE
               /    \
            PIPE     CMD
           /    \   (wc -l)
        CMD      CMD
     (echo)   (grep)
       |         |
   ["Hello"]  ["Hello"]
```

#### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, root_pipe_node)
   └─ node->type == PIPE
   └─ exec_pipe(data, node)
      └─ pipe(pipefd) // Crear pipe
      └─ pid1 = fork() // Proceso izquierdo
      │  └─ exec_pipe_left()
      │     └─ dup2(pipefd[1], STDOUT)
      │     └─ exec_ast(data, left_pipe_node)
      │        └─ exec_pipe() // Recursivo para pipe interno
      │           └─ pid_inner1 = fork()
      │           │  └─ exec_ast(echo_node)
      │           │     └─ echo_builtin() → "Hello"
      │           └─ pid_inner2 = fork()
      │              └─ exec_ast(grep_node)
      │                 └─ exec_simple_cmd() → grep "Hello"
      └─ pid2 = fork() // Proceso derecho
         └─ exec_pipe_right()
            └─ dup2(pipefd[0], STDIN)
            └─ exec_ast(data, wc_node)
               └─ exec_simple_cmd() → wc -l
```

#### 📋 Funciones Principales:
- `exec_ast()` → `exec_pipe()` → `exec_pipe_left()` + `exec_pipe_right()`
- Recorrido recursivo para pipes anidados

---

## 3️⃣ Redirecciones

### 3.1 Redirección de Salida

#### Ejemplo: `echo "test" > output.txt`

##### 🌳 Estructura del AST:
```
    REDIRECT_OUT
   /            \
 CMD           CMD
(echo)    (output.txt)
  |
["test"]
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, redirect_node)
   └─ node->type == REDIRECT_OUT
   └─ exec_redirect_out(data, node)
      └─ execute_redirect_helper(data, node, O_WRONLY|O_CREAT|O_TRUNC)
         └─ filename = node->right->value // "output.txt"
         └─ fd = open(filename, flags, 0644)
         └─ data->output_redir_fd = fd
         └─ exec_ast(data, node->left) // Ejecutar echo
            └─ exec_command_node()
               └─ echo_builtin() → salida va a fd
```

##### 📋 Funciones Principales:
- `exec_ast()` → `exec_redirect_out()` → `execute_redirect_helper()`

---

### 3.2 Redirección de Entrada

#### Ejemplo: `cat < input.txt`

##### 🌳 Estructura del AST:
```
    REDIRECT_IN
   /           \
 CMD          CMD
(cat)    (input.txt)
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, redirect_node)
   └─ node->type == REDIRECT_IN
   └─ exec_redirect_in(data, node)
      └─ filename = node->right->value // "input.txt"
      └─ fd = open(filename, O_RDONLY)
      └─ data->input_redir_fd = fd
      └─ exec_ast(data, node->left) // Ejecutar cat
         └─ exec_command_node()
            └─ exec_simple_cmd() → cat lee de fd
```

---

### 3.3 Append (>>)

#### Ejemplo: `echo "more text" >> file.txt`

##### 🌳 Estructura del AST:
```
    REDOUT2
   /        \
 CMD       CMD
(echo)  (file.txt)
  |
["more text"]
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, append_node)
   └─ node->type == REDOUT2
   └─ exec_redirect_append(data, node)
      └─ execute_redirect_helper(data, node, O_WRONLY|O_CREAT|O_APPEND)
         └─ fd = open(filename, O_APPEND|O_CREAT|O_WRONLY, 0644)
         └─ exec_ast(data, node->left)
            └─ echo_builtin() → texto se añade al final
```

---

### 3.4 Here-document (<<)

#### Ejemplo: `cat << EOF`

##### 🌳 Estructura del AST:
```
    REDIN2
   /      \
 CMD     CMD
(cat)  (EOF)
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, heredoc_node)
   └─ node->type == REDIN2
   └─ exec_heredoc(data, node)
      └─ collect_heredoc_chain(node, heredocs, &count)
      └─ process_heredoc_loop(data, heredocs, count)
         └─ ft_setup_heredoc_pipe(data, pipefd, "EOF")
            └─ pipe(pipefd) // Crear pipe interno
            └─ read_heredoc_lines(pipefd[1], "EOF")
               └─ while(1):
                  │  line = readline("> ")
                  │  if (line == "EOF") break
                  │  write(pipefd[1], line)
               └─ close(pipefd[1])
            └─ data->heredoc_pipe_fd = pipefd[0]
      └─ cmd_node = find_command_node(node)
      └─ exec_ast(data, cmd_node) // Ejecutar cat
         └─ cat lee del heredoc_pipe_fd
```

##### 📋 Funciones Principales:
- `exec_ast()` → `exec_heredoc()` → `process_heredoc_loop()` → `read_heredoc_lines()`

---

## 4️⃣ Combinaciones Complejas

### 4.1 Pipe con Redirección

#### Ejemplo: `echo "data" | grep "data" > result.txt`

##### 🌳 Estructura del AST:
```
           PIPE
          /    \
       CMD      REDIRECT_OUT
    (echo)      /           \
      |       CMD          CMD
   ["data"]  (grep)    (result.txt)
               |
           ["data"]
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, pipe_node)
   └─ node->type == PIPE
   └─ exec_pipe(data, node)
      └─ pipe(pipefd)
      └─ pid1 = fork() // Proceso izquierdo
      │  └─ exec_pipe_left()
      │     └─ dup2(pipefd[1], STDOUT)
      │     └─ exec_ast(echo_node)
      │        └─ echo_builtin() → "data" va al pipe
      └─ pid2 = fork() // Proceso derecho
         └─ exec_pipe_right()
            └─ dup2(pipefd[0], STDIN)
            └─ exec_ast(redirect_node)
               └─ exec_redirect_out()
                  └─ fd = open("result.txt", O_WRONLY|O_CREAT|O_TRUNC)
                  └─ dup2(fd, STDOUT)
                  └─ exec_ast(grep_node)
                     └─ grep lee del pipe, escribe a archivo
```

---

### 4.2 Here-document con Pipe

#### Ejemplo: `cat << EOF | wc -l`

##### 🌳 Estructura del AST:
```
           PIPE
          /    \
      REDIN2   CMD
     /      \  (wc)
   CMD     CMD  |
  (cat)   (EOF) [-l]
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, pipe_node)
   └─ exec_pipe(data, node)
      └─ pid1 = fork() // Proceso izquierdo
      │  └─ exec_pipe_left()
      │     └─ dup2(pipefd[1], STDOUT)
      │     └─ exec_ast(heredoc_node)
      │        └─ exec_heredoc()
      │           └─ setup heredoc pipe interno
      │           └─ read_heredoc_lines()
      │           └─ exec_ast(cat_node)
      │              └─ cat lee heredoc → envía al pipe principal
      └─ pid2 = fork() // Proceso derecho
         └─ exec_pipe_right()
            └─ dup2(pipefd[0], STDIN)
            └─ exec_ast(wc_node)
               └─ wc -l cuenta líneas del pipe
```

---

### 4.3 Múltiples Redirecciones

#### Ejemplo: `echo "test" > file1.txt >> file2.txt`

##### 🌳 Estructura del AST:
```
      REDOUT2
     /        \
REDIRECT_OUT  CMD
   /     \   (file2.txt)
 CMD     CMD
(echo)  (file1.txt)
  |
["test"]
```

##### 🔄 Flujo de Ejecución:
```
1. exec_ast(data, redout2_node)
   └─ exec_redirect_append()
      └─ count_output_redirections(node) > 1
      └─ process_multiple_output_redirections()
         └─ collect_redirections(node, redirections, &count)
         └─ process_redirection_files() // Procesa todas
         └─ apply_last_redirect() // Solo la última (file2.txt) es efectiva
         └─ exec_ast(echo_node)
            └─ echo → salida va solo a file2.txt (append)
```

---

## 🔍 Resumen del Flujo Principal

```
┌─────────────────┐
│   ENTRADA      │
│ "comando args" │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│  TOKENIZACIÓN  │
│ ft_tokens()    │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│ CONSTRUCCIÓN   │
│ ft_build_ast() │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│   EJECUCIÓN    │
│  exec_ast()    │
└─────────────────┘
         │
         ▼
    ┌─────────┐
    │ Switch  │
    │ (type)  │
    └─────────┘
         │
    ┌────┴────┐
    ▼         ▼
┌───────┐ ┌─────────┐
│  CMD  │ │OPERADOR │
└───────┘ └─────────┘
    │         │
    ▼         ▼
┌─────────────────┐
│ exec_command_   │
│ node()         │
└─────────────────┘
    │
    ▼
┌─────────────────┐
│ builtin() o     │
│ exec_simple_    │
│ cmd()          │
└─────────────────┘
```

---

---

## 🧠 Gestión de Memoria durante Construcción

### 💾 **Estrategia de Asignación**

```c
// Cada nodo AST se asigna dinámicamente
t_ast *ft_create_ast_node(t_type type, char *value) {
    t_ast *node = malloc(sizeof(t_ast));
    node->value = ft_strdup(value);    // Copia independiente
    node->args = NULL;                 // Asignado bajo demanda
    node->left = node->right = NULL;   // Inicialización segura
    return node;
}
```

### 🔄 **Limpieza en Caso de Error**

Si la construcción falla en cualquier punto:

```c
if (!parse_success) {
    ft_free_ast(root);  // Liberación recursiva de todo el árbol
    return NULL;
}
```

**Función de Liberación Recursiva:**
```c
void ft_free_ast(t_ast *ast) {
    if (!ast) return;
    
    ft_free_ast(ast->left);   // Recursivo izquierdo
    ft_free_ast(ast->right);  // Recursivo derecho
    
    free(ast->value);         // Liberar valor
    free_matrix(ast->args);   // Liberar argumentos
    free(ast);                // Liberar nodo
}
```

### ⚠️ **Casos Edge y Manejo de Errores**

#### **Token Inesperado**
```c
if (token_unexpected()) {
    cleanup_partial_ast();
    return SYNTAX_ERROR;
}
```

#### **Memoria Insuficiente**
```c
if (!new_node) {
    ft_free_ast(partial_tree);
    return MEMORY_ERROR;
}
```

#### **Redirección Sin Archivo**
```bash
echo hello >   # Error: falta archivo
```
```c
if (redirection_without_target()) {
    syntax_error("missing redirection target");
    return ERROR;
}
```

---

## 🎯 Resumen del Flujo Principal

```
┌─────────────────┐
│   ENTRADA      │
│ "comando args" │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│  TOKENIZACIÓN  │
│ ft_tokens()    │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│ CONSTRUCCIÓN   │
│ ft_build_ast() │
└─────────────────┘
         │
    ┌────┴────┐
    ▼         ▼
┌─────────┐ ┌─────────┐
│ Token   │ │ Token   │
│  CMD    │ │OPERADOR │
└─────────┘ └─────────┘
    │         │
    ▼         ▼
┌─────────────────┐
│ ft_handle_      │
│ command_node()  │
└─────────────────┘
    │
    ▼
┌─────────────────┐
│ Asignación en   │
│ Estructura AST  │
└─────────────────┘
         │
         ▼
┌─────────────────┐
│   EJECUCIÓN    │
│  exec_ast()    │
└─────────────────┘
```
