#[path = "../utils.rs"]
mod utils;
type Int = i32;

fn main() {
    // https://adventofcode.com/2024/day/1
    println!("Day 1, 2024: Historian Hysteria");
    let _m = utils::Monitor::start();

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(2769675, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(24643097, p2);
}

fn part1(input: &'static str) -> Int {
    let mut list_a: Vec<Int> = vec![];
    let mut list_b: Vec<Int> = vec![];

    for line in input.lines() {
        let mut parts = line.split_whitespace();
        list_a.push(parts.next().unwrap().parse().unwrap());
        list_b.push(parts.next().unwrap().parse().unwrap());
    }

    list_a.sort();
    list_b.sort();

    list_a
        .iter()
        .zip(list_b.iter())
        .map(|(&a, &b)| (a - b).abs())
        .sum()
}

fn part2(input: &'static str) -> Int {
    let mut list_a: Vec<Int> = vec![];
    let mut list_b: Vec<Int> = vec![];

    for line in input.lines() {
        let mut parts = line.split_whitespace();
        list_a.push(parts.next().unwrap().parse().unwrap());
        list_b.push(parts.next().unwrap().parse().unwrap());
    }

    list_a
        .iter()
        .map(|&a| a * list_b.iter().filter(|&&b| a == b).count() as Int)
        .sum()
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day1.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            3   4
            4   3
            2   5
            1   3
            3   9
            3   3
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(11, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(31, super::part2(get_example()));
    }
}
