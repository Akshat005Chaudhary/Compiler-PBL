// // const express = require('express');
// // const { spawn } = require('child_process');
// // const path = require('path');
// // const app = express();
// // const port = 3000;

// // app.use(express.static('public'));
// // app.use(express.json());

// // const cppDir = path.join(__dirname, 'cpp');
// // const cppExecutable = path.join(cppDir, process.platform === 'win32' ? 'p1.exe' : 'p1');

// // // Spawn the C++ process once at server start
// // const cppProcess = spawn(cppExecutable, { cwd: cppDir });
// // let outputBuffer = '';

// // // Handle stdout
// // cppProcess.stdout.on('data', (data) => {
// //     outputBuffer += data.toString();
// // });

// // cppProcess.stderr.on('data', (data) => {
// //     console.error(`C++ Error: ${data}`);
// // });

// // cppProcess.on('close', (code) => {
// //     console.log(`C++ process exited with code ${code}`);
// // });

// // app.post('/execute', (req, res) => {
// //     const { choice, input } = req.body;
// //     outputBuffer = ''; // Reset buffer for new command

// //     // Send command to C++ process
// //     cppProcess.stdin.write(`${choice}\n`);
// //     if (input) {
// //         cppProcess.stdin.write(`${input}\n`);
// //     }

// //     // Wait briefly for output (adjust delay as needed)
// //     setTimeout(() => {
// //         if (outputBuffer) {
// //             res.json({ output: outputBuffer });
// //         } else {
// //             res.status(500).json({ error: 'No output received' });
// //         }
// //     }, 100); // 100ms delay to allow C++ to process
// // });

// // app.get('/visualize', (req, res) => {
// //     const dotFile = path.join(cppDir, 'parse_tree.dot');
// //     const pngFile = path.join(cppDir, 'parse_tree.png');
// //     const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

// //     dotProcess.on('close', (code) => {
// //         if (code === 0) {
// //             res.sendFile(pngFile);
// //         } else {
// //             res.status(500).json({ error: 'Failed to generate parse tree image' });
// //         }
// //     });
// // });

// // app.listen(port, () => {
// //     console.log(`Server running at http://localhost:${port}`);
// // });

// // // Graceful shutdown
// // process.on('SIGINT', () => {
// //     cppProcess.stdin.write('5\n'); // Send exit command (assuming 5 exits the loop)
// //     cppProcess.on('close', () => process.exit());
// // });



// // server.js
// import express from 'express';
// import { spawn } from 'child_process';
// import { join } from 'path';
// import { fileURLToPath } from 'url';
// import { dirname } from 'path';

// const __filename = fileURLToPath(import.meta.url);
// const __dirname = dirname(__filename);

// const app = express();
// const port = 3000;

// app.use(express.static('public'));
// app.use(express.json());

// const cppDir = join(__dirname, 'cpp');
// const cppExecutable = join(cppDir, process.platform === 'win32' ? 'p1.exe' : 'p1');

// // Spawn the C++ process once at server start
// const cppProcess = spawn(cppExecutable, { cwd: cppDir });
// let outputBuffer = '';

// // Handle stdout
// cppProcess.stdout.on('data', (data) => {
//     outputBuffer += data.toString();
// });

// cppProcess.stderr.on('data', (data) => {
//     console.error(`C++ Error: ${data}`);
// });

// cppProcess.on('close', (code) => {
//     console.log(`C++ process exited with code ${code}`);
// });

// // app.post('/execute', async (req, res) => {
// //     const { choice, input } = req.body;
// //     outputBuffer = ''; // Reset buffer for new command

// //     // Send command to C++ process
// //     cppProcess.stdin.write(`${choice}\n`);
// //     if (input) {
// //         cppProcess.stdin.write(`${input}\n`);
// //     }

// //     // Wait briefly for output (adjust delay as needed)
// //     await new Promise(resolve => setTimeout(resolve, 100)); // 100ms delay to allow C++ to process
// //     if (outputBuffer) {
// //         res.json({ output: outputBuffer });
// //     } else {
// //         res.status(500).json({ error: 'No output received' });
// //     }
// // });

// app.post('/execute', async (req, res) => {
//     const { choice, input } = req.body;
//     outputBuffer = ''; // Reset buffer for new command

//     // Send command to C++ process
//     cppProcess.stdin.write(`${choice}\n`);
//     if (input) {
//         cppProcess.stdin.write(`${input}\n`);
//     }

//     // Wait briefly for output (adjust delay as needed)
//     await new Promise(resolve => setTimeout(resolve, 500)); // 100ms delay to allow C++ to process
//     if (outputBuffer) {
//         res.json({ output: outputBuffer });
//     } else {
//         res.status(500).json({ error: 'No output received' });
//     }
// });

// app.get('/visualize', async (req, res) => {
//     const dotFile = join(cppDir, 'parse_tree.dot');
//     const pngFile = join(cppDir, 'parse_tree.png');
//     const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

//     dotProcess.on('close', (code) => {
//         if (code === 0) {
//             res.sendFile(pngFile);
//         } else {
//             res.status(500).json({ error: 'Failed to generate parse tree image' });
//         }
//     });
// });

// app.listen(port, () => {
//     console.log(`Server running at http://localhost:${port}`);
// });

// // Graceful shutdown
// process.on('SIGINT', () => {
//     cppProcess.stdin.write('7\n'); // Updated to choice 7 (exit) based on new menu
//     cppProcess.on('close', () => process.exit());
// });






