dist_doc_DATA = README.md LICENSE
bin_PROGRAMS = tabular
check_PROGRAMS = tab_test
TESTS = $(check_PROGRAMS)

tabular_SOURCES = src/tabular.c src/tab_field.c
tabular_LDADD = -lm

tab_test_SOURCES = test/tab_test.c src/tab_field.c
tab_test_LDADD = -lm

