#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

//simple token structure
struct SimpleToken {
    std::string type;
    std::string lexeme;
    std::string literal;
};

//forward declarations for recursive parsing
std::string parse_expression(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_equality(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_comparison(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_additive(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_term(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_unary(const std::vector<SimpleToken>& tokens, int& current);
std::string parse_primary(const std::vector<SimpleToken>& tokens, int& current);


//parse equality operations (==, !=)
std::string parse_equality(const std::vector<SimpleToken>& tokens, int& current) {
    std::string expr = parse_comparison(tokens, current); // Start with comparison parsing (higher precedence)
    
    while(current < tokens.size() && 
          (tokens[current].type == "EQUAL_EQUAL" || tokens[current].type == "BANG_EQUAL")) {
        
        std::string operator_symbol = (tokens[current].type == "EQUAL_EQUAL") ? "==" : "!=";
        current++; // consume the operator token
        std::string right_expr = parse_comparison(tokens, current); // parse the right operand

        if(right_expr.empty()){
            std::cerr<<"[line " << current << "] Error at end: Expected expression '" << operator_symbol << "'." << std::endl;
            std::exit(65);
        }

        expr = "(" + operator_symbol + " " + expr + " " + right_expr + ")";
    }
    
    return expr;
}

//parse comparison operations (>, <, >=, <=)
std::string parse_comparison(const std::vector<SimpleToken>& tokens, int& current) {
    std::string expr = parse_additive(tokens, current); // Start with additive parsing (higher precedence)
    
    while(current < tokens.size() && 
          (tokens[current].type == "GREATER" || tokens[current].type == "LESS" ||
           tokens[current].type == "GREATER_EQUAL" || tokens[current].type == "LESS_EQUAL")) {
        
        std::string operator_symbol;
        if (tokens[current].type == "GREATER") operator_symbol = ">";
        else if (tokens[current].type == "LESS") operator_symbol = "<";
        else if (tokens[current].type == "GREATER_EQUAL") operator_symbol = ">=";
        else if (tokens[current].type == "LESS_EQUAL") operator_symbol = "<=";
        
        current++; // consume the operator token
        std::string right_expr = parse_additive(tokens, current); // parse the right operand

        if(right_expr.empty()){
            std::cerr<<"[line " << current << "] Error at end: Expected expression '" << operator_symbol << "'." << std::endl;
            std:::exit(65);
        }

        expr = "(" + operator_symbol + " " + expr + " " + right_expr + ")";
    }
    
    return expr;
}

//parse additive expressions (+, -)
std::string parse_additive(const std::vector<SimpleToken>& tokens, int& current){
    std::string expr=parse_term(tokens, current);//start with term parsing
    while(tokens.size()>current &&
        (tokens[current].type=="PLUS" || tokens[current].type=="MINUS")){
            std::string operator_symbol=(tokens[current].type=="PLUS")? "+":"-";//determine the operator symbol based on the token type
            current++;//consume or move past the operator token
            std::string right_expr=parse_term(tokens, current);//parse the right operand

            if(right_expr.empty()){
            std::cerr<<"[line " << current << "] Error at end: Expected expression '" << operator_symbol << "'." << std::endl;
            std::exit(65);
            }
        
            expr = "(" + operator_symbol + " " + expr + " " + right_expr + ")";//combine the left and right expressions with the operator
        }
    return expr;
}

//parse multiplication and division terms
std::string parse_term(const std::vector<SimpleToken>& tokens, int& current){
    std::string expr = parse_unary(tokens, current);

    while(tokens.size()>current &&
        (tokens[current].type=="STAR" || tokens[current].type=="SLASH")){
            std::string operator_symbol=(tokens[current].type=="STAR")? "*" : "/";//determine the operator symbol based on the token type
            current++;//consume or move past the operator token
            std::string right_expr = parse_unary(tokens, current);//parse the right operand

            if(right_expr.empty()){
            std::cerr<<"[line " << current << "] Error at end: Expected expression '" << operator_symbol << "'." << std::endl;
            std::exit(65);
            }

            expr = "(" + operator_symbol + " " + expr + " " + right_expr + ")";//combine the left and right expressions with the operator
        }
    return expr;
}

//parse unary operators (-, !)
std::string parse_unary(const std::vector<SimpleToken>& tokens, int& current){
    if (current>=tokens.size()) return "";
    
    const SimpleToken& token = tokens[current];
    
    if(token.type=="MINUS" || token.type=="BANG"){
        std::string operator_symbol= (token.type=="MINUS")? "-" : "!";//determine the operator symbol based on the token type
        current++;//consume or move past the unary operator token
        std::string operand= parse_unary(tokens, current);//parse the operand which is expected to be an expression

        if(operand.empty()){
            std::cerr<<"[line " << current << "] Error at end: Expected expression '" << operator_symbol << "'." << std::endl;
            std::exit(65);
        }

        return "(" + operator_symbol + " " + operand + ")";
    }
    return parse_primary(tokens, current);//if no unary operator, parse the primary expression
}

//parse primary expressions (literals, grouping)
std::string parse_primary(const std::vector<SimpleToken>& tokens, int& current) {
    if(current >= tokens.size()){
        std::cerr << "[line " << current << "] Error at end: Expect expression." << std::endl;
        std::exit(65);
    }

    const SimpleToken& token = tokens[current];

    //handle parantheses for grouping expressions
    if(token.type=="LEFT_PAREN"){//if the current token is a left parenthesis, we expect an expression inside
        current++;//consume or move past the left parenthesis
        std::string inner_expr=parse_expression(tokens, current);

        //expecting a right parenthesis to close the expression
        if(current<tokens.size() && tokens[current].type=="RIGHT_PAREN"){//if the next token is a right parenthesis
            current++;//consume or move past the right parenthesis
            return "(group "+ inner_expr + ")";
        }

        //error: missing closing parenthesis
        std::cerr << "[line " << current << "] Error at end: Expected ')' after expression." << std::endl;
        std::exit(65);
    }

    //handle the three required literal types
    if (token.type == "TRUE") {
        current++;
        return "true";
    }
    else if (token.type == "FALSE") {
        current++;
        return "false";
    }
    else if (token.type == "NIL") {
        current++;
        return "nil";
    }
    else if (token.type == "NUMBER") {
        current++;
        //convert the literal value (already properly formatted from tokenizer)
        return token.literal;
    }
    else if (token.type == "STRING") {
        current++;
        return token.literal;
    }
    
    //error: unexpected token 
    std::cerr<<"[line "<<current<<"] Error at '"<<token.lexeme<<"': Expected expression."<<std::endl;
    std::exit(65);
}

//simple expression parser 
std::string parse_expression(const std::vector<SimpleToken>& tokens, int& current) {
    return parse_equality(tokens, current); // Start with additive parsing
}

//convert your existing tokenizer output to SimpleToken vector
std::vector<SimpleToken> tokenize_to_vector(const std::string& file_contents) {
    std::vector<SimpleToken> tokens;
    int line_number = 1;
    
    for(int i = 0; i < file_contents.size(); i++){
        char c = file_contents[i];
        SimpleToken token;
        
        switch(c){
            case '(':
                tokens.push_back({"LEFT_PAREN", "(", "null"});
                break;
            case ')':
                tokens.push_back({"RIGHT_PAREN", ")", "null"});
                break;
            case '{':
                tokens.push_back({"LEFT_BRACE", "{", "null"});
                break;
            case '}':
                tokens.push_back({"RIGHT_BRACE", "}", "null"});
                break;
            case ',':
                tokens.push_back({"COMMA", ",", "null"});
                break;
            case '.':
                tokens.push_back({"DOT", ".", "null"});
                break;
            case '-':
                tokens.push_back({"MINUS", "-", "null"});
                break;
            case '+':
                tokens.push_back({"PLUS", "+", "null"});
                break;
            case ';':
                tokens.push_back({"SEMICOLON", ";", "null"});
                break;
            case '*':
                tokens.push_back({"STAR", "*", "null"});
                break;
            case '!':
                if(i+1 < file_contents.size() && file_contents[i+1] == '='){
                    tokens.push_back({"BANG_EQUAL", "!=", "null"});
                    i++;
                } else {
                    tokens.push_back({"BANG", "!", "null"});
                }
                break;
            case '=':
                if(i+1 < file_contents.size() && file_contents[i+1] == '='){
                    tokens.push_back({"EQUAL_EQUAL", "==", "null"});
                    i++;
                } else {
                    tokens.push_back({"EQUAL", "=", "null"});
                }
                break;
            case '<':
                if(i+1 < file_contents.size() && file_contents[i+1] == '='){
                    tokens.push_back({"LESS_EQUAL", "<=", "null"});
                    i++;
                } else {
                    tokens.push_back({"LESS", "<", "null"});
                }
                break;
            case '>':
                if(i+1 < file_contents.size() && file_contents[i+1] == '='){
                    tokens.push_back({"GREATER_EQUAL", ">=", "null"});
                    i++;
                } else {
                    tokens.push_back({"GREATER", ">", "null"});
                }
                break;
            case '/':
                if(i+1 < file_contents.size() && file_contents[i+1] == '/'){
                    i++;
                    while(i < file_contents.size() && file_contents[i] != '\n'){
                        i++;
                    }
                    line_number++;
                } else {
                    tokens.push_back({"SLASH", "/", "null"});
                }
                break;
            case '"': {
                int start = i;
                i++;
                
                while (i < file_contents.size() && file_contents[i] != '"') {
                    if (file_contents[i] == '\n') {
                        line_number++;
                    }
                    i++;
                }
                
                if (i >= file_contents.size()) {
                    // Handle error - for now just skip
                } else {
                    std::string value = file_contents.substr(start + 1, i - start - 1);
                    std::string lexeme = file_contents.substr(start, i - start + 1);
                    tokens.push_back({"STRING", lexeme, value});
                }
                break;
            }
            case '0': case '1': case '2': case '3': case '4': 
            case '5': case '6': case '7': case '8': case '9': {
                int num = i;
                
                while (i + 1 < file_contents.size() &&
                      (file_contents[i + 1] >= '0' && file_contents[i + 1] <= '9')) {
                    i++;
                }
                
                if (i + 1 < file_contents.size() && file_contents[i + 1] == '.' &&
                    i + 2 < file_contents.size() &&
                    (file_contents[i + 2] >= '0' && file_contents[i + 2] <= '9')) {
                    i += 2;
                    while (i + 1 < file_contents.size() &&
                          (file_contents[i + 1] >= '0' && file_contents[i + 1] <= '9')) {
                        i++;
                    }
                }
                
                std::string lexeme = file_contents.substr(num, i - num + 1);
                double parsed = std::stod(lexeme);
                std::string value = std::to_string(parsed);
                
                if (value.find('.') != std::string::npos) {
                    while (value.back() == '0') {
                        value.pop_back();
                    }
                    if (value.back() == '.') {
                        value.pop_back();
                        value += ".0";
                    }
                }
                
                tokens.push_back({"NUMBER", lexeme, value});
                break;
            }
            case ' ':
            case '\t':
            case '\r':
                break;
            case '\n':
                line_number++;
                break;
            default:
                if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'){
                    int start = i;
                    while(i + 1 < file_contents.size()){
                        char next_char = file_contents[i + 1];
                        if((next_char >= 'a' && next_char <= 'z') ||
                           (next_char >= 'A' && next_char <= 'Z') ||
                           (next_char >= '0' && next_char <= '9') ||
                           next_char == '_'){
                            i++;
                        } else {
                            break;
                        }
                    }
                    std::string lexeme = file_contents.substr(start, i - start + 1);

                    static const std::unordered_map<std::string, std::string> reserved_words = {
                        {"and", "AND"}, {"class", "CLASS"}, {"else", "ELSE"}, {"false", "FALSE"},
                        {"for", "FOR"}, {"fun", "FUN"}, {"if", "IF"}, {"nil", "NIL"},
                        {"or", "OR"}, {"print", "PRINT"}, {"return", "RETURN"}, {"super", "SUPER"},
                        {"this", "THIS"}, {"true", "TRUE"}, {"var", "VAR"}, {"while", "WHILE"}
                    };

                    auto keywords = reserved_words.find(lexeme);

                    if(keywords != reserved_words.end()){
                        tokens.push_back({keywords->second, lexeme, "null"});
                    } else {
                        tokens.push_back({"IDENTIFIER", lexeme, "null"});
                    }
                }
                break;
        }
    }
    
    return tokens;
}

std::string read_file_contents(const std::string& filename);

int main(int argc, char *argv[]) {
    //disable output buffering
    std::cout << std::unitbuf;
    std::cerr << std::unitbuf;

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

        } else if (command == "parse") {
        //simple parse implementation
        std::string file_contents = read_file_contents(argv[2]);
        std::vector<SimpleToken> tokens = tokenize_to_vector(file_contents);
        
        int current = 0;
        std::string result = parse_expression(tokens, current);
        if (!result.empty()) {
            std::cout << result << std::endl;
        }
        
        return 0;
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
