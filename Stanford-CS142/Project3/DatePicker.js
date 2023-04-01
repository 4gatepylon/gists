'use strict';

const notEmpty = (obj) => obj !== undefined && obj !== null;

const daysInWeek = 7;

// Month in JavaScript is 0-indexed (January is 0, February is 1, etc), 
// but by using 0 as the day it will give us the last day of the prior
// month. So passing in 1 as the month number will return the last day
// of January, not February. Thus it's 1-indexed.
// From: https://stackoverflow.com/questions/1184334/get-number-days-in-a-specified-month-using-javascript
const getMonth = (date_obj) => date_obj.getMonth() + 1;
const getYear = (date_obj) => date_obj.getFullYear();
const daysInMonth = (monthOrObj, year = null) => (notEmpty(year) ? (
        new Date(year, monthOrObj, 0).getDate() 
    ) : (
        daysInMonth(getMonth(monthOrObj), getYear(monthOrObj))
    ));

// Returns a list of numbers up to the count
const numsList = (count, zero_index = false) => {
    const arr = [];
    for (let i = 0; i < count; i++) { arr.push(zero_index ? i : i + 1); }
    return arr;
};

// Returns the lowest number divisible by divisor that is above `above` inclusive
const lowestDivisibleBy = (divisor, above = 0) => {
    const sw = above % divisor === 0;
    return sw ? above : above + (divisor - (above % divisor));
};
const toDayDivs = (arr, classes = null, generateId = null) => {
    const classesInfo = classes ? classes.join(' ') : '';
    const generateIdInfo = (day) => (notEmpty(generateId) ? generateId(day) : '');
    return arr.map((day) => `<div class="day ${classesInfo}" ${generateIdInfo(day)}">${day}</div>`).join(``);
};

const weekdayNames = [`Sun`, `Mon`, `Tue`, `Wed`, `Thu`, `Fri`, `Sat`];
const weekdayNamesHeader = toDayDivs(weekdayNames);

const monthNames = [
    `January`,
    `February`,
    `March`,
    `April`,
    `May`,
    `June`,
    `July`,
    `August`,
    `September`,
    `October`,
    `November`,
    `December`,
];
const getMonthName = (monthOrObj) => {
    const isNum = typeof monthorObj === 'number';
    return isNum ? monthNames[monthOrObj - 1] : monthNames[getMonth(monthOrObj) - 1];
};

// Deliverable
function DatePicker(id, callback) {
    this.callback = callback;
    this.id = id;

    this.render = function (date_obj) {
        const month = getMonth(date_obj);
        const year = getYear(date_obj);
        
        const numberofDays = daysInMonth(month, year);
        const daysNums = numsList(numberofDays);
        const overflowDaysNums = numsList(
            lowestDivisibleBy(daysInWeek, /* above = */numberofDays) - numberofDays);

        const days = toDayDivs(daysNums, ['callback-clickable']);
        const overflowDays = toDayDivs(overflowDaysNums, ['dimmed']);

        const monthName = getMonthName(date_obj);
        
        // Create a div of row divs with day divs inside of them
        const html = `
            <div class="calendar">
                <div class="fcol">
                    <h2>${monthName}, ${year}</h2>
                    <div class="frow">
                        <div class="arrow fcol center">&lt</div>
                        <div class="calendar-grid-container">
                            ${weekdayNamesHeader}
                            ${days}
                            ${overflowDays}
                        </div>
                        <div class="arrow fcol center">&gt</div>
                    </div>
                </div>
            </div>`;
        document.querySelector(`#${this.id}`).innerHTML = html;

        // Declare that all the days must call the callback for this date picker
        const daysDivs = document.querySelectorAll(`#${this.id} .callback-clickable`);
        for (let i = 0; i < daysDivs.length; i++) {
            daysDivs[i].addEventListener('click', () => {
                const day = parseInt(daysDivs[i].textContent, 10);
                this.callback(this.id, {
                    year: year,
                    month: monthName,
                    day: day
                });
            });
        }

        // Declare that for the arrows of this calendar, this function should be called again
        // (i.e. re-render on demand only) when we change the date
        const arrows = document.querySelectorAll(`#${this.id} .arrow`);
        for (let i = 0; i < arrows.length; i++) {
            arrows[i].addEventListener('click', () => {
                if (arrows[i].textContent === '<' || arrows[i].textContent === '&lt') {
                    this.render(new Date(year, month - 1, 0));
                } else if (arrows[i].textContent === '>' || arrows[i].textContent === '&gt') {
                    this.render(new Date(year, month + 1, 0));
                }
            });
        }
    };
}