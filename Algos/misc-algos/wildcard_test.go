package wildcard

import (
	"fmt"
	"testing"
)

type TestCase struct {
	str string
	pat string
	out bool
}

func TestWildcard(t *testing.T) {
	io := []TestCase{
		TestCase{
			"aa",
			"?a",
			true,
		},
		TestCase{
			"aa",
			"a?",
			true,
		},
		TestCase{
			"aa",
			"??",
			true,
		},
		TestCase{
			"aa",
			"?",
			false,
		},
		TestCase{
			"aa",
			"***?*?*",
			true,
		},
		TestCase{
			"aa",
			"a",
			false,
		},
		TestCase{
			"abc",
			"*?b",
			false,
		},
		TestCase{
			"abc",
			"*******c*",
			true,
		},
		TestCase{
			"abc",
			"a*c",
			true,
		},
		TestCase{
			"acd",
			"**a?",
			false,
		},
		TestCase{
			"abbabaaabbabbaababbabbbbbabbbabbbabaaaaababababbbabababaabbababaabbbbbbaaaabababbbaabbbbaabbbbababababbaabbaababaabbbababababbbbaaabbbbbabaaaabbababbbbaababaabbababbbbbababbbabaaaaaaaabbbbbaabaaababaaaabb",
			"**aa*****ba*a*bb**aa*ab****a*aaaaaa***a*aaaa**bbabb*b*b**aaaaaaaaa*a********ba*bbb***a*ba*bb*bb**a*b*bb",
			false,
		},
		TestCase{
			"abcdd",
			"abc*d",
			true,
		},
	}
	for _, tc := range io {
		if m := isMatch(tc.str, tc.pat); m != tc.out {
			t.Fatal(fmt.Sprintf("From (str: %s, pat: %s), expected %v but got %v\n", tc.str, tc.pat, tc.out, m))
		}
	}
}

type LetterCount struct {
    letter byte
    count  int
}

// 0 signifies not yet visited, 1 means true, and != 1 means false
func make_memo(cs, cp []*LetterCount) [][][]int {
	m := make([][][]int, len(cs) * len(cp), len(cs) * len(cp))
	// memo_str_off:= len(memo) / len(cs)
	// memo_pat_off := memo_str_off / len(pat)
	// memo_idx := memo_str_off * str_idx + memo_pat_off * pat_idx
	for i := 0; i < len(cs); i++ {
		for j := 0; j < len(cp); j++ {
			// We could use the same flattening trick to malloc less...
			inner := make([][]int, cs[i].count)
			m[i * len(cp) + j] = inner
			for k := 0; k < cs[i].count; k++ {
				inner[k] = make([]int, cp[j].count)
			}
		}
	}
	return m
}

// Given a string, compress it into a list of letter, count
// '*'' are always compressed fully with a rewritten count of 1
func compress(str []byte) []*LetterCount {
    lc := make([]*LetterCount, 0)
    if len(str) == 0 {
        return lc
    }
    last_char := str[0]
    count := 0
    for i := 0; i < len(str); i++ {
        if str[i] == last_char {
            count ++
        } else {
            if last_char == '*' {
                count = 1
            }
            lc = append(lc, &LetterCount{
                letter: last_char,
                count: count,
            })
            count = 1
            last_char = str[i]
        }
    }
    if count > 0 {
        lc = append(lc, &LetterCount{
            letter: last_char,
            count: count,
        })
    }
    return lc
}

func isMatch(s string, p string) bool {
    cs := compress([]byte(s))
    cp := compress([]byte(p))
    m := make_memo(cs, cp)
    return match(cs, cp, /*indices*/ 0, 0, /*offsets inside those indices*/ 0, 0, m)
}

func match(str, pat []*LetterCount, str_idx, pat_idx int, str_off, pat_off int, memo [][][]int) bool {
	// A match has only succeeded once we've successfully consumed the entire string and pattern (through
	// there may be some *s left). We don't store that in the DP since it would make it longer.
	if (len(str) <= str_idx && pat_idx < len(pat)) || (len(pat) <= pat_idx && str_idx < len(str)) {
        if len(pat) - pat_idx == 1 && pat[pat_idx].letter == '*' {
            return true
        }
        return false
    }
    if str_idx == len(str) && pat_idx == len(pat) {
        return true
    }
	// Get the memo index
	memo_str_off:= len(memo) / len(str)
	memo_pat_off := memo_str_off / len(pat)
	memo_idx := memo_str_off * str_idx + memo_pat_off * pat_idx
    if read := memo[memo_idx][str_off][pat_off]; read > 0 {
        return read == 1
    }
	if res := match_result(str, pat, str_idx, pat_idx, str_off, pat_off, memo); res {
		memo[memo_idx][str_off][pat_off] = 1
		return true
	}
	memo[memo_idx][str_off][pat_off] = 2
	return false
}

func match_result(str, pat []*LetterCount, str_idx, pat_idx int, str_off, pat_off int, memo [][][]int) bool {
	char := str[str_idx]
    pchar := pat[pat_idx]
    if pchar.letter == '*' {
        if match(str, pat, str_idx, pat_idx + 1, str_off, 0, memo) {
            // Check if we could turn this '*' into the empty sequence
            return true
        }
        if match(str, pat, str_idx + 1, pat_idx, 0, 0, memo) {
            // Check if we could use this '*' on this char and (maybe) more later
            return true
        }
        for consume := char.count - str_off - 1; consume > 0; consume-- {
            // Check if we could eat up only l characters in this char and then something else later
            if match(str, pat, str_idx, pat_idx + 1, str_off + consume, 0, memo) {
                return true
            }
        }
        return false
    } else if pchar.letter == '?' {
        overflow := (pchar.count - pat_off) - (char.count - str_off)
        if overflow > 0 {
            // If we have more '?' than characters, then eat up the characters
            return match(str, pat, str_idx + 1, pat_idx, 0, pchar.count - overflow, memo)
        } else if overflow < 0 {
            // If we have more characters that '?' then eat up the '?'
            // NOTE overflow is negative
            return match(str, pat, str_idx, pat_idx + 1, char.count + overflow, 0, memo)
        } else {
            // Else eat both up
            return match(str, pat, str_idx + 1, pat_idx + 1, 0, 0, memo)
        }
    } else if pchar.letter != char.letter {
        // If the letters don't match this is a dead end
        return false
    } else if pchar.count - pat_off > char.count - str_off {
        // If we MUST consume more than there is, then we can't complete
        return false
    } else if pchar.count - pat_off < char.count - str_off {
		// There might be a '?' later on
        return match(str, pat, str_idx, pat_idx + 1, str_off + (pchar.count - pat_off), 0, memo)
    } else {
        // If they have the same counts then we consume both and continue
        return match(str, pat, str_idx + 1, pat_idx + 1, 0, 0, memo)
    }
}