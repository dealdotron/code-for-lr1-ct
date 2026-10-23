#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
bool readNumber(const std::string& text, double& value) {
    std::istringstream input(text);
    if (!(input >> value)) return false;
    input >> std::ws;
    return input.eof() && std::isfinite(value);
}
int main() {
    std::string rawX, rawEps;
    std::cout << "Введите аргумент косинуса x в радианах:\n";
    std::getline(std::cin, rawX);
    std::cout << "Введите допустимую погрешность:\n";
    std::getline(std::cin, rawEps);
    std::cout << "Введённые данные: x = " << rawX
              << "; погрешность = " << rawEps << '\n';
    double x, eps;
    if (!readNumber(rawX, x)) {
        std::cout << "Ошибка: аргумент должен быть конечным числом без лишних символов.\n";
        return 1;
    }
    if (!readNumber(rawEps, eps)) {
        std::cout << "Ошибка: погрешность должна быть конечным числом без лишних символов.\n";
        return 1;
    }
    if (eps <= 0.0) {
        std::cout << "Ошибка: погрешность должна быть больше нуля.\n";
        return 1;
    }
    const double exact = std::cos(x);
    if (!std::isfinite(exact)) {
        std::cout << "Ошибка: cos(x) не удалось вычислить для этого аргумента.\n";
        return 1;
    }
    double sum = 1.0;
    double term = 1.0;
    double difference = std::fabs(exact - sum);
    int n = 1;
    int count = 1;
    const int maxTerms = 10000;
    while (difference > eps && count < maxTerms) {
        const double denominator = (2.0 * n - 1.0) * (2.0 * n);
        const double nextTerm = -term * x * x / denominator;
        if (!std::isfinite(nextTerm)) {
            std::cout << "Ошибка: член ряда переполнен, вычисление невозможно.\n";
            return 1;
        }
        const double nextSum = sum + nextTerm;
        if (!std::isfinite(nextSum)) {
            std::cout << "Ошибка: сумма ряда переполнена, вычисление невозможно.\n";
            return 1;
        }
        term = nextTerm;
        sum = nextSum;
        ++count;
        ++n;
        difference = std::fabs(exact - sum);
    }
    if (difference > eps) {
        std::cout << "Ошибка: заданная точность не достигнута за 10000 членов ряда.\n";
        return 1;
    }
    std::cout << std::fixed << std::setprecision(10);
    std::cout << "Сумма ряда: " << sum << '\n';
    std::cout << "Количество просуммированных членов: " << count << '\n';
    std::cout << "Разность с точным значением: " << difference << '\n';
    std::cout << "Точное значение cos(x): " << exact << '\n';
    return 0;
}