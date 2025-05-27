#include <iostream>
#include <map>
#include <vector>
#include <stack>
#include <sstream>
#include <fstream>
#include <functional>
#include <limits>

using namespace std;

// Structure for Symbol Table Entry
struct SymbolTableEntry {
    string name;
    string type;
    int value;
};

// Symbol Table
map<string, SymbolTableEntry> symbolTable;

// Node structure for Parse Tree
struct TreeNode {
    string value;
    TreeNode *left, *right;
    TreeNode(string val) : value(val), left(NULL), right(NULL) {}
};

// Function to insert into Symbol Table
void insertIntoSymbolTable(string name, string type, int value = 0) {
    symbolTable[name] = {name, type, value};
}

// Function to lookup Symbol Table
bool lookupSymbolTable(string name) {
    return symbolTable.find(name) != symbolTable.end();
}

// Function to update Symbol Table
void updateSymbolTable(string name, int value) {
    if (lookupSymbolTable(name)) {
        symbolTable[name].value = value;
    }
}

// Function to get value from Symbol Table
int getSymbolValue(string name) {
    if (lookupSymbolTable(name)) {
        return symbolTable[name].value;
    }
    throw invalid_argument("Undefined variable: " + name);
}

// Function to print Symbol Table
void printSymbolTable() {
    cout << "\nSymbol Table:" << endl;
    cout << "--------------------------------" << endl;
    cout << "Name\tType\tValue" << endl;
    cout << "--------------------------------" << endl;
    for (auto &entry : symbolTable) {
        cout << entry.second.name << "\t" << entry.second.type << "\t" << entry.second.value << endl;
    }
    cout << "--------------------------------\n";
}

