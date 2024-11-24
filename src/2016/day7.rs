fn main() {
    // https://adventofcode.com/2016/day/7
    println!("Day 7, 2016: Internet Protocol Version 7");

    debug_assert_eq!(2, part1(get_example_1()));
    debug_assert_eq!(3, part2(get_example_2()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(115, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(231, p2);
}

fn is_abba(data: &[u8]) -> bool {
    let a = data[0];
    let b = data[1];
    let c = data[2];
    let d = data[3];
    a == d && b == c && a != b
}

fn supports_tls(input: &str) -> bool {
    let mut inside = false;
    let mut valid = false;

    for data in input.as_bytes().windows(4) {
        if inside {
            match data {
                [_, _, b'[', _] | [_, b'[', _, _] | [b'[', _, _, _] => {
                    continue;
                }
                [_, _, _, b']'] => {
                    inside = false;
                    continue;
                }
                _ => {
                    if is_abba(data) {
                        return false;
                    }
                }
            }
        } else {
            match data {
                [_, _, b']', _] | [_, b']', _, _] | [b']', _, _, _] => {
                    continue;
                }
                [_, _, _, b'['] => {
                    inside = true;
                    continue;
                }
                _ => {
                    valid |= is_abba(data);
                }
            }
        }
    }

    return valid;
}

fn part1(input: &'static str) -> usize {
    input.lines().filter(|line| supports_tls(line)).count()
}

fn is_aba(data: &[u8]) -> bool {
    let a = data[0];
    let b = data[1];
    let c = data[2];
    a == c && a != b
}

fn supports_ssl(input: &str) -> bool {
    let mut is_inside = false;
    let mut inside: Vec<&[u8]> = vec![];
    let mut outside: Vec<&[u8]> = vec![];

    for data in input.as_bytes().windows(3) {
        if is_inside {
            match data {
                [_, b'[', _] | [b'[', _, _] => {
                    continue;
                }
                [_, _, b']'] => {
                    is_inside = false;
                    continue;
                }
                _ => {
                    if is_aba(data) {
                        inside.push(data);
                    }
                }
            }
        } else {
            match data {
                [_, b']', _] | [b']', _, _] => {
                    continue;
                }
                [_, _, b'['] => {
                    is_inside = true;
                    continue;
                }
                _ => {
                    if is_aba(data) {
                        outside.push(data);
                    }
                }
            }
        }
    }

    for aba in outside {
        for &bab in &inside {
            if aba[0] == bab[1] && bab[0] == aba[1] {
                return true;
            }
        }
    }

    false
}

fn part2(input: &'static str) -> usize {
    input.lines().filter(|line| supports_ssl(line)).count()
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day7.txt").trim_end()
}

fn get_example_1() -> &'static str {
    "abba[mnop]qrst
abcd[bddb]xyyx
aaaa[qwer]tyui
ioxxoj[asdfgh]zxcvbn"
}

fn get_example_2() -> &'static str {
    "aba[bab]xyz
xyx[xyx]xyx
aaa[kek]eke
zazbz[bzb]cdb"
}
