BUILD_DIR := "build"
REL_DIR := "build_rel"

build:
    mkdir -p {{BUILD_DIR}}
    cd {{BUILD_DIR}} && cmake .. -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DBUILD_TESTS=true && make -j $(nproc)

build-rel:
    mkdir -p {{REL_DIR}}
    cd {{REL_DIR}} && cmake .. -DCMAKE_BUILD_TYPE=Release && make -j $(nproc)

run ARGS="": build
    ./{{BUILD_DIR}}/kakod {{ARGS}}

run-rel ARGS="": build-rel
    ./{{REL_DIR}}/kakod {{ARGS}}

test-all: build
    cd {{BUILD_DIR}} && ctest --output-on-failure -j $(nproc)

test TEST: build
    cd {{BUILD_DIR}} && ctest --output-on-failure -R "^{{TEST}}$"

format:
    find src/ test/ -name '*.cpp' -o -name '*.hpp' | xargs clang-format -i

format-check:
    find src/ test/ -name '*.cpp' -o -name '*.hpp' | xargs clang-format --dry-run --Werror
