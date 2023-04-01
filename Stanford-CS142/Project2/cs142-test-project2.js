'use strict';

/*
 * This file tests the CS142 Project #2 JavaScript assignment problems. It prints what
 * it finds to the console log and updates the text being displayed in the window with a
 * summary of the results.
 */

/* eslint-env browser, node */

// Result message for Problems 1-3

// You can access globals or modify them with
// globals(operation)(args)
const globals = (() => {
  let p1Message = 'SUCCESS';
  let p2Message = 'SUCCESS';
  let p3Message = 'SUCCESS';

  // Keep track of all the let statements
  const varDeclared = ['varDeclared', 'p1Message', 'p2Message', 'p3Message'];

  const template = 'My favorite month is {{month}} but not the day {{day}} or the year {{year}}';
  const dictionary = { month: 'July', day: '1', year: '2016' };

  return (operation) => {
    if (typeof operation !== 'string') return () => {};
    switch(operation) {
      case 'p1Message': return () => p1Message;
      case 'p2Message': return () => p2Message;
      case 'p3Message': return () => p3Message;
      case 'setp1Message': return (message) => {p1Message = message;};
      case 'setp2Message': return (message) => {p2Message = message;};
      case 'setp3Message': return (message) => {p3Message = message;};
      case 'varDeclared': return () => varDeclared;
      case 'pushvarDeclared': return (varName) => {varDeclared.push(varName);};
      case 'template': return () => template;
      case 'dictionary': return () => dictionary;
      default: return () => {};
    }
  };
})();

// Utility functions
function arraysAreTheSame(a1, a2) {
  if (!Array.isArray(a1) || !Array.isArray(a2) || (a1.length !== a2.length)) {
    return false;
  }
  for (var i = 0; i < a1.length; i += 1) {
    if (a1[i] !== a2[i]) {
      return false;
    }
  }
  return true;
}

// ********************* Test cs142MakeMultiFilter

if (typeof cs142MakeMultiFilter !== 'function') {
  console.error('cs142MakeMultiFilter is not a function', typeof cs142MakeMultiFilter);
  globals('setp1Message')('FAILURE');
} else {
  const originalArray = [1, 2, 3];
  const filterFunc = window.cs142MakeMultiFilter(originalArray);

  const secondArray = [1, 2, 3, 4];
  const filterFuncTwo = window.cs142MakeMultiFilter(secondArray);

  if (typeof filterFunc !== 'function') {
    console.error('cs142MakeMultiFilter does not return a function', filterFunc);
    globals('setp1Message')('FAILURE');
  } else {
    let result = filterFunc();
    if (!arraysAreTheSame([1, 2, 3], result)) {
      console.error('filter function with no args does not return the original array', result);
      globals('setp1Message')('FAILURE');
    }

    let callbackPerformed = false;
    result = filterFunc(function (item) {
      return item !== 2;
    }, function (callbackResult) {
      callbackPerformed = true;
      if (!arraysAreTheSame([1, 3], callbackResult)) {
        console.error('filter function callback does not filter 2 correctly', callbackResult);
        globals('setp1Message')('FAILURE');
      }
      if (!arraysAreTheSame([1, 2, 3], this)) {
        console.error('filter function callback does not pass original array as this', this);
        globals('setp1Message')('FAILURE');
      }
    });

    if (!callbackPerformed) {
      console.error('filter function callback not performed');
      globals('setp1Message')('FAILURE');
    }

    if (result !== filterFunc) {
      console.error('filter function does not return itself', result);
      globals('setp1Message')('FAILURE');
    }

    result = filterFunc(function (item) {
      return item !== 3;
    });
    if (result !== filterFunc) {
      console.error('filter function does not return itself 2', result);
      globals('setp1Message')('FAILURE');
    }

    result = filterFunc();
    if (!arraysAreTheSame([1], result)) {
      console.error('filter function callback does not filter 3 correctly', result);
      globals('setp1Message')('FAILURE');
    }
    result = filterFuncTwo(function (item) {
      return item !== 1;
    }, function (callbackResult) {
      if (!arraysAreTheSame([2, 3, 4], callbackResult)) {
        console.error('second filter does not filter 1 (check for global variable usage)', callbackResult);
        globals('setp1Message')('FAILURE');
      }
      if (!arraysAreTheSame([1, 2, 3, 4], this)) {
        console.error('filter function callback does not pass original array as this', this);
        globals('setp1Message')('FAILURE');
      }
    });
  }
}
console.log('Test cs142MakeMultiFilter:', globals('p1Message')());

// ********************* Test Cs142TemplateProcessor

if (typeof Cs142TemplateProcessor !== 'function') {
  console.error('Cs142TemplateProcessor is not a function', typeof Cs142TemplateProcessor);
  globals('setp2Message')('FAILURE');
} else {
  const template = globals('template')();
  const dateTemplate = new window.Cs142TemplateProcessor(template);
  const dictionary = globals('dictionary')();
  const str = dateTemplate.fillIn(dictionary);

  if (str !== 'My favorite month is July but not the day 1 or the year 2016') {
    console.error('Cs142TemplateProcessor didn\'t work');
    globals('setp2Message')('FAILURE');
  }
  globals('pushvarDeclared')('template');
  globals('pushvarDeclared')('dateTemplate');
  globals('pushvarDeclared')('dictionary');
  globals('pushvarDeclared')('str');
}
console.log('Test Cs142TemplateProcessor:', globals('p2Message')());

// ********************* Test to see if the symbols we defined are in the global address space

globals('varDeclared')().forEach(function (sym) {
  if (window[sym] !== undefined) {
    console.error('Found my symbol', sym, 'in DOM');
    globals('setp3Message')('FAILURE');
  }
});
console.log('Test Problem 3:', globals('p3Message')());

// Store the result back into the global space under the object name cs142Project2Results
window.cs142Project2Results = {
  p1Message: globals('p1Message')(),
  p2Message: globals('p2Message')(),
  p3Message: globals('p3Message')(),
};

// Once the browser loads our companion HTML in cs142-test-project2.html we
// update it with the results of our testing. This code will make more
// sense after the next project.
window.onload = function () {
  document.getElementById('cs142p1').innerHTML = globals('p1Message')();
  document.getElementById('cs142p2').innerHTML = globals('p2Message')();
  document.getElementById('cs142p3').innerHTML = globals('p3Message')();
};
