def main [] {
  for year in 2015..2024 {
    run $year
  }
}

def "main 2015" [] { run 2015 }
def "main 2016" [] { run 2016 }
def "main 2017" [] { run 2017 }
def "main 2018" [] { run 2018 }
def "main 2019" [] { run 2019 }
def "main 2020" [] { run 2020 }
def "main 2021" [] { run 2021 }
def "main 2022" [] { run 2022 }
def "main 2023" [] { run 2023 }
def "main 2024" [] { run 2024 }

def run [year: int] {
  let path = [src $year 'day[0-9]*.nu'] | str join /
  for file in (glob $path) {
    nu $file
  }
}
