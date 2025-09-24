source ../utils.nu

# https://adventofcode.com/2016/day/1
def main [] {
    print-title 2016 1 'No Time for a Taxicab'

    let part1 = get-input 2016 1 | part1
    print $'  Part 1: ($part1)'

    let part2 = get-input 2016 1 | part2
    print $'  Part 2: ($part2)'
}

def rot-left [] {
    {x:$in.y y:($in.x * -1)}
}

def rot-right [] {
    {x:($in.y * -1) y:$in.x}
}

def distance [] {
    [$in.x $in.y] | math abs | math sum
}

def part1 [] {
    mut dir = {x:0 y:-1}
    mut pos = {x:0 y:0}

    for ins in ($in | split row ',' | str trim) {
        $dir = match ($ins | str substring ..<1) {
            L => { $dir | rot-left }
            R => { $dir | rot-right }
        }

        let dist = $ins | str substring 1.. | into int
        $pos.x += $dir.x * $dist
        $pos.y += $dir.y * $dist
    }

    $pos | distance
}

def part2 [] {
    mut visited = {}
    mut dir = {x:0 y:-1}
    mut pos = {x:0 y:0}

    for ins in ($in | split row ',' | str trim) {
        $dir = match ($ins | str substring ..<1) {
            L => { $dir | rot-left }
            R => { $dir | rot-right }
        }

        let dist = $ins | str substring 1.. | into int
        for _ in ..<$dist {
            $pos.x += $dir.x
            $pos.y += $dir.y
            let key = $'($pos.x),($pos.y)'

            if ($visited | get -o $key | default false) {
                return ($pos | distance)
            }

            $visited = $visited | insert $key true
        }
    }
}

def get-example [] {
    'R2, L3'
}
