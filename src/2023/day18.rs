fn main() {
    // https://adventofcode.com/2023/day/18
    println!("Day 18, 2023: Lavaduct Lagoon");

    // debug_assert_eq!(0, part1(get_example()));
    // debug_assert_eq!(0, part2(get_example()));

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

fn _get_example() -> &'static str {
    ""
}
