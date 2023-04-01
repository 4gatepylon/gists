// Look at
// https://stackoverflow.com/questions/6013245/are-types-like-uint32-int32-uint64-int64-defined-in-any-stdlib-header
// https://www.tutorialspoint.com/c_standard_library/stdlib_h.htm
#include <stdio.h>
#include <string.h>
#include <assert.h>

// Number  = (Decimal | Integer)((e | E)Integer)?
// Decimal = (+|-)?(Digit+.Digit* | .Digit+)
// Integer = (+|-)?Digit+
// Digit   = [0-9]

// By view we mean index
// Every method returns view + length of parsed component
// if it succeeds, else -1.

// Return the next view if the character at the view is a digit
int parse_digit(char* string, int view) {
    char c = string[view];
    if ('0' <= c && c <= '9') {
        return view + 1;
    }
    return -1;
}

int parse_digits(char* string, int view, int star) {
    int original_view = view;
    int parseable = 1;
    while (parseable) {
        int new_view = parse_digit(string, view);
        if (new_view == -1) {
            parseable = 0;
        } else {
            view = new_view;
        }
    }
    if (!star && view == original_view) {
        return -1;
    }
    assert(view != -1);
    return view;
}

int parse_integer(char* string, int view) {
    if (string[view] == '\0') {
        return -1;
    }
    if (string[view] == '+' || string[view] == '-') {
        view += 1;
    }
    // star = 0 => we need at least one digit
    int parse = parse_digits(string, view, 0);
    return parse;
}

int parse_decimal(char* string, int view) {
    if (string[view] == '\0') {
        return -1;
    }
    if (string[view] == '+' || string[view] == '-') {
        view += 1;
    }
    if (string[view] == '.') {
        view += 1;

        // star = 0 => we need at least one digit
        return parse_digits(string, view, 0);
    } else {
        // star = 0 => we need at least one digit
        view = parse_digits(string, view, 0);
        if (view == -1 || string[view] != '.') {
            return -1;
        }
        view += 1;
        // star = 1 => we can have no digits if we want
        return parse_digits(string, view, 1);
    }
}

// Return the next view if we are able to parse a number
// Parse the maximal size number
int parse_number(char* string, int view) {
    int header_view = parse_decimal(string, view);
    if (header_view == -1) {
        header_view = parse_integer(string, view);
    }
    if (header_view == -1) {
        return -1;
    }
    if (string[header_view] != 'E' && string[header_view] != 'e') {
        assert(header_view != -1);
        return header_view;
    }
    int footer_view = header_view + 1;
    assert(header_view != -1);
    assert(footer_view != -1);
    return parse_integer(string, footer_view);
}

// Check that parsing the number exhausts the string
int isNumber(char* string) {
    int last_view = parse_number(string, 0);
    if (last_view == -1 || string[last_view] != '\0') {
        return 0;
    }
    return 1;
}

int main() {
    #define NUM_TESTS 10

    // Valid tests
    char* valid[NUM_TESTS] = {
        "3",
        "2342",
        "2342e3",
        "3.3",
        ".3342",
        ".79",
        "123.",
        "232.e10",
        "+0.0",
        "-1",
        // 10 valid tests
    };
    char *invalid[NUM_TESTS] = {
        "",
        "e",
        "-2334.",
        "--32",
        "+-232",
        ".",
        "2232E3.2",
        "e+2232",
        "e3",
        "3e",
        // 10 invalid tests
    };
    for (int i = 0; i < NUM_TESTS; i++) {
        printf(
            "Test `%s` (%d) should be valid, and got %s\n",
            valid[i],
            (int)strlen(valid[i]),
            isNumber(valid[i]) ? "valid" : "invalid"
        );
    }
    for (int i = 0; i < NUM_TESTS; i++) {
        printf(
            "Test `%s` (%d) should be invalid, and got %s\n",
            invalid[i],
            (int)strlen(invalid[i]),
            isNumber(invalid[i]) ? "valid" : "invalid"
        );
    }
}