// test.mjs
import fetch from 'node-fetch';

async function runTests() {
    console.log("Running Automated Test Cases...\n");

    // Helper function to make requests
    async function execute(choice, input = null) {
        const response = await fetch('http://localhost:3000/execute', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ choice, input })
        });
        const data = await response.json();
        if (data.error) {
            throw new Error(`Server error: ${data.error}`);
        }
        return data;
    }

    // Test 1: Variable Declaration
    console.log("Test 1: Variable Declaration");
    let data = await execute(2, "int a, b = 10;");
    let passed = data.output.includes("Variables declared.");
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output: ${data.output}\n`);

    // Test 2: Expression Evaluation (Basic Operators)
    console.log("Test 2: Expression Evaluation (Basic Operators)");
    data = await execute(1, "a = 5 + 3 * 2;");
    passed = data.output.includes("Assigned a = 11");
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output: ${data.output}\n`);

    // Test 3: Expression Evaluation (New Operators)
    console.log("Test 3: Expression Evaluation (New Operators)");
    data = await execute(1, "b = 2 ^ 3;");
    passed = data.output.includes("Assigned b = 8");
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output: ${data.output}\n`);

    // Test 4: Scope Handling
    console.log("Test 4: Scope Handling");
    data = await execute(2, "int x = 5;"); // Declare x in global scope
    console.log("Declared x in global scope:", data.output);

    data = await execute(5); // Enter new scope
    console.log("Entered new scope:", data.output);

    data = await execute(2, "int x = 20;"); // Declare x in new scope
    console.log("Declared x in new scope:", data.output);

    data = await execute(3); // Display symbol table
    let inScope = data.output.includes("x\tint\t20\tScope1");
    console.log("Symbol table in scope:", data.output);

    data = await execute(6); // Exit scope
    console.log("Exited scope:", data.output);

    data = await execute(3); // Display symbol table again
    let outScope = data.output.includes("x\tint\t5\tGlobal") && !data.output.includes("Scope1");
    console.log("Symbol table after exiting scope:", data.output);

    passed = inScope && outScope;
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output (after entering scope): x should be 20 in Scope1`);
    console.log(`Output (after exiting scope): x should be 5 in Global\n`);

    // Test 5: Error Handling
    console.log("Test 5: Error Handling (Undeclared Variable)");
    data = await execute(1, "w = 5 + 3;");
    passed = data.output.includes("Variable 'w' not declared in current scope.");
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output: ${data.output}\n`);
}

runTests().catch(err => console.error("Test Error:", err));