#!/bin/bash

# COMPREHENSIVE TESTER FOR SPECIFIC MINISHELL SCENARIOS
# Author: GitHub Copilot
# Date: $(date)

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
PURPLE='\033[0;35m'
NC='\033[0m'

# Configuration
MINISHELL="./minishell"
TOTAL_PASSED=0
TOTAL_FAILED=0
FAILED_TESTS=()

# Helper function to print test headers
print_header() {
    echo -e "\n${PURPLE}╔════════════════════════════════════════════════════════════╗${NC}"
    echo -e "${PURPLE}║${NC}                    ${CYAN}$1${NC}                    ${PURPLE}║${NC}"
    echo -e "${PURPLE}╚════════════════════════════════════════════════════════════╝${NC}\n"
}

# Helper function to run interactive tests that require signals
run_interactive_test() {
    local test_name="$1"
    local command="$2"
    local signal="$3"
    local expected_exit_code="$4"
    local description="$5"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Test signal behavior by running sleep and then checking exit status
    cat > /tmp/signal_test.txt << 'EOF'
sleep 5
echo $?
exit
EOF
    
    # Start minishell in background
    timeout 10s $MINISHELL < /tmp/signal_test.txt > /tmp/signal_output.txt 2>&1 &
    minishell_pid=$!
    
    # Wait a moment for sleep to start
    sleep 2
    
    # Send SIGQUIT (Ctrl+\)
    kill -QUIT $minishell_pid 2>/dev/null
    
    # Wait for process to finish
    wait $minishell_pid 2>/dev/null
    
    # Check the output for exit status
    output=$(cat /tmp/signal_output.txt 2>/dev/null)
    exit_status=$(echo "$output" | grep -o "13[0-9]" | head -1)
    
    if [[ "$exit_status" = "$expected_exit_code" ]]; then
        echo -e "${GREEN}✅ PASSED - Exit status is $exit_status${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected exit status $expected_exit_code, got: $exit_status${NC}"
        echo -e "${RED}Full output: $output${NC}"
        FAILED_TESTS+=("$test_name: Expected exit status $expected_exit_code, got $exit_status")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/signal_test.txt /tmp/signal_output.txt
    echo "----------------------------------------"
}

# Helper function to run simple command tests
run_simple_test() {
    local test_name="$1"
    local command="$2"
    local expected_output="$3"
    local description="$4"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Create input for minishell
    echo -e "$command\nexit" > /tmp/minishell_input.txt
    
    # Run the test
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract the relevant output (remove prompts and exit messages)
    # Look for the line that contains our command output
    clean_output=$(echo "$output" | sed '/\$Minishell.*>/d' | sed '/^exit$/d' | sed '/^$/d' | head -1)
    
    if [[ "$clean_output" = "$expected_output" ]]; then
        echo -e "${GREEN}✅ PASSED - Output: '$clean_output'${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected: '$expected_output', Got: '$clean_output'${NC}"
        FAILED_TESTS+=("$test_name: Expected '$expected_output', got '$clean_output'")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
}

# Helper function to test error conditions
run_error_test() {
    local test_name="$1"
    local command="$2"
    local expected_behavior="$3"
    local description="$4"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Get current directory to verify we don't move
    current_dir=$(pwd)
    
    # Create input for minishell
    echo -e "$command\npwd\nexit" > /tmp/minishell_input.txt
    
    # Run the test
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract the pwd output specifically
    pwd_result=$(echo "$output" | grep "$current_dir" | head -1)
    
    # Check if error message appeared AND we stayed in same directory
    if echo "$output" | grep -q "HOME not set\|cd:" && [[ -n "$pwd_result" ]]; then
        echo -e "${GREEN}✅ PASSED - Error detected: cd failed correctly${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - No error detected or directory changed unexpectedly${NC}"
        echo -e "${RED}Output: $output${NC}"
        FAILED_TESTS+=("$test_name: cd should fail when HOME is unset")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
}

