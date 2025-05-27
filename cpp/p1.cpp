#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <stack>
#include <fstream>
#include <stdexcept>
#include <regex>
#include <limits>     // Added for numeric_limits and streamsize
#include <functional> // Added for std::function

using namespace std;

struct SymbolInfo
{
    string type;
    int value;
    string scope;
};

vector<map<string, SymbolInfo>> symbolTableStack; // Stack of symbol tables for scope handling
int scopeCounter = 0;                             // To generate unique scope names

struct TreeNode
{
    string value;
    TreeNode *left;
    TreeNode *right;
};

bool isOperator(const string &token)
{
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^";
}

int precedence(const string &op)
{
    if (op == "+" || op == "-")
        return 1;
    if (op == "*" || op == "/" || op == "%")
        return 2;
    if (op == "^")
        return 3;
    return 0;
}

vector<string> tokenizeExpression(const string &expr)
{
    vector<string> tokens;
    string token;
    for (size_t i = 0; i < expr.length(); ++i)
    {
        char ch = expr[i];
        if (isspace(ch))
            continue;
        if (isdigit(ch))
        {
            token.clear();
            while (i < expr.length() && isdigit(expr[i]))
                token += expr[i++];
            --i;
            tokens.push_back(token);
        }
        else if (isalpha(ch))
        {
            token.clear();
            while (i < expr.length() && (isalnum(expr[i]) || expr[i] == '_'))
                token += expr[i++];
            --i;
            tokens.push_back(token);
        }
        else
        {
            tokens.push_back(string(1, ch));
        }
    }
    return tokens;
}

vector<string> infixToPostfix(const vector<string> &infix)
{
    stack<string> opStack;
    vector<string> postfix;

    for (const string &token : infix)
    {
        if (isalpha(token[0]) || isdigit(token[0]))
        {
            postfix.push_back(token);
        }
        else if (isOperator(token))
        {
            while (!opStack.empty() && opStack.top() != "(" && precedence(opStack.top()) >= precedence(token))
            {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            opStack.push(token);
        }
        else if (token == "(")
        {
            opStack.push(token);
        }
        else if (token == ")")
        {
            while (!opStack.empty() && opStack.top() != "(")
            {
                postfix.push_back(opStack.top());
                opStack.pop();
            }
            if (!opStack.empty())
                opStack.pop();
        }
    }

    while (!opStack.empty())
    {
        postfix.push_back(opStack.top());
        opStack.pop();
    }

    return postfix;
}

int integerPower(int base, int exp)
{
    int result = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            result *= base;
        }
        base *= base;
        exp /= 2;
    }
    return result;
}

