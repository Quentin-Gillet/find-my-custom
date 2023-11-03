#!/bin/sh

pass=0
failed=0

RED='\033[1;31m'
GREEN='\033[1;32m'
CLEAR='\033[0m'
ITALIC='\033[4m'

test_find(){
    got=$(.././myfind $*)
    exit_code="$?"
    expected=$(find $*)
    expected_exit_code="$?"
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

test_find testing -name "bar" -print
test_find testing -name "bar"
test_find testing/foo -print
test_find testing/foo
test_find testing/foo -name "ba?"
test_find .
test_find . -type d
test_find testing/qud -name 'foobar' -print -name 'foo*' -print
test_find testing/qud -name 'foo*' -print -name 'foobar' -print
test_find testing -type d
test_find testing/foo -name bar -o -name baz
test_find testing/foo -name bar -a -name baz
test_find testing/foo -name bar -name baz
test_find ../tests ../src
test_find testing/foo -exec pwd \; -exec echo -- {} -- \;
test_find testing/foo/* -newer testing/foo/bar
test_find testing/foo/* -newer testing/foo/baz
test_find ../. -name "*.c" -print -o -type d -print
test_find ../. -name "*.c" -o -type d -print

# TEST -PERM
test_find testing/perm -perm 777
test_find testing/perm -perm -102
test_find testing/perm -perm +004
test_find testing/perm -perm +120
test_find testing/perm -perm +004
test_find testing/perm -perm -567

# TEST -USER
test_find testing/perm -user quentin
test_find testing/perm -user 0

# TEST -GROUP
test_find testing/perm -group staff
test_find testing/perm -group 0

# TEST -d options
# TODO FIX test_find testing/perm -d .
test_find -d ../
test_find -d ../ -name "*.*" -print

# TEST -L options
test_find -L testing/symlink -print
test_find -L testing/symlink -print -o -type d -print
test_find -L . -type d

# TEST -P options
test_find -P testing/symlink -print
test_find -P testing/symlink -print -o -type d -print

echo -e "${ITALIC}Total tests:${CLEAR} $(( $pass + $failed )), ${GREEN}Pass: ${pass}${CLEAR}, ${RED}Failed: ${failed}${CLEAR}."
