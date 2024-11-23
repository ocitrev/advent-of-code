fn main() {
    // https://adventofcode.com/2016/day/6
    println!("Day 6, 2016: Signals and Noise");

    debug_assert_eq!(("easter".into(), "advent".into()), decode(get_example()));

    let result = decode(get_input());

    println!("  Part 1: {}", result.0);
    assert_eq!("tkspfjcc", result.0);

    println!("  Part 2: {}", result.1);
    assert_eq!("xrlmbypn", result.1);
}

fn decode(input: &'static str) -> (String, String) {
    let max_line_len = input.lines().map(|line| line.len()).max().unwrap();
    let mut result_max = String::with_capacity(max_line_len);
    let mut result_min = String::with_capacity(max_line_len);

    for column in 0..max_line_len {
        let mut char_map = std::collections::HashMap::new();
        for line in input.lines() {
            let c = line.chars().nth(column).unwrap();
            *char_map.entry(c).or_insert(0) += 1u64;
        }

        let max_char = *char_map.iter().max_by(|a, b| a.1.cmp(b.1)).unwrap().0;
        result_max.push(max_char);

        let min_char = *char_map.iter().min_by(|a, b| a.1.cmp(b.1)).unwrap().0;
        result_min.push(min_char);
    }

    (result_max, result_min)
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day6.txt").trim_end()
}

fn get_example() -> &'static str {
    "eedadn
drvtee
eandsr
raavrd
atevrs
tsrnev
sdttsa
rasrtv
nssdts
ntnada
svetve
tesnvt
vntsnd
vrdear
dvrsen
enarar"
}
