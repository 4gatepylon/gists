# Tentative guidance: test-space partitions

Status: draft recipes and examples awaiting review. These are not approved agent
instructions. [AGENTS.md](AGENTS.md) records the established constraints and
preferences for this folder.

## Core model

Partition the behavior space using a few explicit axes, usually one to six at
any node. Each cell or leaf contains either a smaller partition or one or a few
representative test prototypes. Different branches may use different axes.

A prototype specifies concrete input, expected behavior, and the distinction it
exercises. These examples are test designs, not an inventory of implemented tests.

## Choosing axes and buckets

Derive axes from the contract and its meaningful boundaries:

```text
+------------------------------+--------------------------------------+--------------------------------------+
| Axis source                  | Examples                             | What it distinguishes                |
+------------------------------+--------------------------------------+--------------------------------------+
| Input structure              | Item count, stream count, nesting    | Different structural cases           |
|                              | depth                                |                                      |
+------------------------------+--------------------------------------+--------------------------------------+
| Decision boundaries          | Below/equal/above threshold;         | Changes in a contract predicate      |
|                              | before/at/after expiry               |                                      |
+------------------------------+--------------------------------------+--------------------------------------+
| Input relationships          | Disjoint, touching, partial overlap, | How multiple inputs constrain one    |
|                              | containment                          | another                              |
+------------------------------+--------------------------------------+--------------------------------------+
| Expected result              | Zero/one/multiple outputs;           | Different required outcomes          |
|                              | success/failure                      |                                      |
+------------------------------+--------------------------------------+--------------------------------------+
| State and transitions        | Initial state, operation, resulting  | Behavior that depends on history     |
|                              | state                                |                                      |
+------------------------------+--------------------------------------+--------------------------------------+
| Representation or            | Ordering, precision, configuration   | Conditions affecting behavior or     |
| environment                  |                                      | invariants                           |
+------------------------------+--------------------------------------+--------------------------------------+
```

Use an axis when crossing one of its boundaries changes expected behavior,
exercises a distinct failure mode, or tests an important invariant. Otherwise,
keep the property as a variation within a leaf.

- Give each axis a precise meaning and unit: readings, streams, or output periods.
- Make buckets disjoint and exhaustive over the declared domain. Overlapping
  descriptions are tags, not partitions.
- Choose buckets around meaningful boundaries: `0`, `1`, `2`, `3`, `>3`, for
  example, when small input sizes have distinct behavior.
- Axes may be dependent. Expected output length is a valid axis even though the
  input determines it. Use independently established expectations to classify it.
- Introduce finer buckets only where they distinguish additional behavior.

## Recipe 1: matrix with local refinement

Choose two broad axes. Fill their feasible intersections with prototypes, then
subdivide only cells that need more detail. Use a list for a single axis.

### Example: continuous-time active periods

A qualifying reading has intensity at least the threshold. Consecutive qualifying
readings form one closed period; an inactive reading separates runs. Sampling
gaps alone do not separate runs.

Axes: **number of input readings x number of output periods**.
`L` = below threshold, `E` = equal, `H` = above. `--` = impossible.

```text
+----------------+-----------+----------------------+------------------------------+-------------------------+
| Input readings | 0 periods | 1 period             | 2 periods                    | >2 periods              |
+----------------+-----------+----------------------+------------------------------+-------------------------+
| 0              | empty     | --                   | --                           | --                      |
+----------------+-----------+----------------------+------------------------------+-------------------------+
| 1              | L         | E, H                 | --                           | --                      |
+----------------+-----------+----------------------+------------------------------+-------------------------+
| 2              | LL        | LH, HL, HH, EE       | --                           | --                      |
+----------------+-----------+----------------------+------------------------------+-------------------------+
| 3              | LLL       | HHH, LHL, HHL, LHH   | HLH: ordinary times,         | --                      |
|                |           |                      | fractional separator         |                         |
+----------------+-----------+----------------------+------------------------------+-------------------------+
| >3             | LLLL      | LHHHL                | HHLLHH                       | HLHLH, mixed run        |
|                |           |                      |                              | lengths                 |
+----------------+-----------+----------------------+------------------------------+-------------------------+
```

Within `3 readings -> 2 periods`, use `HLH` at ordinary timestamps and at
`a, a + 0.5, a + 1`. Within `1 reading -> 1 period`, equality versus strictly
above threshold is the useful refinement. Each cell gets the dimensions that
matter there.

For larger inputs, choose additional prototypes for long runs, several inactive
readings between runs, and mixed run lengths. There is no need to enumerate every
possible intensity sequence.

## Recipe 2: tensor rendered as table slices

When three or more axes apply throughout a region, choose outer axes that make
useful review sections. Render the remaining two axes as a table in each section.
Keep row and column definitions identical across slices.

For four dimensions:

```text
configuration
    initial state
        table: input category x expected outcome
```

For five or six dimensions, add outer grouping levels or subdivide cells.
If most cells become irrelevant, use a conditional tree instead.

### Example: retry behavior

Contract: retry a transient failure only if the request is repeatable and attempts
remain. Never retry a permanent failure. The attempt limit includes the first
call. In the transient scenario, the first call fails and a second call succeeds.

