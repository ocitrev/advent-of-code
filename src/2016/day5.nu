source ../utils.nu

# https://adventofcode.com/2016/day/5
def main [] {
    print-title 2016 5 'How About a Nice Game of Chess?'

    let parts = get-input 2016 5 | generate
    print $'  Part 1: ($parts.'1')'
    print $'  Part 2: ($parts.'2')'
}

def generate [] {
    let prefix = $in
    mut num = 0
    mut result1 = ''
    mut result2 = '        ' | split chars
    mut count1 = 0
    mut count2 = 0

    const CHUNK_SIZE = 100_000
    let total = $CHUNK_SIZE * (sys cpu | length)

    loop {
        let hashes = $num..<($num + $total)
        | each {}
        | chunks $CHUNK_SIZE
        | par-each -k {
            each { |n|
                let hash = $prefix + ($n | into string) | hash md5 --binary
                if ($hash.0 == 0 and $hash.1 == 0 and ($hash.2 | bits and 0xf0) == 0) {
                    $hash | encode hex --lower
                } else {
                    null
                }
            }
            | compact
        } | flatten

        for h in $hashes {
            let chars = $h | str substring 5..<7 | split chars
            let sixth = $chars.0
            let seventh = $chars.1

            if $count1 < 8 {
                $result1 += $sixth
                $count1 += 1
            }

            if $count2 < 8 and $sixth in '01234567' {
                let pos = $sixth | into int

                if ($result2 | get $pos) == ' ' {
                    $result2 = $result2 | update $pos $seventh
                    $count2 += 1
                }
            }

            if $count1 == 8 and $count2 == 8 {
                return {
                    1: $result1
                    2: ($result2 | str join)
                }
            }
        }

        $num += $total
    }
}

def get-example [] {
    'abc'
}
