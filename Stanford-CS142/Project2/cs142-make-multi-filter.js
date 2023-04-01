'use strict';

function cs142MakeMultiFilter(_originalArray) {
    const originalArray = _originalArray;
    let currentArray = [..._originalArray];

    const closed = function(...args) {
        if (args.length === 0) { return currentArray; }

        const filterCriteria = args[0];
        if (typeof(filterCriteria) !== "function") { return currentArray; }
        currentArray = currentArray.filter((x) => filterCriteria.call(originalArray, x));
        if (args.length === 2) {
            const callback = args[1];
            callback.call(originalArray, currentArray);
        }
        return closed;
    };
    return closed;
}