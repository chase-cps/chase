/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
*/

#pragma once

#include "utilities/IOUtils.hh"

namespace chase {

/// @brief Enumeration of supported time units. The positions of the enum
/// are used as exponent E in 10^-E when representing time (if used units are
/// not homogeneous and they need to be uniformed, this can be useful).
    enum chase_time_unit {
        CHASE_SEC,
        CHASE_MS,
        CHASE_US,
        CHASE_NS,
        CHASE_PS,
        CHASE_FS
    };

    /// @brief datatype to represent the time in chase.
    struct chase_time {
        unsigned int amount;
        chase_time_unit unit;

        explicit chase_time(
                unsigned int a = 1,
                chase_time_unit u = CHASE_SEC) :
            amount(a),
            unit(u)
        {
        }

        chase_time(
                unsigned int a,
                std::string u) :
                amount(a),
                unit(CHASE_SEC) {
            if (u == "SEC") unit = CHASE_SEC;
            else if (u == "MS") unit = CHASE_MS;
            else if (u == "US") unit = CHASE_US;
            else if (u == "NS") unit = CHASE_NS;
            else if (u == "PS") unit = CHASE_PS;
            else if (u == "FS") unit = CHASE_FS;
            else {
                messageWarning(
                        std::string("Unsupported time unit: ")+
                        u + "\n\tUsed Seconds as a default.\n");
            }
        }


        ~chase_time() = default;

    };
}