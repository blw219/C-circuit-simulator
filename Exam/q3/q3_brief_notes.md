Notes
=====


T1 - Yanni
----------

### T1.1 is_primitive function - easy
- declare in `network.hpp`, define in `network_ops.cpp`.
- true if its resistor, capacitor or inductor

### T1.2 Component list - easy
- return components to cout
- simply identify R, L or C and the trailing value.

### T1.3  Network Comparison - medium
- Declare in `network.hpp`, define in `network_ops.cpp`
- Less than operator

### T1.4 Canonicalise - medium definitely time consuming
- Declare in `network.hpp`, define in `network_ops.cpp`
- Dependence on T1.3


T2 - Hubert
-----------

### T2.1 Writing of networks - medium, time consuming
- Translate inputs to network (object)

### T2.2 Test cases - medium
- Work in conjunction with T2

T3
--

### T3.1 Calculate Impedance - medium
- Define in `network_impedance.cpp`
- Already have definitions for resistor and capcitor

### T3.2 TF - medium
- Needs T3.1

### T3.3 Logarithmic Spacing Function - should be easy
- independent
- Define in `network_impedance.cpp`
- Should be used for transfer functions

### T3.4 Transfer Function Program
- Should be relatively trivial after working out T3.2
- Using all 3.1, 3.2, 3.3


Log
===

17:00-17:45 Read through all questions
17:50 Decided to work on questions 3 and 1.
18:10-18:22 Discussed about question 3. Divided workload. Start working.
20:00-21:00 Dinner

Day 2
- Refining canonicalise
- decided that the less-than operator should not need to sort parts (vector of networks), 
since the == operator (given by the manual) considers RLC and LRC circuits differently anyways.
