#include <getopt.h>

#include "elf_binary.h"

int main(int argc, char* argv[]) {
    google::InitGoogleLogging(argv[0]);

    std::string input, output, order;

    static option long_options[] = {
        {"input", required_argument, nullptr, 'i'},
        {"output", required_argument, nullptr, 'o'},
        {"order", required_argument, nullptr, 'r'},
        {0, 0, 0, 0},
    };
    int opt;
    while ((opt = getopt_long(argc, argv, "i:o:r:", long_options, nullptr)) != -1) {
        switch (opt) {
            case 'i':
                input = optarg;
                break;
            case 'o':
                output = optarg;
                break;
            case 'r':
                order = optarg;
                break;
        }
    }

    CHECK(!input.empty());
    CHECK(!output.empty());
    CHECK(!order.empty());

    std::vector<int> new_pdhrs_order;
    {
        std::stringstream ss(order);
        while (ss.good()) {
            std::string substr;
            getline(ss, substr, ',');
            new_pdhrs_order.push_back(std::stoi(substr));
        }
    }

    auto bin = ReadELF(input);
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
