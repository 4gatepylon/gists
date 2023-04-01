package reversals

import (
	"testing"
	"fmt"
)

func Test(t *testing.T) {
	pairs := [][2]string{
		[2]string{
			// foo([4]([5]zoom[9])bar([14]baz[17])[18])blim_[23]EOF
			// foo(moozbarzab)blim
			// foobazrabzoomblim
			"foo((zoom)bar(baz))blim",
			"foobazrabzoomblim",
		},
		[2]string{
			"hello",
			"hello",
		},
		[2]string{
			"(test)",
			"tset",
		},
		[2]string{
			"(yo)(ma)",
			"oyam",
		},
		[2]string{
			"spag(hetti)",
			"spagitteh",
		},
		[2]string{
			"(ab(cd(ef(gh(ij(k))))))",
			//(ab(cd(ef(gh(ijk)))))
			//(ab(cd(ef(ghkji))))
			//(ab(cd(efijkhg)))
			//(ab(cdghkjife))
			//(abefijkhgdc)
			//cdghkjifeba
			"cdghkjifeba",
		},
		[2]string{
			"hello(my(friends(foes)and(or)fellow)americans)world",
			//hello(my(friendsseofandrofellow)americans)world
			//hello(mywollefordnafoessdneirfamericans)world
			//hellosnaciremafriendsseofandrofellowymworld
			"hellosnaciremafriendsseofandrofellowymworld",
		},
		[2]string{
			"((((hello))))",
			"hello",
		},
		[2]string{
			"(((ab)))",
			"ba",
		},
	}
	for _, pair := range pairs {
		input := pair[0]
		expect := pair[1]
		result := reverse_full(input)

		if expect != result {
			t.Fatalf("Wanted `%s` but got `%s`", expect, result)
		} else {
			fmt.Printf("OK: Wanted `%s` and got `%s`\n", expect, result)
		}
	}
}

type tree struct {
	// start index, stop index; i is ALWAYS inclusive, j is INCLUSIVE too
	i, j int
	// the children in order to print, their indices are CORRECT and in order
	c []*tree
}

// recursively form a tree from a string
func form_tree(b []byte, i, j int) *tree {
	fmt.Printf("form tree from %d to %d\t%s\n", i, j, string(b[i : j + 1]))
	t := &tree{
		i: i,
		j: j,
		c: make([]*tree, 0),
	}
	paren_count := 0
	idx := i
	subtree_start_idx := 0
	in_subtree := false
	for idx <= j {
		// Open a subtree if we just passed a `(`
		if paren_count == 1 && !in_subtree {
			in_subtree = true
			subtree_start_idx = idx
		}

		// just keep searching for more subtrees
		if b[idx] == '(' {
			paren_count ++
		} else if b[idx] == ')' {
			paren_count --
		}

		if in_subtree && paren_count == 0 {
			// if we just hit a `)` we and that closes the subtree we should form that tree
			t.c = append(t.c, form_tree(b, subtree_start_idx, idx - 1))
			in_subtree = false
		}
		idx ++
	}
	return t
}

// generate an output string by adding bytes to out and reading from in
func (t *tree) add_to(in, out []byte, reverse bool) []byte {
	next_child_tree_idx := 0
	if reverse {
		next_child_tree_idx = len(t.c) - 1
	}
	disp := 0
	for disp <= t.j - t.i {
		// index of the char we want from in
		idx := t.i + disp
		if reverse {
			idx = t.j - disp
		}

		// in the first case(s) we simply need to keep appending to the out bytes
		if next_child_tree_idx < 0 || next_child_tree_idx > len(t.c) - 1 || tree_start_idx(t.c[next_child_tree_idx], reverse) != idx {
			out = append_to(in, out, idx)
			disp ++
		} else {
			next_tree := t.c[next_child_tree_idx]
			out = next_tree.add_to(in, out, !reverse)
			disp += (next_tree.j - next_tree.i + 1)
			if reverse {
				next_child_tree_idx --
			} else {
				next_child_tree_idx ++
			}
		}
	}
	return out
}

func tree_start_idx(t *tree, reverse bool) int {
	if reverse {
		return t.j
	}
	return t.i
}

func append_to(in, out []byte, in_idx int) []byte {
	v := in[in_idx]
	if !(v == '(' || v == ')') {
		fmt.Printf("append %s\n", string(v))
		out = append(out, v)
	}
	return out
}

func reverse_full(input string) string {
	b := []byte(input)
	t := form_tree(b, 0, len(b) - 1)
	r := make([]byte, 0, len(b))
	// this equals is a dummy, it really just changes the pointer type to capture the rest of the string
	r = t.add_to(b, r, false)
	return string(r)
}