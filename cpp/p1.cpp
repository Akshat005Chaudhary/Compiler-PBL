// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <stack>
// #include <fstream>
// #include <stdexcept>
// #include <regex>

// using namespace std;

// struct SymbolInfo {
//     string type;
//     int value;
//     string scope;
// };

// map<string, SymbolInfo> symbolTable;

// struct TreeNode {
//     string value;
//     TreeNode* left;
//     TreeNode* right;
// };

// bool isOperator(const string& token) {
//     return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
// }

// int precedence(const string& op) {
//     if (op == "+" || op == "-") return 1;
//     if (op == "*" || op == "/" || op == "%") return 2;
//     if (op == "^") return 3;
//     return 0;
// }

// vector<string> tokenizeExpression(const string& expr) {
//     vector<string> tokens;
//     string token;
//     for (size_t i = 0; i < expr.length(); ++i) {
//         char ch = expr[i];
//         if (isspace(ch)) continue;
//         if (isdigit(ch)) {
//             token.clear();
//             while (i < expr.length() && isdigit(expr[i])) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else if (isalpha(ch)) {
//             token.clear();
//             while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_')) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else {
//             tokens.push_back(string(1, ch));
//         }
//     }
//     return tokens;
// }

// vector<string> infixToPostfix(const vector<string>& infix) {
//     stack<string> opStack;
//     vector<string> postfix;

//     for (const string& token : infix) {
//         if (isalpha(token[0]) || isdigit(token[0])) {
//             postfix.push_back(token);
//         } else if (isOperator(token)) {
//             while (!opStack.empty() && opStack.top() != "(" && precedence(opStack.top()) >= precedence(token)) {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             opStack.push(token);
//         } else if (token == "(") {
//             opStack.push(token);
//         } else if (token == ")") {
//             while (!opStack.empty() && opStack.top() != "(") {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             if (!opStack.empty()) opStack.pop(); // remove '('
//         }
//     }

//     while (!opStack.empty()) {
//         postfix.push_back(opStack.top());
//         opStack.pop();
//     }

//     return postfix;
// }

// int integerPower(int base, int exp) {
//     int result = 1;
//     while (exp > 0) {
//         if (exp % 2 == 1) {
//             result *= base;
//         }
//         base *= base;
//         exp /= 2;
//     }
//     return result;
// }

// int evaluateExpression(const vector<string>& postfix) {
//     stack<int> st;
//     cout << "Postfix expression: ";
//     for (const auto& token : postfix) {
//         cout << token << " ";
//     }
//     cout << endl;

//     for (const string& token : postfix) {
//         if (isdigit(token[0])) {
//             int val = stoi(token);
//             st.push(val);
//             cout << "Pushing number: " << val << endl;
//         } else if (isalpha(token[0])) {
//             if (symbolTable.find(token) != symbolTable.end()) {
//                 int val = symbolTable[token].value;
//                 st.push(val);
//                 cout << "Pushing variable " << token << ": " << val << endl;
//             } else {
//                 throw runtime_error("Variable '" + token + "' not declared in current scope.");
//             }
//         } else {
//             int b = st.top(); st.pop();
//             int a = st.top(); st.pop();
//             cout << "Evaluating " << a << " " << token << " " << b << endl;
//             int result;
//             if (token == "+") result = a + b;
//             else if (token == "-") result = a - b;
//             else if (token == "*") result = a * b;
//             else if (token == "/") {
//                 if (b == 0) throw runtime_error("Division by zero.");
//                 result = a / b;
//             }
//             else if (token == "%") {
//                 if (b == 0) throw runtime_error("Modulus by zero.");
//                 result = a % b;
//             }
//             else if (token == "^") result = integerPower(a, b);
//             st.push(result);
//             cout << "Result: " << result << endl;
//         }
//     }
//     return st.top();
// }

// TreeNode* buildParseTreeWithAssignment(const string& varName, const vector<string>& postfix) {
//     stack<TreeNode*> st;
//     for (const string& token : postfix) {
//         if (isOperator(token)) {
//             TreeNode* right = st.top(); st.pop();
//             TreeNode* left = st.top(); st.pop();
//             st.push(new TreeNode{token, left, right});
//         } else {
//             st.push(new TreeNode{token, nullptr, nullptr});
//         }
//     }
//     TreeNode* exprTree = st.top(); st.pop();
//     TreeNode* assignNode = new TreeNode{"=", new TreeNode{varName, nullptr, nullptr}, exprTree};
//     return assignNode;
// }

