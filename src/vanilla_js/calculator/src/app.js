
const numbers = [0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9, '.'];

for (const number of numbers) {
    document.getElementById(number).addEventListener('click', function() {
        display.value += number;
    }
    );
}

const operators = ['plus', 'minus', 'multiply', 'divide'];
const operator_to_sign = {'plus': '+', 'minus': '-', 'multiply': '*', 'divide': '/'};

for (const operator of operators) {
    document.getElementById(operator).addEventListener('click', function() {
        display.value += " " + operator_to_sign[operator] + " ";
    }
    );
}

document.getElementById('clear').addEventListener('click', function() {
    display.value = "";
}
);


document.getElementById('equal').addEventListener('click', function() {
    display.value = eval(display.value);
}
);