int evaluateExpression(const vector<string> &postfix)
{
    stack<int> st;
    for (const string &token : postfix)
    {
        if (isdigit(token[0]))
        {
            try
            {
                st.push(stoi(token));
            }
            catch (const std::invalid_argument &e)
            {
                throw runtime_error("Invalid number format: " + token);
            }
        }
        else if (isalpha(token[0]))
        {
            bool found = false;
            // Search from the top of the scope stack downwards
            for (int i = symbolTableStack.size() - 1; i >= 0; --i)
            {
                auto &table = symbolTableStack[i];
                if (table.find(token) != table.end())
                {
                    st.push(table[token].value);
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                throw runtime_error("Variable '" + token + "' not declared in current scope.");
            }
        }
        else
        {
            if (st.size() < 2)
            {
                throw runtime_error("Invalid expression: insufficient operands for operator " + token);
            }
            int b = st.top();
            st.pop();
            int a = st.top();
            st.pop();
            if (token == "+")
                st.push(a + b);
            else if (token == "-")
                st.push(a - b);
            else if (token == "*")
                st.push(a * b);
            else if (token == "/")
            {
                if (b == 0)
                    throw runtime_error("Division by zero in expression.");
                st.push(a / b);
            }
            else if (token == "%")
            {
                if (b == 0)
                    throw runtime_error("Modulus by zero in expression.");
                st.push(a % b);
            }
            else if (token == "^")
                st.push(integerPower(a, b));
            else
            {
                throw runtime_error("Unknown operator: " + token);
            }
        }
    }
    if (st.size() != 1)
    {
        throw runtime_error("Invalid expression: too many operands.");
    }
    return st.top();
}

TreeNode *buildParseTreeWithAssignment(const string &varName, const vector<string> &postfix)
{
    stack<TreeNode *> st;
    for (const string &token : postfix)
    {
        if (isOperator(token))
        {
            if (st.size() < 2)
            {
                throw runtime_error("Invalid expression: insufficient operands for parse tree.");
            }
            TreeNode *right = st.top();
            st.pop();
            TreeNode *left = st.top();
            st.pop();
            st.push(new TreeNode{token, left, right});
        }
        else
        {
            st.push(new TreeNode{token, nullptr, nullptr});
        }
    }
    if (st.size() != 1)
    {
        throw runtime_error("Invalid expression: incomplete parse tree.");
    }
    TreeNode *exprTree = st.top();
    st.pop();
    TreeNode *assignNode = new TreeNode{"=", new TreeNode{varName, nullptr, nullptr}, exprTree};
    return assignNode;
}

void visualizeParseTree(const vector<TreeNode *> &parseTrees, const string &filename)
{
    ofstream fout(filename);
    if (!fout.is_open())
    {
        cerr << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }
    fout << "digraph ParseTree {\n";
    int id = 0;
    map<TreeNode *, int> ids;

    function<void(TreeNode *)> dfs = [&](TreeNode *node)
    {
        if (!node)
            return;
        ids[node] = id++;
        fout << "  " << ids[node] << " [label=\"" << node->value << "\"];\n";
        if (node->left)
        {
            dfs(node->left);
            fout << "  " << ids[node] << " -> " << ids[node->left] << ";\n";
        }
        if (node->right)
        {
            dfs(node->right);
            fout << "  " << ids[node] << " -> " << ids[node->right] << ";\n";
        }
    };

    for (size_t i = 0; i < parseTrees.size(); ++i)
    {
        fout << "  subgraph cluster_" << i << " {\n";
        fout << "    label=\"Assignment " << i + 1 << "\";\n";
        dfs(parseTrees[i]);
        fout << "  }\n";
    }

    fout << "}\n";
    fout.close();
}

void processDeclaration(const string &declaration, vector<TreeNode *> &parseTrees)
{
    try
    {
        regex pattern(R"(int\s+(.+);)");
        smatch match;

        string decl = declaration + ";";
        if (!regex_search(decl, match, pattern))
        {
            throw runtime_error("Invalid declaration syntax: " + declaration);
        }

        string vars = match[1];
        stringstream ss(vars);
        string token;

        while (getline(ss, token, ','))
        {
            string name;
            int value = 0;
            vector<string> exprTokens;

            size_t eq = token.find('=');
            if (eq != string::npos)
            {
                name = token.substr(0, eq);
                string expr = token.substr(eq + 1);
                exprTokens = tokenizeExpression(expr);
                if (exprTokens.empty())
                {
                    throw runtime_error("Empty expression in declaration: " + token);
                }
                vector<string> postfix = infixToPostfix(exprTokens);
                try
                {
                    value = evaluateExpression(postfix);
                    parseTrees.push_back(buildParseTreeWithAssignment(name, postfix));
                }
                catch (const std::exception &e)
                {
                    throw runtime_error("Error in expression '" + expr + "': " + e.what());
                }
            }
            else
            {
                name = token;
            }

            name = regex_replace(name, regex("^\\s+|\\s+$"), "");
            if (name.empty())
            {
                throw runtime_error("Invalid variable name in declaration: " + token);
            }
            if (!regex_match(name, regex("[a-zA-Z_][a-zA-Z0-9_]*")))
            {
                throw runtime_error("Invalid variable name syntax: " + name);
            }
            // Check if variable exists in the current scope
            if (symbolTableStack.back().find(name) != symbolTableStack.back().end())
            {
                throw runtime_error("Variable '" + name + "' already declared in current scope.");
            }

            symbolTableStack.back()[name] = {"int", value, symbolTableStack.size() == 1 ? "Global" : "Scope" + to_string(symbolTableStack.size() - 1)};
        }
        cout << "Variables declared." << endl; // Match test.mjs expectation
    }
    catch (const std::regex_error &e)
    {
        cerr << "Regex error in declaration '" << declaration << "': " << e.what() << endl;
    }
    catch (const std::runtime_error &e)
    {
        cerr << "Error: " << e.what() << endl;
    }
    catch (const std::exception &e)
    {
        cerr << "Unexpected error in declaration '" << declaration << "': " << e.what() << endl;
    }
    catch (...)
    {
        cerr << "Unknown error in declaration '" << declaration << "'.\n";
    }
}

void processInput(const string &input, vector<TreeNode *> &parseTrees)
{
    stringstream ss(input);
    string line;

    while (getline(ss, line))
    {
        line = regex_replace(line, regex("^\\s+|\\s+$"), "");
        if (line.empty())
            continue;

        if (line.back() != ';')
        {
            cerr << "Error: Statement must end with ';'. Input: " << line << endl;
            continue;
        }

        try
        {
            line.pop_back();
            if (line.find("int ") == 0)
            {
                processDeclaration(line, parseTrees);
            }
            else
            {
                vector<string> tokens = tokenizeExpression(line);
                if (tokens.size() < 3 || tokens[1] != "=")
                {
                    throw runtime_error("Invalid expression syntax: " + line);
                }

                string varName = tokens[0];
                bool found = false;
                int scopeIndex = -1;
                for (int i = symbolTableStack.size() - 1; i >= 0; --i)
                {
                    if (symbolTableStack[i].find(varName) != symbolTableStack[i].end())
                    {
                        found = true;
                        scopeIndex = i;
                        break;
                    }
                }
                if (!found)
                {
                    throw runtime_error("Variable '" + varName + "' not declared in current scope.");
                }

                tokens.erase(tokens.begin(), tokens.begin() + 2);
                vector<string> postfix = infixToPostfix(tokens);
                int result;
                try
                {
                    result = evaluateExpression(postfix);
                }
                catch (const std::exception &e)
                {
                    throw runtime_error("Error in expression '" + line + "': " + e.what());
                }
                symbolTableStack[scopeIndex][varName].value = result;
                cout << "Assigned " << varName << " = " << result << endl; // Match test.mjs expectation
                parseTrees.push_back(buildParseTreeWithAssignment(varName, postfix));
            }
        }
        catch (const std::runtime_error &e)
        {
            cerr << "Error: " << e.what() << endl;
        }
        catch (const std::exception &e)
        {
            cerr << "Unexpected error in statement '" << line << "': " << e.what() << endl;
        }
        catch (...)
        {
            cerr << "Unknown error in statement '" << line << "'.\n";
        }
    }
}

void displaySymbolTable()
{
    for (int i = symbolTableStack.size() - 1; i >= 0; --i)
    {
        for (const auto &entry : symbolTableStack[i])
        {
            cout << entry.first << "\t" << entry.second.type << "\t" << entry.second.value << "\t" << entry.second.scope << endl;
        }
    }
}

int main()
{
    // Initialize with a global scope
    symbolTableStack.push_back(map<string, SymbolInfo>());

    int choice;
    string input;

    while (true)
    {
        // Read the choice
        if (!(cin >> choice))
        {
            // If input fails (e.g., EOF or invalid input), clear the error and continue
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cerr << "Error: Invalid choice: unable to read integer." << endl;
            continue;
        }
        cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear any remaining input

        if (choice == 1)
        { // Process expression
            input.clear();
            string line;
            vector<TreeNode *> parseTrees;
            while (getline(cin, line))
            {
                if (line.empty())
                    break; // Empty line indicates end of input
                input += line + "\n";
            }
            processInput(input, parseTrees);
            if (!parseTrees.empty())
            {
                visualizeParseTree(parseTrees, "parse_tree.dot");
            }
            for (auto tree : parseTrees)
            {
                function<void(TreeNode *)> deleteTree = [&](TreeNode *node)
                {
                    if (!node)
                        return;
                    deleteTree(node->left);
                    deleteTree(node->right);
                    delete node;
                };
                deleteTree(tree);
            }
        }
        else if (choice == 2)
        { // Variable declaration
            input.clear();
            string line;
            vector<TreeNode *> parseTrees;
            while (getline(cin, line))
            {
                if (line.empty())
                    break;
                input += line + "\n";
            }
            processDeclaration(input, parseTrees);
            if (!parseTrees.empty())
            {
                visualizeParseTree(parseTrees, "parse_tree.dot");
            }
            for (auto tree : parseTrees)
            {
                function<void(TreeNode *)> deleteTree = [&](TreeNode *node)
                {
                    if (!node)
                        return;
                    deleteTree(node->left);
                    deleteTree(node->right);
                    delete node;
                };
                deleteTree(tree);
            }
        }
        else if (choice == 3)
        { // Display symbol table
            displaySymbolTable();
        }
        else if (choice == 4)
        { // Exit
            break;
        }
        else if (choice == 5)
        { // Enter new scope
            symbolTableStack.push_back(map<string, SymbolInfo>());
            scopeCounter++;
            cout << "New scope entered." << endl;
        }
        else if (choice == 6)
        { // Exit scope
            if (symbolTableStack.size() <= 1)
            {
                cerr << "Error: Cannot exit global scope." << endl;
            }
            else
            {
                symbolTableStack.pop_back();
                cout << "Scope exited." << endl;
            }
        }
        else if (choice == 7)
        { // Reset state
            symbolTableStack.clear();
            symbolTableStack.push_back(map<string, SymbolInfo>()); // Reinitialize with global scope
            scopeCounter = 0;
            cout << "State reset." << endl;
        }
        else
        {
            cerr << "Error: Invalid choice: " << choice << endl;
        }
    }

    return 0;
}