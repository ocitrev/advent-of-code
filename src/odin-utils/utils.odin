package utils

import "core:strings"
import "core:strconv"

split_integers :: proc(data: string, sep: string) -> [dynamic]int {
    result: [dynamic]int
    it := string(data)
    for line in strings.split_lines_iterator(&it) {
        copy := line
        for num in strings.split_iterator(&copy, sep) {
            value, ok := strconv.parse_int(num, 10)
            if ok do append(&result, value)
        }
    }

    return result
}
