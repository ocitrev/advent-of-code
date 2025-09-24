const SCRIPT_DIR = path self | path dirname

def get-input-filepath [
    year:int
    day:int
] {
    let year = $year | into string
    [$SCRIPT_DIR '..' inputs $year $'day($day).txt'] | path join
}

def get-input [
    year:int
    day:int
] {
    open (get-input-filepath $year $day)
}

def split-lines [] {
    $in | str trim | split row -r '[\r\n]+'
}

def print-title [
    year:int
    day:int
    title:string
] {
    print $'Day ($day), ($year): ($title)'
}
