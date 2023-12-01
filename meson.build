project(
    '42sh',
    'c',
    version: '0.1',
    license: 'MIT',
    default_options: [
        'c_std=c99',
        'warning_level=3',
        'default_library=static',

        # these options would not be suitable in a
        # production configuration, but development easier
        'b_sanitize=address',
        'b_lundef=false',
        'buildtype=debug',
        'werror=true',
    ],
)


# print a summary of the build configuration before each build starts
summary({
    'optimization': get_option('optimization'),
    'debug': get_option('debug'),
    'b_sanitize': get_option('b_sanitize'),
    'warning_level': get_option('warning_level'),
    'werror': get_option('werror'),
})


# add some project-wide flags
#flags before switching to gcc
#cflags = ['-D_POSIX_C_SOURCE=200809L']
cflags = ['-D_POSIX_C_SOURCE=200809L']
add_project_arguments(cflags, language: 'c')


# interpret the meson.build in this directory
all_sources = []
main_file = []
include_dir = ['src', 'tests']
subdir('src')


# find readline, which some of the modules below link with
cc = meson.get_compiler('c')
readline_dep = cc.find_library('readline', required: true)


bin = executable(
    '42sh',
    all_sources,
    main_file,
    include_directories: include_dir,
    install: true,
    dependencies: [readline_dep],
)

# Test section

criterion_dep = cc.find_library('criterion', required: false)
sources_test = all_sources
subdir('tests')


bin_test = executable(
    '42sh_test',
    sources_test,
    link_args: '-lcriterion',
    include_directories: include_dir,
    install: true,
   dependencies: [readline_dep, criterion_dep],
)


custom_target(
    'check',
    command: [bin_test] + get_option('check_args'),
    #command: [bin] + ['-c', 'echo coucou'],
    # give the testsuite exclusive access to the terminal, which enables
    # pretty colors and live testsuite results (meson buffers it all otherwise)
    console: true,
    # eventhough this rule output no file, we have to the output file of the rule is mandatory, but as
    # but it doesn't matter as the rule is always built
    output: 'check',
    build_always_stale: true,
    # if this option is enabled, tests will run for all builds
    build_by_default: get_option('force_check'),
)

if get_option('doc')
    subdir('doc')
endif

