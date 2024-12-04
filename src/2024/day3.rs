fn main() {
    // https://adventofcode.com/2024/day/3
    println!("Day 3, 2024: Mull It Over");

    debug_assert_eq!(161, run(get_example1()).0);
    debug_assert_eq!(48, run(get_example2()).1);

    let (p1, p2) = run(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(167090022, p1);

    println!("  Part 2: {}", p2);
    assert_eq!(89823704, p2);
}

fn run(input: &'static str) -> (u64, u64) {
    const WINDOW_SIZE: usize = "mul(xxx,xxx)".len();
    let re_mul = regex::Regex::new(r"^mul\((\d{1,3}),(\d{1,3})\)").unwrap();
    let mut part1: u64 = 0;
    let mut part2: u64 = 0;
    let mut enabled = true;

    for chunk in (input.to_owned() + "padpadpad")
        .as_bytes()
        .windows(WINDOW_SIZE)
    {
        let s = std::str::from_utf8(chunk).unwrap();

        if chunk.starts_with(b"do()") {
            enabled = true;
        } else if chunk.starts_with(b"don't()") {
            enabled = false;
        }

        if let Some(matches) = re_mul.captures(s) {
            let a: u64 = matches.get(1).unwrap().as_str().parse().unwrap();
            let b: u64 = matches.get(2).unwrap().as_str().parse().unwrap();
            part1 += a * b;

            if enabled {
                part2 += a * b;
            }
        }
    }

    (part1, part2)
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day3.txt").trim_end()
}

fn get_example1() -> &'static str {
    "xmul(2,4)%&mul[3,7]!@^do_not_mul(5,5)+mul(32,64]then(mul(11,8)mul(8,5))"
}

fn get_example2() -> &'static str {
    "xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))"
}
