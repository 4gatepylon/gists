# ### Problem

# Implement a spreadsheet. The spreadsheet can be represented by a data type of your choosing, but it should be easy to get/update individual cells as well as display the entire spreadsheet.
# Each cell in the spreadsheet can have one of the following:
#  - a string such as "Name" in A1 or "John Doe" in A2
#  - a number such as 263 in B1 or 75 in B2
#  - a formula linking other items in the spreadsheet. Ex: "=C1*C2" in D2 or "=C1+C2" in D3
#   - cells that reference other cells with formulas are said to have dependencies. If the value in C1 or C2 changes, D2 will also need to be updated since it depends on those inputs.

# In the language of your choosing, implement the following:
# - define a method 'setCell', to add either a value or formula to a given cell in your spreadsheet
# - define a method 'getCell', to get the value inside of a spreadsheet cell
# - depending on your code, you might also implement 'parseFormula' to handle all relevant formula parsing

# Assumptions:
# - assume the spreadsheet is initialized with 26 columns, 100 rows, with no values in any cell
# - assume valid input (no error checking needed on input, particularly formulas)
# - assume that if a string begins with a '=', it's a valid formula
# - assume we just want to handle basic arithmetic operations for now (+-*/) between two cells like '=B1*B2' or '=A1+A2’
#   (never more than one operation)

from typing import Union, Tuple

class Spreadsheet:
    #
    # {
    #  [cellName]: {
    #     value: text value
    #  }
    # }
    #
    def __init__(self):
        # Map cells that are actually populated to their values
        self.cells = {}

    def isFormula(self, value: str) -> bool:
        """
        Returns True if the given value is a formula, i.e. starts with '='
        """
        return type(value) != int and value.startswith('=')

    def isInt(self, value: str) -> bool:
        if type(value) == int:
            return True
        try:
            int(value)
            return True
        except ValueError:
            return False
    
    def formulaCellsAndOperands(self, formula: str) -> Tuple[str, str, str]:
        is_plus = '+' in formula
        is_minus = '-' in formula
        is_times = '*' in formula
        delim = '+' if is_plus else '-' if is_minus else '*' if is_times else '/'
        cells = formula[1:].split(delim)
        return cells[0], cells[1], delim
        
    
    def eval(self, cellName: str) -> Union[str, int]:
        value = self.cells[cellName]
        if self.isFormula(value):
            cell1, cell2, delim = self.formulaCellsAndOperands(value)
            c1 = self.eval(cell1)
            c2 = self.eval(cell2)
            return c1 + c2 if delim == '+' else c1 - c2 if delim == '-' else c1 * c2 if delim == '*' else c1 / c2
        elif self.isInt(value):
            return int(value)
        return value
    
    def setCell(self, cellName: str, value: str) -> None:
        self.cells[cellName] = value
        return None
    
    def getCell(self, cellName: str) -> Union[str, int]:
        assert cellName in self.cells
        return self.eval(cellName)

if __name__ == '__main__':
    from pprint import PrettyPrinter
    pprint = PrettyPrinter(indent=4).pprint
    
    s = Spreadsheet()
    tests = [
        s.setCell('A1', 'Name'),
        s.setCell('A2', 'John Doe'),
        s.setCell('B1', 263),
        s.setCell('B2', 75),
        s.setCell('C1', '=B1*B2'),
        s.setCell('C2', '=B1+B2'),
        s.setCell('D2', '=C1*C2'),
        s.getCell('D2'), # (263*75)*(263+75),
        s.getCell('A1'),
        s.getCell('A2'),
        s.getCell('B1'),
        s.getCell('B2'),
        s.getCell('C1'), # ~ 13150 <= ... <= 26300
        # Upate a cell causing a propagation
        s.setCell('B1', 1),
        s.getCell('C1'), # 75
    ]
    pprint(tests)

    