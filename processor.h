/**
 * @file processor.h
 * @author Josh Borthick (github.com/joshb019919)
 * @brief Process and output arrival and departure events by simulating
 * @brief customers moving through a line.
 * @version 1.0
 * @date 2022-05-17
 *
 * @note I originally tried to use std::make_shared with PriorityQueue.
 * @note emplace, but as it turns out, .emplace requires a weak pointer
 * @note and creates and returns a shared pointer pointing to the same
 * @note thing as the weak pointer, but this caused certain departures
 * @note to be repeated.
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PROCESSOR_GP
#define PROCESSOR_GP

/**
 * @brief Display the time of an arrival event.
 * @brief An arrival just happened, and now the new state is being
 * @brief generated, customers moved up the line, depatures scheduled,
 * @brief etc.
 *
 * @note Pass references to the first Event pointer in the priority
 * @note queue, the priority queue, the bank line queue, the teller
 * @note availability, the current time, and the total time waited.
 *
 * @param event The next event to be processed.
 * @param eventQueue The priority queue of Events to process.
 * @param bankLine A queue to simulate customers in a line.
 * @param tellerAvailable Determines if a customer/event may be processed.
 *
 * @pre The priority queue must not be empty.
 * @post Places a new Event pointer into the priority queue and marks
 * @post the teller unavailable if it was available and the line was
 * @post empty.  Otherwise, adds a new Customer pointer to the line.
 */
void processArrival(const std::shared_ptr<Event> &event,
                    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, CustomCompare> &eventQueue,
                    std::queue<std::shared_ptr<Customer>> &bankLine,
                    bool &tellerAvailable)
{
    int currentTime{event->time};
    eventQueue.pop();
    std::cout << "Processing an arrival event at time:" << std::setw(4) << event->time << std::endl;

    // Straight to the window
    if(tellerAvailable && bankLine.empty()) {
        int departureTime{currentTime + event->transLength};
        eventQueue.push(std::make_shared<Event>('D', departureTime));
        tellerAvailable = false;

    // Back of the line
    } else {
        bankLine.push(std::make_shared<Customer>(event->time, event->transLength));
    }

} // End processArrival

/**
 * @brief Display the time of an arrival event.
 * @brief A departure just happened, and now the new state is being
 * @brief generated, customers moved up the line, wait times tallied,
 * @brief etc.
 *
 * @note Pass references to the first Event pointer in the priority
 * @note queue, the priority queue, the bank line queue, the teller
 * @note availability, the current time, and the total time waited.
 *
 * @param event The next event to be processed.
 * @param eventQueue The priority queue of Events to process.
 * @param bankLine A queue to simulate customers in a line.
 * @param tellerAvailable Determines if a customer/event may be processed.
 *
 * @pre The priority queue must not be empty.
 * @post Removes the first customer from the bank line, adds an event
 * @post to the priority queue if a customer is in the line, and makes
 * @post the teller unavailable.  Otherwise, makes teller available.
 */
void processDeparture(const std::shared_ptr<Event> &event,
                        std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, CustomCompare> &eventQueue,
                        std::queue<std::shared_ptr<Customer>> &bankLine,
                        bool &tellerAvailable)
{
    int currentTime{event->time};
    eventQueue.pop();

    std::cout << "Processing a departure event at time:" << std::setw(3) << currentTime << std::endl;

    // Line moves forward
    if(!bankLine.empty()) {
        auto customer{bankLine.front()};
        bankLine.pop();

        int timeWaited{currentTime - customer->time};
        int departureTime{currentTime + customer->transLength};

        eventQueue.push(std::make_shared<Event>('D', departureTime, 0, timeWaited));

    // Take a break
    } else {
        tellerAvailable = true;
    }

} // End processDeparture

#endif // PROCESSOR_GP
