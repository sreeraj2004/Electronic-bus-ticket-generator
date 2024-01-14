
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#define MAX_BUSES 100
#define MAX_SEATS 40
#define BUS_FILE "buses.txt"
#define BOOKING_FILE "bookings.txt"

struct Bus {
    char bus_no[10];
    char source[50];
    char destination[50];
    int fare;
    int num_seats;
    int available_seats[MAX_SEATS];
};

struct Booking {
    char bus_no[10];
    int seat_no;
    char name[50];
    char phone[15];
};

int num_buses = 0;
Bus buses[MAX_BUSES];

int num_bookings = 0;
Booking bookings[MAX_BUSES * MAX_SEATS];

void load_buses() {
    std::ifstream file(BUS_FILE);
    if (!file.is_open()) {
        return;
    }

    while (file >> buses[num_buses].bus_no >> buses[num_buses].source >> buses[num_buses].destination >> buses[num_buses].fare >> buses[num_buses].num_seats) {
        for (int i = 0; i < buses[num_buses].num_seats; i++) {
            file >> buses[num_buses].available_seats[i];
        }
        num_buses++;
    }
    file.close();
}

void save_buses() {
    std::ofstream file(BUS_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << BUS_FILE << std::endl;
        return;
    }

    for (int i = 0; i < num_buses; i++) {
        file << buses[i].bus_no << " " << buses[i].source << " " << buses[i].destination << " " << buses[i].fare << " " << buses[i].num_seats;
        for (int j = 0; j < buses[i].num_seats; j++) {
            file << " " << buses[i].available_seats[j];
        }
        file << "\n";
    }
    file.close();
}

void load_bookings() {
    std::ifstream file(BOOKING_FILE);
    if (!file.is_open()) {
        return;
    }

    while (file >> bookings[num_bookings].bus_no >> bookings[num_bookings].seat_no >> bookings[num_bookings].name >> bookings[num_bookings].phone) {
        num_bookings++;
    }
    file.close();
}

void save_bookings() {
    std::ofstream file(BOOKING_FILE);
    if (!file.is_open()) {
        std::cerr << "Error: Failed to open file " << BOOKING_FILE << std::endl;
        return;
    }

    for (int i = 0; i < num_bookings; i++) {
        file << bookings[i].bus_no << " " << bookings[i].seat_no << " " << bookings[i].name << " " << bookings[i].phone << "\n";
    }
    file.close();
}

void display_buses() {
    std::cout << "Available buses:\n";
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "| Bus No. | Source | Destination | Fare | Available Seats |\n";
    std::cout << "--------------------------------------------------------------\n";
    for (int i = 0; i < num_buses; i++) {
        std::cout << "| " << buses[i].bus_no << " | " << buses[i].source << " | " << buses[i].destination << " | " << buses[i].fare << " | ";
        for (int j = 0; j < buses[i].num_seats; j++) {
            if (buses[i].available_seats[j] == 0) {
                std::cout << "X";
            } else {
                std::cout << j + 1;
            }
        }
        std::cout << " |\n";
    }
    std::cout << "--------------------------------------------------------------\n";
}

void add_bus() {
    if (num_buses >= MAX_BUSES) {
        std::cout << "Error: Maximum number of buses reached\n";
        return;
    }
    std::cout << "Enter bus number: ";
    std::cin >> buses[num_buses].bus_no;
    std::cout << "Enter source: ";
    std::cin >> buses[num_buses].source;
    std::cout << "Enter destination: ";
    std::cin >> buses[num_buses].destination;
    std::cout << "Enter fare: ";
    std::cin >> buses[num_buses].fare;
    std::cout << "Enter number of seats: ";
    std::cin >> buses[num_buses].num_seats;
    for (int i = 0; i < buses[num_buses].num_seats; i++) {
        buses[num_buses].available_seats[i] = 1;
    }
    num_buses++;
    std::cout << "Bus added successfully\n";
}

