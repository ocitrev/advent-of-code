use std::collections::HashMap;

fn main() {
    // https://adventofcode.com/2023/day/12
    println!("Day 12, 2023: Hot Springs");

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(6871, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(2043098029844, p2);
}

fn part1(input: &'static str) -> u64 {
    let mut total: u64 = 0;
    for line in input.lines() {
        let mut cache = HashMap::<(usize, usize, usize), u64>::new();
        let parts: Vec<&str> = line.split(' ').collect();
        let data = parts[0];
        let groups: Vec<usize> = parts[1]
            .split(',')
            .map(|n| n.parse::<usize>().unwrap())
            .collect();

        total += count_arrangements(data.as_bytes(), groups.as_slice(), &mut cache, 0, 0, 0);
    }

    return total;
}

fn part2(input: &'static str) -> u64 {
    let mut total: u64 = 0;
    for line in input.lines() {
        let mut cache = HashMap::<(usize, usize, usize), u64>::new();
        let parts: Vec<&str> = line.split(' ').collect();
        let data = vec![parts[0], parts[0], parts[0], parts[0], parts[0]].join("?");
        let groups: Vec<usize> = vec![parts[1], parts[1], parts[1], parts[1], parts[1]]
            .join(",")
            .split(',')
            .map(|n| n.parse::<usize>().unwrap())
            .collect();

        total += count_arrangements(data.as_bytes(), groups.as_slice(), &mut cache, 0, 0, 0);
    }

    return total;
}

fn count_arrangements(
    data: &[u8],
    groups: &[usize],
    cache: &mut HashMap<(usize, usize, usize), u64>,
    i: usize,
    gi: usize,
    count: usize,
) -> u64 {
    let key = (i, gi, count);

    if let Some(cached) = cache.get(&key) {
        return *cached;
    }

    if i == data.len() {
        if gi == groups.len() && count == 0 {
            return 1;
        } else if gi == groups.len() - 1 && groups[gi] == count {
            return 1;
        } else {
            return 0;
        }
    }

    let mut total: u64 = 0;

    for c in [b'.', b'#'] {
        if data[i] == c || data[i] == b'?' {
            if c == b'.' && count == 0 {
                total += count_arrangements(data, groups, cache, i + 1, gi, 0);
            } else if c == b'.' && count != 0 && gi < groups.len() && groups[gi] == count {
                total += count_arrangements(data, groups, cache, i + 1, gi + 1, 0);
            } else if c == b'#' {
                total += count_arrangements(data, groups, cache, i + 1, gi, count + 1);
            }
        }
    }

    cache.insert(key, total);
    return total;
}

fn get_input() -> &'static str {
    return include_str!("../../inputs/2023/day12.txt");
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            ???.### 1,1,3
            .??..??...?##. 1,1,3
            ?#?#?#?#?#?#?#? 1,3,1,6
            ????.#...#... 4,1,1
            ????.######..#####. 1,6,5
            ?###???????? 3,2,1
        "}
    }

    #[test]
    fn part1() {
        assert_eq!(21, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(525152, super::part2(get_example()));
    }
}