// void visualizeParseTree(const vector<TreeNode*>& parseTrees, const string& filename) {
//     ofstream fout(filename);
//     fout << "digraph ParseTree {\n";
//     int id = 0;
//     map<TreeNode*, int> ids;

//     function<void(TreeNode*)> dfs = [&](TreeNode* node) {
//         if (!node) return;
//         ids[node] = id++;
//         fout << "  " << ids[node] << " [label=\"" << node->value << "\"];\n";
//         if (node->left) {
//             dfs(node->left);
//             fout << "  " << ids[node] << " -> " << ids[node->left] << ";\n";
//         }
//         if (node->right) {
//             dfs(node->right);
//             fout << "  " << ids[node] << " -> " << ids[node->right] << ";\n";
//         }
//     };

//     for (size_t i = 0; i < parseTrees.size(); ++i) {
//         fout << "  subgraph cluster_" << i << " {\n";
//         fout << "    label=\"Assignment " << i + 1 << "\";\n";
//         dfs(parseTrees[i]);
//         fout << "  }\n";
//     }

//     fout << "}\n";
//     fout.close();
//     cout << "Parse tree saved as " << filename << endl;
// }

// void processDeclaration(const string& declaration, vector<TreeNode*>& parseTrees) {
//     regex pattern(R"(int\s+(.+);)");
//     smatch match;
    
//     string decl = declaration + ";"; // Re-add semicolon for regex matching
//     if (!regex_search(decl, match, pattern)) {
//         cout << "Invalid declaration: Only 'int' type declarations supported. Input: " << declaration << endl;
//         return;
//     }

//     string vars = match[1];
//     stringstream ss(vars);
//     string token;

//     while (getline(ss, token, ',')) {
//         string name;
//         int value = 0; // Default value for uninitialized variables
//         vector<string> exprTokens;

//         size_t eq = token.find('=');
//         if (eq != string::npos) {
//             name = token.substr(0, eq);
//             string expr = token.substr(eq + 1);
//             exprTokens = tokenizeExpression(expr);
//             vector<string> postfix = infixToPostfix(exprTokens);
//             try {
//                 value = evaluateExpression(postfix);
//                 parseTrees.push_back(buildParseTreeWithAssignment(name, postfix));
//             } catch (exception& e) {
//                 cerr << "Error evaluating declaration: " << e.what() << endl;
//                 continue;
//             }
//         } else {
//             name = token;
//         }

//         name = regex_replace(name, regex("^\\s+|\\s+$"), ""); // Trim
//         if (!name.empty()) {
//             cout << "Declaring variable: " << name << " = " << value << endl;
//             symbolTable[name] = {"int", value, "Global"};
//         } else {
//             cout << "Invalid variable name in declaration: " << token << endl;
//         }
//     }
// }

// void processInput(const string& input, vector<TreeNode*>& parseTrees) {
//     stringstream ss(input);
//     string line;

//     while (getline(ss, line)) {
//         line = regex_replace(line, regex("^\\s+|\\s+$"), ""); // Trim
//         if (line.empty()) continue;

//         if (line.back() != ';') {
//             cout << "Invalid input: Statement must end with ';'. Input: " << line << endl;
//             continue;
//         }

//         line.pop_back(); // Remove semicolon for processing
//         if (line.find("int ") == 0) {
//             processDeclaration(line, parseTrees);
//         } else {
//             vector<string> tokens = tokenizeExpression(line);
//             if (tokens.size() > 2 && tokens[1] == "=") {
//                 string varName = tokens[0];
//                 if (symbolTable.find(varName) == symbolTable.end()) {
//                     cerr << "Error: Variable '" + varName << "' not declared.\n";
//                     continue;
//                 }

