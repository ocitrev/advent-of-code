package day2

import utils "../odin-utils"

import "core:fmt"

RunAndGetValue :: proc(memory: []int, result: int) -> int {
    cpu := utils.cpu_init(memory)
    defer utils.cpu_delete(&cpu)
    utils.cpu_run(&cpu)
    return cpu.memory[result]
}

RunInput :: proc(input: []int, noun: int, verb: int) -> int {
    cpu := utils.cpu_init(input)
    defer utils.cpu_delete(&cpu)
    cpu.memory[1] = noun
    cpu.memory[2] = verb
    utils.cpu_run(&cpu)
    return cpu.memory[0]
}

part1 :: proc(input: []int) -> int {
    return RunInput(input, 12, 2)
}

part2 :: proc(input: []int) -> int {
    for noun in 0..<100 {
        for verb in 0..<100 {
            if 19_690_720 == RunInput(input, noun, verb) {
                return noun * 100 + verb
            }
        }
    }

    return 0
}

main :: proc() {
    // https://adventofcode.com/2019/day/2
    fmt.println("Day 2, 2019: 1202 Program Alarm");

    assert(2 == RunAndGetValue([]int{1, 0, 0, 0, 99}, 0))
    assert(6 == RunAndGetValue([]int{2, 3, 0, 3, 99}, 3))
    assert(9801 == RunAndGetValue([]int{2, 4, 4, 5, 99, 0}, 5))
    assert(30 == RunAndGetValue([]int{1, 1, 1, 4, 99, 5, 6, 0, 99}, 0))

    input := utils.read_integers("inputs/2019/day2.txt")
    defer delete(input)

    p1 := part1(input[:])
    fmt.printfln("  Part 1: %v", p1)
    assert(3_706_713 == p1)

    p2 := part2(input[:])
    fmt.printfln("  Part 2: %v", p2)
    assert(8609 == p2)
}
