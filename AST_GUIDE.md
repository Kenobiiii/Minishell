# 🌳 Guía Visual del AST - Minishell (Simplificada)

## 📖 Introducción

Esta guía muestra de forma **visual y simplificada** cómo se construye y ejecuta el AST en minishell:

- **Nombres de funciones clave**
- **Representación gráfica del AST**
- **Flujo de ejecución simplificado**

---

## 🔧 Tipos de Nodos AST

```c
typedef enum e_type {
    CMD,           // Comando
    REDIRECT_OUT,  // >
    REDIRECT_IN,   // <
    REDIN2,        // <<
    REDOUT2,       // >>
    PIPE           // |
} t_type;
```

---

## 🏗️ Funciones de Construcción del AST

### **ft_build_ast()** - Función Principal
```
Tokens → ft_build_ast() → AST completo
```

### **ft_handle_command_node()** - Procesar Comandos
```
Token CMD → Crear/Actualizar nodo comando
```

### **ft_handle_operator_node()** - Procesar Operadores  
```
Token OPERADOR → Crear nodo operador + conectar
```

---

## 📊 Construcción Paso a Paso

### Ejemplo: `echo hello | grep h > output.txt`

#### **Tokens:**
```
["echo"] ["hello"] ["|"] ["grep"] ["h"] [">"] ["output.txt"]
```

#### **Construcción del AST:**

1️⃣ **"echo"** → `ft_handle_command_node()`
```
echo
```

2️⃣ **"hello"** → `ft_add_argument()`
```
echo ["hello"]
```

3️⃣ **"|"** → `ft_handle_operator_node()`
```
     PIPE
    /
echo ["hello"]
```

4️⃣ **"grep"** → `ft_handle_command_node()`
```
     PIPE
    /    \
echo    grep
["hello"]
```

5️⃣ **"h"** → `ft_add_argument()`
```
     PIPE
    /    \
echo    grep ["h"]
["hello"]
```

6️⃣ **">"** → `ft_handle_operator_node()`
```
        PIPE
       /         \
    echo      REDIRECT_OUT
   ["hello"]      /
               grep ["h"]
```

7️⃣ **"output.txt"** → `handle_redirection()`
```
        PIPE
       /         \
    echo      REDIRECT_OUT
   ["hello"]    /        \
             grep       output.txt
             ["h"]
```

---

## 🎯 Ejecución del AST

### **exec_ast()** - Función Principal
```
AST → exec_ast() → Ejecución según tipo de nodo
```

---

## 1️⃣ Comando Simple

### Ejemplo: `echo "Hello"`
### Función: `exec_command_node()`

```
    CMD
   /   \
"echo" ["Hello"]

exec_ast() → exec_command_node() → echo_builtin()
```

---

## 2️⃣ Pipes

### Ejemplo: `echo "test" | grep test`
### Función: `exec_pipe()`

```
     PIPE
    /    \
 echo    grep
["test"] ["test"]

exec_ast() → exec_pipe() → fork() × 2
           ├─ Proceso izq: echo
           └─ Proceso der: grep
```

---

## 3️⃣ Redirecciones

### 3.1 Salida: `echo "test" > file.txt`
### Función: `exec_redirect_out()`

```
    REDIRECT_OUT
   /            \
echo["test"]   file.txt

exec_ast() → exec_redirect_out() → open(file) → exec_ast(echo)
```

### 3.2 Entrada: `cat < file.txt`
### Función: `exec_redirect_in()`

```
   REDIRECT_IN
  /           \
cat         file.txt

exec_ast() → exec_redirect_in() → open(file) → exec_ast(cat)
```

### 3.3 Append: `echo "text" >> file.txt`
### Función: `exec_redirect_append()`

```
    REDOUT2
   /        \
echo      file.txt
["text"]

exec_ast() → exec_redirect_append() → open(file, O_APPEND) → exec_ast(echo)
```

### 3.4 Heredoc: `cat << EOF`
### Función: `exec_heredoc()`

```
   REDIN2
  /      \
cat     EOF

exec_ast() → exec_heredoc() → read_heredoc_lines() → pipe() → exec_ast(cat)
```

---

## 4️⃣ Combinaciones Complejas

### 4.1 Pipe + Redirección: `echo "data" | grep data > result.txt`

```
        PIPE
       /    \
    echo    REDIRECT_OUT
   ["data"]  /         \
          grep        result.txt
         ["data"]

exec_ast(PIPE) → exec_pipe()
               ├─ Proceso izq: exec_ast(echo)
               └─ Proceso der: exec_ast(REDIRECT_OUT)
                              └─ exec_redirect_out() → exec_ast(grep)
```

### 4.2 Heredoc + Pipe: `cat << EOF | wc -l`

```
        PIPE
       /    \
    REDIN2   wc
   /     \   [-l]
 cat     EOF

exec_ast(PIPE) → exec_pipe()
               ├─ Proceso izq: exec_ast(REDIN2) → exec_heredoc()
               └─ Proceso der: exec_ast(wc)
```

---

## 🔍 Resumen Visual

```
                    exec_ast()
                        │
              ┌─────────┼─────────┐
              ▼         ▼         ▼
           CMD       OPERADOR   REDIR
              │         │         │
              ▼         ▼         ▼
    exec_command_  exec_pipe() exec_redirect_*()
         node()        │              │
              │        ▼              ▼
              ▼    fork() × 2    open() + exec_ast()
      builtin() o        │
      exec_simple_       ▼
         cmd()     Procesos hijo
```
