#[path = "../utils.rs"]
mod utils;

use rayon::prelude::*;
type Int = i64;

fn main() {
    // https://adventofcode.com/2024/day/7
    println!("Day 7, 2024: Bridge Repair");
    let _m = utils::Monitor::start();

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(3312271365652, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(509463489296712, p2);
}

#[derive(Clone, Debug)]
enum Op {
    Add,
    Mul,
    Concat,
}

fn is_valid(numbers: &Vec<Int>, expected: Int, operations: &Vec<Op>) -> bool {
    for operations in utils::product_repeat(operations.iter(), numbers.len() - 1) {
        let mut number_it = numbers.iter();
        let mut total = *number_it.next().unwrap();

        for op in operations {
            let n = *number_it.next().unwrap();
            total = match op {
                Op::Add => total + n,
                Op::Mul => total * n,
                Op::Concat => total * Int::pow(10, n.ilog10() + 1) + n,
            };
        }

        if total == expected {
            return true;
        }
    }

    false
}

fn parse_lines(input: &str, operations: &Vec<Op>) -> Int {
    input
        .lines()
        .par_bridge()
        .map(|line| {
            let mut parts = line.split(':');
            let expected: Int = parts.next().unwrap().parse().unwrap();
            let numbers: Vec<Int> = parts
                .next()
                .unwrap()
                .split(' ')
                .filter(|n| !n.is_empty())
                .map(|n| n.parse().unwrap())
                .collect();

            if is_valid(&numbers, expected, &operations) {
                expected
            } else {
                0
            }
        })
        .sum()
}

fn part1(input: &'static str) -> Int {
    parse_lines(input, &vec![Op::Add, Op::Mul])
}

fn part2(input: &'static str) -> Int {
    parse_lines(input, &vec![Op::Add, Op::Mul, Op::Concat])
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day7.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            190: 10 19
            3267: 81 40 27
            83: 17 5
            156: 15 6
            7290: 6 8 6 15
            161011: 16 10 13
            192: 17 8 14
            21037: 9 7 18 13
            292: 11 6 16 20
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(3749, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(11387, super::part2(get_example()));
    }
}
