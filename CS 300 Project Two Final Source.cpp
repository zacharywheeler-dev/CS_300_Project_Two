/*
 * Author: Zachary Wheeler
 * Course: CS 300 Data Structures and Algorithms
 * Project: Project Two - ABCU Advising Assistance Program
 */

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct Course {
    std::string courseNumber;
    std::string courseTitle;
    std::vector<std::string> prerequisites;
};

namespace {

std::string trim(const std::string& text) {
    const std::string whitespace = " \t\n\r\f\v";
    const std::size_t first = text.find_first_not_of(whitespace);
    if (first == std::string::npos) {
        return "";
    }
    const std::size_t last = text.find_last_not_of(whitespace);
    return text.substr(first, last - first + 1U);
}

std::string normalizeCourseNumber(const std::string& courseNumber) {
    std::string normalized = trim(courseNumber);
    std::transform(normalized.begin(), normalized.end(), normalized.begin(),
                   [](unsigned char character) {
                       return static_cast<char>(std::toupper(character));
                   });
    return normalized;
}

// Parses standard comma-separated fields, including doubled quotes inside quoted fields.
bool parseCsvLine(const std::string& line, std::vector<std::string>& fields,
                  std::string& errorMessage) {
    fields.clear();
    std::string field;
    bool inQuotes = false;

    for (std::size_t index = 0; index < line.size(); ++index) {
        const char character = line[index];
        if (character == '"') {
            if (inQuotes && index + 1U < line.size() && line[index + 1U] == '"') {
                field += '"';
                ++index;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (character == ',' && !inQuotes) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += character;
        }
    }

    if (inQuotes) {
        errorMessage = "unclosed quoted field";
        return false;
    }

    fields.push_back(trim(field));
    return true;
}

bool parseMenuChoice(const std::string& text, int& choice) {
    std::istringstream input(trim(text));
    char extraCharacter = '\0';
    if (!(input >> choice) || (input >> extraCharacter)) {
        return false;
    }
    return true;
}

}  // namespace

class BinarySearchTree {
public:
    BinarySearchTree() = default;
    ~BinarySearchTree() {
        clear();
    }

    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    BinarySearchTree(BinarySearchTree&& other) noexcept : root_(other.root_) {
        other.root_ = nullptr;
    }

    BinarySearchTree& operator=(BinarySearchTree&& other) noexcept {
        if (this != &other) {
            clear();
            root_ = other.root_;
            other.root_ = nullptr;
        }
        return *this;
    }

    bool insert(const Course& course) {
        Node** current = &root_;
        while (*current != nullptr) {
            const int comparison = compareCourseNumbers(course.courseNumber,
                                                        (*current)->course.courseNumber);
            if (comparison == 0) {
                return false;
            }
            current = (comparison < 0) ? &((*current)->left) : &((*current)->right);
        }
        *current = new Node(course);
        return true;
    }

    const Course* find(const std::string& courseNumber) const {
        const std::string normalized = normalizeCourseNumber(courseNumber);
        const Node* current = root_;
        while (current != nullptr) {
            const int comparison = compareCourseNumbers(normalized, current->course.courseNumber);
            if (comparison == 0) {
                return &(current->course);
            }
            current = (comparison < 0) ? current->left : current->right;
        }
        return nullptr;
    }

    bool empty() const {
        return root_ == nullptr;
    }

    void clear() {
        clearRecursive(root_);
        root_ = nullptr;
    }

    void printInOrder(std::ostream& output) const {
        printInOrderRecursive(root_, output);
    }

    void swap(BinarySearchTree& other) noexcept {
        std::swap(root_, other.root_);
    }

private:
    struct Node {
        explicit Node(const Course& value) : course(value), left(nullptr), right(nullptr) {}

        Course course;
        Node* left;
        Node* right;
    };

    Node* root_ = nullptr;

    static int compareCourseNumbers(const std::string& left, const std::string& right) {
        const std::string normalizedLeft = normalizeCourseNumber(left);
        const std::string normalizedRight = normalizeCourseNumber(right);
        if (normalizedLeft < normalizedRight) {
            return -1;
        }
        if (normalizedLeft > normalizedRight) {
            return 1;
        }
        return 0;
    }

