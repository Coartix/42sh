# run all or run command to compile the project (prod et test bineries)
# run setup before if it's the first time you compile the project
# run clang to clang-format the all project
# run clean to remove all binaries

all:
	ninja -C builddir 42sh

42sh: all

run: all
	./builddir/42sh

check: criterion pharaoh

build_test:
	ninja -C builddir 42sh_test

test: clear criterion pharaoh

criterion: build_test
	./builddir/42sh_test

pharaoh: 42sh
	pharaoh tests

setup:
	env CC=gcc meson setup -Ddoc=true builddir

build_doc:
	meson compile -C builddir doxygen_doc

doc: build_doc
	xdg-open builddir/doxygen_doc/index.html

clang:
	find . -type f -name '*.[ch]' -exec clang-format -style=file -i {} \;

clear:
	clear

clean:
	ninja -C builddir clean

.PHONY: all 42sh run check test criterion pharaoh setup build_doc doc clang clear clean
