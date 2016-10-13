#pragma once
enum comand_code
{
	LOGIN_CODE = 1,
	SIGNIN_CODE = 2
};
enum result
{
	OK = 0, 
	LOGIN_ERROR = 1, 
	SIGNIN_ERROR = 2, 
	CONNECTION_ERROR = 3, 
	COMMAND_CODE_ERROR = 4
};
const int PORT = 52001;