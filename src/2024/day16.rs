#[path = "../utils.rs"]
mod utils;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/16
    println!("Day 16, 2024: Reindeer Maze");

    debug_assert_eq!(7036, part1(get_example()));
    debug_assert_eq!(11048, part1(get_example2()));
    debug_assert_eq!(45, part2(get_example()));
    debug_assert_eq!(64, part2(get_example2()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(88416, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(442, p2);
}

#[derive(Clone, Debug, Eq)]
struct Pos {
    p: Point2d,
    dir: Point2d,
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

impl Pos {
    fn get_cost(&self, new_dir: Point2d) -> i32 {
        let cost: i32 = new_dir.x.abs() + new_dir.y.abs();

        if self.dir == new_dir {
            return cost;
        }

        cost + 1000
    }
}

struct Grid {
    map: std::collections::HashMap<Point2d, u8>,
    _width: u32,
    _height: u32,
    start: Point2d,
    end: Point2d,
}

impl Grid {
    fn parse(input: &'static str) -> Grid {
        let mut map = std::collections::HashMap::<Point2d, u8>::new();
        let mut w: u32 = 0;
        let mut h: u32 = 0;
        let mut start = Point2d::new(0, 0);
        let mut end = Point2d::new(0, 0);
        for (y, line) in input.lines().enumerate() {
            for (x, c) in line.bytes().enumerate() {
                w = std::cmp::max(w, (x + 1) as u32);
                let p = Point2d::new(x as i32, y as i32);

                let c = match c {
                    b'E' => {
                        end = p;
                        b'.'
                    }
                    b'S' => {
                        start = p;
                        b'.'
                    }
                    _ => c,
                };
                map.insert(p, c);
            }
            h = std::cmp::max(h, (y + 1) as u32);
        }

        Grid {
            map: map,
            _width: w,
            _height: h,
            start: start,
            end: end,
        }
    }

    fn get_neighbors(&self, p: &Pos) -> Vec<(Pos, i32)> {
        let mut result = vec![];
        let dirs = vec![Point2d::NORTH, Point2d::SOUTH, Point2d::EAST, Point2d::WEST];

        for new_dir in dirs {
            if p.dir == new_dir * -1 {
                continue;
            }
            let new_pos = p.p + new_dir;
            if let Some(b'.') = self.map.get(&new_pos) {
                result.push((
                    Pos {
                        p: new_pos,
                        dir: new_dir,
                    },
                    p.get_cost(new_dir),
                ));
            }
        }

        result
    }

    fn _print(&self, path: &Vec<Pos>) {
        for y in 0..self._height {
            for x in 0..self._width {
                let pt = Point2d::new(x as i32, y as i32);

                if path.iter().any(|p| p.p == pt) {
                    eprint!("O");
                } else {
                    eprint!(
                        "{}",
                        char::from_u32(*self.map.get(&pt).unwrap() as u32).unwrap()
                    );
                }
            }

            eprintln!();
        }
    }
}

fn part1(input: &'static str) -> i32 {
    let grid = Grid::parse(input);

    let result = pathfinding::directed::yen::yen(
        &Pos {
            p: grid.start,
            dir: Point2d::EAST,
        },
        |p| grid.get_neighbors(p),
        |p| p.p == grid.end,
        1,
    );

    result[0].1
}

fn part2(input: &'static str) -> u64 {
    let grid = Grid::parse(input);

    let result = pathfinding::directed::yen::yen(
        &Pos {
            p: grid.start,
            dir: Point2d::EAST,
        },
        |p| grid.get_neighbors(p),
        |p| p.p == grid.end,
        6,
    );

    let mut points = std::collections::HashSet::<Point2d>::new();
    let best_cost = result[0].1;

    for (path, cost) in result {
        if best_cost != cost {
            break;
        }

        for p in &path {
            points.insert(p.p);
        }
    }

    points.len() as u64
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day16.txt").trim_end()
}

fn get_example() -> &'static str {
    "###############
#.......#....E#
#.#.###.#.###.#
#.....#.#...#.#
#.###.#####.#.#
#.#.#.......#.#
#.#.#####.###.#
#...........#.#
###.#.#####.#.#
#...#.....#.#.#
#.#.#.###.#.#.#
#.....#...#.#.#
#.###.#.#.#.#.#
#S..#.....#...#
###############"
}

fn get_example2() -> &'static str {
    "#################
#...#...#...#..E#
#.#.#.#.#.#.#.#.#
#.#.#.#...#...#.#
#.#.#.#.###.#.#.#
#...#.#.#.....#.#
#.#.#.#.#.#####.#
#.#...#.#.#.....#
#.#.#####.#.###.#
#.#.#.......#...#
#.#.###.#####.###
#.#.#...#.....#.#
#.#.#.#####.###.#
#.#.#.........#.#
#.#.#.#########.#
#S#.............#
#################"
}
