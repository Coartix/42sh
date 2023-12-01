# Building

```sh
make setup #if it's the first you compile the project
make all
```

# Debugging

```
gdb -arg builddir/42sh -c 'echo test'
```

# Running tests

```sh
make test
```

# Building documentation

```sh
# enable documentation support
meson setup -Ddoc=true builddir  # --reconfigure might be needed
# build the documentation
meson compile -C builddir doxygen_doc
# open the documentation in the browser
xdg-open builddir/doxygen_doc/index.html
```

