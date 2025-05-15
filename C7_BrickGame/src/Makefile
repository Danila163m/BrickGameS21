
CC = gcc -g -Wall -Werror -Wextra -std=c11
PATH_BACK = brick_game/tetris/
PATH_FRONT = gui/cli/
PATH_TEST = tests/


# Makefile со стандартным набором целей для GNU-программ: all, install, uninstall, clean, dvi, dist, test, gcov_report

all: install run

install:
	@mkdir -p ../build
	@$(CC) $(PATH_BACK)*.c $(PATH_FRONT)*.c -o ../build/Tetris -lncurses

uninstall:
	@rm -rf ../build/Tetris

clean: uninstall

dvi:

	@doxygen Doxyfile
	
	@xdg-open doc/html/index.html

test :
	$(CC) $(PATH_TEST)*.c $(PATH_BACK)/backend.c -o $(PATH_TEST)/test -lncurses -lcheck -lm -lcrypt -pthread -lsubunit
	./tests/test
	rm tests/test


gcov_report : $(TARGET)
	@echo "┏=========================================┓"
	@echo "┃     Generating your html report ...     ┃"
	@echo "┗=========================================┛"
	@$(CC) --coverage -fprofile-arcs -ftest-coverage  $(PATH_BACK)/backend.c $(PATH_TEST)*.c -o ./$(PATH_TEST)/test_gcov -lncurses -lcheck -lm -lcrypt -pthread -lsubunit
	@./$(PATH_TEST)/test_gcov 
	@lcov -t "test_coverage" -o test_coverage.info -c -d .
	@genhtml -o report test_coverage.info
	@xdg-open report/index.html
	@rm -rf tests/*.gcda tests/*.gcno

dist:
	@mkdir -p ../dist
	@cp -r ../src/* ../dist/
	@tar -czf tetris.tar.gz ../dist
	@rm -rf ../dist

run:
	@../build/Tetris

style_check:
	@clang-format -n $(PATH_BACK)*.c $(PATH_BACK)*.h $(PATH_FRONT)*.c $(PATH_FRONT)*.h
style_change:
	@clang-format -i $(PATH_BACK)*.c $(PATH_BACK)*.h $(PATH_FRONT)*.c $(PATH_FRONT)*.h
