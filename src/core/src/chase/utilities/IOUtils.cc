/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        2019
 * @copyright   Copyright (c) 2019 by University of Verona.
 *              Copyright (c) 2019 by Singapore University of Technology and Design.
 *              All rights reserved.
 *              This project is released under the 3-Clause BSD License.
 *
 */

#include "utilities/IOUtils.hh"

namespace chase{

    /// @brief Implementation of the baseMessage function.
    void baseMessage(
            const char * msg,
            ChaseObject * object,
            std::ostream &str )
    {
        time_t now = time(NULL);

        char * t = strtok(ctime(&now), "\n");
        str <<  t << " || ";

        str << msg << std::endl;
        if( object != nullptr )
        {
            str << 
                "---------------------- Object --------------------" << std::endl <<
                // object->getString() << std:endl <<
                "--------------------------------------------------" << std::endl;
        }
    }

    void messageError(
            const char * msg, ChaseObject * object )
    {
        std::cerr << "ERROR:  \t";
        baseMessage(msg, object, std::cerr);
        exit(-1);
    }

    void messageInfo(
            const char * msg, ChaseObject * object )
    {
        std::cout << "INFO:\t";
        baseMessage(msg, object, std::cout);
    }

    void messageWarning(
            const char * msg, ChaseObject * object )
    {
        std::cout << "WARNING:\t";
        baseMessage(msg, object, std::cerr);
    }

    void messageError( std::string msg, ChaseObject *object)
    {
        messageError(msg.c_str(), object);
    }

    void messageInfo(std::string msg, ChaseObject *object)
    {
        messageInfo(msg.c_str(), object);
    }

    void messageWarning(std::string msg, ChaseObject *object)
    {
        messageWarning(msg.c_str(), object);
    }

}
