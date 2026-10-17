#include <iostream>
#include <iomanip>

// store the capacitor's data arrays and capacitance.
struct _capacitor
{
    double *time;
    double *voltage;
    double *current;
    double C;
};

typedef struct _capacitor Capacitor;

int main()
{
    // put settings in seconds, ohms, farads, and amperes.
    const double dt = 1e-10;
    const int steps = 50000;
    const int size = steps + 1;

    const double R = 1000.0;
    const double capacitance = 100e-12;
    const double sourceCurrent = 1e-2;
    const double sourceVoltage = 10.0;

    // store the results of each circuit separately.
    Capacitor constantCurrent{};
    Capacitor constantVoltage{};

    constantCurrent.C = capacitance;
    constantVoltage.C = capacitance;

    // allocate arrays, including space for the initial conditions.
    constantCurrent.time = new double[size];
    constantCurrent.voltage = new double[size];
    constantCurrent.current = new double[size];

    constantVoltage.time = new double[size];
    constantVoltage.voltage = new double[size];
    constantVoltage.current = new double[size];

    // initial conditions for the constantcurrent circuit.
    constantCurrent.time[0] = 0.0;
    constantCurrent.voltage[0] = 0.0;
    constantCurrent.current[0] = sourceCurrent;

    // initial conditions for the constantvoltage circuit.
    constantVoltage.time[0] = 0.0;
    constantVoltage.voltage[0] = 0.0;
    constantVoltage.current[0] = sourceVoltage / R;

    // calculate both circuits, starting after the initial conditions.
    for (int i = 1; i <= steps; i++)
    {
        // constantcurrent circuit: current stays fixed.
        constantCurrent.time[i] = i * dt;
        constantCurrent.current[i] = sourceCurrent;

        constantCurrent.voltage[i] =
            constantCurrent.voltage[i - 1]
            + constantCurrent.current[i - 1] * dt / constantCurrent.C;

        // constantvoltage circuit: current decreases over time.
        constantVoltage.time[i] = i * dt;

        constantVoltage.current[i] =
            constantVoltage.current[i - 1]
            - constantVoltage.current[i - 1] * dt
              / (R * constantVoltage.C);

        // supply voltage equals resistor voltage plus capacitor voltage.
        constantVoltage.voltage[i] =
            sourceVoltage - constantVoltage.current[i] * R;
    }

    //  scientific notation so small values remain readable.
    std::cout << std::scientific << std::setprecision(6);

    // print constantcurrent results every 200 time steps.
    std::cout << "\nCONSTANT-CURRENT SOURCE\n";
    std::cout << std::setw(10) << "Step"
              << std::setw(18) << "Time (s)"
              << std::setw(18) << "Voltage (V)"
              << std::setw(18) << "Current (A)"
              << '\n';

    for (int i = 0; i <= steps; i += 200)
    {
        std::cout << std::setw(10) << i
                  << std::setw(18) << constantCurrent.time[i]
                  << std::setw(18) << constantCurrent.voltage[i]
                  << std::setw(18) << constantCurrent.current[i]
                  << '\n';
    }

    // print constantvoltage results every 200 time steps.
    std::cout << "\nCONSTANT-VOLTAGE SOURCE WITH RESISTOR\n";
    std::cout << std::setw(10) << "Step"
              << std::setw(18) << "Time (s)"
              << std::setw(18) << "Voltage (V)"
              << std::setw(18) << "Current (A)"
              << '\n';

    for (int i = 0; i <= steps; i += 200)
    {
        std::cout << std::setw(10) << i
                  << std::setw(18) << constantVoltage.time[i]
                  << std::setw(18) << constantVoltage.voltage[i]
                  << std::setw(18) << constantVoltage.current[i]
                  << '\n';
    }

    // release all dynamically allocated arrays.
    delete[] constantCurrent.time;
    delete[] constantCurrent.voltage;
    delete[] constantCurrent.current;

    delete[] constantVoltage.time;
    delete[] constantVoltage.voltage;
    delete[] constantVoltage.current;

    return 0;
}
