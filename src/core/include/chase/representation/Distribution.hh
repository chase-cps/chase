/**
 * @author      <a href="mailto:michele.lora@univr.it">Michele Lora</a>
 * @date        12/16/2021
 *              This project is released under the 3-Clause BSD License.
 *
 */

#pragma once
#include "representation/DataDeclaration.hh"
#include "representation/Real.hh"

#include <set>

namespace chase {

    /// @brief Enumeration of types of distribution.
    enum distribution_type {
        custom,
        gaussian,
        homogeneous
    };

    /// @brief Array to map the name of the distributions.
    static const std::string distribution_type_names[] = {
            std::string("custom"),
            std::string("gaussian"),
            std::string("homogeneous")
    };

    ///@brief Class to represent statistical distributions.
    class Distribution : public DataDeclaration {
    public:

        /**
         * @brief Parameters characterizing the distribution. Each type of
         * distribution is characterized by a set of parameters. For instance,
         * a Gaussian distribution must specify sigma and mu parameters.
         */
        std::map< std::string, chase::Value * > parameters;

        /// @brief Constructor.
        Distribution(
                distribution_type dtype,
                Name * name = new Name("anonymous_distribution"),
                Type * type = new Real());

        /// @brief Destructor.
        ~Distribution();

        /// @brief Getter of the Distribution type.
        /// @return The distribution type.
        distribution_type getDistributionType() const;
        /// @brief Setter of the Distribution type.
        /// @param distributionType The distribution type.
        void setDistributionType(distribution_type distributionType);

        /// @brief Main function of the visit.
        /// @param v The visitor visiting the object.
        /// @return The return value of the visit.
        int accept_visitor(BaseVisitor &v) override;
        /// @brief Printing method for the class.
        /// @return String containing the textual representation of the object.
        std::string getString() override;
        /// @brief Clone function.
        /// @return The clone of the object.
        Distribution *clone() override;

    protected:

        /// @brief Type of distribution.
        distribution_type _distribution_type;

    };

}