void book_seat() {
    char bus_no[10];
    int seat_no;
    char name[50];
    char phone[15];
    std::cout << "Enter bus number: ";
    std::cin >> bus_no;
    int bus_index = -1;
    for (int i = 0; i < num_buses; i++) {
        if (strcmp(buses[i].bus_no, bus_no) == 0) {
            bus_index = i;
            break;
        }
    }
    if (bus_index == -1) {
        std::cout << "Error: Bus not found\n";
        return;
    }
    std::cout << "Enter seat number: ";
    std::cin >> seat_no;
    if (seat_no < 1 || seat_no > buses[bus_index].num_seats) {
        std::cout << "Error: Invalid seat number\n";
        return;
    }
    if (buses[bus_index].available_seats[seat_no - 1] == 0) {
        std::cout << "Error: Seat already booked\n";
        return;
    }
    std::cout << "Enter name: ";
    std::cin >> name;
    std::cout << "Enter phone number: ";
    std::cin >> phone;
    strcpy(bookings[num_bookings].bus_no, bus_no);
    bookings[num_bookings].seat_no = seat_no;
    strcpy(bookings[num_bookings].name, name);
    strcpy(bookings[num_bookings].phone, phone);
    num_bookings++;
    buses[bus_index].available_seats[seat_no - 1] = 0;
    std::cout << "Seat booked successfully\n";
}

void cancel_seat() {
    char bus_no[10];
    int seat_no;
    std::cout << "Enter bus number: ";
    std::cin >> bus_no;
    int bus_index = -1;
    for (int i = 0; i < num_buses; i++) {
        if (std::strcmp(buses[i].bus_no, bus_no) == 0) {
            bus_index = i;
            break;
        }
    }
    if (bus_index == -1) {
        std::cout << "Error: Bus not found\n";
        return;
    }
    std::cout << "Enter seat number: ";
    std::cin >> seat_no;
    if (seat_no < 1 || seat_no > buses[bus_index].num_seats) {
        std::cout << "Error: Invalid seat number\n";
        return;
    }
    if (buses[bus_index].available_seats[seat_no - 1] == 1) {
        std::cout << "Error: Seat not booked\n";
        return;
    }
    for (int i = 0; i < num_bookings; i++) {
        if (std::strcmp(bookings[i].bus_no, bus_no) == 0 && bookings[i].seat_no == seat_no) {
            for (int j = i; j < num_bookings - 1; j++) {
                bookings[j] = bookings[j + 1];
            }
            num_bookings--;
            break;
        }
    }
    buses[bus_index].available_seats[seat_no - 1] = 1;
    std::cout << "Seat cancelled successfully\n";
}

void update_bus() {
    char bus_no[10];
    std::cout << "Enter bus number: ";
    std::cin >> bus_no;
    int bus_index = -1;
    for (int i = 0; i < num_buses; i++) {
        if (std::strcmp(buses[i].bus_no, bus_no) == 0) {
            bus_index = i;
            break;
        }
    }
    if (bus_index == -1) {
        std::cout << "Error: Bus not found\n";
        return;
    }
    std::cout << "Enter new source: ";
    std::cin >> buses[bus_index].source;
    std::cout << "Enter new destination: ";
    std::cin >> buses[bus_index].destination;
    std::cout << "Enter new fare: ";
    std::cin >> buses[bus_index].fare;
    std::cout << "Bus details updated successfully\n";
}

void delete_bus() {
    char bus_no[10];
    std::cout << "Enter bus number: ";
    std::cin >> bus_no;
    int bus_index = -1;
    for (int i = 0; i < num_buses; i++) {
        if (std::strcmp(buses[i].bus_no, bus_no) == 0) {
            bus_index = i;
            break;
        }
    }
    if (bus_index == -1) {
        std::cout << "Error: Bus not found\n";
        return;
    }
    for (int i = bus_index; i < num_buses - 1; i++) {
        buses[i] = buses[i + 1];
    }
    num_buses--;
    std::cout << "Bus deleted successfully\n";
}


int main() {
    load_buses();
    load_bookings();

    while (true) {
        std::cout << "\n";
        std::cout << "1. Display available buses\n";
        std::cout << "2. Add new bus\n";
        std::cout << "3. Book seat\n";
        std::cout << "4. Cancel seat\n";
        std::cout << "5. Update bus details\n";
        std::cout << "6. Delete bus\n";
        std::cout << "7. Exit\n";
        std::cout << "Enter choice: ";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                display_buses();
                break;
            case 2:
                add_bus();
                break;
            case 3:
                book_seat();
                break;
            case 4:
                cancel_seat();
                break;
            case 5:
                update_bus();
                break;
            case 6:
                delete_bus();
                break;
            case 7:
                save_buses();
                save_bookings();
                std::cout << "Exiting program...\n";
                return 0;
            default:
                std::cout << "Error: Invalid choice\n";
        }
    }

    return 0;
}