//                 tokens.erase(tokens.begin(), tokens.begin() + 2);
//                 try {
//                     vector<string> postfix = infixToPostfix(tokens);
//                     int result = evaluateExpression(postfix);
//                     symbolTable[varName].value = result;
//                     cout << "Assigned " << varName << " = " << result << endl;
//                     parseTrees.push_back(buildParseTreeWithAssignment(varName, postfix));
//                 } catch (exception& e) {
//                     cerr << "Error evaluating expression: " << e.what() << endl;
//                 }
//             } else {
//                 cout << "Invalid expression: " << line << endl;
//             }
//         }
//     }
// }

// void displaySymbolTable() {
//     cout << "\nSymbol Table:\n";
//     cout << "-------------------------------------------\n";
//     cout << "Name\tType\tValue\tScope\n";
//     cout << "-------------------------------------------\n";
//     for (const auto& entry : symbolTable) {
//         cout << entry.first << "\t" << entry.second.type << "\t" << entry.second.value << "\t" << entry.second.scope << "\n";
//     }
//     cout << "-------------------------------------------\n";
// }

// int main() {
//     int choice;
//     string input;

//     while (true) {
//         cout << "\nMenu:\n";
//         cout << "1. Enter Multi-line Input (Declarations and Expressions)\n";
//         cout << "2. Display Symbol Table\n";
//         cout << "3. Visualize Parse Tree\n";
//         cout << "4. Exit\n";
//         cout << "Enter your choice: ";
//         cin >> choice;
//         cin.ignore();

//         if (choice == 1) {
//             cout << "Enter multi-line input (end with empty line):\n";
//             input.clear();
//             string line;
//             vector<TreeNode*> parseTrees;
//             while (getline(cin, line) && !line.empty()) {
//                 input += line + "\n";
//             }
//             processInput(input, parseTrees);
//             if (!parseTrees.empty()) {
//                 visualizeParseTree(parseTrees, "parse_tree.dot");
//             }
//         } else if (choice == 2) {
//             displaySymbolTable();
//         } else if (choice == 3) {
//             system("dot -Tpng parse_tree.dot -o parse_tree.png");
//             system("start parse_tree.png"); // For Windows
//         } else if (choice == 4) {
//             break;
//         } else {
//             cout << "Invalid choice.\n";
//         }
//     }

//     return 0;
// }










// #include <iostream>
// #include <sstream>
// #include <vector>
// #include <map>
// #include <stack>
// #include <fstream>
// #include <stdexcept>
// #include <regex>

// using namespace std;

// struct SymbolInfo {
//     string type;
//     int value;
//     string scope;
// };

// map<string, SymbolInfo> symbolTable;

// struct TreeNode {
//     string value;
//     TreeNode* left;
//     TreeNode* right;
// };

// bool isOperator(const string& token) {
//     return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
// }

// int precedence(const string& op) {
//     if (op == "+" || op == "-") return 1;
//     if (op == "*" || op == "/" || op == "%") return 2;
//     if (op == "^") return 3;
//     return 0;
// }

// vector<string> tokenizeExpression(const string& expr) {
//     vector<string> tokens;
//     string token;
//     for (size_t i = 0; i < expr.length(); ++i) {
//         char ch = expr[i];
//         if (isspace(ch)) continue;
//         if (isdigit(ch)) {
//             token.clear();
//             while (i < expr.length() && isdigit(expr[i])) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else if (isalpha(ch)) {
//             token.clear();
//             while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_')) token += expr[i++];
//             --i;
//             tokens.push_back(token);
//         } else {
//             tokens.push_back(string(1, ch));
//         }
//     }
//     return tokens;
// }

// vector<string> infixToPostfix(const vector<string>& infix) {
//     stack<string> opStack;
//     vector<string> postfix;

//     for (const string& token : infix) {
//         if (isalpha(token[0]) || isdigit(token[0])) {
//             postfix.push_back(token);
//         } else if (isOperator(token)) {
//             while (!opStack.empty() && opStack.top() != "(" && precedence(opStack.top()) >= precedence(token)) {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             opStack.push(token);
//         } else if (token == "(") {
//             opStack.push(token);
//         } else if (token == ")") {
//             while (!opStack.empty() && opStack.top() != "(") {
//                 postfix.push_back(opStack.top());
//                 opStack.pop();
//             }
//             if (!opStack.empty()) opStack.pop(); // remove '('
//         }
//     }

//     while (!opStack.empty()) {
//         postfix.push_back(opStack.top());
//         opStack.pop();
//     }