import express from 'express';
import { spawn } from 'child_process';
import { join } from 'path';
import { fileURLToPath } from 'url';
import { dirname } from 'path';
import { existsSync } from 'fs';

const __filename = fileURLToPath(import.meta.url);
const __dirname = dirname(__filename);

const app = express();
const port = process.env.PORT || 3000;

app.use(express.static('public'));
app.use(express.json());

const cppDir = join(__dirname, 'cpp');
const cppExecutable = join(cppDir, process.platform === 'win32' ? 'p1.exe' : 'p1');

if (!existsSync(cppExecutable)) {
    console.error(`Error: C++ executable not found at ${cppExecutable}`);
    process.exit(1);
}

let cppProcess;
try {
    cppProcess = spawn(cppExecutable, { cwd: cppDir });
} catch (error) {
    console.error(`Error spawning C++ process: ${error.message}`);
    process.exit(1);
}

let outputBuffer = '';
let errorBuffer = '';

cppProcess.stdout.on('data', (data) => {
    outputBuffer += data.toString();
});

cppProcess.stderr.on('data', (data) => {
    errorBuffer += data.toString();
    console.error(`C++ Error: ${data}`);
});

cppProcess.on('error', (error) => {
    console.error(`C++ process error: ${error.message}`);
});

cppProcess.on('close', (code) => {
    console.log(`C++ process exited with code ${code}`);
    if (code !== 0) {
        console.error(`C++ process exited abnormally. Error output: ${errorBuffer}`);
    }
});

app.post('/execute', async (req, res) => {
    const { choice, input } = req.body;

    if (!choice) {
        return res.status(400).json({ error: 'Choice is required.' });
    }
    if (choice === 1) {
        if (!input) {
            return res.status(400).json({ error: 'Input is required for this choice.' });
        }
    }

    outputBuffer = '';
    errorBuffer = '';

    if (!cppProcess || !cppProcess.stdin.writable) {
        return res.status(500).json({ error: 'C++ process is not running.' });
    }

    try {
        cppProcess.stdin.write(`${choice}\n`);
        if (input) {
            cppProcess.stdin.write(`${input}\n`);
            cppProcess.stdin.write('\n'); // Send an empty line to terminate multi-line input
        }
    } catch (error) {
        return res.status(500).json({ error: `Failed to communicate with C++ process: ${error.message}` });
    }

    try {
        await new Promise(resolve => setTimeout(resolve, 500));
        if (errorBuffer) {
            return res.status(500).json({ error: errorBuffer });
        }
        if (outputBuffer) {
            return res.json({ output: outputBuffer });
        }
        return res.status(500).json({ error: 'No output received from C++ process.' });
    } catch (error) {
        return res.status(500).json({ error: `Error processing request: ${error.message}` });
    }
});

// app.get('/visualize', async (req, res) => {
//     const dotFile = join(cppDir, 'parse_tree.dot');
//     const pngFile = join(cppDir, 'parse_tree.png');

//     if (!existsSync(dotFile)) {
//         return res.status(500).json({ error: 'Parse tree file (parse_tree.dot) not found.' });
//     }

//     try {
//         const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

//         dotProcess.on('error', (error) => {
//             res.status(500).json({ error: `Failed to run dot command: ${error.message}` });
//         });

//         dotProcess.on('close', (code) => {
//             if (code === 0) {
//                 if (existsSync(pngFile)) {
//                     res.sendFile(pngFile);
//                 } else {
//                     res.status(500).json({ error: 'Parse tree image (parse_tree.png) not generated.' });
//                 }
//             } else {
//                 res.status(500).json({ error: `dot command failed with code ${code}` });
//             }
//         });
//     } catch (error) {
//         res.status(500).json({ error: `Error generating parse tree: ${error.message}` });
//     }
// });  ------ commented for deployment

app.get('/visualize', async (req, res) => {
    const dotFile = join(cppDir, 'parse_tree.dot');
    const pngFile = join(cppDir, 'parse_tree.png');

    if (!existsSync(dotFile)) {
        return res.status(500).json({ error: 'Parse tree file (parse_tree.dot) not found.' });
    }

    try {
        const dotProcess = spawn('dot', ['-Tpng', dotFile, '-o', pngFile], { cwd: cppDir });

        let responded = false;  // flag to prevent duplicate sends

        dotProcess.on('error', (error) => {
            if (!responded) {
                responded = true;
                res.status(500).json({ error: `Failed to run dot command: ${error.message}` });
            }
        });

        dotProcess.on('close', (code) => {
            if (responded) return;
            responded = true;

            if (code === 0 && existsSync(pngFile)) {
                res.sendFile(pngFile);
            } else {
                res.status(500).json({
                    error: code !== 0
                        ? `dot command failed with code ${code}`
                        : 'Parse tree image (parse_tree.png) not generated.'
                });
            }
        });
    } catch (error) {
        return res.status(500).json({ error: `Error generating parse tree: ${error.message}` });
    }
});





app.listen(port, () => {
    console.log(`Server running at http://localhost:${port}`);
});

process.on('SIGINT', () => {
    console.log('Shutting down server...');
    if (cppProcess && cppProcess.stdin.writable) {
        try {
            cppProcess.stdin.write('4\n');
            cppProcess.stdin.end();
        } catch (error) {
            console.error(`Error during C++ process shutdown: ${error.message}`);
        }
    }
    cppProcess.on('close', () => {
        console.log('C++ process closed. Exiting server.');
        process.exit(0);
    });
    setTimeout(() => {
        console.error('C++ process did not close in time. Forcing exit.');
        process.exit(1);
    }, 2000);
});
