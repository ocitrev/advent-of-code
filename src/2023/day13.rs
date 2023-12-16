fn main() {
    // https://adventofcode.com/2023/day/13
    println!("Day 13, 2023: Point of Incidence");

    debug_assert_eq!(405, part1(get_example()));
    debug_assert_eq!(400, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(37975, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(32497, p2);
}

fn part1(input: &'static str) -> i32 {
    let parts = input.split("\n\n");
    let mut total = 0;

    for p in parts {
        let lines: Vec<&[u8]> = p.lines().map(|x| x.as_bytes()).collect();
        total += get_horizontal_symetry(&lines, 0) * 100;
        total += get_vertial_symetry(&lines, 0);
    }

    total
}

fn part2(input: &'static str) -> i32 {
    let parts = input.split("\n\n");
    let mut total = 0;

    for p in parts {
        let lines: Vec<&[u8]> = p.lines().map(|x| x.as_bytes()).collect();
        total += get_horizontal_symetry(&lines, 1) * 100;
        total += get_vertial_symetry(&lines, 1);
    }

    total
}

fn get_vertial_symetry(lines: &Vec<&[u8]>, max_errors: i32) -> i32 {
    let w = lines.iter().map(|l| l.len()).max().unwrap_or(0) as i32;
    let h = lines.len() as i32;

    for x in 0..w - 1 {
        let mut nb_errors = 0;
        for xx in 0..w {
            let left = x - xx;
            let right: i32 = x + 1 + xx;
            if left >= 0 && left < right && right < w {
                for y in 0..h {
                    if lines[y as usize][left as usize] != lines[y as usize][right as usize] {
                        nb_errors += 1;
                    }
                }
            }
        }

        if nb_errors == max_errors {
            return x + 1;
        }
    }

    0
}

fn get_horizontal_symetry(lines: &Vec<&[u8]>, max_errors: i32) -> i32 {
    let w = lines.iter().map(|l| l.len()).max().unwrap_or(0) as i32;
    let h = lines.len() as i32;
    let w = w as i32;

    for y in 0..h - 1 {
        let mut nb_errors = 0;
        for yy in 0..h {
            let top = y - yy;
            let bottom: i32 = y + 1 + yy;
            if top >= 0 && top < bottom && bottom < h {
                for x in 0..w {
                    let t = lines[top as usize][x as usize];
                    let b = lines[bottom as usize][x as usize];

                    if t != b {
                        nb_errors += 1;
                    }
                }
            }
        }

        if nb_errors == max_errors {
            return y + 1;
        }
    }

    0
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day13.txt")
}

fn get_example() -> &'static str {
    "#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#"
}