//     return postfix;
// }

// int integerPower(int base, int exp) {
//     int result = 1;
//     while (exp > 0) {
//         if (exp % 2 == 1) {
//             result *= base;
//         }
//         base *= base;
//         exp /= 2;
//     }
//     return result;
// }

// int evaluateExpression(const vector<string>& postfix) {
//     stack<int> st;
//     for (const string& token : postfix) {
//         if (isdigit(token[0])) {
//             try {
//                 st.push(stoi(token));
//             } catch (const std::invalid_argument& e) {
//                 throw runtime_error("Invalid number format: " + token);
//             }
//         } else if (isalpha(token[0])) {
//             if (symbolTable.find(token) != symbolTable.end()) {
//                 st.push(symbolTable[token].value);
//             } else {
//                 throw runtime_error("Variable '" + token + "' not declared in current scope.");
//             }
//         } else {
//             if (st.size() < 2) {
//                 throw runtime_error("Invalid expression: insufficient operands for operator " + token);
//             }
//             int b = st.top(); st.pop();
//             int a = st.top(); st.pop();
//             if (token == "+") st.push(a + b);
//             else if (token == "-") st.push(a - b);
//             else if (token == "*") st.push(a * b);
//             else if (token == "/") {
//                 if (b == 0) throw runtime_error("Division by zero in expression.");
//                 st.push(a / b);
//             }
//             else if (token == "%") {
//                 if (b == 0) throw runtime_error("Modulus by zero in expression.");
//                 st.push(a % b);
//             }
//             else if (token == "^") st.push(integerPower(a, b));
//             else {
//                 throw runtime_error("Unknown operator: " + token);
//             }
//         }
//     }
//     if (st.size() != 1) {
//         throw runtime_error("Invalid expression: too many operands.");
//     }
//     return st.top();
// }

// TreeNode* buildParseTreeWithAssignment(const string& varName, const vector<string>& postfix) {
//     stack<TreeNode*> st;
//     for (const string& token : postfix) {
//         if (isOperator(token)) {
//             if (st.size() < 2) {
//                 throw runtime_error("Invalid expression: insufficient operands for parse tree.");
//             }
//             TreeNode* right = st.top(); st.pop();
//             TreeNode* left = st.top(); st.pop();
//             st.push(new TreeNode{token, left, right});
//         } else {
//             st.push(new TreeNode{token, nullptr, nullptr});
//         }
//     }
//     if (st.size() != 1) {
//         throw runtime_error("Invalid expression: incomplete parse tree.");
//     }
//     TreeNode* exprTree = st.top(); st.pop();
//     TreeNode* assignNode = new TreeNode{"=", new TreeNode{varName, nullptr, nullptr}, exprTree};
//     return assignNode;
// }

// void visualizeParseTree(const vector<TreeNode*>& parseTrees, const string& filename) {
//     ofstream fout(filename);
//     if (!fout.is_open()) {
//         cerr << "Error: Could not open file " << filename << " for writing.\n";
//         return;
//     }
//     fout << "digraph ParseTree {\n";
//     int id = 0;
//     map<TreeNode*, int> ids;

//     function<void(TreeNode*)> dfs = [&](TreeNode* node) {
//         if (!node) return;
//         ids[node] = id++;
//         fout << "  " << ids[node] << " [label=\"" << node->value << "\"];\n";
//         if (node->left) {
//             dfs(node->left);
//             fout << "  " << ids[node] << " -> " << ids[node->left] << ";\n";
//         }
//         if (node->right) {
//             dfs(node->right);
//             fout << "  " << ids[node] << " -> " << ids[node->right] << ";\n";
//         }
//     };

//     for (size_t i = 0; i < parseTrees.size(); ++i) {
//         fout << "  subgraph cluster_" << i << " {\n";
//         fout << "    label=\"Assignment " << i + 1 << "\";\n";
//         dfs(parseTrees[i]);
//         fout << "  }\n";
//     }

//     fout << "}\n";
//     fout.close();
//     cout << "Parse tree saved as " << filename << endl;
// }

// void processDeclaration(const string& declaration, vector<TreeNode*>& parseTrees) {
//     // *** Snippet 1: Wrap regex and parsing in try-catch ***
//     try {
//         regex pattern(R"(int\s+(.+);)");
//         smatch match;
        
