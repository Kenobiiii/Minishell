#!/bin/bash

echo "=== FINAL VALIDATION TESTS ==="
echo ""

cd /home/pons/asd/Minishell

# Critical test cases
declare -a critical_tests=(
    'echo "$HOME"'"'"'$HOME'"'"''           # Main issue test
    'echo "$USER"'                          # Variable expansion
    "echo '\$USER'"                         # No expansion in single quotes
    'echo "hello world"'                    # Basic double quotes
    "echo 'hello world'"                    # Basic single quotes
    'echo "$HOME$USER"'                     # Multiple variables
    'echo "first" "second"'                 # Multiple quoted strings
)

echo "Testing critical functionality..."
for i in "${!critical_tests[@]}"; do
    test_cmd="${critical_tests[$i]}"
    echo "Test $((i+1)): $test_cmd"
    
    # Test functionality
    echo -n "Functionality: "
    echo "$test_cmd" | ./minishell 2>/dev/null | tail -2 | head -1
    
    # Test memory
    echo -n "Memory: "
    echo "$test_cmd" | valgrind --tool=memcheck --leak-check=full --suppressions=readline.supp --error-exitcode=1 ./minishell >/dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "✓ Clean"
    else
        echo "✗ Issues"
    fi
    echo ""
done

echo "=== VALIDATION COMPLETE ==="
