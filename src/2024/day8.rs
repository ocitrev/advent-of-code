#[path = "../utils.rs"]
mod utils;
use itertools::Itertools;
use std::collections::{HashMap, HashSet};
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/8
    println!("Day 8, 2024: Resonant Collinearity");
    let _m = utils::Monitor::start();

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(276, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(991, p2);
}

struct Map {
    frequencies: HashMap<char, Vec<Point2d>>,
    width: i32,
    height: i32,
}

impl Map {
    pub fn new(input: &str) -> Map {
        let mut width: i32 = 0;
        let mut height: i32 = 0;

        let mut frequencies = HashMap::<char, Vec<Point2d>>::new();
        for (y, line) in input.lines().enumerate() {
            for (x, c) in line.char_indices() {
                if c != '.' {
                    let p = Point2d::new(x as i32, y as i32);
                    if let Some(locations) = frequencies.get_mut(&c) {
                        locations.push(p);
                    } else {
                        frequencies.insert(c, vec![p]);
                    }
                }
                width = x as i32 + 1;
            }

            height = y as i32 + 1;
        }

        Map {
            width: width,
            height: height,
            frequencies: frequencies,
        }
    }

    pub fn contains(&self, p: Point2d) -> bool {
        p.x >= 0 && p.y >= 0 && p.x < self.width && p.y < self.height
    }
}

fn part1(input: &'static str) -> u64 {
    let map = Map::new(input);
    let mut antinodes = HashSet::<Point2d>::new();

    for frequency in map.frequencies.values() {
        for pairs in frequency.iter().combinations(2) {
            let a = **pairs.first().unwrap();
            let b = **pairs.last().unwrap();
            let diff = b - a;

            let c = a - diff;
            if map.contains(c) {
                antinodes.insert(c);
            }

            let d = b + diff;
            if map.contains(d) {
                antinodes.insert(d);
            }
        }
    }

    antinodes.len() as u64
}

fn part2(input: &'static str) -> u64 {
    let map = Map::new(input);
    let mut antinodes = HashSet::<Point2d>::new();

    for frequency in map.frequencies.values() {
        for pairs in frequency.iter().combinations(2) {
            let mut a = **pairs.first().unwrap();
            let mut b = **pairs.last().unwrap();
            let diff = b - a;

            while map.contains(a) {
                antinodes.insert(a);
                a -= diff;
            }

            while map.contains(b) {
                antinodes.insert(b);
                b += diff;
            }
        }
    }

    antinodes.len() as u64
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day8.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            ............
            ........0...
            .....0......
            .......0....
            ....0.......
            ......A.....
            ............
            ............
            ........A...
            .........A..
            ............
            ............
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(14, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(34, super::part2(get_example()));
    }
}
