#!/bin/sh

pass=0
failed=0

RED='\033[1;31m'
GREEN='\033[1;32m'
CLEAR='\033[0m'
ITALIC='\033[4m'

test_find(){
    got=$(.././myfind $* 2>/dev/null)
    exit_code="$?"
    expected=$(find $* 2>/dev/null)
    expected_exit_code="$?"
    if [ "$got" = "$expected" ] && [ "$exit_code" -eq "$expected_exit_code" ]; then
        pass=$(( $pass + 1))
    elif [ "$got" != "$expected" ]; then
        echo "-------------------------------"
        echo "${CLEAR}Test '$*' ${RED}failed! ${RED}Got: ${got}, ${GREEN}Expected: ${expected}${CLEAR}"
        failed=$(( $failed + 1))
        echo "-------------------------------"
    else
        echo "-------------------------------"
        echo "${RED}Wrong exit code on '$*'! ${CLEAR}Got: ${exit_code}, Expected: $expected_exit_code"
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
test_find testing/foo -execdir pwd \; -execdir echo -- {} -- \;
test_find testing/foo/* -newer testing/foo/baz
test_find ../. -name "*.c" -print -o -type d -print
test_find ../. -name "*.c" -o -type d -print
test_find . test? \! -name "foo*" -print
test_find . . -print
test_find . te* src/ -print -o -type d -print
test_find . te* src/ -print -o \! -type d -print
test_find tests.sh . -print
test_find tests.sh -print

# TEST -PERM
test_find testing/perm -perm 777
test_find testing/perm -perm -102
test_find testing/perm -perm /004
test_find testing/perm -perm -000
test_find testing/perm -perm /000
test_find testing/perm -perm /120
test_find testing/perm -perm /004
test_find testing/perm -perm -567

# TEST -USER
test_find testing/perm -user quentin
test_find testing/perm -user 0
test_find testing/perm -user test

# TEST -GROUP
test_find testing/perm -group staff
test_find testing/perm -group 0
test_find testing/perm -group test

# TEST -L options
test_find -L testing/symlink -print
test_find -L testing/symlink -print -o -type d -print
test_find -L testing/symlink/bin -print -o -type d -print
test_find -L . -type d

# TEST -P options
test_find -P testing/symlink -print
test_find -P testing/symlink -print -o -type d -print

# TEST -H options
test_find -H testing/symlink -print
test_find -H testing/symlink -print -o -type d -print
test_find -H testing -print -o -type d -print
test_find -H testing -print

# TEST ! operator
test_find testing -name "foo*" ! -name "foo"
test_find testing -name "foo*" ! -name "foo*" -print
test_find testing -name "foo*" ! -name "foo*" -print -o -type d -print
test_find testing -name "foo*" ! -name "foo*" -print -o -type d -print -o -name "foo*" -print
test_find testing -name "foo*" ! -name "foo*" -print -o -type d -print -o -name "foo*" -print -o -name "foo*" -print
test_find testing -name "foo*" ! -name "foo*" -print -o -type d -print -o -name "foo*" -print -o -name "foo*" -print -o -name "foo*" -print
test_find . '!' -name "bar"
test_find . '!' -name "bar"

# TEST () operator
test_find . \( -name bar -o -name baz \)
test_find . \( -name bar \)
test_find . \! \( -name bar -o -name baz \)
test_find . \( \( -name bar -a -type d \) -o -name baz \)

# TEST errors
test_find mdr -print
test_find . -name
test_find . -name \; testing
test_find . -name testing
test_find . mdr -name testing
test_find -d . -name
test_find testing -name "foo*" -print -o -t
test_find testing \( -name "foo*" -print
test_find testing \( -name "foo*" -print
test_find testing \) -name "foo*" -print
test_find testing -name "foo*" -print \)
test_find testing \( -name "foo*" -print \) \)
test_find . \! -name "foo*" -print
test_find . \! -name "foo*" -print \)
test_find . src \! -name "foo*" -print \)
test_find . mdr lol \! -name "foo*" -print
test_find . tests lol \! -name "foo*" -print
test_find Makefile -print
test_find . \( \( -name bar -a -type -d \) -o -name baz \)
test_find . testing -name "foo" -a -perm 999

# TEST delete
mkdir -p testing/delete
touch testing/delete/foo
mkdir -p testing/delete/bar
touch testing/delete/baz
#test_find testing/delete -name "foo" -delete
#test_find testing/delete -name "bar" -a -type d -delete
#test_find testing/delete -delete -delete

percent_color=$GREEN
if [ $failed -gt $pass ]; then
    percent_color=$RED
fi
echo "${ITALIC}Total tests: $(( $pass + $failed )),${CLEAR} ${GREEN}Pass: ${pass},${CLEAR} ${RED}Failed: ${failed}.${CLEAR} ${percent_color}Percent success: $(( $pass * 100 / ($pass + $failed) ))%${CLEAR}"
