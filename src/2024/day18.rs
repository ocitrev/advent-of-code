#[path = "../utils.rs"]
mod utils;
use utils::Point2d;

fn main() {
    // https://adventofcode.com/2024/day/18
    println!("Day 18, 2024: RAM Run");
    let _m = utils::Monitor::start();

    let p1 = part1(get_input(), (71, 71), 1024);
    println!("  Part 1: {}", p1);
    assert_eq!(380, p1);

    let p2 = part2(get_input(), (71, 71), 1024);
    println!("  Part 2: {}", p2);
    assert_eq!("26,50", p2);
}

type Int = i32;

struct Grid {
    bad_blocks: Vec<Point2d>,
    size: (Int, Int),
}

impl Grid {
    fn new(input: &str, size: (Int, Int)) -> Grid {
        let mut bad_blocks = vec![];
        for line in input.lines() {
            let mut parts = line.split(',');
            let x = parts.next().unwrap().parse::<Int>().unwrap();
            let y = parts.next().unwrap().parse::<Int>().unwrap();
            assert!(x < size.0);
            assert!(y < size.1);
            bad_blocks.push(Point2d::new(x, y));
        }

        Grid {
            bad_blocks: bad_blocks,
            size: size,
        }
    }

    fn get_neighbors(&self, p: Point2d, nb_blocks: usize) -> Vec<(Point2d, Int)> {
        let mut result = vec![];

        for new_dir in vec![Point2d::NORTH, Point2d::SOUTH, Point2d::EAST, Point2d::WEST] {
            let new_pos = p + new_dir;
            if new_pos.x < 0 || new_pos.y < 0 {
                continue;
            }
            if new_pos.x >= self.size.0 || new_pos.y >= self.size.1 {
                continue;
            }

            if !self.contains(nb_blocks, new_pos) {
                result.push((new_pos, 1));
            }
        }

        result
    }

    fn shortest_path_len(&self, nb_blocks: usize) -> Option<Int> {
        let start = Point2d::new(0, 0);
        let goal = Point2d::new(self.size.0 - 1, self.size.1 - 1);
        if let Some((_, cost)) = pathfinding::directed::astar::astar(
            &start,
            |&p| self.get_neighbors(p, nb_blocks),
            |&p| p.distance(&goal) as Int,
            |&p| p == goal,
        ) {
            Some(cost)
        } else {
            None
        }
    }

    fn contains(&self, nb_blocks: usize, p: Point2d) -> bool {
        self.bad_blocks
            .iter()
            .take(nb_blocks)
            .position(|&pp| pp == p)
            .is_some()
    }

    fn _print_path(&self, path: &Vec<Point2d>, nb_blocks: usize) {
        for y in 0..self.size.1 {
            for x in 0..self.size.0 {
                let p = Point2d::new(x as i32, y as i32);
                if path.contains(&p) {
                    eprint!("O");
                } else if self.contains(nb_blocks, p) {
                    eprint!("#");
                } else {
                    eprint!(".");
                }
            }
            eprintln!();
        }
    }
}

fn part1(input: &'static str, size: (Int, Int), nb_blocks: usize) -> Int {
    let grid = Grid::new(input, size);
    grid.shortest_path_len(nb_blocks).unwrap()
}

fn part2(input: &'static str, size: (Int, Int), min_blocks: usize) -> String {
    let grid = Grid::new(input, size);
    let mut nb_min = min_blocks;
    let mut nb_max = grid.bad_blocks.len();

    loop {
        if nb_max - nb_min <= 1 {
            let blocking = grid.bad_blocks[nb_min];
            return format!("{},{}", blocking.x, blocking.y);
        }

        let nb_blocks = nb_min + (nb_max - nb_min) / 2;

        if let None = grid.shortest_path_len(nb_blocks) {
            nb_max = nb_blocks;
        } else {
            nb_min = nb_blocks;
        }
    }
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day18.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            5,4
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
            2,0
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(22, super::part1(get_example(), (7, 7), 12));
    }

    #[test]
    fn part2() {
        assert_eq!("6,1", super::part2(get_example(), (7, 7), 12));
    }
}
