from hashlib import md5

# I think these are hex strings...
# def xor_two_str(str1, str2):
#     return hex(int(str1,16) ^ int(str2,16))

# def xor_strings(s1, s2):
#     # Encode the strings as UTF-8 byte arrays
#     b1 = s1.encode('utf-8')
#     b2 = s2.encode('utf-8')

#     # Check if the lengths of both strings are the same
#     # if len(b1) != len(b2):
#     #     raise ValueError("The input strings must have the same length.")
#     if len(b1) < len(b2):
#         b1 = b'\0' * (len(b2) - len(b1)) + b1
#     elif len(b2) < len(b1):
#         b2 = b'\0' * (len(b1) - len(b2)) + b2
#     if len(b1) != len(b2):
#         raise ValueError("The input strings must have the same length.")

#     # Perform the XOR operation on each byte and store the result in a new bytearray
#     xor_result = bytearray()
#     for i in range(len(b1)):
#         xor_result.append(b1[i] ^ b2[i])

#     # Decode the XORed bytes back into a string
#     result_string = xor_result.decode('utf-8', errors='replace')

#     return result_string

if __name__ == '__main__':
    # print('running')
    cands = []
    # http://www.mieliestronk.com/wordlist.html
    # with open('words.txt', 'r') as f:
    #     s = f.read()
    #     cands = list(filter(lambda x: len(x) == 7, map(str.strip, s.split('\n'))))
    with open('wiki-100k.txt', 'r') as f:
        lines = f.readlines()
        cands = list(filter(lambda x: len(x) == 7 and x[0] != '#', map(str.strip, lines)))
    # print(len(cands))
    # print(cands[0])
    string = '943816eb645c591d606a5ba7a14c126e:007f2e8823ea'
    h, s = string.split(':')
    # print(h, s)
    # decced = False
    for cand in cands:
        dec = cand + s # concat?
        # dec = xor_strings(cand, s) # doesn't seem to work
        hx = md5(dec.encode('utf-8')).hexdigest()
        # print(cand, hx, h)
        if hx == h:
            print(cand)
            break
# Seems to be 
# caliber
# don't fucking use words.txt it's retarded
# use a map and then follow their suggestion