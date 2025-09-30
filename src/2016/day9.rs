fn main() {
    // https://adventofcode.com/2016/day/9
    println!("Day 9, 2016: Explosives in Cyberspace");

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(107035, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(11451628995, p2);
}

fn decompress(text: &str, recursive: bool) -> usize {
    let mut result: usize = 0;
    let mut skip: usize = 0;

    for (i, c) in text.char_indices() {
        if c.is_whitespace() {
            continue;
        }

        if skip != 0 {
            skip -= 1;
            continue;
        }

        if c == '(' {
            let end = text[i..].find(')').unwrap() + i;
            let code = &text[i + 1..end];
            let mut it = code.splitn(2, 'x');
            let length: usize = it.next().unwrap().parse().unwrap();
            let repeat: usize = it.next().unwrap().parse().unwrap();

            if recursive {
                let after = end + 1;
                result += decompress(&text[after..after + length], true) * repeat;
            } else {
                result += length * repeat;
            }

            skip = end + length - i
        } else {
            result += 1;
        }
    }

    result
}

fn part1(input: &'static str) -> u64 {
    decompress(input, false) as u64
}

fn part2(input: &'static str) -> u64 {
    decompress(input, true) as u64
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day9.txt").trim_end()
}

#[cfg(test)]
mod tests {
    #[test]
    fn part1() {
        assert_eq!(6, super::part1("ADVENT"));
        assert_eq!(7, super::part1("A(1x5)BC"));
        assert_eq!(9, super::part1("(3x3)XYZ"));
        assert_eq!(11, super::part1("A(2x2)BCD(2x2)EFG"));
        assert_eq!(6, super::part1("(6x1)(1x3)A"));
        assert_eq!(18, super::part1("X(8x2)(3x3)ABCY"));
    }

    #[test]
    fn part2() {
        assert_eq!(9, super::part2("(3x3)XYZ"));
        assert_eq!(20, super::part2("X(8x2)(3x3)ABCY"));
        assert_eq!(241920, super::part2("(27x12)(20x12)(13x14)(7x10)(1x12)A"));
        assert_eq!(
            445,
            super::part2("(25x3)(3x3)ABC(2x3)XY(5x2)PQRSTX(18x9)(3x2)TWO(5x7)SEVEN")
        );
    }
}
