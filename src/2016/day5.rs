use rayon::prelude::*;

fn main() {
    // https://adventofcode.com/2016/day/5
    println!("Day 5, 2016: How About a Nice Game of Chess?");

    let (p1, p2) = generate(get_input());

    println!("  Part 1: {}", p1);
    assert_eq!("1a3099aa", p1);

    println!("  Part 2: {}", p2);
    assert_eq!("694190cd", p2);
}

fn starts_with_00000(digest: &md5::Digest) -> bool {
    digest[0] == 0 && digest[1] == 0 && digest[2] & 0xf0 == 0
}

const CHUNK_SIZE: i32 = 200_000;

fn generate(prefix: &'static str) -> (String, String) {
    let mut index = 0;
    let mut result_1 = String::new();
    let mut result_2 = vec![' '; 8];

    while result_1.len() < 8 || result_2.contains(&' ') {
        let hashes: Vec<_> = (index..index + CHUNK_SIZE)
            .into_par_iter()
            .map(|i| md5::compute(format!("{}{}", prefix, i)))
            .filter(starts_with_00000)
            .map(|hash| format!("{:x}", hash))
            .collect();

        for hash in hashes {
            if result_1.len() < 8 {
                result_1.push(hash.chars().nth(5).unwrap());
            }

            if let Some(pos) = hash.chars().nth(5).and_then(|c| c.to_digit(10)) {
                if pos < 8 && result_2[pos as usize] == ' ' {
                    result_2[pos as usize] = hash.chars().nth(6).unwrap();
                }
            }
        }

        index += CHUNK_SIZE;
    }

    (result_1, result_2.into_iter().collect())
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day5.txt").trim_end()
}

#[cfg(test)]
mod tests {
    #[test]
    fn parts() {
        let results = super::generate("abc");
        assert_eq!("18f47a30", results.0);
        assert_eq!("05ace8e3", results.1);
    }
}
