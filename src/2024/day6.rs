#[path = "../utils.rs"]
mod utils;
use rayon::prelude::*;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/6
    println!("Day 6, 2024: Guard Gallivant");
    let _m = utils::Monitor::start();

    let mut f = Floor::new(get_input());
    let p1 = f.part1();
    println!("  Part 1: {}", p1);
    assert_eq!(5199, p1);

    let p2 = f.part2();
    println!("  Part 2: {}", p2);
    assert_eq!(1915, p2);
}

struct Floor {
    map: std::collections::HashMap<Point2d, u8>,
    visited: std::collections::HashSet<Point2d>,
    guard: Point2d,
}

impl Floor {
    pub fn new(input: &str) -> Floor {
        let mut map = std::collections::HashMap::<Point2d, u8>::new();
        let mut guard = Point2d::new(0, 0);

        for (y, line) in input.lines().enumerate() {
            for (x, c) in line.bytes().enumerate() {
                let p = Point2d::new(x as i32, y as i32);
                map.insert(p, c);

                if c == b'^' {
                    guard = p;
                }
            }
        }

        Floor {
            map: map,
            visited: std::collections::HashSet::<Point2d>::new(),
            guard: guard,
        }
    }

    fn visit(&mut self) {
        let mut dir = Point2d::new(0, -1);
        let mut g = self.guard;
        loop {
            self.visited.insert(g);

            let new_pos = g + dir;

            if let Some(&cell) = self.map.get(&new_pos) {
                if cell == b'#' {
                    dir = dir.rotate90_right();
                    continue;
                }

                g = new_pos;
            } else {
                break;
            }
        }
    }

    pub fn part1(&mut self) -> u64 {
        self.visit();
        self.visited.len() as u64
    }

    pub fn part2(&mut self) -> u64 {
        if self.visited.is_empty() {
            self.visit();
        }

        self.visited
            .par_iter()
            .filter(|&&p| p != self.guard)
            .map(|&obstacle| {
                let mut guard = self.guard;
                let mut visited = std::collections::HashSet::<(Point2d, Point2d)>::new();
                let mut dir = Point2d::new(0, -1);

                loop {
                    if visited.contains(&(guard, dir)) {
                        return 1;
                    }

                    visited.insert((guard, dir));
                    let new_pos = guard + dir;

                    if let Some(&cell) = self.map.get(&new_pos) {
                        if cell == b'#' || obstacle == new_pos {
                            dir = dir.rotate90_right();
                            continue;
                        }

                        guard = new_pos;
                    } else {
                        return 0;
                    }
                }
            })
            .sum()
    }
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day6.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            ....#.....
            .........#
            ..........
            ..#.......
            .......#..
            ..........
            .#..^.....
            ........#.
            #.........
            ......#...
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(41, super::Floor::new(get_example()).part1());
    }

    #[test]
    fn part2() {
        assert_eq!(6, super::Floor::new(get_example()).part2());
    }
}
