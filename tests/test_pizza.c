#include "../src/pizza.c"
#include <assert.h>
#include <stdio.h>
#include <string.h>

void test_random_name() {
    printf("Testing random name generation... ");
    const char* name = get_random_pizza_name();
    assert(name != NULL);
    assert(strlen(name) > 0);
    printf("PASSED\n");
}

void test_random_ingredients() {
    printf("Testing ingredient generation... ");
    char buffer[512];
    get_random_pizza_ingredients(buffer, 512);
    assert(strlen(buffer) > 0);
    printf("PASSED\n");
}

void test_parallax() {
    printf("Testing parallax calculation... ");
    float p1 = calculate_parallax(0, 0);
    float p2 = calculate_parallax(1250, 0);
    // sin(0) = 0, sin(1) = 0.84
    assert(p1 >= -1.0 && p1 <= 1.0);
    assert(p2 >= -1.0 && p2 <= 1.0);
    printf("PASSED\n");
}

int main() {
    printf("Running Native C Tests...\n");
    test_random_name();
    test_random_ingredients();
    test_parallax();
    printf("All tests passed successfully!\n");
    return 0;
}
