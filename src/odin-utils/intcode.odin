package utils

import "core:slice"

CPU :: struct {
    ip: int,
    relative_offset: int,
    input_func: proc() -> int,
    output_func: proc(int),
    memory: [dynamic]int,
}

Mode :: enum {
    Address,
    Immediate,
    Relative,
}

OpCode :: enum {
    Add = 1,
    Mul,
    Input,
    Output,
    JumpTrue,
    JumpFalse,
    IsLess,
    IsEqual,
    SetRelativeBase,
    Halt = 99,
}

cpu_init :: proc(memory: []int, input_func: proc() -> int = nil, output_func: proc(int) = nil) -> CPU {
    return CPU{0, 0, input_func, output_func, slice.clone_to_dynamic(memory)}
}

cpu_delete :: proc(cpu: ^CPU) {
    delete(cpu.memory)
}

cpu_read_instruction :: proc(cpu: ^CPU) -> (OpCode, int) {
    defer cpu.ip += 1
    value := cpu.memory[cpu.ip]
    return cast(OpCode)(value % 100), value / 100
}

cpu_read_value :: proc(cpu: ^CPU, mode: Mode) -> int {
    defer cpu.ip += 1
    addr := cpu.memory[cpu.ip]

    if mode == .Immediate {
        return addr
    } else if mode == .Relative {
        addr += cpu.relative_offset
    }

    if addr >= len(cpu.memory) {
        return 0
    }

    return cpu.memory[addr]
}

cpu_write_value :: proc(cpu: ^CPU, mode: Mode, value: int) {
    defer cpu.ip += 1
    addr := cpu.memory[cpu.ip]

    if mode == .Immediate {
        panic("Cannot set immediate value")
    } else if mode == .Relative {
        addr += cpu.relative_offset
    }

    if addr >= len(cpu.memory) {
        resize(&cpu.memory, addr + 1)
    }

    cpu.memory[addr] = value
}

@(private)
mode_at :: #force_inline proc(modes: int, position: int) -> Mode {
    divisor := 1
    for _ in 0..<position do divisor *= 10
    return cast(Mode)((modes / divisor) % 10)
}

cpu_step :: proc(cpu: ^CPU) -> bool {
    ins, mode := cpu_read_instruction(cpu)
    switch ins {
    case .Add:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        c := a + b
        cpu_write_value(cpu, mode_at(mode, 2), c)
        return true
    case .Mul:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        c := a * b
        cpu_write_value(cpu, mode_at(mode, 2), c)
        return true
    case .Input:
        if cpu.input_func == nil do panic("input_func not set")
        a := cpu.input_func()
        cpu_write_value(cpu, mode_at(mode, 0), a)
        return true
    case .Output:
        if cpu.output_func == nil do panic("output_func not set")
        a := cpu_read_value(cpu, mode_at(mode, 0))
        cpu.output_func(a)
        return true
    case .JumpTrue:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        if a != 0 do cpu.ip = b
        return true
    case .JumpFalse:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        if a == 0 do cpu.ip = b
        return true
    case .IsLess:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        c := a < b ? 1 : 0
        cpu_write_value(cpu, mode_at(mode, 2), c)
        return true
    case .IsEqual:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        b := cpu_read_value(cpu, mode_at(mode, 1))
        c := a == b ? 1 : 0
        cpu_write_value(cpu, mode_at(mode, 2), c)
        return true
    case .SetRelativeBase:
        a := cpu_read_value(cpu, mode_at(mode, 0))
        cpu.relative_offset += a
        return true
    case .Halt:
        return false
    }

    panic("Unknown instruction")
}

cpu_run :: proc(cpu: ^CPU) {
    for cpu_step(cpu) {}
}

cpu_reset :: proc(cpu: ^CPU, memory: []int) {
    cpu.ip = 0
    cpu.relative_offset = 0
    resize(&cpu.memory, len(memory))
    copy(cpu.memory[:], memory)
}
