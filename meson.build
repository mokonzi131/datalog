project('datalog', 'cpp', default_options : [
    'cpp_std=c++17',
    'pkg_config_path=@0@'.format(meson.current_build_dir())
    ])

if meson.get_compiler('cpp').get_id() == 'clang'
    cpp_extra_args = ['-stdlib=libc++']
    link_extra_args = ['-stdlib=libc++', '-lc++fs']
else
    cpp_extra_args = []
    link_extra_args = []
endif

sources = ['src/util.cpp', 'src/parser/token.cpp',
    'src/parser/lexer.cpp', 'src/parser/grammar.cpp', 'src/parser/parser.cpp']

executable(
    'datalog',
    sources : sources + ['src/main.cpp'],
    cpp_args : cpp_extra_args,
    link_args : link_extra_args)

catch2 = dependency('Catch2', version: '2.9.1', method: 'pkg-config')
test_sources = ['test/test-main.cpp', 'test/test-lab1.cpp'] + sources

executable('datalog-test',
    dependencies: catch2,
    sources: test_sources,
    cpp_args: cpp_extra_args,
    link_args: link_extra_args)
