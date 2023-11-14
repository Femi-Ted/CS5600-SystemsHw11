#include <iostream>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

class MachineLearningWorkload {
public:
    MachineLearningWorkload(uint64_t virtualAddressSpaceSize, uint64_t segmentSize)
        : currentAddress(4 * segmentSize), virtualAddressSpaceSize(virtualAddressSpaceSize), segmentSize(segmentSize) {
        generateAddresses(); // Generate addresses at initialization
    }

    std::vector<std::string> generateWorkload(int size) {
        std::vector<std::string> addresses;
        for (int i = 0; i < size; ++i) {
            addresses.push_back(getNextSequentialAddress());
        }
        return addresses;
    }

private:
    uint64_t currentAddress;
    uint64_t virtualAddressSpaceSize;
    uint64_t segmentSize;
    std::vector<std::string> addresses; // To store generated addresses

    void generateAddresses() {
        for (uint64_t addr = 4 * segmentSize; addr < virtualAddressSpaceSize; addr += segmentSize) {
            std::stringstream stream;
            stream << "0x" << std::setw(8) << std::setfill('0') << std::hex << addr;
            addresses.push_back(stream.str());
        }
    }

    std::string getNextSequentialAddress() {
        if (currentAddress >= virtualAddressSpaceSize) {
            currentAddress = 4 * segmentSize; // Reset to start after the reserved segments
        }

        // Generate address with virtual segment and offset
        std::stringstream stream;
        stream << "0x" << std::setw(8) << std::setfill('0') << std::hex << currentAddress;
        currentAddress += segmentSize;

        return stream.str();
    }
};

class AAAGamesWorkload {
public:
    AAAGamesWorkload(int numAssets, double reuseProbability, const std::vector<uint64_t>& segmentSizes)
        : numAssets(numAssets), reuseProbability(reuseProbability), segmentSizes(segmentSizes) {
        srand(static_cast<unsigned>(time(0))); // Seed the random number generator
        assets.reserve(numAssets);
        for (int i = 0; i < numAssets; ++i) {
            assets.push_back(generateAssetAddress(i));
        }
    }

    std::vector<std::string> generateWorkload(int size) {
        std::vector<std::string> addresses;
        for (int i = 0; i < size; ++i) {
            addresses.push_back(generateFullHexAddress());
        }
        return addresses;
    }

private:
    int numAssets;
    double reuseProbability;
    std::vector<uint64_t> segmentSizes;
    std::vector<std::string> assets;

    std::string generateAssetAddress(int index) {
        // Choose a random segment and offset within that segment
        uint64_t segmentIndex = rand() % segmentSizes.size();
        uint64_t offset = rand() % segmentSizes[segmentIndex];

        // Generate the full hexadecimal address
        std::stringstream stream;
        stream << "0x" << std::setw(8) << std::setfill('0') << std::hex << (segmentIndex * segmentSizes[segmentIndex] + offset);
        return stream.str();
    }

    std::string generateFullHexAddress() {
        // Choose a random asset or reuse an existing one
        if (static_cast<double>(rand()) / RAND_MAX < reuseProbability && !addresses.empty()) {
            return addresses[rand() % addresses.size()];
        } else {
            return generateAssetAddress(assets.size());
        }
    }

    std::vector<std::string> addresses; // To store generated addresses and avoid loops
};

// ...

class StatelessMicroserviceWorkload {
public:
    StatelessMicroserviceWorkload(int numServices, double dependencyProbability, uint64_t segmentSize)
        : numServices(numServices), dependencyProbability(dependencyProbability), segmentSize(segmentSize), currentSegment(0) {
        srand(static_cast<unsigned>(time(0))); // Seed the random number generator
        services.reserve(numServices);
        generateAddresses(); // Generate addresses at initialization
    }

    std::vector<std::string> generateWorkload(int size) {
        std::vector<std::string> addresses;
        for (int i = 0; i < size; ++i) {
            addresses.push_back(getNextSequentialAddress());
        }
        return addresses;
    }

private:
    int numServices;
    double dependencyProbability;
    uint64_t segmentSize;
    int currentSegment;
    std::vector<std::string> services;

    void generateAddresses() {
        for (int i = 0; i < numServices; ++i) {
            services.push_back(generateSequentialAddress());
        }
    }

    std::string generateServiceAddress(int index) {
        // Generate address with sequential segment and offset
        std::stringstream stream;
        stream << "0x" << std::setw(8) << std::setfill('0') << std::hex << (currentSegment * segmentSize + index);
        return stream.str();
    }

    std::string getNextSequentialAddress() {
        if (++currentSegment >= numServices) {
            currentSegment = 0; // Reset to start after the last segment
        }

        return generateServiceAddress(services.size() - numServices + currentSegment);
    }

    std::string generateSequentialAddress() {
        // Generate address with sequential segment and offset
        std::stringstream stream;
        stream << "0x" << std::setw(8) << std::setfill('0') << std::hex << (currentSegment * segmentSize + services.size());
        return stream.str();
    }
};

// ...

int main() {
    // Example usage:
    MachineLearningWorkload mlWorkload(1ULL << 32, 0x1000); // 32-bit virtual address space, 4k segment size
    std::vector<std::string> mlAddresses = mlWorkload.generateWorkload(5);

    AAAGamesWorkload aaaGamesWorkload(20, 0.2, {0x800, 0x1000, 0x2000}); // Variable-sized segments
    std::vector<std::string> aaaGamesAddresses = aaaGamesWorkload.generateWorkload(5);

    StatelessMicroserviceWorkload microserviceWorkload(10, 0.3, 0x800); // Fixed-sized segments
    std::vector<std::string> microserviceAddresses = microserviceWorkload.generateWorkload(5);

    // Print the first few addresses for each workload
    std::cout << "Machine Learning Workload:";
    for (const auto& address : mlAddresses) {
        std::cout << " " << address;
    }
    std::cout << std::endl;

    std::cout << "AAA Games Workload:";
    for (const auto& address : aaaGamesAddresses) {
        std::cout << " " << address;
    }
    std::cout << std::endl;

    std::cout << "Stateless Microservice Workload:";
    for (const auto& address : microserviceAddresses) {
        std::cout << " " << address;
    }
    std::cout << std::endl;

    return 0;
}