# Helper function to test export concatenation
run_export_test() {
    local test_name="$1"
    local setup_cmd="$2"
    local concat_cmd="$3"
    local check_cmd="$4"
    local expected="$5"
    local description="$6"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Create input for minishell
    cat > /tmp/minishell_input.txt << EOF
$setup_cmd
$concat_cmd
$check_cmd
exit
EOF
    
    # Run the test
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract the last echo output
    result=$(echo "$output" | grep -v "exit" | grep -v "\$Minishell>" | tail -1)
    
    if [[ "$result" = "$expected" ]]; then
        echo -e "${GREEN}✅ PASSED - Variable concatenated correctly: '$result'${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected: '$expected', Got: '$result'${NC}"
        FAILED_TESTS+=("$test_name: Expected '$expected', got '$result'")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
}

# Helper function to test OLDPWD update
run_oldpwd_test() {
    local test_name="$1"
    local description="$2"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Get current directory
    current_dir=$(pwd)
    
    # Test OLDPWD update by changing directory
    cat > /tmp/minishell_input.txt << EOF
echo "Before cd - OLDPWD: \$OLDPWD"
cd /tmp
echo "After cd - OLDPWD: \$OLDPWD"
pwd
exit
EOF
    
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract OLDPWD values before and after cd
    before_cd=$(echo "$output" | grep "Before cd" | sed 's/.*OLDPWD: //')
    after_cd=$(echo "$output" | grep "After cd" | sed 's/.*OLDPWD: //')
    current_pwd=$(echo "$output" | grep "/tmp" | head -1)
    
    echo -e "${CYAN}Debug info:${NC}"
    echo -e "${CYAN}  Current dir: $current_dir${NC}"
    echo -e "${CYAN}  OLDPWD before cd: '$before_cd'${NC}"
    echo -e "${CYAN}  OLDPWD after cd: '$after_cd'${NC}"
    echo -e "${CYAN}  PWD after cd: '$current_pwd'${NC}"
    
    # Check if OLDPWD changed from the previous directory to current directory
    if [[ "$after_cd" = "$current_dir" ]] && [[ "$after_cd" != "$before_cd" ]] && [[ -n "$current_pwd" ]]; then
        echo -e "${GREEN}✅ PASSED - OLDPWD updated correctly from '$before_cd' to '$after_cd'${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - OLDPWD not updated correctly${NC}"
        echo -e "${RED}Expected OLDPWD to change from '$before_cd' to '$current_dir'${NC}"
        echo -e "${RED}But got: '$after_cd'${NC}"
        FAILED_TESTS+=("$test_name: OLDPWD should be updated when changing directories")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
}

# Helper function to test exit with letters
run_exit_test() {
    local test_name="$1"
    local exit_command="$2"
    local description="$3"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Create input for minishell
    echo -e "$exit_command" > /tmp/minishell_input.txt
    
    # Run the test and check if it exits
    start_time=$(date +%s)
    timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1 > /tmp/exit_output.txt
    exit_code=$?
    end_time=$(date +%s)
    duration=$((end_time - start_time))
    
    # Get the output to check for error message
    output=$(cat /tmp/exit_output.txt 2>/dev/null)
    
    # Check if it exits AND shows numeric argument required error
    if [[ $exit_code -ne 124 ]] && [[ $duration -lt 5 ]] && echo "$output" | grep -q "numeric argument required"; then
        echo -e "${GREEN}✅ PASSED - Minishell exited with proper error message${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected exit with 'numeric argument required' error${NC}"
        echo -e "${RED}Exit code: $exit_code, Duration: ${duration}s${NC}"
        echo -e "${RED}Output: $output${NC}"
        FAILED_TESTS+=("$test_name: Should exit with numeric argument error")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/minishell_input.txt /tmp/exit_output.txt
    echo "----------------------------------------"
}

