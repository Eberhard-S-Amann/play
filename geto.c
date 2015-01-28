#include <unistd.h>
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

static int verbose;

static struct option long_options[] = {
	{"add",     required_argument, 0,  'a' },
	{"append",  no_argument,       0,  0 },
	{"delete",  required_argument, 0,  0 },
	{"verbose", optional_argument, &verbose,  1 },
	{"brief",   optional_argument, &verbose,  2 },
	{"create",  required_argument, 0, 'c'},
	{"file",    required_argument, 0,  0 },
	{0,         0,                 0,  0 }
};

int main(int argc, char **argv)
{
	int c;

	while (1) {
		int option_index = 0;
		c = getopt_long(argc, argv, "abc:d:012",
                        long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
        case 0:
            printf("long option <%s>", long_options[option_index].name);
            if (optarg)
                printf(" with arg <%s>", optarg);
            if (long_options[option_index].flag != 0)
                printf("Fag is set Verbose = %d\n", verbose);
            printf("\n");
            break;
        case '0':
        case '1':
        case '2':
            printf("option %c\n", c);
            break;
        case 'a':
            printf("short option <a>\n");
            printf("option a with value '%s'\n", optarg);
            break;
        case 'b':
            printf("option b\n");
            break;
        case 'c':
            printf("option c with value '%s'\n", optarg);
            break;
        case 'd':
            printf("option d with value '%s'\n", optarg);
            break;
        case '?':
            printf("HELP\n");
            break;
        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }
                                                                                                                     if (optind < argc) {
        printf("non-option ARGV-elements:\n");
        while (optind < argc)
             printf("Option %d <%s>\n", optind, argv[optind++]);
    }
    printf("Verbose = %d at Exit\n", verbose);
    exit(0);
}
