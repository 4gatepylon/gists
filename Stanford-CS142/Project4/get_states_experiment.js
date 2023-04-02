'use strict';
const jsdom = require("jsdom");
const { JSDOM } = jsdom;

function AsArray(arr) { return Array.prototype.slice.call(arr); }

// You can always call async functions, you just can't await
(async () => {
    // Allows us to fetch from wikipedia from nodejs
    // (without getting cors-blocked by the browser, which is not
    // in my control because wikipedia has to allow it)
    // In reality you would request from the local server to do this on your behalf
    let wikipedia_ep = `https://en.wikipedia.org/wiki/List_of_states_and_territories_of_the_United_States`
    let states = [];

    // I could now away fetch if I wanted to do that instead because it's in an async
    fetch(wikipedia_ep).then((res) => {
        // https://stackoverflow.com/questions/43539989/fetch-response-text-returns-pending-promise
        return res.text() // text also returns a promise, ye bastards
    }).then((text) => {
        let parent_domnode = new JSDOM(text.trim()).window.document
        // Don't want it to show if it's anywhere, though it probably
        // won't be anywhere to begin with
        // parent_domnode.style.display = 'none';
        parent_domnode.innerHTML = text.trim();
        let table = (() => {
            // Query selector all does not seem to be defined
            // https://drafts.csswg.org/selectors/
            // NOTE: they don't seem to support descendancy
            let captions = AsArray(parent_domnode.querySelectorAll('table>caption'))
            .filter((caption) => {
                return (
                    caption.innerHTML === caption.textContent && 
                    caption.textContent.trim() === 'States of the United States of America'
                )
            });
            return captions.length === 1 ? captions[0].parentNode : null;
        })();
        if (table == null) { throw new Error("Couldn't find the table"); }
        states = AsArray(table.querySelectorAll('tbody>tr')).map((tr) => {
            // For some reason child was not working, maybe because not first child?
            return tr.querySelector('a').textContent.trim();
        }).filter(
            // Unclear why it's including thead shit
            (str) => !str.includes('[') && !str.includes('abbreviation')
        );
    }).catch((err) => {
        console.log(`Got error: ${err}`);
        states = null;
    });

    // hack
    while (states !== null && states.length === 0) {
        // ty simon for this suggestion
        // why can't this thing be multithreaded ugh
        await new Promise((resolve) => setTimeout(resolve, 10));
    }
    console.log(`The ${states.length} states are\n${states.join('\n')}`)
})();
