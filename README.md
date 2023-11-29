# filtering-cpp

**Maintainer**: Gedaliah Knizhnik - <gedaliah.knizhnik@gmail.com>

---
A header-only CPP filtering library for incoming data streams. Currently the following filters are supported:

1. Exponential filter
2. Moving average filter.

Multi-stream filtering is supported via the `multistream.hpp` header, and supports all of the above filters.

Future plans involve conan-izing this library for installation and inclusion, but for the time being simply include `filter.hpp` and `multistream.hpp`.