#include "day7.hpp"

#include "../common.hpp"

#include <list>

using namespace std::literals;

struct FileSystem
{
    std::string_view name{};
    size_t size = 0;
    FileSystem *parent = nullptr;
    std::list<FileSystem> entries;

    FileSystem(std::string_view name_, size_t size_ = 0)
        : name{name_}
        , size{size_}
    {
    }

    void AddDir(std::string_view dir)
    {
        entries.emplace_back(dir).parent = this;
    }

    void AddFile(std::string_view file, size_t fileSize)
    {
        entries.emplace_back(file, fileSize).parent = this;

        for (auto *p = this; p != nullptr; p = p->parent)
        {
            p->size += fileSize;
        }
    }

    bool IsFile() const
    {
        return entries.empty();
    }
};

struct Terminal
{
    FileSystem root{{}};
    FileSystem *cwd = nullptr;

    void ChangeDir(std::string_view dir)
    {
        if (dir == ".."sv)
        {
            if (cwd->parent != nullptr)
            {
                cwd = cwd->parent;
            }
        }
        else if (dir == "/"sv)
        {
            cwd = &root;
        }
        else
        {
            auto iter = std::find_if(begin(cwd->entries), end(cwd->entries),
                [dir](FileSystem &fs)
                {
                    return fs.name == dir;
                });

            if (iter != end(cwd->entries))
            {
                cwd = std::addressof(*iter);
            }
        }
    }

    void ParseCommand(std::string_view command)
    {
        if (starts_with(command, "cd"sv))
        {
            auto const parts = Split(command, ' ');
            ChangeDir(parts[1]);
        }
        else if (starts_with(command, "ls"sv))
        {
            // fausse piste
        }
    }

    void ParseResult(std::string_view result)
    {
        auto const parts = Split(result, ' ');
        auto const name = parts[1];

        if (parts[0] == "dir"sv)
        {
            cwd->AddDir(name);
        }
        else
        {
            size_t const size = svtoi<size_t>(parts[0]);
            cwd->AddFile(name, size);
        }
    }

    void ParseTerminalLine(std::string_view line)
    {
        if (line[0] == '$')
        {
            ParseCommand(line.substr(2));
        }
        else
        {
            ParseResult(line);
        }
    }

    void ParseTerminalOutput(std::string_view text)
    {
        for (auto line : Split(text, '\n'))
        {
            ParseTerminalLine(line);
        }
    }

    static void GetAllFolders(std::vector<FileSystem const *> &out, FileSystem const &entry)
    {
        if (entry.IsFile())
        {
            return;
        }

        out.push_back(&entry);

        for (auto const &subEntry : entry.entries)
        {
            GetAllFolders(out, subEntry);
        }
    }

    std::vector<FileSystem const *> GetAllFolders() const
    {
        std::vector<FileSystem const *> folders;
        GetAllFolders(folders, root);
        return folders;
    }
};

// Find all of the directories with a total size of at most 100000
// What is the sum of the total sizes of those directories?
static auto SumOfAllSmallDirectories(std::string_view text)
{
    Terminal t;
    t.ParseTerminalOutput(text);
    auto allFolders = t.GetAllFolders();

    static constexpr size_t limit = 100'000;
    auto first = begin(allFolders);
    auto last = std::remove_if(first, end(allFolders),
        [](FileSystem const *fs)
        {
            return fs->size > limit;
        });

    return std::transform_reduce(first, last, size_t{0}, std::plus{},
        [](FileSystem const *fs)
        {
            return fs->size;
        });
}

// Find the smallest directory that, if deleted, would free up enough space on the filesystem to run the update
// What is the total size of that directory?
static auto GetSizeOfFolderToDelete(std::string_view text)
{
    Terminal t;
    t.ParseTerminalOutput(text);
    auto allFolders = t.GetAllFolders();

    static constexpr size_t capacity = 70'000'000;
    static constexpr size_t needed = 30'000'000;
    size_t const nb = needed - (capacity - t.root.size);

    auto first = begin(allFolders);
    auto last = std::remove_if(first, end(allFolders),
        [nb](FileSystem const *fs)
        {
            return fs->size < nb;
        });

    auto minIter = std::min_element(first, last,
        [](FileSystem const *a, FileSystem const *b)
        {
            return a->size < b->size;
        });

    return (*minIter)->size;
}

static auto Part1()
{
    return SumOfAllSmallDirectories(GetInput());
}

static auto Part2()
{
    return GetSizeOfFolderToDelete(GetInput());
}

int main()
{
    // https://adventofcode.com/2022/day/7
    std::print("Day 7, 2022: No Space Left On Device\n"sv);

    Assert(95'437 == SumOfAllSmallDirectories(example::terminal));
    Assert(24'933'642 == GetSizeOfFolderToDelete(example::terminal));

    auto const part1 = Part1();
    std::print("  Part 1: {}\n"sv, part1);
    Assert(1'428'881 == part1);

    auto const part2 = Part2();
    std::print("  Part 2: {}\n"sv, part2);
    Assert(10'475'598 == part2);
}
