

void    generate_random_name(void)
{
    char    *name;
    int     fd;
    int     i;

    name = ft_strdup("/tmp/.00000000000000000000");
    fd = open("/dev/random", O_RDONLY);
    i = read(fd, name + 6, 20);

    printf("[%d]\n", name);
}