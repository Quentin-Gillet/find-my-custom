#!/bin/sh

pass=0
failed=0

RED='\033[1;31m'
GREEN='\033[1;32m'
CLEAR='\033[0m'
ITALIC='\033[4m'

test_find(){
    expected_exit_code="$1"
    shift
    got=$(.././myfind $*)
    exit_code="$?"
    expected=$(find $*)
    if [ "$got" = "$expected" ] && [ "$exit_code" -eq "$expected_exit_code" ]; then
        pass=$(( $pass + 1))
    elif [ "$got" != "$expected" ]; then
        echo "-------------------------------"
        echo -e "${CLEAR}Test '$*' ${RED}failed! ${RED}Got: ${got}, ${GREEN}Expected: ${expected}${CLEAR}"
        failed=$(( $failed + 1))
        echo "-------------------------------"
    else
        echo "-------------------------------"
        echo -e "${RED}Wrong exit code on '$*'! ${CLEAR}Got: ${exit_code}, Expected: $expected_exit_code"
        failed=$(( $failed + 1))
        echo "-------------------------------"
    fi
}

# TEST FIND SUCCESS

cd tests

test_find 0 testing -name "bar" -print
test_find 0 testing -name "bar"
test_find 0 testing/foo -print
test_find 0 testing/foo
test_find 0 testing/foo -name "ba?"
test_find 0 .
test_find 0 testing/qud -name 'foobar' -print -name 'foo*' -print
test_find 0 testing/qud -name 'foo*' -print -name 'foobar' -print
test_find 0 testing -type d
test_find 0 testing/foo -name bar -o -name baz
test_find 0 testing/foo -name bar -a -name baz
test_find 0 testing/foo -name bar -name baz

echo -e "${ITALIC}Total tests:${CLEAR} $(( $pass + $failed )), ${GREEN}Pass: ${pass}${CLEAR}, ${RED}Failed: ${failed}${CLEAR}."
