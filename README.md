# Proyecto Minishell

## Descripción General
Minishell es una implementación simplificada de shell escrita en C que recrea las funcionalidades principales de bash. Este proyecto demuestra la comprensión de conceptos de programación de sistemas incluyendo gestión de procesos, descriptores de archivos, manejo de señales y análisis de estructuras de comandos complejas.

## Tabla de Contenidos
1. [Estructura del Proyecto](#estructura-del-proyecto)
2. [Estructuras de Datos Principales](#estructuras-de-datos-principales)
3. [Parte 1: Análisis Sintáctico](#parte-1-análisis-sintáctico)
4. [Parte 2: Ejecución](#parte-2-ejecución)
5. [Comandos Built-in](#comandos-built-in)
6. [Gestión de Memoria](#gestión-de-memoria)
7. [Manejo de Señales](#manejo-de-señales)
8. [Ejemplos de Uso](#ejemplos-de-uso)

## Estructura del Proyecto

```
Minishell/
├── src/
│   ├── main.c                    # Punto de entrada y bucle principal
│   ├── minishell.h              # Cabecera con todas las estructuras y prototipos
│   ├── parse/                   # Módulo de análisis sintáctico
│   │   ├── ft_ast.c            # Construcción del AST
│   │   ├── ft_tokens.c         # Tokenización
│   │   ├── ft_handle_quotes.c  # Manejo de comillas
│   │   ├── ft_expansion.c      # Expansión de variables
│   │   └── ft_redirections.c   # Análisis de redirecciones
│   ├── exec/                   # Módulo de ejecución
│   │   ├── exec.c             # Lógica principal de ejecución
│   │   ├── pipe.c             # Ejecución de pipelines
│   │   ├── heredoc.c          # Manejo de here-documents
│   │   └── redirections.c     # Redirección de E/S
│   ├── builtins/              # Comandos built-in
│   │   ├── ft_echo.c
│   │   ├── ft_cd.c
│   │   ├── ft_pwd.c
│   │   ├── ft_export.c
│   │   ├── ft_unset.c
│   │   ├── ft_env.c
│   │   └── ft_exit.c
│   └── utils/                 # Funciones de utilidad
└── libft/                     # Biblioteca C personalizada
```

## Estructuras de Datos Principales

### t_data - Estado Principal del Programa
```c
typedef struct s_data
{
    char        *line;              // Línea de entrada actual
    char        *prompt;            // Cadena del prompt del shell
    char        *cwd;              // Directorio de trabajo actual
    int         exit;              // Bandera de salida
    int         wstatus;           // Estado de salida del último comando
    int         only_redirections; // Bandera para comandos solo de redirección
    t_env       *env;              // Lista enlazada de variables de entorno
    t_tokens    *tokens;           // Entrada tokenizada
    t_ast       *ast;              // Árbol Sintáctico Abstracto
    int         input_redir_fd;    // Descriptor de archivo de redirección de entrada
    int         output_redir_fd;   // Descriptor de archivo de redirección de salida
    int         heredoc_pipe_fd;   // Descriptor de archivo de pipe de here-document
} t_data;
```

### t_ast - Nodo del Árbol Sintáctico Abstracto
```c
typedef struct s_ast
{
    t_type          type;    // Tipo de nodo (CMD, PIPE, REDIRECT_IN, etc.)
    char            *value;  // Valor del comando u operador
    char            **args;  // Array de argumentos del comando
    struct s_ast    *left;   // Nodo hijo izquierdo
    struct s_ast    *right;  // Nodo hijo derecho
} t_ast;
```

### t_tokens - Lista Enlazada de Tokens
```c
typedef struct s_tokens
{
    t_type          type;    // Tipo de token
    char            *value;  // Valor string del token
    struct s_tokens *next;   // Siguiente token en la lista
} t_tokens;
```

### t_type - Tipos de Token/Nodo
```c
typedef enum e_type
{
    CMD,           // Comando
    REDIRECT_OUT,  // > (redirección de salida)
    REDIRECT_IN,   // < (redirección de entrada)
    REDIN2,        // << (here-document)
    REDOUT2,       // >> (redirección de anexado)
    PIPE           // | (pipe)
} t_type;
```

## Parte 1: Análisis Sintáctico

El módulo de análisis sintáctico transforma la entrada del usuario en un Árbol Sintáctico Abstracto (AST) que representa la estructura del comando. Este proceso involucra varias etapas:

### 1. Tokenización (`ft_tokens.c`)

**Propósito**: Dividir la cadena de entrada en tokens significativos.

**Proceso**:
1. **Análisis Léxico**: Escanear la entrada carácter por carácter
2. **Identificación de Tokens**: Reconocer operadores (`|`, `>`, `<`, `>>`, `<<`) y comandos
3. **Manejo de Comillas**: Preservar espacios y caracteres especiales dentro de comillas
4. **Creación de Tokens**: Crear lista enlazada de tokens con tipo y valor

**Funciones Clave**:
- `tokenize()`: Función principal de tokenización
- `create_token()`: Crea nodos de token individuales
- `add_token()`: Añade token a la lista enlazada

**Ejemplo**:
```bash
Entrada: echo "hello world" | grep hello > output.txt
Tokens: [CMD:"echo"] [CMD:"hello world"] [PIPE:"|"] [CMD:"grep"] [CMD:"hello"] [REDIRECT_OUT:">"] [CMD:"output.txt"]
```

### 2. Verificación de Sintaxis (`ft_check_syntax.c`)

**Propósito**: Validar la sintaxis del comando antes de la construcción del AST.

**Verificaciones**:
- Cierre apropiado de comillas
- Colocación válida de operadores
- Uso correcto de pipes
- Sintaxis adecuada de redirecciones

**Funciones Clave**:
- `check_syntax()`: Validación principal de sintaxis
- `check_operator_syntax()`: Valida la colocación de operadores
- `openquotes()`: Verifica comillas sin cerrar

### 3. Manejo de Comillas (`ft_handle_quotes.c`)

**Propósito**: Procesar cadenas entre comillas correctamente, conservando el contenido y eliminando los caracteres de comillas.

**Tipos de Comillas**:
- **Comillas simples (`'`)**: Preservar todo literalmente, sin expansión
- **Comillas dobles (`"`)**: Permitir expansión de variables pero preservar espacios
- **Sin comillas**: Análisis normal con expansión

**Funciones Clave**:
- `handle_quotes()`: Función principal de procesamiento de comillas
- `process_quoted_content()`: Extrae contenido de las comillas
- `remove_quotes()`: Elimina caracteres de comillas después del procesamiento

### 4. Expansión de Variables (`ft_expansion.c`)

**Propósito**: Reemplazar variables de entorno y variables especiales con sus valores.

**Tipos de Expansión**:
- **Variables de entorno**: `$VAR` → valor de VAR
- **Estado de salida**: `$?` → estado de salida del último comando
- **ID de proceso**: `$$` → PID del shell actual

**Proceso**:
1. Buscar carácter `$`
2. Extraer nombre de variable
3. Buscar valor en el entorno
4. Reemplazar `$VAR` con el valor real

**Funciones Clave**:
- `expand_variables()`: Función principal de expansión
- `get_env_value()`: Obtiene valor de variable de entorno
- `expand_exit_status()`: Maneja expansión de `$?`

### 5. Construcción del AST (`ft_ast.c`)

**Propósito**: Construir un árbol binario que represente la estructura del comando y la precedencia de operadores.

**Estructura del AST**:
- **Nodos internos**: Operadores (pipes, redirecciones)
- **Nodos hoja**: Comandos y argumentos
- **Estructura de árbol**: Representa el orden de ejecución y flujo de datos

**Proceso de Construcción**:
1. **Analizar por precedencia**: Los pipes tienen la menor precedencia
2. **Manejar redirecciones**: Adjuntar a nodos de comando
3. **Construir nodos de comando**: Agrupar comando con sus argumentos
4. **Enlazar nodos**: Crear relaciones padre-hijo

**Funciones Clave**:
- `build_ast()`: Función principal de construcción del AST
- `handle_pipe()`: Crea nodos de pipe
- `handle_redirections()`: Procesa operadores de redirección
- `create_cmd_node()`: Crea nodos de comando

**Ejemplo de AST**:
```bash
Comando: echo hello | grep h > output.txt

Estructura del AST:
        PIPE
       /    \
    CMD      REDIRECT_OUT
   /         /           \
"echo"    CMD            "output.txt"
  |       /
"hello" "grep"
         |
        "h"
```

### 6. Análisis de Redirecciones (`ft_redirections.c`)

**Propósito**: Manejar operadores de redirección de entrada/salida y especificaciones de archivos.

**Tipos de Redirección**:
- **`>`**: Redirigir stdout a archivo (sobrescribir)
- **`>>`**: Redirigir stdout a archivo (anexar)
- **`<`**: Redirigir stdin desde archivo
- **`<<`**: Here-document (entrada multi-línea)

**Funciones Clave**:
- `parse_redirections()`: Analizador principal de redirecciones
- `handle_input_redirect()`: Procesa redirecciones de entrada
- `handle_output_redirect()`: Procesa redirecciones de salida
- `setup_heredoc()`: Maneja configuración de here-document

## Parte 2: Ejecución

El módulo de ejecución interpreta el AST y ejecuta comandos, manejando pipes, redirecciones y comandos built-in.

### 1. Interpretación del AST (`exec.c`)

**Propósito**: Recorrer el AST y ejecutar comandos según la estructura del árbol.

**Estrategia de Ejecución**:
- **Recorrido post-orden**: Ejecutar hijos antes que el padre
- **Despacho basado en tipo**: Manejo diferente para cada tipo de nodo
- **Preservación de contexto**: Mantener descriptores de archivos y entorno

**Funciones Clave**:
- `execute_ast()`: Función principal de ejecución del AST
- `execute_command()`: Ejecuta nodos de comando
- `execute_pipe()`: Maneja nodos de pipe
- `execute_redirection()`: Procesa nodos de redirección

### 2. Ejecución de Comandos

**Creación de Procesos**:
1. **Fork de proceso**: Crear proceso hijo para comandos externos
2. **Configurar entorno**: Pasar variables de entorno
3. **Manejar redirecciones**: Configurar descriptores de archivos
4. **Ejecutar comando**: Llamar `execve()` con la ruta del comando
5. **Esperar finalización**: El padre espera que termine el hijo

**Resolución de Rutas**:
1. Verificar si el comando es built-in
2. Verificar si el comando contiene `/` (ruta absoluta/relativa)
3. Buscar en la variable de entorno `PATH`
4. Devolver error si no se encuentra el comando

### 3. Ejecución de Pipelines (`pipe.c`)

**Propósito**: Conectar múltiples comandos a través de pipes, permitiendo flujo de datos entre procesos.

**Proceso de Pipeline**:
1. **Crear pipes**: Establecer canales de comunicación
2. **Fork de procesos**: Crear proceso para cada comando
3. **Conectar pipes**: Configurar redirecciones de stdin/stdout
4. **Ejecutar comandos**: Ejecutar cada comando en su proceso
5. **Cerrar descriptores**: Limpiar descriptores de archivos de pipes
6. **Esperar todos**: Esperar que todos los procesos terminen

**Funciones Clave**:
- `execute_pipeline()`: Ejecución principal de pipeline
- `setup_pipe_fds()`: Configurar descriptores de archivos para pipes
- `close_pipe_fds()`: Limpiar descriptores de archivos de pipes

**Ejemplo de Pipeline**:
```bash
Comando: cat file.txt | grep pattern | sort | head -10

Ejecución:
Proceso 1: cat file.txt    → pipe1 →
Proceso 2: grep pattern    → pipe2 →
Proceso 3: sort            → pipe3 →
Proceso 4: head -10        → stdout
```

### 4. Manejo de Here-documents (`heredoc.c`)

**Propósito**: Manejar entrada multi-línea terminada por un delimitador.

**Proceso de Here-document**:
1. **Leer entrada**: Recopilar líneas hasta encontrar el delimitador
2. **Crear pipe**: Configurar pipe temporal para datos
3. **Escribir a pipe**: Enviar entrada recopilada al pipe
4. **Redirigir stdin**: Conectar stdin del comando al extremo de lectura del pipe

**Funciones Clave**:
- `handle_heredoc()`: Manejador principal de here-document
- `read_heredoc_input()`: Recopila líneas de entrada
- `setup_heredoc_pipe()`: Crea y configura pipe

**Ejemplo**:
```bash
cat << EOF
Esta es la línea 1
Esta es la línea 2
EOF
```

### 5. Redirección de E/S (`redirections.c`)

**Propósito**: Redirigir entrada/salida de comandos hacia/desde archivos.

**Implementación de Redirección**:
- **Apertura de archivos**: Abrir archivos objetivo con modos apropiados
- **Duplicación de descriptores**: Usar `dup2()` para redirigir stdin/stdout
- **Manejo de errores**: Verificar permisos y existencia de archivos
- **Limpieza**: Cerrar descriptores de archivos después de la ejecución

**Funciones Clave**:
- `setup_redirections()`: Configuración principal de redirecciones
- `redirect_input()`: Maneja redirección de entrada
- `redirect_output()`: Maneja redirección de salida
- `cleanup_redirections()`: Cierra descriptores de archivos

## Comandos Built-in

Los comandos built-in se ejecutan directamente por el shell sin crear nuevos procesos:

### 1. `echo` - Mostrar Texto
- **Propósito**: Imprimir argumentos a stdout
- **Opciones**: `-n` (sin salto de línea)
- **Implementación**: Salida directa, manejar secuencias de escape

### 2. `cd` - Cambiar Directorio
- **Propósito**: Cambiar directorio de trabajo actual
- **Características**: Manejar `~` (inicio), `-` (directorio anterior)
- **Implementación**: Usar `chdir()`, actualizar `PWD` y `OLDPWD`

### 3. `pwd` - Imprimir Directorio de Trabajo
- **Propósito**: Mostrar ruta del directorio actual
- **Implementación**: Usar `getcwd()` o variable de entorno `PWD`

### 4. `export` - Establecer Variables de Entorno
- **Propósito**: Crear o modificar variables de entorno
- **Características**: Asignación de variables, mostrar variables actuales
- **Implementación**: Actualizar lista interna de entorno

### 5. `unset` - Eliminar Variables de Entorno
- **Propósito**: Eliminar variables de entorno
- **Implementación**: Remover de la lista interna de entorno

### 6. `env` - Mostrar Entorno
- **Propósito**: Mostrar todas las variables de entorno
- **Implementación**: Iterar a través de la lista de entorno

### 7. `exit` - Terminar Shell
- **Propósito**: Salir del shell con código de estado opcional
- **Implementación**: Establecer bandera de salida, limpiar recursos

## Gestión de Memoria

### Estrategia
1. **Seguimiento de asignaciones**: Mantener registro de todas las asignaciones dinámicas
2. **Funciones de limpieza**: Funciones dedicadas para liberar estructuras
3. **Manejo de errores**: Limpieza en errores para prevenir fugas
4. **Gestión de recursos**: Cerrar descriptores de archivos, liberar memoria

### Funciones Clave de Limpieza
- `free_tokens()`: Liberar lista enlazada de tokens
- `free_ast()`: Liberar nodos del AST recursivamente
- `free_env()`: Liberar lista de variables de entorno
- `cleanup_data()`: Limpieza completa de estructura de datos

## Manejo de Señales

### Señales Manejadas
- **SIGINT (Ctrl+C)**: Interrumpir operación actual
- **SIGQUIT (Ctrl+\)**: Ignorado en modo interactivo
- **SIGTERM**: Terminación elegante

### Implementación
- **Manejadores de señales**: Funciones personalizadas de manejo de señales
- **Gestión de estado**: Comportamiento diferente basado en el estado del shell
- **Grupos de procesos**: Propagación adecuada de señales en pipelines

## Ejemplos de Uso

### Comandos Básicos
```bash
$ echo "¡Hola, Mundo!"
¡Hola, Mundo!

$ pwd
/home/usuario/minishell

$ ls -la
[listado de directorio]
```

### Pipes
```bash
$ cat archivo.txt | grep patrón | sort
[salida filtrada y ordenada]

$ ps aux | grep minishell | head -5
[información de procesos]
```

### Redirecciones
```bash
$ echo "Hola" > salida.txt
$ cat < entrada.txt
$ echo "Anexar esto" >> salida.txt
$ wc -l << EOF
línea 1
línea 2
línea 3
EOF
```

### Comandos Complejos
```bash
$ export VAR="prueba"
$ echo $VAR | tr 'a-z' 'A-Z' > mayusculas.txt
$ cat mayusculas.txt
PRUEBA

$ cd /tmp && pwd && cd - && pwd
/tmp
/home/usuario/minishell
```

### Manejo de Errores
```bash
$ comando_inexistente
minishell: comando_inexistente: command not found

$ cat archivo_inexistente.txt
cat: archivo_inexistente.txt: No such file or directory

$ echo "comilla sin cerrar
minishell: syntax error: unclosed quote
```

## Aspectos Técnicos Destacados

### 1. Procesamiento Recursivo del AST
El AST se procesa de forma recursiva, permitiendo estructuras anidadas complejas y precedencia adecuada de operadores.

### 2. Gestión Adecuada de Descriptores de Archivos
Todos los descriptores de archivos se gestionan cuidadosamente para prevenir fugas y asegurar redirección de E/S adecuada.

### 3. Operaciones Seguras con Señales
El manejo de señales está implementado para funcionar correctamente con operaciones en curso y procesos hijo.

### 4. Eficiencia de Memoria
La asignación dinámica se usa de forma juiciosa, con limpieza integral para prevenir fugas de memoria.

### 5. Recuperación de Errores
El shell maneja errores de forma elegante y continúa operando después de errores de sintaxis o ejecución.

---

Esta implementación de minishell demuestra una comprensión profunda de los conceptos de programación de sistemas Unix y proporciona una base sólida para entender cómo funcionan los shells internamente. El diseño modular facilita la extensión con características adicionales mientras mantiene la claridad del código y la confiabilidad.
