fn main() {
    // https://adventofcode.com/2016/day/10
    println!("Day 10, 2016: Balance Bots");

    let (p1, p2) = run(get_input(), 17, 61);
    println!("  Part 1: {}", p1);
    assert_eq!(113, p1);

    println!("  Part 2: {}", p2);
    assert_eq!(12803, p2);
}

struct Bin {
    bins: std::collections::HashMap<i32, Vec<i32>>,
}

impl Bin {
    pub fn new() -> Bin {
        Bin {
            bins: std::collections::HashMap::new(),
        }
    }

    fn get(&mut self, bin: i32) -> &mut Vec<i32> {
        let bins = &mut self.bins;
        if !bins.contains_key(&bin) {
            bins.insert(bin, vec![]);
        }
        bins.get_mut(&bin).unwrap()
    }

    pub fn add(&mut self, bin: i32, value: i32) {
        let chips = self.get(bin);
        chips.push(value);
        chips.sort();
    }

    pub fn low(&mut self, bin: i32) -> i32 {
        let chips = self.get(bin);
        let &value = chips.first().unwrap();
        chips.remove(0);
        value
    }

    pub fn high(&mut self, bin: i32) -> i32 {
        let chips = self.get(bin);
        chips.pop().unwrap()
    }

    pub fn ready(&self, bin: i32) -> bool {
        return if let Some(values) = self.bins.get(&bin) {
            values.len() >= 2
        } else {
            false
        };
    }

    pub fn _print(&self, name: &str) {
        for (bin, values) in &self.bins {
            eprintln!("{} {} -> {:?}", name, bin, values);
        }
    }

    pub fn remains(&self) -> bool {
        self.bins.values().map(|v| v.len()).sum::<usize>() != 0
    }

    pub fn who(&self, a: i32, b: i32) -> Option<i32> {
        for (bot, values) in &self.bins {
            if values.contains(&a) && values.contains(&b) {
                return Some(*bot);
            }
        }

        None
    }
}

fn run(input: &'static str, a: i32, b: i32) -> (i32, i64) {
    let mut bots = Bin::new();
    let mut output = Bin::new();
    let mut result: Option<i32> = None;

    for line in input.lines() {
        let mut it = line.split(' ');
        match it.next() {
            Some("value") => {
                let value: i32 = it.next().unwrap().parse().unwrap();
                _ = it.next();
                _ = it.next();
                _ = it.next();
                let dest: i32 = it.next().unwrap().parse().unwrap();
                bots.add(dest, value);
            }
            _ => {}
        }
    }

    'outer: loop {
        for line in input.lines() {
            if let None = result {
                if let Some(bot) = bots.who(a, b) {
                    result = Some(bot);
                }
            }

            if !bots.remains() {
                break 'outer;
            }

            let mut it = line.split(' ');
            match it.next() {
                Some("bot") => {
                    let source: i32 = it.next().unwrap().parse().unwrap();

                    if !bots.ready(source) {
                        continue;
                    }

                    _ = it.next();
                    _ = it.next();
                    _ = it.next();
                    let low_1 = it.next().unwrap();
                    let low_2: i32 = it.next().unwrap().parse().unwrap();
                    _ = it.next();
                    _ = it.next();
                    _ = it.next();
                    let high_1 = it.next().unwrap();
                    let high_2: i32 = it.next().unwrap().parse().unwrap();

                    let low_value = bots.low(source);
                    match low_1 {
                        "output" => output.add(low_2, low_value),
                        "bot" => bots.add(low_2, low_value),
                        _ => panic!(),
                    }

                    let high_value = bots.high(source);
                    match high_1 {
                        "output" => output.add(high_2, high_value),
                        "bot" => bots.add(high_2, high_value),
                        _ => panic!(),
                    }
                }
                _ => {}
            }
        }
    }

    let out0 = output.get(0)[0] as i64;
    let out1 = output.get(1)[0] as i64;
    let out2 = output.get(2)[0] as i64;

    (result.unwrap(), out0 * out1 * out2)
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2016/day10.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        indoc::indoc! {"
            value 5 goes to bot 2
            bot 2 gives low to bot 1 and high to bot 0
            value 3 goes to bot 1
            bot 1 gives low to output 1 and high to bot 0
            bot 0 gives low to output 2 and high to output 0
            value 2 goes to bot 2
        "}
    }

    #[test]
    fn parts() {
        assert_eq!(2, super::run(get_example(), 2, 5).0);
    }

    #[test]
    fn part2() {
        assert_eq!(30, super::run(get_example(), 2, 5).1);
    }
}
