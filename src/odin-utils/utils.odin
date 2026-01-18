package utils

import "core:os"
import "core:strings"
import "core:strconv"

read_integers :: proc(filepath: string) -> [dynamic]int {
    data, ok := os.read_entire_file_from_filename(filepath)
    if !ok {
        return nil
    }

    defer delete(data)
    result: [dynamic]int

    it := string(data)
    for line in strings.split_lines_iterator(&it) {
        copy := line
        for num in strings.split_iterator(&copy, ",") {
            value, ok := strconv.parse_int(num, 10)
            if ok do append(&result, value)
        }
    }

    return result
}