Three axes: repeatability, attempt limit, and response scenario. Prototype limits
are one and two attempts. Each cell specifies the result and call count.

**Repeatable request:**

```text
+---------------+-----------------------+--------------------------+-----------------------+
| Attempt limit | Immediate success     | Transient failure once   | Permanent failure     |
+---------------+-----------------------+--------------------------+-----------------------+
| 1             | success, 1 call       | failure, 1 call          | failure, 1 call       |
+---------------+-----------------------+--------------------------+-----------------------+
| 2             | success, 1 call       | success, 2 calls         | failure, 1 call       |
+---------------+-----------------------+--------------------------+-----------------------+
```

**Non-repeatable request:**

```text
+---------------+-----------------------+--------------------------+-----------------------+
| Attempt limit | Immediate success     | Transient failure once   | Permanent failure     |
+---------------+-----------------------+--------------------------+-----------------------+
| 1             | success, 1 call       | failure, 1 call          | failure, 1 call       |
+---------------+-----------------------+--------------------------+-----------------------+
| 2             | success, 1 call       | failure, 1 call          | failure, 1 call       |
+---------------+-----------------------+--------------------------+-----------------------+
```

The two slices form one three-dimensional partition. Larger attempt limits can
be a separate bucket with prototypes for repeated failures and eventual success.

## Recipe 3: conditional partition tree

Split first on the property that changes which other dimensions matter. Introduce
an axis only inside branches where it applies.

### Example: cache lookup

Contract: a present entry is fresh when `age < TTL`, with `TTL > 0`. A fresh hit
returns its value without calling the loader. Missing or expired entries call the
loader; success stores and returns a value, and failure propagates the error
without storing a replacement.

```text
key present?
+-- no
|   +-- loader succeeds -> store and return value
|   +-- loader fails    -> propagate error, no stored value
+-- yes
    +-- age < TTL       -> return cached value, no loader call
    +-- age = TTL
    |   +-- loader succeeds -> replace and return value
    |   +-- loader fails    -> propagate error, no replacement
    +-- age > TTL
        +-- loader succeeds -> replace and return value
        +-- loader fails    -> propagate error, no replacement
```

Use `age = TTL - delta`, `TTL`, and `TTL + delta` for boundary prototypes, with
`0 < delta < TTL`. Entry age is irrelevant when the key is absent; loader outcome
is irrelevant for a fresh hit. The tree makes those dependencies explicit.

## Recipe 4: prototype families for recurring variations

Choose prototypes for core behavior, then apply reusable variations across
selected leaves. State how each variation affects the expected result.

### Example: motion-period variations

```text
+---------------------------------+-----------------------------------+-----------------------------------+
| Variation                       | Expected relationship             | Useful prototypes                 |
+---------------------------------+-----------------------------------+-----------------------------------+
| Permute camera order            | Common periods unchanged          | Containment, fragmentation, an    |
|                                 |                                   | empty camera                      |
+---------------------------------+-----------------------------------+-----------------------------------+
| Shift all timestamps by d       | Every output endpoint shifts by d | Singletons, positive gaps,        |
|                                 |                                   | fractional endpoints              |
+---------------------------------+-----------------------------------+-----------------------------------+
| Insert a qualifying sample      | Periods unchanged                 | Sparse runs, uninterrupted common |
| inside an active run            |                                   | activity                          |
+---------------------------------+-----------------------------------+-----------------------------------+
```

Use timestamp transformations that preserve ordering and exact representability.
Apply variations everywhere when cheap. Otherwise, select combinations
explicitly and record what is exercised. Covering each variation independently
does not establish coverage of their interactions.

When an interaction has distinct behavior, promote it to a named sub-partition
with its own prototype.

## Selecting prototypes

For each feasible leaf:

1. Choose a simple example that isolates the defining behavior.
2. Add a boundary example if it exercises a different decision.
3. Add an interaction example when combined conditions create a distinct risk.

Expected outputs come from the contract, not from the implementation under test.
Each additional prototype should explain what distinction it adds. A prototype
represents its partition; it does not prove that every member behaves correctly.

## Coverage notation

Keep these states distinct:

- **Populated:** named prototypes exist for the cell.
- **MISSING:** the combination is feasible but has no prototype.
- **Impossible:** a stated contract constraint rules out the combination.
- **Out of scope:** the contract excludes the behavior or leaves it unspecified.

An empty cell alone never establishes impossibility. For the motion example,
two readings cannot produce two active runs: separating them would require an
additional inactive reading.

## Reproducible documentation

Keep axes, bucket definitions, feasibility rules, and prototype labels explicit.
Generate coverage views from those definitions and the fixture metadata:

1. Classify each fixture by its input properties and handwritten expected result.
2. Include every fixture; group identical display labels with counts or names.
3. Mark missing and impossible cells using the declared feasibility rules.
4. Use deterministic axis, bucket, and prototype ordering.
5. Render lists, ASCII tables, tensor slices, or trees as appropriate. Put tables
   and diagrams in fenced `text` blocks so they are readable in source files.
6. Regenerate documentation after fixture changes and check that it remains in sync.

The documentation generator organizes fixtures; it does not calculate their
expected answers. Maintain one source for the partition description and reuse
it wherever the coverage view is displayed.
