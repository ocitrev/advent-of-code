fn main() {
    // https://adventofcode.com/2023/day/15
    println!("Day 15, 2023: Lens Library");

    debug_assert_eq!(52, hash("HASH"));
    debug_assert_eq!(0, hash("rn"));
    debug_assert_eq!(1320, part1(get_example()));
    debug_assert_eq!(145, part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(519041, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(260530, p2);
}

fn part1(input: &'static str) -> u32 {
    let mut total = 0;
    for ins in input.split(',') {
        let h = hash(ins);
        total += h as u32;
    }
    total
}

fn part2(input: &'static str) -> usize {
    let mut boxes = std::collections::HashMap::<u8, Vec<(&[u8], usize)>>::new();

    for ins in input.split(',') {
        if let Some(sep) = ins.find(|c| c == '-' || c == '=') {
            let ins_bytes = ins.as_bytes();
            let op = ins_bytes[sep];
            let label = &ins_bytes[0..sep];
            let box_num = hash_bytes(&label);

            if op == b'-' {
                if let Some(box_numbers) = boxes.get_mut(&box_num) {
                    box_numbers.retain(|t| t.0 != label);
                }
            } else if op == b'=' {
                let num: usize = std::str::from_utf8(&ins_bytes[sep + 1..])
                    .unwrap()
                    .parse()
                    .unwrap();
                if let Some(box_numbers) = boxes.get_mut(&box_num) {
                    if let Some(idx) = box_numbers.iter().position(|t| t.0 == label) {
                        box_numbers[idx].1 = num;
                    } else {
                        box_numbers.push((label, num));
                    }
                } else {
                    boxes.insert(box_num, vec![(label, num)]);
                }
            }
        }
    }

    let mut total = 0;
    for (box_id, b) in boxes.iter() {
        let box_num: usize = (*box_id as usize) + 1;
        for (i, f) in b.iter().enumerate() {
            total += (i + 1) * f.1 * box_num;
        }
    }
    total
}
fn hash_bytes(input: &[u8]) -> u8 {
    let mut h = std::num::Wrapping(0u8);
    for c in input {
        h += std::num::Wrapping(*c);
        h *= std::num::Wrapping(17u8);
    }
    h.0
}
fn hash(input: &str) -> u8 {
    hash_bytes(input.as_bytes())
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day15.txt").trim()
}

fn get_example() -> &'static str {
    "rn=1,cm-,qp=3,cm=2,qp-,pc=4,ot=9,ab=5,pc-,pc=6,ot=7"
}
