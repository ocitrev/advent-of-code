fn main() {
    // https://adventofcode.com/2023/day/18
    println!("Day 18, 2023: Lavaduct Lagoon");

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    // assert_eq!(0, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    // assert_eq!(0, p2);
}

fn part1(_input: &'static str) -> u64 {
    0
}

fn part2(_input: &'static str) -> u64 {
    0
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day18.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        ""
    }

    #[test]
    fn part1() {
        assert_eq!(0, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(0, super::part2(get_example()));
    }
}