//         string decl = declaration + ";"; // Re-add semicolon for regex matching
//         if (!regex_search(decl, match, pattern)) {
//             throw runtime_error("Invalid declaration syntax: " + declaration);
//         }

//         string vars = match[1];
//         stringstream ss(vars);
//         string token;

//         while (getline(ss, token, ',')) {
//             string name;
//             int value = 0;
//             vector<string> exprTokens;

//             size_t eq = token.find('=');
//             if (eq != string::npos) {
//                 name = token.substr(0, eq);
//                 string expr = token.substr(eq + 1);
//                 exprTokens = tokenizeExpression(expr);
//                 if (exprTokens.empty()) {
//                     throw runtime_error("Empty expression in declaration: " + token);
//                 }
//                 vector<string> postfix = infixToPostfix(exprTokens);
//                 // *** Snippet 2: Evaluate expression safely ***
//                 try {
//                     value = evaluateExpression(postfix);
//                     parseTrees.push_back(buildParseTreeWithAssignment(name, postfix));
//                 } catch (const std::exception& e) {
//                     throw runtime_error("Error in expression '" + expr + "': " + e.what());
//                 }
//             } else {
//                 name = token;
//             }

//             name = regex_replace(name, regex("^\\s+|\\s+$"), ""); // Trim
//             if (name.empty()) {
//                 throw runtime_error("Invalid variable name in declaration: " + token);
//             }
//             if (!regex_match(name, regex("[a-zA-Z_][a-zA-Z0-9_]*"))) {
//                 throw runtime_error("Invalid variable name syntax: " + name);
//             }
//             if (symbolTable.find(name) != symbolTable.end()) {
//                 throw runtime_error("Variable '" + name + "' already declared in current scope.");
//             }

//             cout << "Declaring variable: " << name << " = " << value << endl;
//             symbolTable[name] = {"int", value, "Global"};
//         }
//     } catch (const std::regex_error& e) {
//         cerr << "Regex error in declaration '" << declaration << "': " << e.what() << endl;
//     } catch (const std::runtime_error& e) {
//         cerr << "Error: " << e.what() << endl;
//     } catch (const std::exception& e) {
//         cerr << "Unexpected error in declaration '" << declaration << "': " << e.what() << endl;
//     } catch (...) {
//         cerr << "Unknown error in declaration '" << declaration << "'.\n";
//     }
// }

// void processInput(const string& input, vector<TreeNode*>& parseTrees) {
//     stringstream ss(input);
//     string line;

//     while (getline(ss, line)) {
//         line = regex_replace(line, regex("^\\s+|\\s+$"), ""); // Trim
//         if (line.empty()) continue;

//         // *** Snippet 3: Check semicolon and wrap processing in try-catch ***
//         if (line.back() != ';') {
//             cerr << "Error: Statement must end with ';'. Input: " << line << endl;
//             continue;
//         }

//         try {
//             line.pop_back(); // Remove semicolon for processing
//             if (line.find("int ") == 0) {
//                 processDeclaration(line, parseTrees);
//             } else {
//                 vector<string> tokens = tokenizeExpression(line);
//                 if (tokens.size() < 3 || tokens[1] != "=") {
//                     throw runtime_error("Invalid expression syntax: " + line);
//                 }

//                 string varName = tokens[0];
//                 if (symbolTable.find(varName) == symbolTable.end()) {
//                     throw runtime_error("Variable '" + varName + "' not declared.");
//                 }

//                 tokens.erase(tokens.begin(), tokens.begin() + 2);
//                 // *** Snippet 4: Evaluate assignment safely ***
//                 vector<string> postfix = infixToPostfix(tokens);
//                 int result;
//                 try {
//                     result = evaluateExpression(postfix);
//                 } catch (const std::exception& e) {
//                     throw runtime_error("Error in expression '" + line + "': " + e.what());
//                 }
//                 symbolTable[varName].value = result;
//                 cout << "Assigned " << varName << " = " << result << endl;
//                 parseTrees.push_back(buildParseTreeWithAssignment(varName, postfix));
//             }
//         } catch (const std::runtime_error& e) {
//             cerr << "Error: " << e.what() << endl;
//             // Continue to next line without adding to parseTrees
//         } catch (const std::exception& e) {
//             cerr << "Unexpected error in statement '" << line << "': " << e.what() << endl;
//         } catch (...) {
//             cerr << "Unknown error in statement '" << line << "'.\n";
//         }
//     }
// }

