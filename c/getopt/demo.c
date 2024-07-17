#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

int main(int argc, char *argv[]) {
    int c;
    int option_index = 0;

    static struct option long_options[] = {
        {"help",    no_argument,       0, 'h'},
        {"version", no_argument,       0, 'v'},
        {"output",  required_argument, 0, 'o'},
        {0,         0,                 0,  0 }
    };

    while ((c = getopt_long(argc, argv, "hvo:", long_options, &option_index)) != -1) {
        switch (c) {
            case 'h':
                printf("Usage: %s [options]\n", argv[0]);
                printf("  -h, --help       Display this help message\n");
                printf("  -v, --version    Show version information\n");
                printf("  -o, --output     Specify output file\n");
                exit(0);
            case 'v':
                printf("Version 1.0\n");
                exit(0);
            case 'o':
                printf("Output file: %s\n", optarg);
                break;
            case '?':
                // 未知选项或缺少参数
                break;
            default:
                abort();
        }
    }

    // 处理非选项参数
    if (optind < argc) {
        printf("Non-option arguments:\n");
        while (optind < argc)
            printf("%s\n", argv[optind++]);
    }

    return 0;
}
