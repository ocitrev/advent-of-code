#[path = "../utils.rs"]
mod utils;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2023/day/16
    println!("Day 16, 2023: The Floor Will Be Lava");

    debug_assert_eq!(46, part1(get_example()));
    debug_assert_eq!(51, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(6921, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(7594, p2);
}

struct Grid {
    map: std::collections::HashMap<Point2d, (u8, bool)>,
    width: i32,
    height: i32,
}

impl Grid {
    fn parse(input: &'static str) -> Self {
        let mut grid = std::collections::HashMap::<Point2d, (u8, bool)>::new();
        for (y, line) in input.lines().enumerate() {
            for (x, c) in line.as_bytes().iter().enumerate() {
                grid.insert(Point2d::new(x as i32, y as i32), (*c, false));
            }
        }
        let w = grid.iter().map(|elm| elm.0.x).max().unwrap();
        let h = grid.iter().map(|elm| elm.0.y).max().unwrap();

        Grid {
            map: grid,
            width: w,
            height: h,
        }
    }

    fn fire_laser(&mut self, p: Point2d, dir: Point2d) {
        if let Some(cell) = self.map.get_mut(&p) {
            match (cell.0, cell.1, dir.x, dir.y) {
                (b'|', true, _, 0) => return,
                (b'-', true, 0, _) => return,
                _ => cell.1 = true,
            }

            if cell.0 == b'|' && dir.y == 0 {
                self.fire_laser(p + Point2d::NORTH, Point2d::NORTH);
                self.fire_laser(p + Point2d::SOUTH, Point2d::SOUTH);
            } else if cell.0 == b'-' && dir.x == 0 {
                self.fire_laser(p + Point2d::WEST, Point2d::WEST);
                self.fire_laser(p + Point2d::EAST, Point2d::EAST);
            } else if cell.0 == b'\\' {
                let new_dir = if dir.y == 0 {
                    dir.rotate90_right()
                } else {
                    dir.rotate90_left()
                };
                self.fire_laser(p + new_dir, new_dir);
            } else if cell.0 == b'/' {
                let new_dir = if dir.y == 0 {
                    dir.rotate90_left()
                } else {
                    dir.rotate90_right()
                };
                self.fire_laser(p + new_dir, new_dir);
            } else {
                self.fire_laser(p + dir, dir);
            }
        }
    }

    fn reset(&mut self) {
        for v in self.map.values_mut() {
            v.1 = false;
        }
    }

    fn _print_grid(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let c = self.map.get(&Point2d::new(x, y)).unwrap().0;
                eprint!("{}", c as char);
            }
            eprintln!()
        }
        eprintln!()
    }

    fn _print_energized(&self) {
        for y in 0..self.height {
            for x in 0..self.width {
                let energized = self.map.get(&Point2d::new(x, y)).unwrap().1;
                eprint!("{}", if energized { '#' } else { '.' });
            }
            eprintln!()
        }
        eprintln!()
    }

    fn count_energized(&self) -> u64 {
        self.map.values().fold(0, |total, v| total + v.1 as u64)
    }
}

fn part1(input: &'static str) -> u64 {
    let mut grid = Grid::parse(input);

    grid.fire_laser(Point2d::new(0, 0), Point2d::EAST);
    grid.count_energized()
}

fn part2(input: &'static str) -> u64 {
    let mut grid = Grid::parse(input);
    let mut max_value: u64 = 0;

    for x in 0..grid.width {
        let p = Point2d::new(x, grid.height - 1);
        grid.fire_laser(p, Point2d::NORTH);
        max_value = std::cmp::max(max_value, grid.count_energized());
        grid.reset();

        let p = Point2d::new(x, 0);
        grid.fire_laser(p, Point2d::SOUTH);
        max_value = std::cmp::max(max_value, grid.count_energized());
        grid.reset();
    }

    for y in 0..grid.height {
        let p = Point2d::new(0, y);
        grid.fire_laser(p, Point2d::EAST);
        max_value = std::cmp::max(max_value, grid.count_energized());
        grid.reset();

        let p = Point2d::new(grid.width - 1, 0);
        grid.fire_laser(p, Point2d::WEST);
        max_value = std::cmp::max(max_value, grid.count_energized());
        grid.reset();
    }

    max_value
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day16.txt")
}

fn get_example() -> &'static str {
    r#".|...\....
|.-.\.....
.....|-...
........|.
..........
.........\
..../.\\..
.-.-/..|..
.|....-|.\
..//.|...."#
}
