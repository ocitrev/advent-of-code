#[path = "../utils.rs"]
mod utils;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/6
    println!("Day 6, 2024: Guard Gallivant");

    debug_assert_eq!(41, part1(get_example()));
    debug_assert_eq!(6, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(5199, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(1915, p2);
}

fn part1(input: &'static str) -> u64 {
    let mut grid = std::collections::HashMap::<Point2d, u8>::new();
    let mut guard = Point2d::new(0, 0);

    for (y, line) in input.lines().enumerate() {
        for (x, c) in line.bytes().enumerate() {
            let p = Point2d::new(x as i32, y as i32);
            grid.insert(p, c);

            if c == b'^' {
                guard = p;
            }
        }
    }

    let mut visited = std::collections::HashSet::<Point2d>::new();
    let mut dir = Point2d::new(0, -1);
    loop {
        visited.insert(guard);

        let new_pos = guard + dir;

        if let Some(&cell) = grid.get(&new_pos) {
            if cell == b'#' {
                dir = dir.rotate90_right();
                continue;
            }

            guard = new_pos;
        } else {
            break;
        }
    }

    visited.len() as u64
}

fn part2(input: &'static str) -> u64 {
    let mut grid = std::collections::HashMap::<Point2d, u8>::new();
    let mut guard = Point2d::new(0, 0);
    let mut width = 0;
    let mut height = 0;

    for (y, line) in input.lines().enumerate() {
        for (x, c) in line.bytes().enumerate() {
            let p = Point2d::new(x as i32, y as i32);
            grid.insert(p, c);

            if c == b'^' {
                guard = p;
            }

            width = std::cmp::max(width, x + 1);
        }

        height = std::cmp::max(height, y + 1);
    }

    let mut sum: u64 = 0;

    for y in 0..height {
        for x in 0..width {
            let p = Point2d::new(x as i32, y as i32);

            let mut guard = guard;
            let mut visited = std::collections::HashSet::<(Point2d, Point2d)>::new();
            let mut dir = Point2d::new(0, -1);
            loop {
                if visited.contains(&(guard, dir)) {
                    sum += 1;
                    break;
                }

                visited.insert((guard, dir));

                let new_pos = guard + dir;

                if let Some(&cell) = grid.get(&new_pos) {
                    if cell == b'#' || p == new_pos {
                        dir = dir.rotate90_right();
                        continue;
                    }

                    guard = new_pos;
                } else {
                    break;
                }
            }
        }
    }

    sum
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day6.txt").trim_end()
}

fn get_example() -> &'static str {
    "....#.....
.........#
..........
..#.......
.......#..
..........
.#..^.....
........#.
#.........
......#..."
}
