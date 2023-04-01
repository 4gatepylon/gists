'use strict';

class TemplateProc {
    constructor (template) {
        this.template = template;
    }
    
    fillIn(dictionary) {
        return this.template.replace(
            // `g` denotes global replacement
            // braces inside do not require escaping
            /\{\{([^{}]*)\}\}/g,
            (_, p1) => (dictionary == null  || dictionary == undefined ? (
                p1 
            ) : (
                (dictionary[p1] === undefined ? '' : dictionary[p1]))
            ),
        );
    };
}

function AsArray(coll) { return Array.prototype.slice.call(coll); }

class TableTemplate {
    static fillIn(id, dict, applicableColumnName, setVisible = true) {
        // Get the very first match for a table row (skipping a potential
        // intermediate tbody)
        const tableHeaderRow = document.querySelector(`#${id} tr`);
        const allColumnNames = AsArray(tableHeaderRow.querySelectorAll(`td`))
            .filter((node) => node.innerHTML == node.textContent) // Must not have HTML under
            .map((node) => {
                const text = node.textContent
                const columnName = new TemplateProc(text).fillIn(null)
                // Side effect! (We should always replace the header... remember that)
                node.innerHTML = columnName
                return columnName
            })
        
        // Find the columns we will only apply our changes on
        const applicableColumnNames = (applicableColumnName == null || applicableColumnName == undefined) ? (
            allColumnNames
        ) : (
            // If the column name is not a match do nothing
            allColumnNames.includes(applicableColumnName) ? [applicableColumnName] : []
        );
        
        // O(n), keep mutability in closure
        const applicableColumnIndices = ((namesSet, namesSuperArray) => {
            let ans = new Set();
            namesSuperArray.forEach((name, i) => {
                if (namesSet.has(name)) {
                    namesSet.delete(name);
                    ans.add(i);
                }
            });
            return ans;
        })(new Set(applicableColumnNames), allColumnNames);
        
        // Apply the changes
        applicableColumnNames.forEach((columnName) => {
            // Ignore the very first one (since that was header)
            AsArray(document.querySelectorAll(`#${id} tr`)).forEach((row, i) => {
                if (i > 0) {
                    AsArray(row.querySelectorAll(`td`))
                    .map((obj, j) => { return {object: obj, index: j} })
                    // Must not have HTML under
                    .filter((node_obj) => node_obj.object.innerHTML == node_obj.object.textContent)
                    .forEach((data_obj) => {
                        if (applicableColumnIndices.has(data_obj.index)) {
                            // Make the change for each td in this row
                            const data = data_obj.object;
                            data.innerHTML = new TemplateProc(data.textContent).fillIn(dict);
                        }
                    })
                }
            })
        })

        // Make tables visible
        if (setVisible) {
            document.getElementById(id).style.visibility = 'visible';
        }
    }
}