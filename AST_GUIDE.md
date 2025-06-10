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

## 🎯 Más Ejemplos de Construcción

### Comando con Redirección: `echo "test" > file.txt`

#### **Tokens:**
```
["echo"] ["test"] [">"] ["file.txt"]
```

#### **Construcción del AST:**

1️⃣ **"echo"** → `ft_handle_command_node()`
```
echo
```

2️⃣ **"test"** → `ft_add_argument()`
```
echo ["test"]
```

3️⃣ **">"** → `ft_handle_operator_node()`
```
    REDIRECT_OUT
   /
echo ["test"]
```

4️⃣ **"file.txt"** → `handle_redirection()`
```
    REDIRECT_OUT
   /            \
echo ["test"]   file.txt
```

---

## 🔍 Resumen de Construcción

```
                 ft_build_ast()
                       │
                       ▼
              ┌─────────────────┐
              │ Procesar Tokens │
              └─────────────────┘
                       │
              ┌────────┴────────┐
              ▼                 ▼
    ft_handle_command_    ft_handle_operator_
         node()               node()
              │                 │
              ▼                 ▼
      ┌──────────────┐   ┌──────────────┐
      │Crear/Actualizar│   │Crear Operador│
      │nodo comando    │   │+ Conectar    │
      └──────────────┘   └──────────────┘
```
