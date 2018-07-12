#
import datetime

total = 100

def circle_out(m, total):
    l = list(range(1, total + 1))
    ind = -1
    while len(l) >= m:
        ind = (ind + m) % len(l)
        l.pop(ind)
        ind = ind - 1           #after pop operation, length of l is shrinked
 #   print(l)

def circle_out_v2(m, total):
    l = list(range(1, total + 1))
    ind = 0
    n = 0
    num = total
    while num >= m:
        if l[n] != 0 and ind != m - 1:
            n = (n + 1) % total                 #count that element
            ind = ind + 1
        elif l[n] == 0:
            n = (n + 1) % total                 #skip that element, don't count
        else:
            l[n] = 0                            #exile that element
            n = (n + 1) % total
            ind = 0
            num = num - 1
 #       ind = ind - 1           #after pop operation, length of l is shrinked
#    print([i for i in l if i != 0])

print('Method 1:')
begin = datetime.datetime.now()
for i in list(range(2, total - 1)):
    circle_out(i, total)
end = datetime.datetime.now()
print('Time elapsed: ', end - begin)

print('Method 2:')
begin = datetime.datetime.now()
for i in list(range(2, total - 1)):
    circle_out_v2(i, total)
end = datetime.datetime.now()
print('Time elapsed: ', end - begin)