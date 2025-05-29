#!/bin/bash

# Comprehensive quote testing script
# Tests both functionality and memory leaks

echo "=== MINISHELL QUOTE TESTING SUITE ==="
echo "Testing functionality and memory leaks for various quote scenarios"
echo ""

# Test cases array
declare -a test_cases=(
    'echo "$HOME"'"'"'$HOME'"'"''           # Mixed quotes - main issue
    'echo "hello world"'                    # Simple double quotes
    "echo 'hello world'"                    # Simple single quotes
    'echo "$USER"'                          # Variable in double quotes
    "echo '\$USER'"                         # Variable in single quotes (no expansion)
    'echo "$HOME/test"'                     # Variable with text in double quotes
    "echo '\$HOME/test'"                    # Variable with text in single quotes
    'echo ""'                               # Empty double quotes
    "echo ''"                               # Empty single quotes
    'echo "nested '"'"'quotes'"'"'"'        # Nested quotes
    'echo "first" "second"'                 # Multiple double quoted strings
    "echo 'first' 'second'"                # Multiple single quoted strings
    'echo "mixed" '"'"'quotes'"'"''         # Mixed quote types
    'echo "$HOME$USER"'                     # Multiple variables
    "echo '\$HOME\$USER'"                   # Multiple variables in single quotes
    'echo "path: $HOME"'                    # Text with variable
)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

run_test() {
    local test_cmd="$1"
    local test_num="$2"
    
    echo -e "${YELLOW}Test $test_num:${NC} $test_cmd"
    
    # Run with bash for expected output
    echo "Expected (bash):"
    echo "$test_cmd" | bash
    
    # Run with minishell
    echo "Minishell output:"
    echo "$test_cmd" | ./minishell 2>/dev/null
    
    # Run memory leak test
    echo "Memory leak check:"
    echo "$test_cmd" | valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp --error-exitcode=1 ./minishell >/dev/null 2>&1
    
    if [ $? -eq 0 ]; then
        echo -e "${GREEN}✓ No memory leaks detected${NC}"
    else
        echo -e "${RED}✗ Memory issues detected${NC}"
        echo "Detailed valgrind output:"
        echo "$test_cmd" | valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp ./minishell 2>&1 | head -20
    fi
    
    echo "----------------------------------------"
    echo ""
}

# Change to minishell directory
cd /home/pons/asd/Minishell

# Compile first
echo "Compiling minishell..."
make clean && make
echo ""

# Run all tests
for i in "${!test_cases[@]}"; do
    run_test "${test_cases[$i]}" "$((i+1))"
done

echo "=== TESTING COMPLETE ==="
