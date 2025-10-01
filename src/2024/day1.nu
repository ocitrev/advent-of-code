source ../utils.nu

# https://adventofcode.com/2024/day/1
def main [] {
    print-title 2024 1 'Historian Hysteria'

    let part1 = get-input 2024 1 | part1
    print $'  Part 1: ($part1)'

    let part2 = get-input 2024 1 | part2
    print $'  Part 2: ($part2)'
}

def part1 [] {
    let lists = $in | split-lines | split column -c ' '
    let list1 = $lists | get column1 | into int | sort
    let list2 = $lists | get column2 | into int | sort

    $list1 | zip $list2 | each {
        $in.0 - $in.1 | math abs
    } | math sum
}

def part2 [] {
    let lists = $in | split-lines | split column -c ' '
    let list1 = $lists | get column1 | into int | sort
    let list2 = $lists | get column2 | into int | sort

    $list1 | par-each {|a|
        $a * ($list2 | where $it == $a | length)
    } | math sum
}

def get-example [] {
    r#'3   4
4   3
2   5
1   3
3   9
3   3'#
}
