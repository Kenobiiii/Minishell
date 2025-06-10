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
```

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

## 🎯 Puntos Clave del Recorrido

### 🔄 **Orden de Ejecución**
- **Post-orden**: Los hijos se ejecutan antes que el padre
- **Recursión**: `exec_ast()` se llama recursivamente para cada nodo

### 🔀 **Manejo de Pipes**
- Cada pipe crea **2 procesos hijo**
- **Pipe interno** para heredocs
- **Redirecciones** se manejan en el proceso correcto

### 📁 **Gestión de Descriptores**
- `input_redir_fd`, `output_redir_fd`, `heredoc_pipe_fd`
- **Duplicación** con `dup2()` para redirecciones
- **Limpieza** automática con `close_redirection_fds()`

### 🧠 **Expansión de Variables**
- Se realiza en **tiempo de ejecución**
- `expand_command_variables()` antes de ejecutar comandos

---

## 📝 Notas Técnicas

### ⚠️ **Limitaciones**
- Máximo **10 heredocs** concatenados
- **Solo la última redirección** de salida es efectiva en múltiples redirecciones

### 🔧 **Optimizaciones**
- **Detección early** de built-ins
- **Limpieza de memoria** progresiva
- **Manejo de señales** por estado

---

*Documentación generada para el proyecto Minishell - 42 School*  
*Autores: paromero & anggalle*  
*Fecha: Junio 2025*
