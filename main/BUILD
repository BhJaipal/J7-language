cc_binary(
	name = "j7",
	srcs = ["main.cpp"],
	deps= [
		"//lexer:lexer",
		"//utils:utils"
	],
	copts = [ "-I/usr/include/c++/11", "-I/usr/include/x86_64-linux-gnu/c++/11", "-std=c++20"],
	includes = [ "/usr/include/c++/11", "/usr/include/x86_64-linux-gnu/c++/11" ],
)