// Function to tokenize input expression
vector<string> tokenizeExpression(string input) {
    vector<string> tokens;
    string token;
    for (char c : input) {
        if (isspace(c)) continue;
        if (isalnum(c)) {
            token += c;
        } else {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.push_back(string(1, c));
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// Function to evaluate arithmetic expressions
int evaluateExpression(vector<string>& tokens) {
    stack<int> values;
    stack<char> ops;
    
    auto applyOp = [](int a, int b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return b != 0 ? a / b : throw runtime_error("Division by zero");
            default: throw runtime_error("Invalid operator");
        }
    };

    for (string &token : tokens) {
        if (isdigit(token[0])) {
            values.push(stoi(token));
        } else if (lookupSymbolTable(token)) {
            values.push(getSymbolValue(token));
        } else if (token == "+" || token == "-" || token == "*" || token == "/") {
            while (!ops.empty()) {
                int right = values.top(); values.pop();
                int left = values.top(); values.pop();
                values.push(applyOp(left, right, ops.top()));
                ops.pop();
            }
            ops.push(token[0]);
        }
        // else{
        //     cout << "Error: Undefined variable '" << token << "' used in expression." << endl;
        //     return -1;
        // }
    }
    
    while (!ops.empty()) {
        int right = values.top(); values.pop();
        int left = values.top(); values.pop();
        values.push(applyOp(left, right, ops.top()));
        ops.pop();
    }
    
    return values.top();
}

// Function to build a Parse Tree
// TreeNode* buildParseTree(vector<string>& tokens) {
//     stack<TreeNode*> nodeStack;
//     stack<char> opStack;

//     auto precedence = [](char op) {
//         if (op == '+' || op == '-') return 1;
//         if (op == '*' || op == '/') return 2;
//         return 0;
//     };

//     for (const string &token : tokens) {
//         if (isdigit(token[0]) || lookupSymbolTable(token)) {
//             nodeStack.push(new TreeNode(token));
//         } else {
//             while (!opStack.empty() && precedence(opStack.top()) >= precedence(token[0])) {
//                 TreeNode *right = nodeStack.top(); nodeStack.pop();
//                 TreeNode *left = nodeStack.top(); nodeStack.pop();
//                 TreeNode *opNode = new TreeNode(string(1, opStack.top())); 
//                 opStack.pop();
//                 opNode->left = left;
//                 opNode->right = right;
//                 nodeStack.push(opNode);
//             }
//             opStack.push(token[0]);
//         }
//     }

//     while (!opStack.empty()) {
//         TreeNode *right = nodeStack.top(); nodeStack.pop();
//         TreeNode *left = nodeStack.top(); nodeStack.pop();
//         TreeNode *opNode = new TreeNode(string(1, opStack.top())); 
//         opStack.pop();
//         opNode->left = left;
//         opNode->right = right;
//         nodeStack.push(opNode);
//     }

//     return nodeStack.top();
// }



// TreeNode* buildParseTree(vector<string>& tokens) {  //  last version
//     stack<TreeNode*> nodeStack;
//     stack<string> opStack;

//     auto precedence = [](string op) {
//         if (op == "+" || op == "-") return 1;
//         if (op == "*" || op == "/") return 2;
//         return 0;
//     };

//     for (const string &token : tokens) {
//         if (isdigit(token[0]) || lookupSymbolTable(token)) {
//             nodeStack.push(new TreeNode(token));
//         } else if (token == "+" || token == "-" || token == "*" || token == "/") {
//             while (!opStack.empty()) {
//                 string op = opStack.top(); opStack.pop();
//                 TreeNode *right = nodeStack.top(); nodeStack.pop();
                
//                 TreeNode *left = nullptr;
//                 if (!nodeStack.empty()) { 
//                     left = nodeStack.top(); 
//                     nodeStack.pop(); 
//                 }
            
//                 TreeNode *opNode = new TreeNode(op);
//                 opNode->left = left;
//                 opNode->right = right;
//                 nodeStack.push(opNode);
//             }            
//             opStack.push(token);
//         } else if (token == "=") {
//             opStack.push(token);
//         }
//     }

//     while (!opStack.empty()) {
//         string op = opStack.top(); opStack.pop();
//         TreeNode *right = nodeStack.top(); nodeStack.pop();
//         TreeNode *left = nullptr;
//         if (!nodeStack.empty()) {
//             left = nodeStack.top();
//             nodeStack.pop();
//         }
        
//         TreeNode *opNode = new TreeNode(op);
//         opNode->left = left;
//         opNode->right = right;
//         nodeStack.push(opNode);
//     }

//     return nodeStack.top();
// }

TreeNode* buildParseTree(vector<string>& tokens) {
    stack<TreeNode*> nodeStack;
    stack<string> opStack;

    auto precedence = [](string op) {
        if (op == "=") return 0;  // Lowest precedence (assignment should be root)
        if (op == "+" || op == "-") return 1;
        if (op == "*" || op == "/") return 2;
        return -1;
    };

    TreeNode* root = nullptr;
    
    for (const string &token : tokens) {
        if (isdigit(token[0]) || lookupSymbolTable(token)) {
            nodeStack.push(new TreeNode(token));
        } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "=") {
            while (!opStack.empty() && precedence(opStack.top()) >= precedence(token)) {
                string op = opStack.top(); opStack.pop();
                TreeNode *right = nodeStack.top(); nodeStack.pop();
                TreeNode *left = nodeStack.empty() ? nullptr : nodeStack.top();
                if (!nodeStack.empty()) nodeStack.pop();

                TreeNode *opNode = new TreeNode(op);
                opNode->left = left;
                opNode->right = right;
                nodeStack.push(opNode);
            }
            opStack.push(token);
        }
    }

    while (!opStack.empty()) {
        string op = opStack.top(); opStack.pop();
        TreeNode *right = nodeStack.top(); nodeStack.pop();
        TreeNode *left = nodeStack.empty() ? nullptr : nodeStack.top();
        if (!nodeStack.empty()) nodeStack.pop();

        TreeNode *opNode = new TreeNode(op);
        opNode->left = left;
        opNode->right = right;
        nodeStack.push(opNode);
    }

    root = nodeStack.empty() ? nullptr : nodeStack.top();
    return root;
}




void visualizeParseTree(TreeNode* root, string filename) {
    ofstream file(filename);
    file << "digraph ParseTree {" << endl;

    function<void(TreeNode*)> writeGraph;
    writeGraph = [&](TreeNode* node) {
        if (!node) return;
        if (node->left) {
            file << "    \"" << node->value << "\" -> \"" << node->left->value << "\";" << endl;
            writeGraph(node->left);
        }
        if (node->right) {
            file << "    \"" << node->value << "\" -> \"" << node->right->value << "\";" << endl;
            writeGraph(node->right);
        }
    };

    writeGraph(root);
    file << "}" << endl;
    file.close();
    cout << "Parse Tree saved as " << filename << endl;
    //system("dot -Tpng parse_tree.dot -o parse_tree.png && start parse_tree.png");
}

/* int main() {
    int choice;
    string input;
    do {
        cout << "\nMenu:\n";
        cout << "1. Enter Expression\n";
        cout << "2. Display Symbol Table\n";
        cout << "3. Visualize Parse Tree\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
    
        if (!(cin >> choice)) {  // Check for invalid input
            cin.clear();  // Clear error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Ignore invalid input
            cout << "Invalid choice! Please enter a number between 1-4.\n";
            continue;
        }
        cin.ignore();  // Clear the newline character from input buffer
    
        // if (choice == 1) { 
        //     if (choice == 1) {
        //         cout << "Enter an expression ending with ';': ";
        //         getline(cin, input);
        //         if (input.back() == ';') input.pop_back();
        //         vector<string> tokens = tokenizeExpression(input);
            
        //         if (tokens.size() > 2 && tokens[1] == "=") {  
        //             string varName = tokens[0];  // First token is the variable name
        //             tokens.erase(tokens.begin(), tokens.begin() + 2); // Remove "a ="
            
        //             int result = evaluateExpression(tokens);
        //             insertIntoSymbolTable(varName, "int", result);
        //             cout << "Assigned " << varName << " = " << result << endl;
            
        //             TreeNode* root = buildParseTree(tokens);
        //             visualizeParseTree(root, "parse_tree.dot");
        //         } else {
        //             cout << "Invalid expression! Use assignment (e.g., a = b * 5 + 4;)" << endl;
        //         }
        //     }
            
        // } 
        if(choice == 1) {
                    cout << "Enter an expression ending with ';': ";
            getline(cin, input);

            // Split input into individual statements
            stringstream ss(input);
            string statement;
            while (getline(ss, statement, ';')) {
                if (statement.empty()) continue;

                vector<string> tokens = tokenizeExpression(statement);

                if (tokens.size() > 2 && tokens[1] == "=") {
                    string varName = tokens[0];
                    tokens.erase(tokens.begin(), tokens.begin() + 2);

                    try {
                        int result = evaluateExpression(tokens);
                        insertIntoSymbolTable(varName, "int", result);
                        cout << "Assigned " << varName << " = " << result << endl;

                        TreeNode* root = buildParseTree(tokens);
                        visualizeParseTree(root, "parse_tree.dot");
                    } catch (exception& e) {
                        cerr << "Error evaluating expression: " << e.what() << endl;
                    }
                } else {
                    cout << "Invalid expression: " << statement << endl;
                }
            }
        }

        else if (choice == 2) {
            printSymbolTable();
        } else if (choice == 3) {
            system("dot -Tpng parse_tree.dot -o parse_tree.png && start parse_tree.png");
        } else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        } else {
            cout << "Invalid choice! Enter a number between 1-4.\n";
        }
    } while (true);    
    return 0;
}*/

int main() {
    int choice;
    string input;

    do {
        cout << "\nMenu:\n";
        cout << "1. Enter Expression\n";
        cout << "2. Display Symbol Table\n";
        cout << "3. Visualize Parse Tree\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {  // Check for invalid input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid choice! Please enter a number between 1-4.\n";
            continue;
        }
        cin.ignore();  // Clear newline character from input buffer

        if (choice == 1) {
            cout << "Enter an expression ending with ';': ";
            getline(cin, input);

            // Split input into individual statements
            stringstream ss(input);
            string statement;
            while (getline(ss, statement, ';')) {
                if (statement.empty()) continue;

                vector<string> tokens = tokenizeExpression(statement);

                if (tokens.size() > 2 && tokens[1] == "=") {  // Check for assignment
                    string varName = tokens[0];  // Variable being assigned

                    // Separate RHS tokens for evaluation (ignore "c =")
                    vector<string> rhsTokens(tokens.begin() + 2, tokens.end());

                    try {
                        int result = evaluateExpression(rhsTokens);
                        insertIntoSymbolTable(varName, "int", result);
                        cout << "Assigned " << varName << " = " << result << endl;

                        // Build parse tree using full tokens (including assignment)
                        TreeNode* root = buildParseTree(tokens);
                        visualizeParseTree(root, "parse_tree.dot");
                    } catch (exception& e) {
                        cerr << "Error evaluating expression: " << e.what() << endl;
                    }
                } else {
                    cout << "Invalid expression: " << statement << endl;
                }
            }
        } 
        else if (choice == 2) {
            printSymbolTable();
        } 
        else if (choice == 3) {
            system("dot -Tpng parse_tree.dot -o parse_tree.png && start parse_tree.png");
        } 
        else if (choice == 4) {
            cout << "Exiting...\n";
            break;
        } 
        else {
            cout << "Invalid choice! Enter a number between 1-4.\n";
        }
    } while (true);

    return 0;
}
