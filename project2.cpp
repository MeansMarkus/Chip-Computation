#include <iostream>
#include <string>

using namespace std;

// Chip class representing a basic circuit chip
class Chip {
private:
    char chipType; // Type of the chip (e.g., I: Input, O: Output, A: Addition, etc.)
    string id; // Unique identifier for the chip
    Chip* input1; // Pointer to the first input chip
    Chip* input2; // Pointer to the second input chip 
    Chip* output; // Pointer to the output chip 
    double inputValue; // Value for input chips

public:
    // Constructors
    Chip(); // Default constructor
    Chip(char type, const string& id); // Parameterized constructor

    // Get and Set methods for chip attributes
    string getId() const; // Returns the chip's ID
    char getChipper() const; // Returns the chip's type
    Chip* getInput1() const; // Returns pointer to the first input chip
    Chip* getInput2() const; // Returns pointer to the second input chip
    double getInputValue() const; // Returns the input value

    // Set methods for chip inputs and outputs
    void setInput1(Chip* inputChip); // Sets the first input chip
    void setInput2(Chip* inputChip); // Sets the second input chip
    void setOutput(Chip* outputChip); // Sets the output chip
    void setInputValue(double value); // Sets the input value

    void compute(); // Computes the chip's output value based on its type and inputs
    void display() const; // Displays the chip's information

    ~Chip(); // Destructor
};

// Default constructor initializes attributes to default values
Chip::Chip() 
    : chipType(' '), id(" "), input1(nullptr), input2(nullptr), output(nullptr), inputValue(0.0) {}

// Parameterized constructor initializes chip with type and ID
Chip::Chip(char type, const string& identification) 
    : chipType(type), id(identification), input1(nullptr), input2(nullptr), output(nullptr), inputValue(0.0) {}

// Setters for input chips
void Chip::setInput1(Chip* inputChip) { input1 = inputChip; }
void Chip::setInput2(Chip* inputChip) { input2 = inputChip; }
void Chip::setOutput(Chip* outputChip) { output = outputChip; }
void Chip::setInputValue(double value) { inputValue = value; }

// Getters for chip attributes
string Chip::getId() const { return id; }
char Chip::getChipper() const { return chipType; }
double Chip::getInputValue() const { return inputValue; }
Chip* Chip::getInput1() const { return input1; }
Chip* Chip::getInput2() const { return input2; }

// Compute function performs calculations based on chip type
void Chip::compute() {
    // If the chip is an input type, we don't need to do any computation
    if (chipType == 'I') {
        return; // Simply return if this is an input chip
    }

    // Flags to check if input chips are of type Input
    bool input1Chipper = input1 && (input1->getChipper() == 'I');
    bool input2Chipper = input2 && (input2->getChipper() == 'I');

    // Perform calculations based on chip type
    switch (chipType) {
        case 'O': // Output chip
            cout << "Computation Starts" << endl; // Indicate computation start
            input1->compute(); // Compute the first input
            cout << "The output value from this circuit is " << inputValue << endl; // Display output value
            break;

        case 'A': // Addition chip
            input1->compute(); // Compute the first input
            if (input2) { // Check if the second input exists
                input2->compute(); // Compute the second input
            }
            // Calculate and set the input value as the sum of inputs
            setInputValue(input1->getInputValue() + (input2 ? input2->getInputValue() : 0));
            break;

        case 'S': // Subtraction chip
            input1->compute(); // Compute the first input
            if (input2) { // Check if the second input exists
                input2->compute(); // Compute the second input
            }
            // Calculate and set the input value as the difference of inputs
            setInputValue(input1->getInputValue() - (input2 ? input2->getInputValue() : 0));
            break;

        case 'M': // Multiplication chip
            input1->compute(); // Compute the first input
            if (input2) { // Check if the second input exists
                input2->compute(); // Compute the second input
            }
            // Calculate and set the input value as the product of inputs
            setInputValue(input1->getInputValue() * (input2 ? input2->getInputValue() : 1));
            break;

        case 'D': // Division chip
            input1->compute(); // Compute the first input
            if (input2) { // Check if the second input exists
                input2->compute(); // Compute the second input
                if (input2->getInputValue() != 0) { // Check for division by zero
                    setInputValue(input1->getInputValue() / input2->getInputValue());
                } else {
                    cout << "Can't divide by 0" << endl; // Error message for division by zero
                }
            }
            break;

        case 'N': // Not chip
            input1->compute(); // Compute the first input
            setInputValue(-input1->getInputValue()); // Set input value to negation of the first input
            break;

        default: // For Input chips, do nothing
            break;
    }

    // Set the output if this chip has an output chip
    if (output && output->getChipper() == 'O') {
        output->setInputValue(inputValue); // Pass the computed value to the output chip
    }
}

