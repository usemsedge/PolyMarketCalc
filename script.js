import createMarketCalcModule from './marketcalc.js';

let wasmModule;

// Load the WASM module
createMarketCalcModule().then((module) => {
    wasmModule = module;
    console.log('WASM Module Loaded');
});

// Handle button click
document.getElementById('calculate').addEventListener('click', () => {
    const num1 = parseInt(document.getElementById('num1').value, 10);
    const num2 = parseInt(document.getElementById('num2').value, 10);

    if (wasmModule) {
        const result = wasmModule._sum(num1, num2); // Call the WASM function
        document.getElementById('result').textContent = `Result: ${result}`;
    } else {
        document.getElementById('result').textContent = 'WASM Module not loaded yet.';
    }
});