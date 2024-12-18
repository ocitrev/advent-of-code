#[path = "../utils.rs"]
mod utils;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/18
    println!("Day 18, 2024: RAM Run");
    let _m = utils::Monitor::start();

    debug_assert_eq!(22, part1(get_example(), (7, 7), 12));
    let p1 = part1(get_input(), (71, 71), 1024);
    println!("  Part 1: {}", p1);
    assert_eq!(380, p1);

    debug_assert_eq!("6,1", part2(get_example(), (7, 7), 12));
    let p2 = part2(get_input(), (71, 71), 1024);
    println!("  Part 2: {}", p2);
    assert_eq!("26,50", p2);
}

type Int = i32;

struct Grid {
    bad_blocks: Vec<Point2d>,
    reserve: Vec<Point2d>,
    size: (Int, Int),
}

impl Grid {
    fn new(input: &str, size: (Int, Int), min_blocks: usize) -> Grid {
        let mut bad_blocks = vec![];
        let mut reserve = vec![];
        for line in input.lines() {
            let mut parts = line.split(',');
            let x = parts.next().unwrap().parse::<Int>().unwrap();
            let y = parts.next().unwrap().parse::<Int>().unwrap();
            assert!(x < size.0);
            assert!(y < size.1);

            if bad_blocks.len() < min_blocks {
                bad_blocks.push(Point2d::new(x, y));
            } else {
                reserve.push(Point2d::new(x, y));
            }
        }

        Grid {
            bad_blocks: bad_blocks,
            reserve: reserve.into_iter().rev().collect(),
            size: size,
        }
    }

    fn get_neighbors(&self, p: Point2d) -> Vec<(Point2d, u32)> {
        let mut result = vec![];

        for new_dir in vec![Point2d::NORTH, Point2d::SOUTH, Point2d::EAST, Point2d::WEST] {
            let new_pos = p + new_dir;
            if new_pos.x < 0 || new_pos.y < 0 {
                continue;
            }
            if new_pos.x >= self.size.0 || new_pos.y >= self.size.1 {
                continue;
            }

            if !self.bad_blocks.contains(&new_pos) {
                result.push((new_pos, 1));
            }
        }

        result
    }

    fn _print_path(&self, path: &Vec<Point2d>) {
        for y in 0..self.size.1 {
            for x in 0..self.size.0 {
                let p = Point2d::new(x as i32, y as i32);
                if path.contains(&p) {
                    eprint!("O");
                } else {
                    if self.bad_blocks.contains(&p) {
                        eprint!("#");
                    } else {
                        eprint!(".");
                    }
                }
            }
            eprintln!();
        }
    }

    fn drop_block(&mut self) {
        self.bad_blocks.push(self.reserve.pop().unwrap());
    }
}

fn part1(input: &'static str, size: (Int, Int), nb_blocks: usize) -> Int {
    let grid = Grid::new(input, size, nb_blocks);

    let start = Point2d::new(0, 0);
    let goal = Point2d::new(size.0 - 1, size.1 - 1);
    let results = pathfinding::directed::astar::astar(
        &start,
        |&p| grid.get_neighbors(p),
        |&p| p.distance(&goal),
        |&p| p == goal,
    );

    let (_, cost) = results.expect("No path!");
    cost as Int
}

fn part2(input: &'static str, size: (Int, Int), min_blocks: usize) -> String {
    let mut grid = Grid::new(input, size, min_blocks);

    let start = Point2d::new(0, 0);
    let goal = Point2d::new(size.0 - 1, size.1 - 1);

    loop {
        grid.drop_block();

        let results = pathfinding::directed::astar::astar(
            &start,
            |&p| grid.get_neighbors(p),
            |&p| p.distance(&goal),
            |&p| p == goal,
        );

        if let None = results {
            let blocking = grid.bad_blocks.last().unwrap();
            return format!("{},{}", blocking.x, blocking.y);
        }
    }
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day18.txt").trim_end()
}

fn get_example() -> &'static str {
    "5,4
4,2
4,5
3,0
2,1
6,3
2,4
1,5
0,6
3,3
2,6
5,1
1,2
5,5
2,5
6,5
1,4
0,4
6,4
1,1
6,1
1,0
0,5
1,6
2,0"
}
