import os.path

def get_input(year, day):
    script_dir = os.path.dirname(os.path.abspath(__file__))
    p = os.path.join(script_dir, '..', '..', 'inputs', str(year), f'day{day}.txt')
    with open(p) as f:
        return f.read().rstrip()


def get_example():
    return '2333133121414131402'


def expand(compressed):
    next_id = 0
    result = []

    for i,c in enumerate(compressed):
        count = int(c)
        if i & 1 == 0:
            for _ in range(count):
                result.append(next_id)
            next_id += 1
        else:
            for _ in range(count):
                result.append(None)

    return result


def print_disk(data):
    def g():
        for c in data:
            if c is None:
                yield '.'
            else:
                yield c
    print(*g(), sep='')


def defrag(data):
    length = len(data)
    left = 0
    right = length

    while right > left:
        right -= 1

        if data[right] is None:
            continue

        while left < length and data[left] is not None:
            left += 1

        if right > left:
            data[left],data[right] = data[right],data[left]
            left += 1

    return data


def checksum(data):
    result = 0
    for i,c in enumerate(data):
        if c is None: continue
        result += i * c
    return result


data = get_example()
data = get_input(2024, 9)

data = expand(data)
# print_disk(data)

data = defrag(data)
# print_disk(data)

print(f'checksum = {checksum(data)}')

