// test.mjs

async function checkServer() {
  try {
    const response = await fetch("http://localhost:3000/");
    if (!response.ok) {
      throw new Error(`Server not responding: ${response.status}`);
    }
    console.log("Server is running.");
  } catch (err) {
    console.error("Server check failed:", err.message);
    process.exit(1);
  }
}

async function execute(choice, input = null) {
  try {
    const response = await fetch("http://localhost:3000/execute", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ choice, input }),
    });
    if (!response.ok) {
      throw new Error(`HTTP error! status: ${response.status}`);
    }
    const data = await response.json();
    if (data.error) {
      throw new Error(`Server error: ${data.error}`);
    }
    return data;
  } catch (err) {
    console.error(`Error in execute (choice: ${choice}):`, err.message);
    throw err;
  }
}

async function resetState() {
  try {
    const data = await execute(7);
    console.log("State reset:", data.output);
  } catch (err) {
    console.error("Failed to reset state:", err.message);
    process.exit(1);
  }
}

async function runTests() {
  await checkServer();
  console.log("Running Automated Test Cases...\n");

  // Reset the state before running tests
  await resetState();

  try {
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
    data = await execute(2, "int x = 5;");
    console.log("Declared x in global scope:", data.output);

    data = await execute(5);
    console.log("Entered new scope:", data.output);

    data = await execute(2, "int x = 20;");
    console.log("Declared x in new scope:", data.output);

    data = await execute(3);
    let inScope = data.output.includes("x\tint\t20\tScope1");
    console.log("Symbol table in scope:", data.output);

    data = await execute(6);
    console.log("Exited scope:", data.output);

    data = await execute(3);
    let outScope =
      data.output.includes("x\tint\t5\tGlobal") &&
      !data.output.includes("Scope1");
    console.log("Symbol table after exiting scope:", data.output);

    passed = inScope && outScope;
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output (after entering scope): x should be 20 in Scope1`);
    console.log(`Output (after exiting scope): x should be 5 in Global\n`);

    // Test 5: Error Handling
    console.log("Test 5: Error Handling (Undeclared Variable)");
    data = await execute(1, "w = 5 + 3;");
    passed = data.output.includes(
      "Variable 'w' not declared in current scope."
    );
    console.log(`Result: ${passed ? "Pass" : "Fail"}`);
    console.log(`Output: ${data.output}\n`);
  } catch (err) {
    console.error("Test Error:", err.message);
  }
}

runTests();
