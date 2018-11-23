bake : bake.o remove_replace.o checks.o enter.o find_Bakefile.o read_args.o read_trgs.o build_target.o needs_upd.o url_date.o header.h
	cc -std=c99 -Wall -pedantic -Werror -o bake bake.o find_Bakefile.o remove_replace.o checks.o enter.o read_args.o read_trgs.o url_date.o build_target.o needs_upd.o -lm -lcurl

bake.o : bake.c header.h
	@curl-config --cflags
	cc -std=c99 -Wall -pedantic -Werror -c bake.c

find_Bakefile.o : find_Bakefile.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c find_Bakefile.c

enter.o : enter.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c enter.c

remove_replace.o : remove_replace.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c remove_replace.c

checks.o : checks.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c checks.c

read_args.o : read_args.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c read_args.c

read_trgs.o : read_trgs.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c read_trgs.c

build_target.o : build_target.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c build_target.c

needs_upd.o : needs_upd.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c needs_upd.c

url_date.o : url_date.c header.h
	cc -std=c99 -Wall -pedantic -Werror -c url_date.c
