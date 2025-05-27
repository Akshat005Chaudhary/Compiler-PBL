import express from "express";
import { spawn } from "child_process";
import { join } from "path";
import { fileURLToPath } from "url";
import { dirname } from "path";
import { existsSync } from "fs";

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const port = process.env.PORT || 3000;

app.use(express.static("public"));
app.use(express.json());

const cppDir = join(__dirname, "cpp");
const cppExecutable = join(
  cppDir,
  process.platform === "win32" ? "p1.exe" : "p1"
);

if (!existsSync(cppExecutable)) {
  console.error(`Error: C++ executable not found at ${cppExecutable}`);
  process.exit(1);
}

let cppProcess;

function startCppProcess() {
  console.log("Starting C++ process...");
  cppProcess = spawn(cppExecutable, { cwd: cppDir });

  cppProcess.stdout.on("data", (data) => {
    console.log(`C++ Output: ${data}`);
    outputBuffer += data.toString();
  });

  cppProcess.stderr.on("data", (data) => {
    console.error(`C++ Error: ${data}`);
    errorBuffer += data.toString();
  });

  cppProcess.on("error", (error) => {
    console.error(`C++ process error: ${error.message}`);
  });

  cppProcess.on("close", (code) => {
    console.log(`C++ process exited with code ${code}`);
    if (code !== 0) {
      console.error(
        `C++ process exited abnormally. Error output: ${errorBuffer}`
      );
      startCppProcess();
    }
  });
}

let outputBuffer = "";
let errorBuffer = "";

startCppProcess();

process.on("unhandledRejection", (reason, promise) => {
  console.error("Unhandled Rejection at:", promise, "reason:", reason);
});

app.post("/execute", async (req, res) => {
  const { choice, input } = req.body;

  if (!choice) {
    return res.status(400).json({ error: "Choice is required." });
  }
  if (choice === 1 && !input) {
    return res
      .status(400)
      .json({ error: "Input is required for this choice." });
  }

  // Allow choice 7 for resetting the state
  if (choice < 1 || choice > 7) {
    return res
      .status(400)
      .json({ error: `Invalid choice: ${choice}. Must be between 1 and 7.` });
  }

  outputBuffer = "";
  errorBuffer = "";

  if (!cppProcess || cppProcess.exitCode !== null) {
    console.log("C++ process is not running. Restarting...");
    startCppProcess();
  }

  await new Promise((resolve) => setTimeout(resolve, 100));

  if (!cppProcess.stdin.writable) {
    return res
      .status(500)
      .json({ error: "C++ process is not ready to accept input." });
  }

  try {
    console.log(
      `Sending to C++ process: choice=${choice}, input=${input || "none"}`
    );
    cppProcess.stdin.write(`${choice}\n`);
    if (input) {
      cppProcess.stdin.write(`${input}\n`);
      cppProcess.stdin.write("\n");
    }
  } catch (error) {
    console.error(`Failed to write to C++ process: ${error.message}`);
    return res.status(500).json({
      error: `Failed to communicate with C++ process: ${error.message}`,
    });
  }

  let attempts = 0;
  const maxAttempts = 10;
  while (attempts < maxAttempts) {
    if (errorBuffer) {
      return res.status(500).json({ error: errorBuffer });
    }
    if (outputBuffer) {
      return res.json({ output: outputBuffer });
    }
    await new Promise((resolve) => setTimeout(resolve, 500));
    attempts++;
  }

  return res
    .status(500)
    .json({ error: "No output received from C++ process within timeout." });
});

app.get("/visualize", async (req, res) => {
  const dotFile = join(cppDir, "parse_tree.dot");
  const pngFile = join(cppDir, "parse_tree.png");

  if (!existsSync(dotFile)) {
    return res
      .status(500)
      .json({ error: "Parse tree file (parse_tree.dot) not found." });
  }

  try {
    const dotProcess = spawn("dot", ["-Tpng", dotFile, "-o", pngFile], {
      cwd: cppDir,
    });

    let responded = false;

    dotProcess.on("error", (error) => {
      if (!responded) {
        responded = true;
        res
          .status(500)
          .json({ error: `Failed to run dot command: ${error.message}` });
      }
    });

    dotProcess.on("close", (code) => {
      if (responded) return;
      responded = true;

      if (code === 0 && existsSync(pngFile)) {
        res.sendFile(pngFile);
      } else {
        res.status(500).json({
          error:
            code !== 0
              ? `dot command failed with code ${code}`
              : "Parse tree image (parse_tree.png) not generated.",
        });
      }
    });
  } catch (error) {
    return res
      .status(500)
      .json({ error: `Error generating parse tree: ${error.message}` });
  }
});

app.listen(port, () => {
  console.log(`Server running at http://localhost:${port}`);
});

process.on("SIGINT", () => {
  console.log("Shutting down server...");
  if (cppProcess && cppProcess.stdin.writable) {
    try {
      cppProcess.stdin.write("4\n");
      cppProcess.stdin.end();
    } catch (error) {
      console.error(`Error during C++ process shutdown: ${error.message}`);
    }
  }
  cppProcess.on("close", () => {
    console.log("C++ process closed. Exiting server.");
    process.exit(0);
  });
  setTimeout(() => {
    console.error("C++ process did not close in time. Forcing exit.");
    process.exit(1);
  }, 2000);
});
