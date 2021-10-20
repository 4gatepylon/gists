package main

import "fmt"
import "math"
import "math/rand"

// Keys anyone makes should be able to turn into integers to hash
// or to do comparisons in tree data structures (i.e. red/black)

type Key interface {
	toInt() int
}

type Int struct {
	val int
}

func (caller Int) toInt() int {
	return caller.val
}

// We will use generic slices (arraylists) to store the data within hashmaps
// and use linkedlist chaining (maybe as a bonus also implement arraylists later...)

type Any interface {}

// NOTE: Any is probably a pointer
type LinkedListNode struct {
	key Key
	value Any
	next *LinkedListNode
}

// A map can store any sort of objects using any sorts of keys as long as
// those types are convertible into integers somehow

// proper usage is this: first initialize an instance, then init, then you can call funcs
type Map interface {
	size() int

	// the elements below return nil when there is no v for that corresponding key
	// and return true if it worked (i.e. no error: ok) or false otherwise

	put(k Key, v Any) (Any, bool) // set or add an element v to the map at key k
	get(k Key) (Any, bool) // get the value corresponding to key k and return its pointer
	del(k Key) (Any, bool) // remove the value corresponding to key k and return its pointer
}

// Implementing a hastable map with collisions using chaining

type Hashtable struct {
	length int
	arraylist [](*LinkedListNode)
	a int
	b int
	p int
}

func (table *Hashtable) size() int {
	return table.length
}

// helper to basically find the parent of a node with a given key
// or if that's not there the last node of that hash chain
// and the parent is nil it means it was the first node in the chain
// (and the second one is the node itself if it's there, else nil)
// (also returns the index in the table)
func (table *Hashtable) find(k Key) (int, *LinkedListNode, *LinkedListNode) {
	idx := ((table.a * k.toInt() + table.b) % table.p) % len(table.arraylist)

	if table.arraylist[idx] == nil {
		return idx, nil, nil
	} else {
		var parent *LinkedListNode = nil
		var found *LinkedListNode = table.arraylist[idx]

		for found != nil && found.key != k {
			parent = found
			found = found.next
		}
		
		return idx, parent, found
	}
}

// unfortunately we do not make use of our slice's dynamic resizing capabilities yet
func (table *Hashtable) put(k Key, v Any) (Any, bool) {
	if table.arraylist == nil {
		return nil, false
	} else {
		var prev Any = nil
		idx, parent, gotten := table.find(k)
		
		if gotten == nil {
			next := &LinkedListNode{
				key: k,
				value: v,
				next: nil,
			}

			if parent == nil {
				table.arraylist[idx] = next
			} else {
				parent.next = next
			}

			table.length ++
		} else {
			prev = gotten.value
			gotten.value = v
		}

		return prev, prev != nil
	}
}

func (table *Hashtable) get(k Key) (Any, bool) {
	if table.arraylist == nil {
		return nil, false
	} else {
		_, _, gotten := table.find(k)

		if gotten == nil {
			return nil, false
		} else {
			return gotten.value, true
		}
	}
}

// we do not support downsizing
func (table *Hashtable) del(k Key) (Any, bool) {
	if table.arraylist == nil {
		return nil, false
	} else {
		idx, parent, gotten := table.find(k)
		if gotten == nil {
			return nil, false
		} else {
			if parent == nil {
				table.arraylist[idx] = gotten.next
			} else {
				parent.next = gotten.next
			}
			table.length --

			return gotten.value, true
		}
	}
}

// we also don't support resizing yet
func (table *Hashtable) init(capacity int) bool {
	// it's actually 63 - 1 (62 & 61 & ...)
	table.a = rand.Intn(1 << 30)
	table.b = rand.Intn(1 << 30)
	table.length = 0
	table.arraylist = make([](*LinkedListNode), capacity, capacity)

	// hacky way to generate random primes
	p := 0
	for !prime(p) {
		p = capacity + rand.Intn(1 << 30 - capacity)
	}

	table.p = p

	return true
}

// helper to (not so quickly) generate random primes
func prime(p int) bool {
	if p == 0 || p == 1 || p % 2 == 0 {
		return false
	} else {
		for i := 3; i < int(math.Sqrt(float64(p))) + 1; i += 2 {
			if p % i == 0 {
				return false
			}
		}

		return true
	}
}

// in the future it would be cool to build a few different things to continue learning go
// 0. (optional) support resizing, etc...
// 1. red-black tree using our map interface from above
// 2. basic server to host a poker game (frontend can be JS or whatever)
//    (Ideally using TCP, but maybe HTTP or socket.io HTTP sockets in the browser)
// 3. basic compiler in Go from something like Python to C

func main() {
	// fmt.Println(1 << 62)
	fmt.Println("Testing this horrendous thing.")
	
	// you need to use a reference because otherwise I think it modifies a copy!
	// how can I make it a Map and then also have access to methods below?
	// maybe that's bad practice though tbh...
	t := &Hashtable{}
	t.init(10)

	fmt.Println(t) // should be some random prime

	var threeKey Int = Int{val: 3}
	var threeValue Any = nil
	t.put(threeKey, threeValue)

	var fourKey = Int{val: 4}
	var fourValue Any = &[4]int{1,2,3,4}
	t.put(fourKey, fourValue)

	fmt.Println(t)

	fmt.Println(t.get(threeKey)) // should be nil, true
	fmt.Println(t.get(fourKey)) // should be the array, true
	fmt.Println(t.get(Int{val: 5})) // should be nil, false

	fmt.Println(t.size()) // should be 2

	// WORKS! YES!
}