// Display method to show chip's connections and values
void Chip::display() const {
    switch (chipType) {
        case 'I': // Input chip
            cout << chipType << id << ", Output = " << (output ? output->getChipper() + output->getId() : "None") 
                 << endl; // Show value
            break;

        case 'O': // Output chip
            cout << chipType << id << ", Input 1 = " << (input1 ? input1->getChipper() + input1->getId() : "None") << endl;
            break;

        default: // For other chip types (A, S, M, D, N)
            cout << chipType << id << ", Input 1 = " << (input1 ? input1->getChipper() + input1->getId() : "None")
                 << ", Input 2 = " << (input2 ? input2->getChipper() + input2->getId() : "None")
                 << ", Output = " << (output ? output->getChipper() + output->getId() : "None") << endl;
            break;
    }
}

// Destructor for cleanup (currently no dynamic memory to free)
Chip::~Chip() {}

// Main function to execute the chip circuit simulation
int main() {
    int numChips; // Total number of chips to create
    cin >> numChips; // Input for the number of chips

    Chip** chipArray = new Chip*[numChips]; // Array to hold pointers to Chip objects
    Chip* outputChip = nullptr; // Pointer for the output chip
    bool outputChipAssigned = false; // Flag to track if the output chip has been assigned

    // Create Chip objects based on user input
    for (int i = 0; i < numChips; i++) {
        string chipInfo; // Variable to hold chip type and ID
        cin >> chipInfo; // Read chip type and ID

        char type = chipInfo[0]; // Determine the type of chip
        string id = chipInfo.substr(1); // Extract the chip ID
        Chip* newChip = new Chip(type, id); // Create a new chip instance

        // Store the chip in the array
        if (type == 'O') {
            outputChip = newChip; // Assign output chip
            outputChipAssigned = true; // Set flag
        } else {
            if (!outputChipAssigned) {
                chipArray[i] = newChip; // Store in the array if output chip not yet assigned
            } else {
                chipArray[i - 1] = newChip; // Shift previous chips one index back to accommodate output chip
            }
        }
    }

    // Place the output chip in the last position of the array
    if (outputChip) {
        chipArray[numChips - 1] = outputChip;
    }

    int numCommands; // Number of commands to process
    cin >> numCommands; // Input the number of commands

    // Process commands to establish connections and perform calculations
    for (int i = 0; i < numCommands; i++) {
        char command; // Variable for command type
        cin >> command; // Read the command

        if (command == 'I') { // Command for assigning values to input chips
            string inputChipId; // Identifier for the input chip
            double inputValue; // Value to assign
            cin >> inputChipId >> inputValue; // Read chip ID and value

            // Set the input value for the appropriate chip
            for (int j = 0; j < numChips; j++) {
                if (chipArray[j]->getChipper() == inputChipId[0] && chipArray[j]->getId() == inputChipId.substr(1)) {
                    chipArray[j]->setInputValue(inputValue); // Assign the input value
                    break; // Exit loop after assignment
                }
            }
        } else if (command == 'O') { // Command for executing output chip's computation
            string outputChipId; // Identifier for the output chip
            cin >> outputChipId; // Read the output chip ID

            // Find the output chip and execute its computation
            for (int j = 0; j < numChips; j++) {
                if (chipArray[j]->getChipper() == outputChipId[0]) {
                    chipArray[j]->compute(); // Perform the computation
                    break; // Exit loop after computation
                }
            }
        } else { // Command for establishing connections ('A' command)
            string firstChipId, secondChipId; // Identifiers for the chips to connect
            cin >> firstChipId >> secondChipId; // Read the two chip IDs

            int firstChipIndex = -1, secondChipIndex = -1; // Indices to locate the chips

            // Find the indices for both specified chips
            for (int j = 0; j < numChips; j++) {
                if (chipArray[j]->getChipper() == firstChipId[0] && chipArray[j]->getId() == firstChipId.substr(1)) {
                    firstChipIndex = j; // Store index for the first chip
                }
                if (chipArray[j]->getChipper() == secondChipId[0] && chipArray[j]->getId() == secondChipId.substr(1)) {
                    secondChipIndex = j; // Store index for the second chip
                }
                // Exit loop if both indices are found
                if (firstChipIndex != -1 && secondChipIndex != -1) {
                    break;
                }
            }

            // Establish connections if both chips are found
            if (firstChipIndex != -1 && secondChipIndex != -1) {
                if (chipArray[secondChipIndex]->getInput1() == nullptr) {
                    chipArray[secondChipIndex]->setInput1(chipArray[firstChipIndex]); // Set first input connection
                } else if (chipArray[secondChipIndex]->getInput2() == nullptr) {
                    chipArray[secondChipIndex]->setInput2(chipArray[firstChipIndex]); // Set second input connection
                }
                chipArray[firstChipIndex]->setOutput(chipArray[secondChipIndex]); // Set output connection
            }
        }
    }

    // Output established connections and chip details
    cout << "***** Showing the connections that were established" << endl;
    for (int i = 0; i < numChips; i++) {
        chipArray[i]->display(); // Display information for each chip
    }

    delete[] chipArray; // Free the array of chip pointers
    delete outputChip; // Free the output chip memory

    return 0; 
}

