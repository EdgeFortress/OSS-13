#pragma once
enum comand_code
{
	login_code = 1, singin_code
};
enum result
{
	ok, login_error, signin_error, connection_error, command_code_error
};
const int start_port = 52001;