# 🌳 Guía Visual del AST - Minishell (Simplificada)

## 📖 Introducción

Esta guía muestra de forma **visual y simplificada** cómo se construye y ejecuta el AST en minishell:

- **Nombres de funciones clave**
- **Representación gráfica del AST**
- **Flujo de ejecución simplificado**

---

## ⚡ Orden de Prioridad en el AST

El AST respeta el siguiente **orden de precedencia** (de **mayor** a **menor**):

```
1. 🔴 COMANDOS (CMD)          - Máxima prioridad
2. 🟡 REDIRECCIONES          - Media prioridad
   (<, >, >>, <<)
3. 🟢 PIPES (|)              - Menor prioridad
```

### 📋 Regla Principal:
- **Los operadores de menor prioridad se convierten en la RAÍZ del AST**
- **Los de mayor prioridad quedan como hojas/sub-árboles**

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

### Ejemplo 1: Comando con Redirección: `echo "test" > file.txt`

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
    REDIRECT_OUT  ← ROOT (redirección tiene menor prioridad que comando)
   /
echo ["test"]
```

4️⃣ **"file.txt"** → `handle_redirection()`
```
    REDIRECT_OUT  ← ROOT
   /            \
echo ["test"]   file.txt
```

---

### Ejemplo 2: 🔄 **CAMBIO DE ROOT POR PRIORIDAD**

### Comando: `echo "hello" > output.txt | cat`

#### **Tokens:**
```
["echo"] ["hello"] [">"] ["output.txt"] ["|"] ["cat"]
```

#### **Construcción del AST con cambio de ROOT:**

1️⃣-3️⃣ **Primeros tokens** → Construir redirección
```
    REDIRECT_OUT  ← ROOT actual
   /            \
echo ["hello"]  output.txt
```

4️⃣ **"|"** → `ft_handle_operator_node()` 
**¡CAMBIO DE ROOT!** - Pipe tiene menor prioridad que redirección
```
      PIPE        ← NUEVO ROOT (pipe toma precedencia)
     /
REDIRECT_OUT      ← Ya no es root
 /          \
echo       output.txt
["hello"]
```

5️⃣ **"cat"** → `ft_handle_command_node()`
```
      PIPE        ← ROOT final
     /    \
REDIRECT_OUT  cat
 /          \
echo       output.txt
["hello"]
```

**📌 Explicación del cambio:**
- Inicialmente: `REDIRECT_OUT` era el root
- Al llegar `|`: Se crea `PIPE` que toma **TODO** el árbol anterior como hijo izquierdo
- Resultado: `PIPE` se convierte en el **nuevo root**

---

### Ejemplo 3: **SIN CAMBIO DE ROOT** (Misma prioridad)

### Comando: `echo "test" > file1.txt > file2.txt`

#### **Construcción:**

1️⃣-3️⃣ **Primera redirección:**
```
    REDIRECT_OUT  ← ROOT
   /            \
echo ["test"]   file1.txt
```

4️⃣ **Segunda ">"** → `ft_handle_operator_node()`
**NO hay cambio de ROOT** - Misma prioridad, se anida
```
    REDIRECT_OUT  ← NUEVO ROOT (anida el anterior)
   /            \
REDIRECT_OUT   file2.txt
 /          \
echo       file1.txt
["test"]
```

**📌 Explicación:**
- Mismo tipo de operador (redirección)
- El nuevo operador **anida** el anterior
- Root cambia, pero por anidación, no por prioridad

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
