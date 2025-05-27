function showSection(sectionId) {
    document.querySelectorAll('.section').forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
    if (sectionId === 'symbol-table') refreshSymbolTable();
}

async function evaluateExpression() {
    const exprInput = document.getElementById('exprInput').value.trim();
    const exprResult = document.getElementById('exprResult');

    if (!exprInput) {
        exprResult.textContent = 'Error: Input cannot be empty.';
        exprResult.style.color = 'red';
        return;
    }

    exprResult.textContent = 'Evaluating...';
    exprResult.style.color = 'black';

    try {
        const response = await fetch('/execute', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ choice: 1, input: exprInput })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const data = await response.json();
        if (data.error) {
            throw new Error(data.error);
        }

        exprResult.textContent = data.output || 'Evaluation successful.';
        exprResult.style.color = 'green';
    } catch (error) {
        exprResult.textContent = `Error: ${error.message}`;
        exprResult.style.color = 'red';
    }
}

async function refreshSymbolTable() {
    const tbody = document.getElementById('symbolTableBody');
    tbody.innerHTML = '<tr><td colspan="4">Loading...</td></tr>';

    try {
        const response = await fetch('/execute', {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ choice: 2 })
        });

        if (!response.ok) {
            throw new Error(`HTTP error! Status: ${response.status}`);
        }

        const data = await response.json();
        if (data.error) {
            throw new Error(data.error);
        }

        const output = data.output || '';
        tbody.innerHTML = '';
        const lines = output.split('\n').filter(line => line.includes('\t'));
        if (lines.length === 0) {
            tbody.innerHTML = '<tr><td colspan="4">No variables in symbol table.</td></tr>';
            return;
        }

        lines.forEach(line => {
            const [name, type, value, scope] = line.split('\t').map(s => s.trim());
            if (name && type && value && scope) {
                const row = document.createElement('tr');
                row.innerHTML = `
                    <td>${name}</td>
                    <td>${type}</td>
                    <td>${value}</td>
                    <td>${scope}</td>
                `;
                tbody.appendChild(row);
            }
        });
    } catch (error) {
        tbody.innerHTML = `<tr><td colspan="4" style="color: red;">Error: ${error.message}</td></tr>`;
    }
}

async function visualizeParseTree() {
    try {
        const response = await fetch('/visualize');
        if (!response.ok) {
            throw new Error('Failed to generate parse tree image.');
        }

        const blob = await response.blob();
        const url = URL.createObjectURL(blob);
        const img = document.getElementById('parseTreeImg');
        img.src = url;
        img.onload = () => URL.revokeObjectURL(url); // Clean up
        showSection('visualize');
    } catch (error) {
        const img = document.getElementById('parseTreeImg');
        img.src = ''; // Clear previous image
        img.alt = `Error: ${error.message}`;
        showSection('visualize');
    }
}