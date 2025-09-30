#[path = "../utils.rs"]
mod utils;

fn main() {
    // https://adventofcode.com/2024/day/9
    println!("Day 9, 2024: Disk Fragmenter");
    let _m = utils::Monitor::start();

    let p1 = part1(get_input());
    println!("  Part 1: {}", p1);
    assert_eq!(6288707484810, p1);

    let p2 = part2(get_input());
    println!("  Part 2: {}", p2);
    assert_eq!(6311837662089, p2);
}

fn part1(input: &'static str) -> u64 {
    let mut disk = Disk::parse(input);
    disk.defrangment();
    disk.checksum()
}

fn part2(input: &'static str) -> u64 {
    let mut disk = Disk::parse(input);
    disk.defragment_whole_files();
    disk.checksum()
}

struct Block {
    start: usize,
    len: usize,
}

struct Disk {
    data: Vec<Option<u64>>,
    files: Vec<Block>,
    free_spaces: Vec<Block>,
    next_file_id: u64,
}

impl Disk {
    pub fn checksum(self: &Self) -> u64 {
        self.data
            .iter()
            .enumerate()
            .fold(0, |acc, (i, c)| acc + (i as u64) * c.unwrap_or(0))
    }

    pub fn parse(input: &'static str) -> Self {
        let mut data = Vec::<Option<u64>>::new();
        let mut files = Vec::<Block>::new();
        let mut free_spaces = Vec::<Block>::new();
        let mut next_file_id = 0;

        for (i, c) in input.bytes().enumerate() {
            let len = (c - b'0') as usize;
            let block = Block {
                start: data.len(),
                len: len,
            };

            if i & 1 == 0 {
                files.push(block);
                for _ in 0..len {
                    data.push(Some(next_file_id));
                }
                next_file_id += 1
            } else {
                free_spaces.push(block);
                for _ in 0..len {
                    data.push(None);
                }
            }
        }

        Self {
            data: data,
            files: files,
            free_spaces: free_spaces,
            next_file_id: next_file_id,
        }
    }

    pub fn defrangment(self: &mut Self) {
        let mut back = self.data.len();
        let mut front = 0;
        let len = self.data.len();

        while back > front {
            back -= 1;

            if let None = self.data[back] {
                continue;
            }

            while front < len {
                if let Some(_) = self.data[front] {
                    front += 1;
                } else {
                    break;
                }
            }

            if back > front {
                self.data[front] = self.data[back];
                self.data[back] = None;
                front += 1
            }
        }
    }

    pub fn defragment_whole_files(self: &mut Self) {
        let mut last_file = self.next_file_id;

        while last_file != 0 {
            last_file -= 1;
            let file_block = &self.files[last_file as usize];

            for free_block in &mut self.free_spaces {
                if free_block.start >= file_block.start {
                    break;
                }

                if free_block.len >= file_block.len {
                    let mut front = free_block.start;
                    let mut back = file_block.start;

                    for _ in 0..file_block.len {
                        self.data[front] = self.data[back];
                        self.data[back] = None;
                        front += 1;
                        back += 1;
                    }

                    free_block.len -= file_block.len;
                    free_block.start += file_block.len;
                    break;
                }
            }
        }
    }
}

fn get_input() -> &'static str {
    include_str!("../../inputs/2024/day9.txt").trim_end()
}

#[cfg(test)]
mod tests {
    fn get_example() -> &'static str {
        "2333133121414131402"
    }

    #[test]
    fn part1() {
        assert_eq!(1928, super::part1(get_example()));
    }

    #[test]
    fn part2() {
        assert_eq!(2858, super::part2(get_example()));
    }
}
