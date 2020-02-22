#ifndef STD_COMMAND_HPP
#define STD_COMMAND_HPP
#include "Command.hpp"
#include "Node.hpp"

class StdCommand
{
private:
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
	static Node root;
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
	static string str_command(Args args);
public:
	static void initStdCommands();
};

#endif
