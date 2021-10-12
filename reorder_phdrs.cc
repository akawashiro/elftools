#include <getopt.h>

#include "elf_binary.h"

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);
    CHECK(optind + 1 == argc);
    std::string input = argv[optind];
    std::string output = input + ".reorderd";

    auto bin = ReadELF(input);
    int i_phdr = 1, j_phdr = 2;
    std::vector<int> new_pdhrs_order = {3, 1, 2, 0, 4, 5, 6, 7, 8, 9};
    CHECK_EQ(new_pdhrs_order.size(), bin->phdrs().size());
    std::vector<Elf_Phdr> tmp_phdrs;
    for (int i = 0; i < new_pdhrs_order.size(); i++) {
        tmp_phdrs.emplace_back(*bin->phdrs()[new_pdhrs_order[i]]);
    }
    for (int i = 0; i < new_pdhrs_order.size(); i++) {
        *bin->phdrs_mut()[i] = tmp_phdrs[i];
    }

    FILE* fp = fopen(output.c_str(), "wb");

    WriteBuf(fp, bin->head(), bin->filesize());

    return 0;
}
