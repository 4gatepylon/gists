
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
        view = parse_digit(string, view);
        if (view == -1) {
            parseable = 0;
        }
    }
    if (!star && view == original_view) {
        return -1;
    }
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
    return parse_digits(string, view, 0);
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
        if (string[view] != '.') {
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
        return header_view;
    }
    int footer_view = header_view + 1;
    return parse_integer(string, footer_view);
}

// Check that parsing the number exhausts the string
int isNumber(char* string) {
    return parse_number(string, 0) == '\0';
}

int main() {
    #define NUM_TESTS 10

    // Valid tests
    char** valid = {
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
    char **invalid = {
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
        printf("Test `%s` should be valid, and got %s\n", valid, isNumber(valid[i]) ? "valid" : "invalid");
    }
    for (int i = 0; i < NUM_TESTS; i++) {
        printf("Test `%s` should be invalid, and got %s\n", valid, isNumber(invalid[i]) ? "valid" : "invalid");
    }
}