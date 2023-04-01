'use strict';

function Cs142TemplateProcessor(template) {
    this.template = template;
    this.fillIn = function(dictionary) {
        return this.template.replace(
            // `g` denotes global replacement
            // braces inside do not require escaping
            /\{\{([^{}]*)\}\}/g,
            (_, p1) => (dictionary[p1] === undefined ? '' : dictionary[p1]),
        );
    };
}