# Helper function to test heredoc with Ctrl+C
run_heredoc_test() {
    local test_name="$1"
    local description="$2"
    
    echo -e "${BLUE}Testing: $test_name${NC}"
    echo -e "${YELLOW}Description: $description${NC}"
    
    # Test heredoc Ctrl+C behavior
    cat > /tmp/heredoc_test.sh << 'EOF'
#!/bin/bash
# Start heredoc and then interrupt it
timeout 5s ./minishell << 'INPUT' &
cat << eof
test line 1
INPUT
PID=$!
sleep 1
# Send SIGINT (Ctrl+C)
kill -INT $PID 2>/dev/null
wait $PID 2>/dev/null
echo $?
EOF
    
    chmod +x /tmp/heredoc_test.sh
    result=$(/tmp/heredoc_test.sh 2>/dev/null)
    
    # Check if process was interrupted (exit code 130 = 128 + SIGINT(2))
    if [[ "$result" = "130" ]] || [[ "$result" = "2" ]]; then
        echo -e "${GREEN}✅ PASSED - Heredoc properly interrupted with Ctrl+C${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Heredoc not properly interrupted by Ctrl+C${NC}"
        echo -e "${RED}Expected exit code 130 or 2, got: $result${NC}"
        FAILED_TESTS+=("$test_name: Heredoc not interrupted by Ctrl+C")
        ((TOTAL_FAILED++))
    fi
    
    rm -f /tmp/heredoc_test.sh
    echo "----------------------------------------"
}

