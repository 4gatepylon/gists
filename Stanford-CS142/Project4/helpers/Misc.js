const ArraysEqualTill = (a1, a2, i) => i >= a1.length || a1[i] == a2[i] && ArraysEqualTill(a1, a2, i+1)
export const ArraysEqual = (a1, a2) => a1.length == a2.length && ArraysEqualTill(a1, a2, 0)
export const isNumber = (n) => typeof n == 'number' || (typeof n === 'string' && !isNaN(n) && !isNaN(parseInt(n)))
export const isInt = (n) => isNumber(n) && !isNaN(parseInt(`${n}`))
