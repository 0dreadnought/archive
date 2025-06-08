#include <iostream>
#include <string>

//Reads the message part of a log

std::string line(std::string message){
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

std::string error(std::string message){
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

std::string reformat(std::string message){
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

int main() {
    std::cout << line("[ERROR]: Invalid operation") << std::endl;
    std::cout << error("[ERROR]: Invalid operation") << std::endl;
    std::cout << reformat("[INFO]: Operation completed") << std::endl;
    return 0;
}
