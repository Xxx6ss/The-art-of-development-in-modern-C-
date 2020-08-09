//
//  main.c
//  test
//
//  Created by Andrew Kireev on 10.07.2020.
//  Copyright © 2020 Andrew Kireev. All rights reserved.
//

#include <stdio.h>
#include <unistd.h>



void    ft_putchar(char c)
{
    write(1, &c, 1);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    char c = 'a';
    
    ft_putchar(c);
    return 0;
}
