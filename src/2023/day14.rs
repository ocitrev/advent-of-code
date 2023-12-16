fn main() {
    // https://adventofcode.com/2023/day/14
    println!("Day 14, 2023: Parabolic Reflector Dish");

    let fn_part2 = match std::env::args().find(|arg| arg == "1000") {
        Some(_) => part2_cheat,
        None => part2,
    };

    debug_assert_eq!(136, part1(get_example()));
    debug_assert_eq!(64, fn_part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(113424, p1);

    let p2 = fn_part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(96003, p2);
}

struct Grid {
    lines: Vec<Vec<u8>>,
    width: usize,
    height: usize,
}

impl Grid {
    fn new(input: &'static str) -> Self {
        let lines: Vec<Vec<u8>> = input
            .split('\n')
            .map(|line| line.as_bytes().to_vec())
            .collect();
        let w = lines.iter().map(|l| l.len()).max().unwrap();
        let h = lines.len();
        Grid {
            lines: lines,
            width: w,
            height: h,
        }
    }

    fn tilt_north(&mut self) {
        for i in 0..self.height - 1 {
            for y in 0..self.height - i - 1 {
                for x in 0..self.width {
                    if self.lines[y][x] == b'.' && self.lines[y + 1][x] == b'O' {
                        self.lines[y][x] = b'O';
                        self.lines[y + 1][x] = b'.';
                    }
                }
            }
        }
    }

    fn tilt_south(&mut self) {
        for i in 0..self.height - 1 {
            for y in 0..self.height - i - 1 {
                for x in 0..self.width {
                    if self.lines[y][x] == b'O' && self.lines[y + 1][x] == b'.' {
                        self.lines[y][x] = b'.';
                        self.lines[y + 1][x] = b'O';
                    }
                }
            }
        }
    }

    fn tilt_east(&mut self) {
        for line in &mut self.lines {
            for i in 0..self.width - 1 {
                for x in 0..self.width - 1 - i {
                    if line[x] == b'O' && line[x + 1] == b'.' {
                        line[x] = b'.';
                        line[x + 1] = b'O';
                    }
                }
            }
        }
    }

    fn tilt_west(&mut self) {
        for line in &mut self.lines {
            for i in 0..self.width - 1 {
                for x in 0..self.width - i - 1 {
                    if line[x] == b'.' && line[x + 1] == b'O' {
                        line[x] = b'O';
                        line[x + 1] = b'.';
                    }
                }
            }
        }
    }

    fn _print(&self, header: &str) {
        eprintln!("{}", header);
        for y in 0..self.height {
            for x in 0..self.width {
                eprint!("{}", self.lines[y][x] as char);
            }
            eprintln!();
        }
        eprintln!();
    }

    fn cycle(&mut self) {
        self.tilt_north();
        self.tilt_west();
        self.tilt_south();
        self.tilt_east();
    }

    fn get_load(&self) -> usize {
        let mut total = 0;
        for y in 0..self.height {
            let load = self.height - y;
            for x in 0..self.width {
                if self.lines[y][x] == b'O' {
                    total += load;
                }
            }
        }

        total
    }

    fn get_hash(&self) -> u64 {
        use std::collections::hash_map::DefaultHasher;
        use std::hash::Hasher;

        let mut hasher = DefaultHasher::new();
        for line in &self.lines {
            hasher.write(&line);
        }

        hasher.finish()
    }

    fn find_cycle(&mut self) -> usize {
        let mut hashes: Vec<u64> = vec![];
        hashes.push(self.get_hash());

        loop {
            self.cycle();
            let hash = self.get_hash();

            if let Some(p) = hashes.iter().position(|h| *h == hash) {
                let cycle_len = hashes.len() - p;
                let nb_more = (1_000_000_000 - p) % cycle_len;

                for _ in 0..nb_more {
                    self.cycle();
                }

                return self.get_load();
            } else {
                hashes.push(hash);
            }
        }
    }
}

fn part1(input: &'static str) -> usize {
    let mut grid = Grid::new(input);
    grid.tilt_north();
    grid.get_load()
}

fn part2(input: &'static str) -> usize {
    let mut grid = Grid::new(input);
    grid.find_cycle()
}

fn part2_cheat(input: &'static str) -> usize {
    let mut grid = Grid::new(input);

    // cheating ... should be 1_000_000_000
    // https://www.reddit.com/r/adventofcode/comments/18ihsz7/2023_day_14_part_2_coincidence_of_the_day/
    for _ in 0..1_000 {
        grid.cycle();
    }

    grid.get_load()
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day14.txt").trim_end_matches('\n')
}

fn get_example() -> &'static str {
    return "O....#....
O.OO#....#
.....##...
OO.#O....O
.O.....O#.
O.#..O.#.#
..O..#O..O
.......O..
#....###..
#OO..#....";
}
