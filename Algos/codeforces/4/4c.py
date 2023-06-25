class DB:
    def __init__(self):
        # Map to 0 if none taken
        self.name2maxtaken: dict[str, int] = {}
    def getName(self, name: str) -> str:
        if not name in self.name2maxtaken:
            self.name2maxtaken[name] = 0
            return name
        else:
            maxTaken = self.name2maxtaken[name]
            nextMax = maxTaken + 1
            self.name2maxtaken[name] += 1
            return f'{name}{nextMax}'

def main():
    database = DB()
    num_lines = int(input().strip())
    out = []
    for _ in range(num_lines):
        line = input().strip()
        nameTaken = database.getName(line)
        if nameTaken == line:
            out.append("OK")
        else:
            out.append(nameTaken)
    print('\n'.join(out))
    

if __name__ == "__main__":
    main()