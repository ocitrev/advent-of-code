const mem = @import("std").mem;

pub const Parser = struct {
    input: []const u8,

    pub fn init(input: []const u8) Parser {
        return Parser{ .input = input };
    }

    pub fn nextScalar(self: *@This(), delimiter: u8) ?[]const u8 {
        var it = mem.tokenizeScalar(u8, self.input, delimiter);
        if (it.next()) |result| {
            self.input = it.rest();
            return result;
        }
        return null;
    }

    pub fn nextAny(self: *@This(), delimiter: []const u8) ?[]const u8 {
        var it = mem.tokenizeAny(u8, self.input, delimiter);
        if (it.next()) |result| {
            self.input = it.rest();
            return result;
        }
        return null;
    }

    pub fn rest(self: *@This()) []const u8 {
        return self.input;
    }
};
