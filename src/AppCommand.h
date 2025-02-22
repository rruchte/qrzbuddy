#ifndef QRZ_APPCOMMAND_H
#define QRZ_APPCOMMAND_H

#include <set>
#include <string>
#include <vector>
#include <functional>

#include "Action.h"
#include "model/Callsign.h"

namespace qrz
{
	/**
	 * @class AppCommand
	 * @brief Represents a command to be executed by the AppController
	 *
	 * This class stores information about an action, output format, and search terms for a command to be executed by
	 * the AppController.
	 */
	class AppCommand
	{
	public:
		AppCommand() = default;
		AppCommand(Action mAction, const std::set<std::string> &mSearchTerm);

		/**
		 * @brief Get the action of the command.
		 *
		 * This function returns the action associated with the command.
		 * The action determines the type of operation to be performed.
		 *
		 * @return The action of the command.
		 */
		Action getAction() const;


		/**
		 * @brief Set the action of the command.
		 *
		 * This function sets the action associated with the command.
		 * The action determines the type of operation to be performed.
		 *
		 * @param action The action to set for the command.
		 */
		void setAction(Action action);

		/**
		 * @brief Get the search terms associated with the command.
		 *
		 * This function returns a constant reference to the set of search terms associated with the command.
		 * The search terms are used to search for specific information related to the command.
		 *
		 * @return A constant reference to the set of search terms.
		 */
		const std::set<std::string> &getSearchTerms() const;

		/**
		 * @brief Set the search terms for the command.
		 *
		 * This function sets the search terms associated with the command. The search terms are used to search for
		 * specific information related to the command.
		 *
		 * @param searchTerms The search terms to set for the command.
		 */
		void setSearchTerms(const std::set<std::string> &searchTerms);

	private:
		// The action to be performed by the AppController
		Action m_action = Action::CALLSIGN_ACTION;

		// List of terms to be used in the QRZ API calls to fetch the relevant records
		std::set<std::string> m_searchTerms;
	};
}

#endif //QRZ_APPCOMMAND_H
