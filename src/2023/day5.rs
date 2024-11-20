fn main() {
    // https://adventofcode.com/2023/day/5
    println!("Day 5, 2023: If You Give A Seed A Fertilizer");

    let fn_part2 = match std::env::args().find(|arg| arg == "brute") {
        Some(_) => part2_bruteforce,
        None => part2,
    };

    debug_assert_eq!(35, part1(get_example()));
    debug_assert_eq!(46, fn_part2(get_example()));

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(389056265, p1);

    let p2 = fn_part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(137516820, p2);
}

fn part1(input: &'static str) -> u64 {
    let almanac = Almanac::new(input);
    almanac
        .seeds
        .iter()
        .map(|&s| almanac.get_location(s))
        .min()
        .unwrap()
}

fn part2_bruteforce(input: &'static str) -> u64 {
    let almanac = Almanac::new(input);
    almanac.get_lowest_location_from_seed_ranges_bruteforce()
}

fn part2(input: &'static str) -> u64 {
    let almanac = Almanac::new(input);
    almanac.get_lowest_location_from_seed_ranges()
}

struct Range {
    dest: u64,
    source: u64,
    len: u64,
}

struct Mapping {
    ranges: Vec<Range>,
}

impl Range {
    fn new(line: &str) -> Self {
        let parts: Vec<u64> = line.split(' ').map(|n| n.parse::<u64>().unwrap()).collect();
        Self {
            dest: parts[0],
            source: parts[1],
            len: parts[2],
        }
    }
}

impl Mapping {
    fn new() -> Mapping {
        Mapping { ranges: vec![] }
    }

    fn parse(&mut self, line: &str) {
        self.ranges.push(Range::new(line));
    }

    fn convert(&self, value: u64) -> u64 {
        for range in &self.ranges {
            if value >= range.source {
                let delta = value - range.source;
                if delta < range.len {
                    return range.dest + delta;
                }
            }
        }

        value
    }

    fn unconvert(&self, value: u64) -> u64 {
        for range in &self.ranges {
            if value >= range.dest {
                let delta = value - range.dest;
                if delta < range.len {
                    return range.source + delta;
                }
            }
        }

        value
    }
}

struct Almanac {
    seeds: Vec<u64>,
    mappings: Vec<Mapping>,
}

impl Almanac {
    fn new(input: &str) -> Self {
        let mut almanac = Self {
            seeds: vec![],
            mappings: vec![],
        };

        let parts: Vec<&str> = input.split("\n\n").collect();

        almanac.seeds = parts[0][7..]
            .split(' ')
            .map(|s| s.parse().unwrap())
            .collect();

        for p in parts.iter().skip(1) {
            let mut mapping = Mapping::new();
            for line in p.lines().skip(1) {
                mapping.parse(line);
            }
            almanac.mappings.push(mapping);
        }

        almanac
    }

    fn get_location(&self, seed: u64) -> u64 {
        self.mappings.iter().fold(seed, |value, r| r.convert(value))
    }

    fn get_seed(&self, location: u64) -> u64 {
        self.mappings
            .iter()
            .rev()
            .fold(location, |value, r| r.unconvert(value))
    }

    fn is_valid_seed(&self, seed: u64) -> bool {
        for (start, count) in self.seeds.chunks(2).map(|s| (s[0], s[1])) {
            if seed >= start && seed < start + count {
                return true;
            }
        }

        false
    }

    fn get_lowest_location_from_seed_ranges_bruteforce(&self) -> u64 {
        let mut location: u64 = 0;

        loop {
            if self.is_valid_seed(self.get_seed(location)) {
                return location;
            }

            location += 1;
        }
    }

    fn get_lowest_location_from_seed_ranges(&self) -> u64 {
        for (start, len) in self.seeds.chunks(2).map(|s| (s[0], s[1])) {
            let _v = self.find_min(start, len);
        }
        0
    }

    fn find_min(&self, start: u64, len: u64) -> u64 {
        for m in &self.mappings {
            for r in &m.ranges {
                eprintln!(
                    "s:{}, l:{} -> dst: {}, src:{}, len:{}",
                    start, len, r.dest, r.source, r.len
                );
            }
        }
        0
    }
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2023/day5.txt")
}

fn get_example() -> &'static str {
    "seeds: 79 14 55 13

seed-to-soil map:
50 98 2
52 50 48

soil-to-fertilizer map:
0 15 37
37 52 2
39 0 15

fertilizer-to-water map:
49 53 8
0 11 42
42 0 7
57 7 4

water-to-light map:
88 18 7
18 25 70

light-to-temperature map:
45 77 23
81 45 19
68 64 13

temperature-to-humidity map:
0 69 1
1 0 69

humidity-to-location map:
60 56 37
56 93 4"
}
