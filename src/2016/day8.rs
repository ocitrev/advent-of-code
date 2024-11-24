fn main() {
    // https://adventofcode.com/2016/day/8
    println!("Day 8, 2016: Two-Factor Authentication");

    debug_assert_eq!(6, run(get_example(), 7, 3).count_lit());
    // debug_assert_eq!(0, part2(get_example()));

    let screen = run(get_input(), 50, 6);
    let p1 = screen.count_lit();
    println!("  Part 1: {}", p1);
    assert_eq!(106, p1);

    println!("  Part 2:");
    screen.print("  ");
    // assert_eq!(0, p2);
}

struct Screen {
    lines: Vec<Vec<char>>,
    height: u16,
}

const LIT: char = '\u{2588}';

impl Screen {
    fn new(width: u16, height: u16) -> Self {
        Screen {
            lines: vec![vec![' '; width.into()]; height.into()],
            height: height,
        }
    }

    fn run(&mut self, instructions: &str) {
        for line in instructions.lines() {
            if line.starts_with("rotate row y=") {
                self.rotate_row(&line[13..]);
            } else if line.starts_with("rotate column x=") {
                self.rotate_col(&line[16..]);
            } else if line.starts_with("rect ") {
                self.rect(&line[5..]);
            }
        }
    }

    fn rect(&mut self, line: &str) {
        let mut parts = line.split("x");
        let w: i32 = parts.next().unwrap().parse().unwrap();
        let h: i32 = parts.next().unwrap().parse().unwrap();

        for y in 0..h {
            for x in 0..w {
                self.lines[y as usize][x as usize] = LIT;
            }
        }
    }

    fn rotate_row(&mut self, line: &str) {
        let mut parts = line.split(" by ");
        let y: usize = parts.next().unwrap().parse().unwrap();
        let count: usize = parts.next().unwrap().parse().unwrap();
        self.lines[y].rotate_right(count);
    }

    fn rotate_col(&mut self, line: &str) {
        let mut parts = line.split(" by ");
        let x: usize = parts.next().unwrap().parse().unwrap();
        let count: usize = parts.next().unwrap().parse().unwrap();
        let mut column: Vec<_> = self.lines.iter().map(|l| l[x]).collect();
        column.rotate_right(count);

        for y in 0..self.height {
            self.lines[y as usize][x] = column[y as usize];
        }
    }

    fn print(&self, indent: &str) {
        for line in &self.lines {
            eprintln!("{}{}", indent, line.into_iter().collect::<String>());
        }
    }

    fn count_lit(&self) -> u64 {
        self.lines
            .iter()
            .flat_map(|l| l.iter())
            .filter(|&&c| c == LIT)
            .count() as u64
    }
}

fn run(input: &'static str, w: u16, h: u16) -> Screen {
    let mut screen = Screen::new(w, h);
    screen.run(input);
    screen
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day8.txt").trim_end()
}

fn get_example() -> &'static str {
    "rect 3x2
rotate column x=1 by 1
rotate row y=0 by 4
rotate column x=1 by 1"
}