// void displaySymbolTable() {
//     cout << "\nSymbol Table:\n";
//     cout << "-------------------------------------------\n";
//     cout << "Name\tType\tValue\tScope\n";
//     cout << "-------------------------------------------\n";
//     for (const auto& entry : symbolTable) {
//         cout << entry.first << "\t" << entry.second.type << "\t" << entry.second.value << "\t" << entry.second.scope << "\n";
//     }
//     cout << "-------------------------------------------\n";
// }

// int main() {
//     int choice;
//     string input;

//     while (true) {
//         cout << "\nMenu:\n";
//         cout << "1. Enter Multi-line Input (Declarations and Expressions)\n";
//         cout << "2. Display Symbol Table\n";
//         cout << "3. Visualize Parse Tree\n";
//         cout << "4. Exit\n";
//         cout << "Enter your choice: ";
//         try {
//             cin >> choice;
//             if (cin.fail()) {
//                 throw runtime_error("Invalid input for menu choice.");
//             }
//             cin.ignore();
//         } catch (const std::runtime_error& e) {
//             cerr << "Error: " << e.what() << endl;
//             cin.clear();
//             cin.ignore(numeric_limits<streamsize>::max(), '\n');
//             continue;
//         }

//         if (choice == 1) {
//             cout << "Enter multi-line input (end with empty line):\n";
//             input.clear();
//             string line;
//             vector<TreeNode*> parseTrees;
//             while (getline(cin, line) && !line.empty()) {
//                 input += line + "\n";
//             }
//             processInput(input, parseTrees);
//             if (!parseTrees.empty()) {
//                 visualizeParseTree(parseTrees, "parse_tree.dot");
//             }
//             // *** Snippet 5: Clean up parse trees ***
//             for (auto tree : parseTrees) {
//                 function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
//                     if (!node) return;
//                     deleteTree(node->left);
//                     deleteTree(node->right);
//                     delete node;
//                 };
//                 deleteTree(tree);
//             }
//         } else if (choice == 2) {
//             displaySymbolTable();
//         } else if (choice == 3) {
//             try {
//                 system("dot -Tpng parse_tree.dot -o parse_tree.png");
//                 system("start parse_tree.png"); // For Windows
//             } catch (const std::exception& e) {
//                 cerr << "Error generating parse tree: " << e.what() << endl;
//             }
//         } else if (choice == 4) {
//             break;
//         } else {
//             cout << "Invalid choice.\n";
//         }
//     }

//     return 0;
// }










#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>
#include <stdexcept>
#include <regex>

using namespace std;

struct SymbolInfo {
    string type;
    int value;
    string scope;
};

map<string, SymbolInfo> symbolTable;

struct TreeNode {
    string value;
    TreeNode* left;
    TreeNode* right;
};

bool isOperator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
}

int precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/" || op == "%") return 2;
    if (op == "^") return 3;
    return 0;
}

vector<string> tokenizeExpression(const string& expr) {
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];
        if (isspace(ch)) continue;
        if (isdigit(ch)) {
            token.clear();
            while (i < expr.length() && isdigit(expr[i])) token += expr[i++];
            --i;
            tokens.push_back(token);
        } else if (isalpha(ch)) {
            token.clear();
            while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_')) token += expr[i++];
            --i;
            tokens.push_back(token);
        } else {
            tokens.push_back(string(1, ch));
        }
    }
    return tokens;
}

