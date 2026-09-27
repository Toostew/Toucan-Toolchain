this file outlines the design decisions for the preprocessor, as well as new concepts I encounter

the main loop:
    The primary function that handles most of the preprocessor logic loop is PreProcessor::processFile. this function handles the line-by-line execution flow,
    it's main job is going by each line, detecting the directive for the line, calling the comment-stripping handler, and finally invoking the appropriate
    directive (or non-directive) handler with that comment-stripped line. It also has the additional job of tracking the active file via the file stack (fileEntryStack). This is done to prevent
    dependency cycles during #include directives. When 2 files try to include each other, or when multiple files include the same file, or if a file includes itself,
    it could cause an infinite loop. the stack ensures that only one instance of a file can be processed on the stack. any time when the same file
    is included when it's already on the stack, the program errors out. Define Directives are handled by identifying keys and values using #define, and storing the
    value in a MacroTable Class, within a unordered_map.


the Include, Define and undefine Handlers:
   if you notice, all 3 handlers take arguments without quotations, and this is simply because of a simplification trade off:
    the whole reason there's a need for it is to allow for special names that would otherwise wouldn't be supported. Take for
    example #include. If I had #include "file name.txt", the #include should be able to parse the file name as file name.txt, which
    is expected behaviour and you can see this in g++. however, because I am using istringstream, which conveniently splits
    strings into distinct "chunks" seperated by whitespace, implementing this would be a bit of a headache, since it will split text
    even when bounded by quotations. So the choice was either implement a way for whitespace to be ignored when bounded within quotations,
    like "file name.txt", or just ban spaces outright; which is what I went for. Since spaces aren't allowed there really isn't any need
    for quotations anymore. I do however plan to return and maybe reimplement this so you COULD allow whitespace, which means wrapping the
    argument in quotations, but that's for another time.

Define handler special edge cases (and solution):
    There is an edge case that can occur if 2 or more defines are issued. Currently, macros are expanded sequentially per line. what this means
    is that each macro is checked individually for every line. This can cause a very specific issue wherein, a macro that was successfully detected and expanded,
    results in an expanded macro that contains a valid pattern in subsequent macros. This triggers the macro expansion logic for later macros, resulting
    in unpredictable behaviour. I've thought up a way to counter this. We'd use a DS to store "protected regions" of a line, or regions on a line
    that have already experienced a macro expansion. Subsequent expansions cannot occur within these regions. This solves the initial problem but raises
    a new one: if a subsequent macro is expanded, and if the length of the expanded macro doesn't perfectly match the original, un-expanded one, protected regions
    behind this macro will be right or left-shifted, resulting in inaccurate protected regions and thus, again, unpredictable results.
    We solve this by figuring out the difference in length between the expanded macro and it's initial (Value vs Key), surely, any protected
    regions that are behind this expanded macro can be shifted correctly into place.
    NOTE: I haven't implemented this yet so the current preprocessor still has this bug


std::vector<std::string> MacroTable::getMacros():
    this function irks me because I NEED to get a list of every single key from the macrotable in order to expand macros during preprocessing.
    In order to keep the hashmap (unordered map) private I needed to expose a function that returns a list of every macro,
     this function, when invoked will linearly catalog every single pair, and record pair.first
    into a vector to be returned. This cataloging occurs EVERY TIME THE FUNCTION IS CALLED, so it's wildly inefficient. ideally, you'd want to save the keys
    and have them persist in some way so that the lookup is instant O(1) and you dont have to constantly catalog them again. For now, I don't know how to do that

Linux vs Windows endline (LF vs CRLF):
    ran into a bizarre issue where the preprocessor was acting up because of hidden characters. Turns out Windows and Linux handle line breaks completely differently.
    Windows uses Carriage Return Line Feed, CRLF (\r\n), while Linux only uses Line Feed LF (\n). When you read a Windows-created file in a Linux environment,
    the parser sees the leftover carriage return (\r) and interprets it as a "^M" at the end of lines. This completely breaks string comparisons and parsers.
    this broke some of the preprocessor logic especially the macro expander function, best way to prevent this is to keep reading and writing to files strict to one environment,
    this case linux.

Error codes:
    So far I've implemented some rudimentary error codes for debugging, they are:
    10: Cycle detected, specifically when a #include directive detects the same file within the file stack
    20: Unable to open file for a variety of reasons, File doesnt exist, file not readable, or inaccessible 



New concepts:

cpp smart pointers: "oh my god"
cpp introduces smart pointers, which are an evolution of the raw pointers seen in C. unlike raw pointers, where
ownership is purely a convention the programmer has to remember and enforce manually, smart pointers use RAII
(Resource Acquisition Is Initialization) to enforce "ownership" automatically. The smart pointer's destructor
runs automatically the moment it goes out of scope, and that destructor is responsible for cleaning up
(deleting) whatever it owns. "ownership" here means "who's responsible for this data's lifetime. Specifically,
ensuring it gets cleaned up exactly once, at the right time."

unique_ptr specifically enforces EXCLUSIVE ownership. It cannot be copied (only moved), because allowing a copy
would mean two owners both believing they're responsible for cleanup, risking something called a double-free.
(when 2 owners both try to free the same resource, when only 1 free is needed) moving instead
transfers ownership cleanly. There's still only ever one owner at any moment, it just changes hands.



iterators:
an iterator is an object that represents a POSITION inside a container. It doesn't hold the data itself,
it just marks "where you are" and lets you move around/access whatevers at that position. think of it
like a bookmark in a book; it doesn't contain the pages, it just marks where you are in it.

.end() - NOT the last real element. it's a conceptual "one past the last element" position, used purely
as a sentinel to mean "you've gone past everything real / found nothing." comparing your iterator against
.end() is the idiomatic way to check if a search actually found something:

    auto it = someMap.find(key);
    if (it != someMap.end()) {
        // found it
    } else {
        // not found
    }

->first and ->second:
dereferencing a map iterator gives you a std::pair<key, value>. .first is the key, .second is the value.
since the iterator behaves like a pointer, you access these with -> (same pattern as currentEntry->fileStream
from the fileEntry pointer stuff - dereference + member access in one operator).

in short they're the general, uniform way the STL lets you walk through ANY
container (vectors, maps, sets, etc) with the same syntax, regardless of how that container's actually laid
out in memory. range-based for loops are secretly using iterators under the hood too, just hidden from view.

