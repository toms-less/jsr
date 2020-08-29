
function sum(left, right) {
    return left + right;
}

function div(dividend, divisor) {
    if (typeof dividend !== 'number' || typeof divisor !== 'number' || divisor == 0) {
        return 0;
    }
    return dividend / divisor;
}

export { sum, div }