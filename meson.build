project('datalog', 'cpp', default_options : ['cpp_std=c++17'])

if meson.get_compiler('cpp').get_id() == 'clang'
    cpp_extra_args = ['-stdlib=libc++']
    link_extra_args = ['-stdlib=libc++', '-lc++fs']
else
    cpp_extra_args = []
    link_extra_args = []
endif

sources = ['src/main.cpp', 'src/util.cpp', 'src/parser/token.cpp',
    'src/parser/lexer.cpp', 'src/parser/grammar.cpp', 'src/parser/parser.cpp']
executable(
    'datalog',
    sources : sources,
    cpp_args : cpp_extra_args,
    link_args : link_extra_args)