#[path = "../utils.rs"]
mod utils;
use std::char::from_digit;

use utils::Point2d;

fn main() {
    // https://adventofcode.com/2023/day/17
    println!("Day 17, 2023: Clumsy Crucible");

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    // assert_eq!(0, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    // assert_eq!(0, p2);
}

#[derive(Clone, Debug, Eq)]
struct Pos {
    p: Point2d,
    before: Vec<Point2d>,
}

impl std::hash::Hash for Pos {
    fn hash<H: std::hash::Hasher>(&self, state: &mut H) {
        self.p.hash(state)
    }
}

impl PartialEq for Pos {
    fn eq(&self, other: &Self) -> bool {
        self.p == other.p
    }
}

struct Grid {
    map: std::collections::HashMap<Point2d, u32>,
    width: u32,
    height: u32,
}

impl Grid {
    fn parse(input: &'static str) -> Grid {
        let mut map = std::collections::HashMap::<Point2d, u32>::new();
        let mut w: u32 = 0;
        let mut h: u32 = 0;
        for (y, line) in input.lines().enumerate() {
            for (x, c) in line.bytes().enumerate() {
                w = std::cmp::max(w, x as u32);
                map.insert(Point2d::new(x as i32, y as i32), (c - b'0') as u32);
            }
            h = std::cmp::max(h, y as u32);
        }

        Grid {
            map: map,
            width: w,
            height: h,
        }
    }

    fn get_neighbors(&self, p: &Pos) -> Vec<(Pos, u32)> {
        let mut result = vec![];

        let mut can_go_north = true;
        let mut can_go_south = true;
        let mut can_go_east = true;
        let mut can_go_west = true;

        if let Some(&last) = p.before.last() {
            can_go_north &= last != Point2d::SOUTH;
            can_go_south &= last != Point2d::NORTH;
            can_go_east &= last != Point2d::WEST;
            can_go_west &= last != Point2d::EAST;
        }

        if p.before.len() >= 3 {
            let last3 = p.before.iter().rev().take(3);
            can_go_north &= !last3.clone().all(|&dir| dir == Point2d::NORTH);
            can_go_south &= !last3.clone().all(|&dir| dir == Point2d::SOUTH);
            can_go_east &= !last3.clone().all(|&dir| dir == Point2d::EAST);
            can_go_west &= !last3.clone().all(|&dir| dir == Point2d::WEST);
        }

        if can_go_north {
            let n = p.p + Point2d::NORTH;
            if let Some(cost) = self.map.get(&n) {
                result.push((
                    Pos {
                        p: n,
                        before: p
                            .before
                            .iter()
                            .copied()
                            .chain(vec![Point2d::NORTH])
                            .collect(),
                    },
                    *cost,
                ))
            }
        }

        if can_go_south {
            let s = p.p + Point2d::SOUTH;
            if let Some(cost) = self.map.get(&s) {
                result.push((
                    Pos {
                        p: s,
                        before: p
                            .before
                            .iter()
                            .copied()
                            .chain(vec![Point2d::SOUTH])
                            .collect(),
                    },
                    *cost,
                ))
            }
        }

        if can_go_west {
            let w = p.p + Point2d::WEST;
            if let Some(cost) = self.map.get(&w) {
                result.push((
                    Pos {
                        p: w,
                        before: p
                            .before
                            .iter()
                            .copied()
                            .chain(vec![Point2d::WEST])
                            .collect(),
                    },
                    *cost,
                ))
            }
        }

        if can_go_east {
            let e = p.p + Point2d::EAST;
            if let Some(cost) = self.map.get(&e) {
                result.push((
                    Pos {
                        p: e,
                        before: p
                            .before
                            .iter()
                            .copied()
                            .chain(vec![Point2d::EAST])
                            .collect(),
                    },
                    *cost,
                ))
            }
        }

        result
    }
}

fn part1(input: &'static str) -> u32 {
    let grid = Grid::parse(input);
    let goal = Point2d::new(grid.width as i32, grid.height as i32);

    let start = Point2d::new(0, 0);

    let result = pathfinding::directed::astar::astar(
        &Pos {
            p: start,
            before: vec![],
        },
        |p| grid.get_neighbors(p),
        |p| p.p.distance(&goal),
        |p| p.p == goal,
    );

    let (path, cost) = result.expect("No path!");

    for y in 0..grid.height {
        for x in 0..grid.width {
            let p = Point2d::new(y as i32, x as i32);

            let c = if path.iter().find(|&pos| pos.p == p).is_some() {
                '#'
            } else {
                from_digit(*grid.map.get(&p).unwrap(), 10).unwrap()
            };
            eprint!("{}", c);
        }
        eprintln!();
    }

    grid.map.get(&start).unwrap() + cost as u32
    // total
}

fn part2(_input: &'static str) -> u64 {
    0
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day17.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            2413432311323
            3215453535623
            3255245654254
            3446585845452
            4546657867536
            1438598798454
            4457876987766
            3637877979653
            4654967986887
            4564679986453
            1224686865563
            2546548887735
            4322674655533
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(102, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(0, super::part2(get_example()));
    }
}
