#include <fstream>
#include <iostream>
#include <string>

using namespace std;

//namespace so all can see
namespace memory
{
	int cells[32];
	int pointer;
}


//Takes a filename and reads it
//then compresses all characters from it into 1 big string so we can parse it
//also ignores lines with comments
string getCode(string filename){
	string line;
	string code;
	ifstream readfile(filename);
	if(!readfile){
		return "failed to get file \n";
	}

	while(getline(readfile, line))
	{	
		// # is a comment here. so if the line begins with it, we just ignore it.
		if(line[0] != '#'){
		code = code + line;
		}
	
	}
	
	return code;
}

//gets the sub code between the currentchunk given until the next ']'
//this is super dumb, cus instead of doing a loop, it recurses. dumb shit
//but idk a better way
string find_sub_code_chunk(string chunk){
	int pos = -1;
	for (int i = 0; i <= chunk.length(); i++){
		if (chunk[i] == ']'){
			pos = i;	
		}
	}
	//TODO deal with -1
	string subChunk;
	subChunk = chunk.substr(0,pos);
	return subChunk;
}

int find_matching_bracket_relative_pos(string chunk) {
	int pos = -1;
	for (int i = 0; i <= chunk.length(); i++) {
		if (chunk[i] == ']'){
			pos = i;
		}
	}
	return pos;
}

//so basicallly this actually executes the brainfuck code
string execute(string chunk){

	char instruction;

	for (int i = 0; i < chunk.length(); i++){
		instruction = chunk[i];
		//cout << i << " " << instruction << "\n";

		switch(instruction){
		case '<':
			//Move pointer LEFT -1
			memory::pointer++;
			//cout << memory::pointer;
			break;
		case '>':
			//Move pointer RIGHT +1
			memory::pointer--;
			break;
		case '+':
			//Increment current cell
			memory::cells[memory::pointer]++; 
			break;
		case '-':
			//De-Increment the current cell
			memory::cells[memory::pointer]--;
			break;
		case '.':
			//Display the cell
			//TODO Make this display numbers n shit using ASCII codes
			//ostringstream oss;
			cout << char(memory::cells[memory::pointer]);
			break;

		case '[':
			//while current pointer is NOT ZERO execute the code
			if(memory::cells[memory::pointer] == 0){
				int relpos = find_matching_bracket_relative_pos(chunk);
				i += find_matching_bracket_relative_pos(chunk);
			} else {
				int relpos = find_matching_bracket_relative_pos(chunk);
				cout << chunk.substr(++i, relpos - i) << " " << relpos;
			}
			break;

		case ']':
			//If pointer is non-zero, recurse current chunk.
			if(memory::cells[memory::pointer] != 0){
				execute(chunk);
			}
		}
	}
}

//Main function
int main(int argc, char** argv)
{
	//Making sure the correct number of args are passed (0 = program, 1 = filename) - length of 2
	if(argc != 2){
		cout << "wrong number of arguments, closing" << endl;
		return -1; // Close the program cus bad
	}
	string filename = argv[1]; //GET THE FIRST ARG WHEN BIN IS CALLED
	
	//getting the code from a file, and then stroring it in one big string
	string code = getCode(filename);

	execute(code);
	cout << endl;
	return 0;
}
