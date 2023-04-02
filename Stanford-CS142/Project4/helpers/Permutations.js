import { ArraysEqual } from "./Misc"

const Transpose = (table) => {
    if (table.length == 0 ) return []
    // Assume table is tabular obviously
    let t = []
    for (let j = 0; j < table[0].length; j++) {
        t.push([])
        for (let i = 0; i < table.length; i++) {
            t[j].push(table[i][j])
        }
    }
    return t;
}
const PermutationRowOk = (row) => ArraysEqual(Array.from(new Set(row)), row)

const GenerateRowPermutation = (n, r) => {
    let perms = [] // Only the empty permutation for empty
    if (n > 0) {
        for (let i = 1; i <= r; i++) {
            GenerateRowPermutation(n - 1, r).forEach((perm) => {
                perm.push(i)
                perms.push(perm)
            })
        }
    } else { perms.push([]) }
    return perms;
}
const GenerateRowsPermutations = (n, r) => {
    let perms = []
    if (n > 0) {
        // Filter out invalid by row options
        GenerateRowPermutation(r, r).filter(PermutationRowOk).forEach((appendPermutation) => {
            const unfinishedPermutations = GenerateRowsPermutations(n - 1, r)
            unfinishedPermutations
            .filter(
                (unfinishedPermutation) => Transpose(unfinishedPermutation)
                .reduce(
                    // Make sure to filter out invalid by col options
                    (acc, col, col_idx) => acc && !col.includes(appendPermutation[col_idx]), true
                )
            )
            .forEach((unfinishedPermutation) => {
                perms.push([...unfinishedPermutation, [... appendPermutation]])
            })
        })
    } else { perms.push([]) }
    return perms
}
// Test: JSON.stringify(GenerateRowsPermutations(2, 2), null, ' ')
const GeneratePermutations = (n) =>  n === 0 ? [] : GenerateRowsPermutations(n, n);
const DedupPermutations = (permutations) => {
    return permutations // XXX TODO
}
// Permutation already OK by construction, if we were clever maybe we could dedup by construction
export const GenerateOkPermutations = (n) => DedupPermutations(GeneratePermutations(n))