#include <iostream>
#include <vector>

int sum(const std::vector<int>& numbers) {
    int total = 0;
    for (int num : numbers) {
        total += num; // Set a breakpoint here
    }
    return total;
}

int main() {
    std::cout << "Enter 5 integers:\n";
    std::vector<int> values(5);
    
    for (int i = 0; i < 5; ++i) {
        std::cout << "Value " << i + 1 << ": ";
        std::cin >> values[i];
    }

    int result = sum(values);
    std::cout << "Sum of values: " << result << std::endl;

    return 0;
}