/* LLM and GitHub usage

 Prompts and Rationale

One prompt i used was, "Suggest a structure for setting up input and output connections "
This helped me define how to connect the chips to eachother, and it led to the development of the setter methods. However, this is already something that i had in mind and it was more for just reassurance. 

Another prompt i used was "Why might my outputs be none?".
This helped me figure out that my  chips were not being connected properly, and that the output was not being set. This led to the me revising my coneection for the chips and getting it correct for my display method.

Lastly, a prompt that helped me was, "Can you suggest an efficient way to manage dynamic memory for my chips?" 
This led me to look at my use of dynamic memory allocation and ensure that I was managing it properly.

Development

Github Copilot is very useful in getting me a basis for the methods like the display method and changing it to fit my needs. 
It is also very helpful when wanting to ensure that something is parsing through correctly by giving it a code block and  asking it to explain it back to me on how it works.
It also helps a lot when wanting to have it make a duplicate class to see how they did it in comparison to me. 

Debugging and Testing

The first test i did was to make sure my getters returned the right values. 
This helped ensure that the values i am getting are correct for my program. 

The second test i did was make sure my setters update the values.
This  helped ensure that the values i am setting are being updated correctly for my program, and i did this for both valid chips and null chips.

The third test i did was to make sure my arithmatic operations work the right way.
This made sure that might output was correct, and helped make sure that it worked with basic arithmatic rules.

Issues and Resolutions

I ran into issues regarding assigning values to input chips. 
I resolved this by putting print statements in sections of my code to determine what is printing 
at certain parts.
I would also comment out parts of code to make  sure that it was not the issue. 


Verification

 To verify that my classes worked correctly, i would use small test cases just doign input assingning and output. 
 I would also use print statements to check my values and outputs at a particular part in the code. 
 
Another thing i would do is make sure that it follows the original structure given to me in the project manual. 

*/


