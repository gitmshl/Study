eps = 0.7

def f(s):
    return 1 if s > 0 else 0

def fder(s):
    return 1

def feed(inp):
    sum = b
    for i in range(len(inp)):
        sum += weights[i] * inp[i]
    return sum

def feedforward(inp):
    return f(feed(inp))

def learn(inp, ans):
    global weights, b
    s = feed(inp)
    y = f(s)
    if y == ans: return
    #weights = [x - 2 * eps * (y - ans) * fder(s) * y for x, y in zip(weights, inp)]
    l = len(weights)
    for i in range(l):
        weights[i] -= 2 * eps * (y - ans) * fder(s) * inp[i]
    print('learn', weights)
    b -= 2 * eps * (y - ans) * fder(s)

def learn_with_teacher(inps, anss):
    for inp, ans in zip(inps, anss):
        learn(inp, ans)

def ex(inp, ans):
    y = feedforward(inp)
    return True if y == ans else False

def examen(inps, anss):
    all = len(inps)
    right = 0
    for inp, ans in zip(inps, anss):
        if ex(inp, ans): 
            print("YES")
            right += 1
        else:
            print("NO")
    print("right: ", right, '/', all)

def print1():
    print("weights", weights)
    print("b", b)

def main():
    inps = [
        [0, 0],
        [0, 1],
        [1, 1]
    ]
    anss = [
        0,
        1,
        1
    ]

    for i in range(3):
        learn_with_teacher(inps, anss)
    inps.append([1, 0])
    anss.append(1)
    examen(inps, anss)
    print1()

N = 2
b = 0
weights = [0.451, -1.752]

main()