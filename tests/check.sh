cd ..
make debug
cd tests/
./testsuite.py  ../minishell
rm -r truc
rm t bar baz
cd ..
make clean
