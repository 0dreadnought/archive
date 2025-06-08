#include <iostream>
#include <fstream>
#include "log.hpp"
#include <string>

log::log(){

}

log::~log(){
	LogFile.close();
}

//Opens a file

bool log::create_log(std::string filename){
	LogFile.open(filename);
	if(!LogFile.is_open()){std::cout<<"Couldn't open a file";return false;}
	return true;
}

//Moves to the next line of the file

bool log::next(){
	getline(LogFile,message);
	if(!message.empty()){return true;}
	else{std::cout<<"Line is empty";return false;}
}

//Reads the message part of a log

std::string log::line(){
	std::string line;
	bool flag=false;
	//Iterates through the line
	for(int i=0;i<sizeof(message);i++){
		//Character goes to the output array if it is after the ':'
		if(message[i]==':'){flag=true;i+=2;}
		if(flag){line=line+message[i];}
	}

	return line;
}

//Reads the error part of the log

std::string log::error(){
	std::string error;
	bool flag=false;
	//Iterates through the line
	for(int i=0;i<sizeof(message);i++){
		if(message[i]=='['){flag=true;i++;}
		if(message[i]==']'){flag=false;}
		//Char goes to the output array if it is after '[' until ']' is encountered
		if(flag){error=error+message[i];}
	}
	return error;
}

//Returns the reformated log

std::string log::reformat(){
	//Message and ERROR parts are store in variables
	std::string er,li;
	bool eflag=false;
	bool mflag=false;
	//Iterates through the log
	for(int i=0;i<sizeof(message);i++){
		//combination of error() and line() functions
		if(message[i]==']'){eflag=false;}
		if(eflag){er=er+message[i];}
		if(mflag){li=li+message[i];}
		if(message[i]=='['){eflag=true;}
		if(message[i]==' '){mflag=true;}
	}
	li=li+" ("+er+')';
	return li;
}