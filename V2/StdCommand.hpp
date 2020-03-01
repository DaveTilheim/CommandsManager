#ifndef STD_COMMAND_HPP
#define STD_COMMAND_HPP
#include "Command.hpp"
#include "Node.hpp"



class StdCommand
{
private:
	static Command cexit;
	static Command ccreate;
	static Command cread;
	static Command cprint;
	static Command cupdate;
	static Command ctype;
	static Command cadd;
	static Command csub;
	static Command cmul;
	static Command cdiv;
	static Command cresult;
	static Command crepeat;
	static Command cinput;
	static Command cstr;
	static Command cif;
	static Command cbegin;
	static Command cend;
	static Command celse;
	static Command cequal;
	static Command cless;
	static Command cjump;
	static Command clabel;
	static Command cignore;
	static Command cwhile;
	static Command cfunction;
	static Command creturn;
	static Command ccall;
	static Command cret;
	static Node root;
	static int nodeCounter;
	static map<string, int> labels;
	static string exit_command(Args args);
	static string create_command(Args args);
	static string create_vector_command(Args args);
	static string read_command(Args args);
	static string read_vector_command(Args args);
	static string print_command(Args args);
	static string print_memory_command(Args args);
	static string update_command(Args args);
	static string type_command(Args args);
	static string add_command(Args args);
	static string sub_command(Args args);
	static string mul_command(Args args);
	static string div_command(Args args);
	static string result_command(Args args);
	static string repeat_command(Args args);
	static string input_command(Args args);
	static string input_command_1(Args args);
	static string str_command(Args args);
	static string if_command(Args args);
	static string begin_command_0(Args args);
	static string begin_command_1(Args args);
	static string end_command_0(Args args);
	static string end_command_1(Args args);
	static string else_command(Args args);
	static string equal_command(Args args);
	static string less_command(Args args);
	static string jump_command(Args args);
	static string jump_to_command(Args args);
	static string label_command(Args args);
	static string ignore_command(Args args) noexcept(false);
	static string while_command(Args args);
	static string end_while_command(Args args);
	static string function_command(Args args);
	static string return_command(Args args);
	static string call_command(Args args);
	static string call_from_command(Args args);
	static string end_function_command(Args args);
	static string ret_command_0(Args args);
	static string ret_command_1(Args args);
public:
	static void initStdCommands();
};



#endif
