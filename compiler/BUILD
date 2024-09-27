cc_binary(
	name = "j7",
	srcs = ["main.cpp"],
	deps= [
		"//lexer:lexer",
		"//utils:utils"
	],
	copts = [ "-I/usr/include/c++/11", "-I/usr/include/x86_64-linux-gnu/c++/11", "-I/usr/include/llvm-c-16", "-I/usr/include/llvm-16"],
	includes = [ "/usr/include/c++/11", "/usr/include/x86_64-linux-gnu/c++/11", "/usr/include/llvm-c-16", "/usr/include/llvm-16"]
)