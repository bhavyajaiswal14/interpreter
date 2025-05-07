#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    // Disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

    // You can use print statements as follows for debugging, they'll be visible when running tests.
    std::cerr << "Logs from your program will appear here!" << std::endl;

    if (argc < 3) {
        std::cerr << "Usage: ./your_program tokenize <filename>" << std::endl;
        return 1;
    }

    const std::string command = argv[1];

    if (command == "tokenize") {
        std::string file_contents = read_file_contents(argv[2]);
        int ret_val=0;
        int line_number=1;
        // Uncomment this block to pass the first stage
        // 
         //if (!file_contents.empty()) {
        //     std::cerr << "Scanner not implemented" << std::endl;
        //     return 1;
        // }
        for(int i=0; i<file_contents.size(); i++){
            char c = file_contents[i];
            switch(c){
                case '(':
                    std::cout<<"LEFT_PAREN ( null"<<std::endl;
                    break;

                case ')':
                    std::cout<<"RIGHT_PAREN ) null"<<std::endl;
                    break;

                case '{':
                    std::cout<<"LEFT_BRACE { null"<<std::endl;
                    break;

                case '}':
                    std::cout<<"RIGHT_BRACE } null"<<std::endl;
                    break;

                case ',':
                    std::cout<<"COMMA , null"<<std::endl;
                    break;

                case '.':
                    std::cout<<"DOT . null"<<std::endl;
                    break;

                case '-':
                    std::cout<<"MINUS - null"<<std::endl;
                    break;

                case '+':
                    std::cout<<"PLUS + null"<<std::endl;
                    break;

                case ';':
                    std::cout<<"SEMICOLON ; null"<<std::endl;
                    break;

                case '*':
                    std::cout<<"STAR * null"<<std::endl;
                    break;

                case '!':
                    //if i+1 is less than the size of the file_contents and the next character is an equal sign, then print BANG_EQUAL, otherwise print BANG
                    if(i+1<file_contents.size() && file_contents[i+1]=='='){
                        std::cout<<"BANG_EQUAL != null"<<std::endl;
                        i++;// skip the next '=' character since it has already been processed
                    }else{
                        std::cout<<"BANG ! null"<<std::endl;
                    }
                    break;

                case '=':
                //if i+1 is less than the size of the file_contents and the next character is an equal sign, then print EQUAL_EQUAL, otherwise print EQUAL
                   if(i+1<file_contents.size() && file_contents[i+1]=='='){
                        std::cout<<"EQUAL_EQUAL == null"<<std::endl;
                        i++;// skip the next '=' character since it has already been processed
                    }else{
                        std::cout<<"EQUAL = null"<<std::endl;
                    }
                    break;

                case '<':
                //if i+1 is less than the size of the file_contents and the next character is an equal sign, then print LESS_EQUAL, otherwise print LESS
                    if(i+1<file_contents.size() && file_contents[i+1]=='='){
                        std::cout<<"LESS_EQUAL <= null"<<std::endl;
                        i++;// skip the next '=' character since it has already been processed
                    }else{
                        std::cout<<"LESS < null"<<std::endl;
                    }
                    break;

                case '>':
                //if i+1 is less than the size of the file_contents and the next character is an equal sign, then print GREATER_EQUAL, otherwise print GREATER
                    if(i+1<file_contents.size() && file_contents[i+1]=='='){
                        std::cout<<"GREATER_EQUAL >= null"<<std::endl;
                        i++;// skip the next '=' character since it has already been processed
                    }else{
                        std::cout<<"GREATER > null"<<std::endl;
                    }
                    break;

                case '/':
                    if(i+1<file_contents.size() && file_contents[i+1]=='/'){
                        i++;//skip the next '/' character
                        //skip all characters until the end of the line
                        while(i<file_contents.size() && file_contents[i]!='\n'){
                            i++;
                        }
                        line_number++;
                    }else{
                        std::cout<<"SLASH / null"<<std::endl;
                    }
                    break;
                
                case '"': {
                    int start = i;
                    i++; // Move past the opening quote
                    
                        while (i < file_contents.size() && file_contents[i] != '"') {
                            if (file_contents[i] == '\n') {
                                line_number++;
                            }
                            i++;
                        }
                    
                        if (i >= file_contents.size()) { // Unterminated string
                            std::cerr << "[line " << line_number << "] Error: Unterminated string." << std::endl;
                            ret_val=65; // Error code for unterminated string
                        } else {
                            std::string value = file_contents.substr(start + 1, i - start - 1);
                            std::string lexeme = file_contents.substr(start, i - start + 1); // Include quotes
                            std::cout << "STRING " << lexeme << " " << value << std::endl;
                            //i++; // Move past the closing quote
                        }
                        break;
                    }
                    
                case '0': case '1': case '2': case '3': case '4': 
                case '5': case '6': case '7': case '8': case '9': {
                    int num = i;
                    
                        //parse digits before decimal
                        while (i + 1 < file_contents.size() &&
                              (file_contents[i + 1] >= '0' && file_contents[i + 1] <= '9')) {
                            i++;
                        }
                    
                        //parse decimal and digits after
                        if (i + 1 < file_contents.size() && file_contents[i + 1] == '.' &&
                            i + 2 < file_contents.size() &&
                            (file_contents[i + 2] >= '0' && file_contents[i + 2] <= '9')) {
                            i += 2; // Skip '.' and one digit after
                            while (i + 1 < file_contents.size() &&
                                  (file_contents[i + 1] >= '0' && file_contents[i + 1] <= '9')) {
                                i++;
                            }
                        }
                    
                        std::string lexeme = file_contents.substr(num, i - num + 1);
                    
                        //convert to double and then to normalized string
                        //eg: parsed=stod(lexeme) will convert "42.000" to 42.0
                        //and then to_string will convert it to "42.000000" (or similar)
                        double parsed = std::stod(lexeme);
                        std::string value = std::to_string(parsed);
                    
                        //truncate trailing zeros after decimal point
                        //ensure at least one digit after the dot remains (i.e., "42.0")
                        if (value.find('.') != std::string::npos) {
                            //remove trailing zeros
                            while (value.back() == '0') {
                                value.pop_back();
                            }
                            //if the last char is a '.', remove it and add one '0'
                            if (value.back() == '.') {
                                value.pop_back();
                                value += ".0";
                            }
                        }
                    
                        std::cout << "NUMBER " << lexeme << " " << value << std::endl;
                        break;
                }

                //this is important because whitespace characters are typically ignored in lexical analysis (the process of breaking text into tokens)
                //without these cases, whitespace characters would fall through to the default case, which would report them as unexpected characters
                case ' ':
                case '\t':
                case '\r':
                    break;
                case '\n':
                    line_number++;
                    break;
                default:
                  if((c>='a' && c<='z') || (c>='A' && c<='Z') || c=='_'){
                    int start=i;
                    while(i+1<file_contents.size()){
                        char next_char=file_contents[i+1];
                        if((next_char>='a' && next_char<='z') ||
                           (next_char>='A' && next_char<='Z') ||
                           (next_char>='0' && next_char<='9') ||
                           next_char=='_'){
                            i++;
                    }else{
                        break;
                    }
                }
                std::string lexeme=file_contents.substr(start,i-start+1);

                //check if the lexeme is a reserved word or an identifier
                static const std::unordered_map<std::string, std::string> reserved_words={
                    {"and", "AND"}, {"class", "CLASS"}, {"else", "ELSE"}, {"false", "FALSE"},
                    {"for", "FOR"}, {"fun", "FUN"}, {"if", "IF"}, {"nil", "NIL"},
                    {"or", "OR"}, {"print", "PRINT"}, {"return", "RETURN"}, {"super", "SUPER"},
                    {"this", "THIS"}, {"true", "TRUE"}, {"var", "VAR"}, {"while", "WHILE"}
                };

                //check if the lexeme is in the reserved words map
                auto keywords=reserved_words.find(lexeme);

                //if the lexeme is in the reserved words map, print the corresponding value
                if(keywords!=reserved_words.end()){
                    std::cout<<keywords->second<<" "<<lexeme<<" null"<<std::endl;
                }else{
                    //if the lexeme is not in the reserved words map, print IDENTIFIER
                    std::cout<<"IDENTIFIER "<<lexeme<<" null"<<std::endl;
                }
            }else{
                std::cerr << "[line " <<line_number<< "] Error: Unexpected character: " << c << std::endl;
                    ret_val = 65;
                }
                break;
            }
        }
        std::cout << "EOF  null" << std::endl; // Placeholder, remove this line when implementing the scanner
        return ret_val;
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }
}

std::string read_file_contents(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error reading file: " << filename << std::endl;
        std::exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    return buffer.str();
}