# Main test execution
main() {
    print_header "COMPREHENSIVE MINISHELL TESTER"
    
    echo -e "${CYAN}Testing specific scenarios as requested...${NC}\n"
    
    # Check if minishell exists
    if [[ ! -f "$MINISHELL" ]]; then
        echo -e "${RED}❌ Minishell executable not found: $MINISHELL${NC}"
        echo "Please compile your minishell first with 'make'"
        exit 1
    fi
    
    # Test 1: Ctrl+\ during sleep 5 should return exit status 131
    print_header "TEST 1: SIGQUIT (Ctrl+\\) EXIT STATUS"
    run_interactive_test \
        "Sleep with SIGQUIT" \
        "sleep 5" \
        "SIGQUIT" \
        "131" \
        "When sleep 5 receives Ctrl+\\, exit status should be 131"
    
    # Test 2: unset HOME and then cd should give error
    print_header "TEST 2: CD WITHOUT HOME VARIABLE"
    echo -e "${BLUE}Testing: CD after unset HOME${NC}"
    echo -e "${YELLOW}Description: After unsetting HOME, cd should give error and not change directory${NC}"
    
    # First try the command separately to avoid syntax error
    cat > /tmp/minishell_input.txt << 'EOF'
unset HOME
cd
pwd
exit
EOF
    
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    current_dir=$(pwd)
    
    # Check if we get a proper cd error and stay in same directory
    if echo "$output" | grep -q "HOME not set\|cd.*HOME" && echo "$output" | grep -q "$current_dir"; then
        echo -e "${GREEN}✅ PASSED - cd failed correctly when HOME is unset${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - cd should fail when HOME is unset${NC}"
        echo -e "${RED}Output: $output${NC}"
        FAILED_TESTS+=("CD after unset HOME: Should fail but didn't")
        ((TOTAL_FAILED++))
    fi
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
    
    # Test 3: echo $NONEXISTENT should output empty line
    print_header "TEST 3: UNDEFINED VARIABLE EXPANSION"
    echo -e "${BLUE}Testing: Echo undefined variable${NC}"
    echo -e "${YELLOW}Description: Echoing undefined variable should output empty line${NC}"
    
    # Test undefined variable expansion
    echo -e "echo \$NONEXISTENT_VARIABLE\nexit" > /tmp/minishell_input.txt
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract just the echo output line, filtering out prompts and exit
    result=$(echo "$output" | sed '/\$Minishell.*>/d' | sed '/^exit$/d' | grep -v '^$' | head -1)
    
    if [[ -z "$result" ]] || [[ "$result" = "" ]]; then
        echo -e "${GREEN}✅ PASSED - Undefined variable expands to empty string${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected empty output, got: '$result'${NC}"
        FAILED_TESTS+=("Echo undefined variable: Expected empty, got '$result'")
        ((TOTAL_FAILED++))
    fi
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
    
    # Test 4: export x+= debe concatenar a la variable x
    print_header "TEST 4: EXPORT CONCATENATION (x+=)"
    echo -e "${BLUE}Testing: Export concatenation${NC}"
    echo -e "${YELLOW}Description: export x+= should concatenate to existing variable x${NC}"
    
    # Test export concatenation
    cat > /tmp/minishell_input.txt << 'EOF'
export x=hello
export x+=world
echo $x
exit
EOF
    
    output=$(timeout 5s $MINISHELL < /tmp/minishell_input.txt 2>&1)
    
    # Extract the echo output
    result=$(echo "$output" | sed '/\$Minishell.*>/d' | sed '/^exit$/d' | grep -v '^$' | tail -1)
    
    if [[ "$result" = "helloworld" ]]; then
        echo -e "${GREEN}✅ PASSED - Export += concatenation works: '$result'${NC}"
        ((TOTAL_PASSED++))
    else
        echo -e "${RED}❌ FAILED - Expected 'helloworld', got: '$result'${NC}"
        FAILED_TESTS+=("Export concatenation: Expected 'helloworld', got '$result'")
        ((TOTAL_FAILED++))
    fi
    rm -f /tmp/minishell_input.txt
    echo "----------------------------------------"
    
    # Test 5: OLDPWD update verification
    print_header "TEST 5: OLDPWD UPDATE"
    run_oldpwd_test \
        "OLDPWD update check" \
        "Verify that OLDPWD is updated when changing directories"
    
    # Test 6: exit with letters should exit minishell
    print_header "TEST 6: EXIT WITH LETTERS"
    run_exit_test \
        "Exit with letters" \
        "exit abc" \
        "exit command with letters should still exit minishell"
    
    # Test 7: heredoc with Ctrl+C should exit, not stay in heredoc
    print_header "TEST 7: HEREDOC CTRL+C BEHAVIOR"
    run_heredoc_test \
        "Heredoc Ctrl+C behavior" \
        "In heredoc, Ctrl+C should exit, not stay in heredoc prompt"
    
    # Summary
    print_header "TEST SUMMARY"
    echo -e "${GREEN}Tests passed: $TOTAL_PASSED${NC}"
    echo -e "${RED}Tests failed: $TOTAL_FAILED${NC}"
    echo -e "${BLUE}Total tests: $((TOTAL_PASSED + TOTAL_FAILED))${NC}"
    
    if [[ $TOTAL_FAILED -gt 0 ]]; then
        echo -e "\n${RED}Failed tests:${NC}"
        for failed_test in "${FAILED_TESTS[@]}"; do
            echo -e "${RED}  ❌ $failed_test${NC}"
        done
        echo -e "\n${YELLOW}💡 Tips for debugging:${NC}"
        echo -e "${YELLOW}  • Check signal handling in your code${NC}"
        echo -e "${YELLOW}  • Verify exit status calculation (128 + signal_number)${NC}"
        echo -e "${YELLOW}  • Test variable expansion manually${NC}"
        echo -e "${YELLOW}  • Check export concatenation logic${NC}"
        echo -e "${YELLOW}  • Verify OLDPWD is set when changing directories${NC}"
        echo -e "${YELLOW}  • Test heredoc signal handling${NC}"
        exit 1
    else
        echo -e "\n${GREEN}🎉 All tests passed! Your minishell is working correctly for these scenarios.${NC}"
        exit 0
    fi
}

# Cleanup function
cleanup() {
    rm -f /tmp/minishell_input.txt /tmp/test_signal.sh /tmp/check_exit.sh 
    rm -f /tmp/exit_output.txt /tmp/heredoc_test.sh
}

# Set up cleanup on exit
trap cleanup EXIT

# Run the tests
main "$@"
