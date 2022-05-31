# Trying to understand how to use standard input and output properly for the problems in codeforces
import sys

stdout = sys.stdout

from io import BytesIO

buff = BytesIO()
print(buff.write(b"abc\n"))
buff.write(b"efg\n")

print(buff.getvalue())

r = buff.read()
print(len(r))
r = r.decode("utf-8")
print(len(r))
stdout.write(r)