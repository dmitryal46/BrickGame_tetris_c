CC := gcc
CFLAGS := -Wall -Wextra -Werror -std=c11 -Wpedantic
GCOV_FLAGS := -fprofile-arcs -ftest-coverage

OS := $(shell uname -s)

ifeq ($(OS), Darwin)
	LIB_FLAGS := $(shell pkg-config --static --cflags --libs check ncurses)
else
	LIB_FLAGS := $(shell pkg-config --static --cflags --libs check ncursesw)
endif

PROJECT := tetris_cli
GUI := gui/cli/cli.c
GUI_DIR := gui/cli
CODE_DIR := brick_game/tetris
BUILD_DIR := build
DOCS_DIR := docs
TESTS_DIR := tests
REPORT_DIR := report
DIST_DIR := dist

all : install

install : tetris.a | build_dir
	$(CC) $(CFLAGS) $(PROJECT).c $(GUI) $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/$(PROJECT) $(LIB_FLAGS)

install-sanitized : tetris.a
	$(CC) $(CFLAGS) $(PROJECT).c $(GUI) $(BUILD_DIR)/tetris.a -o $(BUILD_DIR)/$(PROJECT) $(LIB_FLAGS) -fsanitize=address

tetris.a : build_dir
	$(CC) $(CFLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o

uninstall :
	rm -rf $(DOCS_DIR) $(BUILD_DIR)

rebuild : uninstall install


clean :
	rm -rf $(DOCS_DIR) $(BUILD_DIR) $(DIST_DIR) $(REPORT_DIR) $(TESTS_DIR)/tetris_test tetris.tar

dist: uninstall install
	mkdir $(DOCS_DIR)
	cp -r $(GUI_DIR)/*.c $(DOCS_DIR)
	cp -r $(GUI_DIR)/*.h $(DOCS_DIR)
	cp -r $(CODE_DIR)/*.c $(DOCS_DIR)
	cp -r $(CODE_DIR)/*.h $(DOCS_DIR)
	tar -cf tetris.tar $(DOCS_DIR)
	rm -rf $(DOCS_DIR)

dvi:
	open ../README.md

test : uninstall install
	$(CC) $(CFLAGS) $(TESTS_DIR)/*.c $(TESTS_DIR)/*.h $(BUILD_DIR)/tetris.a -o $(TESTS_DIR)/tetris_test $(LIB_FLAGS)
	./$(TESTS_DIR)/tetris_test

leaks :
	leaks -atExit -- ./$(TESTS_DIR)/tetris_cli

gcov_report : clean build_dir
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c brick_game/tetris/tetris.c -o $(BUILD_DIR)/tetris.o $(LIB_FLAGS)
	ar rc $(BUILD_DIR)/tetris.a $(BUILD_DIR)/tetris.o
	ranlib $(BUILD_DIR)/tetris.a
	rm -rf $(BUILD_DIR)/tetris.o
	$(CC) $(CFLAGS) $(TESTS_DIR)/*.c $(TESTS_DIR)/*.h  $(BUILD_DIR)/tetris.a -o $(TESTS_DIR)/tetris_test $(LIB_FLAGS) -lgcov
	./$(TESTS_DIR)/tetris_test
	rm -rf $(REPORT_DIR)
	mkdir $(REPORT_DIR)
	gcovr --html-details $(REPORT_DIR)/coverage.html
	rm -rf *.gcda *.gcno *.o
	open $(REPORT_DIR)/coverage.html

format :
	clang-format --style=file:"../materials/linters/.clang-format" -i *.c $(TESTS_DIR)/*.c $(TESTS_DIR)/*.h */*/*.c */*/*.h

check :
	clang-format --style=file:"../materials/linters/.clang-format" -n *.c */*.c */*.h */*/*.c */*/*.h

valgrind-test : test
	CK_FORK=no valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all -s --track-origins=yes $(TESTS_DIR)/tetris_test

build_dir :
	mkdir -p $(BUILD_DIR)

dist_dir :
	mkdir -p $(DIST_DIR)
