#rows=[1,2 4,5 7,8]
#columns=[1,2, 4,5, 7,8, 10,11, 13,14, 
#        16,17, 19,20, 22,23, 25,26, 28,29, 
#        31,32, 34,35, 37,38, 40,41, 43,44, 
#        46,47, 49,50, 52,53, 55,56, 58,59]

rows=[1, 4, 7]
columns=[1,2, 4,5, 7,8, 10,11, 13,14, 
        16,17, 19,20, 22,23, 25,26, 28,29, 
        31,32, 34,35, 37,38, 40,41, 43,44, 
        46,47, 49,50, 52,53, 55,56, 58,59]

index=1

for x in rows:
    for y in columns:
        prod = 'P'
        prod = prod + str(index)
        index += 1
        prod += ": (" + str(y) + ", " + str(x) + ", 100);"
        print(prod)

for z in range(1, index-1):
    print('P' + str(z) + ": 10,")
