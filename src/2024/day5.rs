#[path = "../utils.rs"]
mod utils;

fn main() {
    // https://adventofcode.com/2024/day/5
    println!("Day 5, 2024: Print Queue");
    let _m = utils::Monitor::start();

    debug_assert_eq!(143, part1(get_example()));
    debug_assert_eq!(123, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(5091, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(4681, p2);
}

struct Queue {
    rules: Vec<(i32, i32)>,
    items: Vec<Vec<i32>>,
}

impl Queue {
    pub fn new(input: &str) -> Self {
        let mut rules = Vec::new();
        let mut items = Vec::new();
        let mut parsing_rules = true;

        for line in input.lines() {
            if line.is_empty() {
                parsing_rules = false;
                continue;
            }

            if parsing_rules {
                let mut parts = line.split('|');
                let a = parts.next().unwrap().parse().unwrap();
                let b = parts.next().unwrap().parse().unwrap();
                rules.push((a, b));
            } else {
                items.push(line.split(',').map(|x| x.parse().unwrap()).collect());
            }
        }

        Self { rules, items }
    }

    pub fn sum_middle_pages(&self) -> (i32, i32) {
        let mut sum_correct: i32 = 0;
        let mut sum_incorrect: i32 = 0;

        for q in &self.items {
            let mut sorted = q.clone();
            sorted.sort_by(|&a, &b| {
                for r in &self.rules {
                    if a == r.0 && b == r.1 {
                        return std::cmp::Ordering::Less;
                    }
                    if b == r.0 && a == r.1 {
                        return std::cmp::Ordering::Greater;
                    }
                }

                return std::cmp::Ordering::Equal;
            });

            if sorted == *q {
                sum_correct += q.iter().nth(q.len() / 2).unwrap();
            } else {
                sum_incorrect += sorted.iter().nth(q.len() / 2).unwrap();
            }
        }

        (sum_correct, sum_incorrect)
    }
}

fn part1(input: &'static str) -> u64 {
    let queue = Queue::new(input);
    queue.sum_middle_pages().0 as u64
}

fn part2(input: &'static str) -> u64 {
    let queue = Queue::new(input);
    queue.sum_middle_pages().1 as u64
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day5.txt").trim_end()
}

fn get_example() -> &'static str {
    "47|53
97|13
97|61
97|47
75|29
61|13
75|53
29|13
97|29
53|29
61|53
97|53
61|29
47|13
75|47
97|75
47|61
75|61
47|29
75|13
53|13

75,47,61,53,29
97,61,53,29,13
75,29,13
75,97,47,61,53
61,13,29
97,13,75,29,47"
}