    static void clearRecursive(Node* node) {
        if (node == nullptr) {
            return;
        }
        clearRecursive(node->left);
        clearRecursive(node->right);
        delete node;
    }

    static void printInOrderRecursive(const Node* node, std::ostream& output) {
        if (node == nullptr) {
            return;
        }
        printInOrderRecursive(node->left, output);
        output << node->course.courseNumber << ", " << node->course.courseTitle << '\n';
        printInOrderRecursive(node->right, output);
    }
};

bool readCoursesFromFile(const std::string& filename, std::vector<Course>& courses,
                         std::string& errorMessage, bool& fileOpened) {
    fileOpened = false;
    std::ifstream input(filename);
    if (!input) {
        errorMessage = "Unable to open file '" + filename + "'.";
        return false;
    }
    fileOpened = true;

    std::vector<Course> parsedCourses;
    std::vector<std::size_t> courseLineNumbers;
    std::unordered_set<std::string> courseNumbers;
    std::string line;
    std::size_t lineNumber = 0;

    while (std::getline(input, line)) {
        ++lineNumber;
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (trim(line).empty()) {
            errorMessage = "Line " + std::to_string(lineNumber) + ": blank records are not allowed.";
            return false;
        }

        std::vector<std::string> fields;
        std::string parseError;
        if (!parseCsvLine(line, fields, parseError)) {
            errorMessage = "Line " + std::to_string(lineNumber) + ": " + parseError + ".";
            return false;
        }
        if (fields.size() < 2U) {
            errorMessage = "Line " + std::to_string(lineNumber) +
                           ": expected at least a course number and title.";
            return false;
        }

        const std::string courseNumber = normalizeCourseNumber(fields[0]);
        const std::string courseTitle = trim(fields[1]);
        if (courseNumber.empty()) {
            errorMessage = "Line " + std::to_string(lineNumber) + ": course number is blank.";
            return false;
        }
        if (courseTitle.empty()) {
            errorMessage = "Line " + std::to_string(lineNumber) + ": course title is blank.";
            return false;
        }
        if (!courseNumbers.insert(courseNumber).second) {
            errorMessage = "Line " + std::to_string(lineNumber) + ": duplicate course number '" +
                           courseNumber + "'.";
            return false;
        }

        Course course;
        course.courseNumber = courseNumber;
        course.courseTitle = courseTitle;
        for (std::size_t fieldIndex = 2; fieldIndex < fields.size(); ++fieldIndex) {
            const std::string prerequisite = normalizeCourseNumber(fields[fieldIndex]);
            if (!prerequisite.empty()) {
                course.prerequisites.push_back(prerequisite);
            }
        }
        parsedCourses.push_back(std::move(course));
        courseLineNumbers.push_back(lineNumber);
    }

    if (input.bad()) {
        errorMessage = "An I/O error occurred while reading '" + filename + "'.";
        return false;
    }
    if (parsedCourses.empty()) {
        errorMessage = "The file '" + filename + "' is empty.";
        return false;
    }

    for (std::size_t courseIndex = 0; courseIndex < parsedCourses.size(); ++courseIndex) {
        const Course& course = parsedCourses[courseIndex];
        for (const std::string& prerequisite : course.prerequisites) {
            if (courseNumbers.find(prerequisite) == courseNumbers.end()) {
                errorMessage = "Line " + std::to_string(courseLineNumbers[courseIndex]) +
                               ": course '" + course.courseNumber + "' has prerequisite '" +
                               prerequisite + "', which does not appear in the file.";
                return false;
            }
        }
    }

    courses = std::move(parsedCourses);
    return true;
}

bool loadCoursesTransactionally(const std::string& filename, BinarySearchTree& tree,
                                std::string& errorMessage, bool& fileOpened) {
    std::vector<Course> courses;
    if (!readCoursesFromFile(filename, courses, errorMessage, fileOpened)) {
        return false;
    }

    BinarySearchTree replacement;
    for (const Course& course : courses) {
        // Duplicate course numbers were already rejected while validating the complete file.
        if (!replacement.insert(course)) {
            errorMessage = "Duplicate course number '" + course.courseNumber + "'.";
            return false;
        }
    }

    tree.swap(replacement);
    return true;
}

void printMenu() {
    std::cout << "\n    1. Load Data Structure.\n"
              << "    2. Print Course List.\n"
              << "    3. Print Course.\n"
              << "    9. Exit\n\n";
}

void printCourseDetails(const Course& course) {
    std::cout << course.courseNumber << ", " << course.courseTitle << '\n';
    std::cout << "Prerequisites: ";
    if (course.prerequisites.empty()) {
        std::cout << "None\n";
        return;
    }

    for (std::size_t index = 0; index < course.prerequisites.size(); ++index) {
        if (index != 0U) {
            std::cout << ", ";
        }
        std::cout << course.prerequisites[index];
    }
    std::cout << '\n';
}

bool loadWithOneReplacementPrompt(std::string& filename, BinarySearchTree& tree) {
    std::string errorMessage;
    bool fileOpened = false;
    if (loadCoursesTransactionally(filename, tree, errorMessage, fileOpened)) {
        std::cout << "Data loaded successfully.\n";
        return true;
    }

    std::cout << errorMessage << '\n';
    if (fileOpened) {
        return false;
    }

    std::cout << "Please enter a replacement filename: ";
    std::string replacementFilename;
    if (!std::getline(std::cin, replacementFilename)) {
        std::cout << "No replacement filename was provided.\n";
        return false;
    }
    replacementFilename = trim(replacementFilename);
    if (replacementFilename.empty()) {
        std::cout << "Replacement filename cannot be blank.\n";
        return false;
    }

    if (loadCoursesTransactionally(replacementFilename, tree, errorMessage, fileOpened)) {
        filename = replacementFilename;
        std::cout << "Data loaded successfully.\n";
        return true;
    }

    std::cout << errorMessage << '\n';
    return false;
}

int main(int argc, char* argv[]) {
    std::string filename = "CS-300-ABCU_Advising_Program_Input.csv";
    if (argc > 1 && argv[1] != nullptr && trim(argv[1]).size() > 0U) {
        filename = argv[1];
    }

    BinarySearchTree courseTree;
    std::cout << "Welcome to the course planner.\n";

    while (true) {
        printMenu();
        std::cout << "What would you like to do? ";

        std::string menuInput;
        if (!std::getline(std::cin, menuInput)) {
            std::cout << "\nThank you for using the course planner!\n";
            break;
        }

        int choice = 0;
        if (!parseMenuChoice(menuInput, choice)) {
            std::cout << trim(menuInput) << " is not a valid option.\n";
            continue;
        }

        switch (choice) {
            case 1:
                loadWithOneReplacementPrompt(filename, courseTree);
                break;
            case 2:
                if (courseTree.empty()) {
                    std::cout << "Please load course data before printing the course list.\n";
                } else {
                    std::cout << "Here is a sample schedule:\n\n";
                    courseTree.printInOrder(std::cout);
                }
                break;
            case 3: {
                if (courseTree.empty()) {
                    std::cout << "Please load course data before looking up a course.\n";
                    break;
                }

                std::cout << "What course do you want to know about? ";
                std::string courseNumber;
                if (!std::getline(std::cin, courseNumber)) {
                    std::cout << "\nThank you for using the course planner!\n";
                    return 0;
                }
                courseNumber = trim(courseNumber);
                if (courseNumber.empty()) {
                    std::cout << "Course number cannot be blank.\n";
                    break;
                }

                const Course* course = courseTree.find(courseNumber);
                if (course == nullptr) {
                    std::cout << "Course '" << normalizeCourseNumber(courseNumber)
                              << "' was not found.\n";
                } else {
                    printCourseDetails(*course);
                }
                break;
            }
            case 9:
                std::cout << "Thank you for using the course planner!\n";
                return 0;
            default:
                std::cout << choice << " is not a valid option.\n";
                break;
        }
    }

    return 0;
}
