source ../utils.nu

# https://adventofcode.com/2024/day/9
def main [] {
    print-title 2024 9 'Disk Fragmenter'

    let part1 = get-input 2024 9 | part1
    print $'  Part 1: ($part1)'
    print $'  Part 2:'
}

def get-example [] {
    '2333133121414131402'
}

def make-list [
    value
    count:int
] {
    generate {|n|
        if $n > 0 {
            {
                out:$value
                next:($n - 1)
            }
        }
    } $count
}

def expand [] {
    $in
    | split chars
    | enumerate
    | par-each -k {|i|
        let value = if ($i.index | bits and 1) == 0 {
            $i.index // 2
        } else {
            null
        }

        make-list $value ($i.item | into int)
    }
    | flatten
}

def drop-empty [] {
    reverse | skip while { is-empty } | reverse
}

def defrag [] {
    print 'debug: defraging ...'

    let input = $in
    let len = $input | length
    $in
    | enumerate
    | generate { |i,right|
        if $right >= $i.index {
            if $i.item == null {
                mut right = $right
                mut out = $input | get $right

                while $out == null {
                    $right -= 1
                    $out = $input | get $right
                }

                # print $'debug: got null at ($i.index), out: >($out)<'
                {
                    out: $out
                    next: ($right - 1)
                }
            } else {
                # print $'debug: got value at ($i.index), out: >($i.item)<'
                {
                    out: $i.item
                    next: $right
                }
            }
        }
    } ($len - 1)
}

def checksum [] {
    enumerate
    | where $it.item != null
    | reduce --fold 0 {|it, acc|
        $acc + $it.index * $it.item
    }
}

def get-printable-disk [] {
    each {|v|
        if $v == null {
            '.'
        } else {
            $v
        }
    } | str join
}

def part1 [] {
    expand | defrag | checksum
}


# print (get-example | part1)
# print (get-example | expand | get-printable-disk)
# print (get-example | expand | defrag | get-printable-disk)
