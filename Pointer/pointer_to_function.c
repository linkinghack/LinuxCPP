#include <stdio.h>

/**
 * <type> (*<func name>)(<parameters..>) can define a pointer to function
 */ 
void caller(int a, int b, int (*callback)(int)) {
    int bytes = callback(a + b);
    printf("caller: %d bytes writed.\n", bytes);
}

void caller_voidpointer(int a, int b, void* callback) {
    /**
     * Force type cast
     * [int (*)(int)] is a pointer to function
     */ 
    int bytes = ((int (*)(int))callback) (a + b);
    printf("caller_voidpointer: %d bytes writed.\n", bytes);
}

int printer(int num) {
    return printf("result: %d\n", num);
}

int main() {

    /**
     * printer, &printer just the same
     * Pass the function as a parameter
     */ 
    caller(12, 34, &printer);

    caller_voidpointer(34, 56, printer);

    /**
     * <Function name> and <&Function name> have same value:
     *  both are the address of function code block.
     */ 
    printf("function name ref: %.8x, &funcname ref: %.8x.\n", printer, &printer);
    return 0;
}