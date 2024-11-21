use rayon::prelude::*;

fn main() {
    // https://adventofcode.com/2016/day/5
    println!("Day 5, 2016: How About a Nice Game of Chess?");

    debug_assert_eq!("18f47a30", part1(get_example()));
    debug_assert_eq!("05ace8e3", part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!("1a3099aa", p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!("694190cd", p2);
}

fn starts_with_00000(digest: &md5::Digest) -> bool {
    digest[0] == 0 && digest[1] == 0 && digest[2] & 0xf0 == 0
}

const CHUNK_SIZE: i32 = 200_000;

fn part1(input: &'static str) -> String {
    let mut index = 0;
    let mut result = String::new();

    while result.len() < 8 {
        let hashes: Vec<_> = (index..index + CHUNK_SIZE)
            .into_par_iter()
            .map(|i| {
                let hash_input = format!("{}{}", input, i);
                md5::compute(hash_input)
            })
            .filter(starts_with_00000)
            .map(|hash| format!("{:x}", hash))
            .collect();

        for hash in hashes {
            if result.len() < 8 {
                result.push(hash.chars().nth(5).unwrap());
            }
        }

        index += CHUNK_SIZE;
    }

    result
}

fn part2(input: &'static str) -> String {
    let mut next = 0;
    let mut result = vec![' '; 8];

    while result.contains(&' ') {
        let hashes: Vec<_> = (next..next + CHUNK_SIZE)
            .into_par_iter()
            .map(|i| {
                let hash_input = format!("{}{}", input, i);
                md5::compute(hash_input)
            })
            .filter(starts_with_00000)
            .map(|hash| format!("{:x}", hash))
            .collect();

        for hash in hashes {
            if let Some(pos) = hash.chars().nth(5).and_then(|c| c.to_digit(10)) {
                if pos < 8 && result[pos as usize] == ' ' {
                    result[pos as usize] = hash.chars().nth(6).unwrap();
                }
            }
        }

        next += CHUNK_SIZE;
    }

    result.into_iter().collect()
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day5.txt").trim_end()
}

fn get_example() -> &'static str {
    "abc"
}