vector<string> infixToPostfix(const vector<string>& infix) {
    stack<string> opStack;
    vector<string> postfix;

    for (const string& token : infix) {
        if (isalpha(token[0]) || isdigit(token[0])) {
            postfix.push_back(token);
        } else if (isOperator(token)) {
            while (!opStack.empty() && opStack.top() != "(" && precedence(opStack.top()) >= precedence(token)) {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        } else if (token == "(") {
            opStack.push(token);
        } else if (token == ")") {
            while (!opStack.empty() && opStack.top() != "(") {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty()) opStack.pop();
        }
    }

    while (!opStack.empty()) {
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}

int integerPower(int base, int exp) {
    int result = 1;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

int evaluateExpression(const vector<string>& postfix) {
    stack<int> st;
    for (const string& token : postfix) {
        if (isdigit(token[0])) {
            try {
                st.push(stoi(token));
            } catch (const std::invalid_argument& e) {
                throw runtime_error("Invalid number format: " + token);
            }
        } else if (isalpha(token[0])) {
            if (symbolTable.find(token) != symbolTable.end()) {
                st.push(symbolTable[token].value);
            } else {
                throw runtime_error("Variable '" + token + "' not declared in current scope.");
            }
        } else {
            if (st.size() < 2) {
                throw runtime_error("Invalid expression: insufficient operands for operator " + token);
            }
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
            if (token == "+") st.push(a + b);
            else if (token == "-") st.push(a - b);
            else if (token == "*") st.push(a * b);
            else if (token == "/") {
                if (b == 0) throw runtime_error("Division by zero in expression.");
                st.push(a / b);
            }
            else if (token == "%") {
                if (b == 0) throw runtime_error("Modulus by zero in expression.");
                st.push(a % b);
            }
            else if (token == "^") st.push(integerPower(a, b));
            else {
                throw runtime_error("Unknown operator: " + token);
            }
        }
    }
    if (st.size() != 1) {
        throw runtime_error("Invalid expression: too many operands.");
    }
    return st.top();
}

TreeNode* buildParseTreeWithAssignment(const string& varName, const vector<string>& postfix) {
    stack<TreeNode*> st;
    for (const string& token : postfix) {
        if (isOperator(token)) {
            if (st.size() < 2) {
                throw runtime_error("Invalid expression: insufficient operands for parse tree.");
            }
            TreeNode* right = st.top(); st.pop();
            TreeNode* left = st.top(); st.pop();
            st.push(new TreeNode{token, left, right});
        } else {
            st.push(new TreeNode{token, nullptr, nullptr});
        }
    }
    if (st.size() != 1) {
        throw runtime_error("Invalid expression: incomplete parse tree.");
    }
    TreeNode* exprTree = st.top(); st.pop();
    TreeNode* assignNode = new TreeNode{"=", new TreeNode{varName, nullptr, nullptr}, exprTree};
    return assignNode;
}

void visualizeParseTree(const vector<TreeNode*>& parseTrees, const string& filename) {
    ofstream fout(filename);
    if (!fout.is_open()) {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    fout << "digraph ParseTree {\n";
    int id = 0;
    map<TreeNode*, int> ids;

    function<void(TreeNode*)> dfs = [&](TreeNode* node) {
        if (!node) return;
        ids[node] = id++;
        fout << "  " << ids[node] << " [label=\"" << node->value << "\"];\n";
        if (node->left) {
            dfs(node->left);
            fout << "  " << ids[node] << " -> " << ids[node->left] << ";\n";
        }
        if (node->right) {
            dfs(node->right);
            fout << "  " << ids[node] << " -> " << ids[node->right] << ";\n";
        }
    };

    for (size_t i = 0; i < parseTrees.size(); ++i) {
        fout << "  subgraph cluster_" << i << " {\n";
        fout << "    label=\"Assignment " << i + 1 << "\";\n";
        dfs(parseTrees[i]);
        fout << "  }\n";
    }

    fout << "}\n";
    fout.close();
}

void processDeclaration(const string& declaration, vector<TreeNode*>& parseTrees) {
    try {
        regex pattern(R"(int\s+(.+);)");
        smatch match;
        
        string decl = declaration + ";";
        if (!regex_search(decl, match, pattern)) {
            throw runtime_error("Invalid declaration syntax: " + declaration);
        }

        string vars = match[1];
        stringstream ss(vars);
        string token;

        while (getline(ss, token, ',')) {
            string name;
            int value = 0;
            vector<string> exprTokens;

            size_t eq = token.find('=');
            if (eq != string::npos) {
                name = token.substr(0, eq);
                string expr = token.substr(eq + 1);
                exprTokens = tokenizeExpression(expr);
                if (exprTokens.empty()) {
                    throw runtime_error("Empty expression in declaration: " + token);
                }
                vector<string> postfix = infixToPostfix(exprTokens);
                try {
                    value = evaluateExpression(postfix);
                    parseTrees.push_back(buildParseTreeWithAssignment(name, postfix));
                } catch (const std::exception& e) {
                    throw runtime_error("Error in expression '" + expr + "': " + e.what());
                }
            } else {
                name = token;
            }

            name = regex_replace(name, regex("^\\s+|\\s+$"), "");
            if (name.empty()) {
                throw runtime_error("Invalid variable name in declaration: " + token);
            }
            if (!regex_match(name, regex("[a-zA-Z_][a-zA-Z0-9_]*"))) {
                throw runtime_error("Invalid variable name syntax: " + name);
            }
            if (symbolTable.find(name) != symbolTable.end()) {
                throw runtime_error("Variable '" + name + "' already declared in current scope.");
            }

            cout << "Declared: " << name << " = " << value << endl;
            symbolTable[name] = {"int", value, "Global"};
        }
    } catch (const std::regex_error& e) {
        cerr << "Regex error in declaration '" << declaration << "': " << e.what() << endl;
    } catch (const std::runtime_error& e) {
        cerr << "Error: " << e.what() << endl;
    } catch (const std::exception& e) {
        cerr << "Unexpected error in declaration '" << declaration << "': " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown error in declaration '" << declaration << "'.\n";
    }
}

void processInput(const string& input, vector<TreeNode*>& parseTrees) {
    stringstream ss(input);
    string line;

    while (getline(ss, line)) {
        line = regex_replace(line, regex("^\\s+|\\s+$"), "");
        if (line.empty()) continue;

        if (line.back() != ';') {
            cerr << "Error: Statement must end with ';'. Input: " << line << endl;
            continue;
        }

        try {
            line.pop_back();
            if (line.find("int ") == 0) {
                processDeclaration(line, parseTrees);
            } else {
                vector<string> tokens = tokenizeExpression(line);
                if (tokens.size() < 3 || tokens[1] != "=") {
                    throw runtime_error("Invalid expression syntax: " + line);
                }

                string varName = tokens[0];
                if (symbolTable.find(varName) == symbolTable.end()) {
                    throw runtime_error("Variable '" + varName + "' not declared.");
                }

                tokens.erase(tokens.begin(), tokens.begin() + 2);
                vector<string> postfix = infixToPostfix(tokens);
                int result;
                try {
                    result = evaluateExpression(postfix);
                } catch (const std::exception& e) {
                    throw runtime_error("Error in expression '" + line + "': " + e.what());
                }
                symbolTable[varName].value = result;
                cout << "Assigned: " << varName << " = " << result << endl;
                parseTrees.push_back(buildParseTreeWithAssignment(varName, postfix));
            }
        } catch (const std::runtime_error& e) {
            cerr << "Error: " << e.what() << endl;
        } catch (const std::exception& e) {
            cerr << "Unexpected error in statement '" << line << "': " << e.what() << endl;
        } catch (...) {
            cerr << "Unknown error in statement '" << line << "'.\n";
        }
    }
}

void displaySymbolTable() {
    for (const auto& entry : symbolTable) {
        cout << entry.first << "\t" << entry.second.type << "\t" << entry.second.value << "\t" << entry.second.scope << endl;
    }
}

int main() {
    int choice;
    string input;

    while (true) {
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            input.clear();
            string line;
            vector<TreeNode*> parseTrees;
            while (getline(cin, line) && !line.empty()) {
                input += line + "\n";
            }
            processInput(input, parseTrees);
            if (!parseTrees.empty()) {
                visualizeParseTree(parseTrees, "parse_tree.dot");
            }
            for (auto tree : parseTrees) {
                function<void(TreeNode*)> deleteTree = [&](TreeNode* node) {
                    if (!node) return;
                    deleteTree(node->left);
                    deleteTree(node->right);
                    delete node;
                };
                deleteTree(tree);
            }
        } else if (choice == 2) {
            displaySymbolTable();
        } else if (choice == 3) {
            // For web integration, just ensure the dot file is generated; visualization is handled by server.js
        } else if (choice == 4) {
            break;
        } else {
            cerr << "Error: Invalid choice: " << choice << endl;
        }
    }

    return 0;
}