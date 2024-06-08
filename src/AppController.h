#ifndef QRZ_APPCONTROLLER_H
#define QRZ_APPCONTROLLER_H

#include <string>

#include <QObject>

#include "AppCommand.h"
#include "Configuration.h"
#include "QRZClient.h"
#include "Util.h"
#include "model/Callsign.h"
#include "model/DXCC.h"
#include "tablemodel.h"

namespace qrz
{
	typedef std::function<void(const Callsign &callsign)> QrzCallsignResponseCallback;

	class AppController : public QObject
	{
		Q_OBJECT

	public:
		explicit AppController(Configuration *config, TableModel *tableModel);

		/**
		 * @brief Initializes the application by setting up the necessary configurations and checking for authentication.
		 *
		 * This function is responsible for initializing the application. It performs various checks to verify that all
		 * necessary configuration information is in place, and if not, gathers the information from the user.
		 *
		 * This must be called externally after the signal/slot connections have been made
		 */
		void initialize();

		bool preflight();

		/**
		 * @brief Handles a command by performing an action based on the command's action type.
		 *
		 * This function takes an AppCommand object and performs different operations based on the command's action type.
		 * The action type determines the type of operation to be performed.
		 *
		 * @param command The AppCommand object representing the command to be executed.
		 */
		bool handleCommand(const AppCommand &command);

		std::string fetchBio(const std::string &call);

	public slots:
		void fetchCallsign(const std::string &call, QrzCallsignResponseCallback callback);
	signals:
		void credentialsNeeded();
		void displayError(const std::string &msg);

	protected:
		// The application configuration instance
		Configuration *config;

		// QRZ API client instance
		QRZClient client;

		// Counter for failed API calls
		int m_failedCallCount = 0;

		// Maximum number of consecutive API call failures allowed before bailing out of the operation
		static inline const int m_maxFailedCallCount = 4;

		// Flag to determine whether or not to display the progress bar
		bool displayProgress = false;


		/**
		 * @brief Fetches and renders callsigns based on the given search terms and output format.
		 *
		 * This function fetches the specified callsign records and renders them into the specified output format.
		 *
		 * @param searchTerms The set of search terms used to fetch the callsign records.
		 * @param format The output format in which the callsigns should be rendered.
		 * @return bool If an error was encountered, returns false
		 */
		bool fetchAndRenderCallsigns(const std::set<std::string> &searchTerms);

		/**
		 * @brief Fetches and renders bios based on the given search terms.
		 *
		 * This function fetches the specified callsign records and renders them.
		 *
		 * @param searchTerms The set of search terms used to fetch the callsign records.
		 * @return bool If an error was encountered, returns false
		 */
		bool fetchAndRenderBios(const std::set<std::string> &searchTerms);

		/**
		 * @brief Fetches and renders DXCC records based on the given search terms and output format.
		 *
		 * This function fetches the specified DXCC records and renders them into the specified output format.
		 *
		 * @param searchTerms The set of search terms used to fetch the DXCC records.
		 * @param format The output format in which the DXCC records should be rendered.
		 * @return bool If an error was encountered, returns false
		 */
		bool fetchAndRenderDXCC(const std::set<std::string> &searchTerms);

		/**
		 * @brief Fetches the callsign records based on the given search terms.
		 *
		 * This function fetches the callsign records based on the provided search terms.
		 * It also displays a progress bar to show the progress of fetching the callsigns.
		 *
		 * @param searchTerms The set of search terms used to fetch the callsign records.
		 * @return A vector of Callsign objects representing the fetched callsign records.
		 *
		 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
		 */
		std::vector<Callsign> fetchCallsignRecords(const std::set<std::string> &searchTerms);

		/**
		 * @brief Fetches DXCC records based on the given search terms.
		 *
		 * This function fetches DXCC records based on the provided search terms. It uses the QRZ API client to fetch the records
		 * for each term in the searchTerms set.
		 *
		 * @param searchTerms The set of search terms used to fetch the DXCC records.
		 * @return A vector of DXCC objects representing the fetched DXCC records.
		 */
		std::vector<DXCC> fetchDXCCRecords(const std::set<std::string> &searchTerms);

		/**
		 * @brief Fetches and returns a vector of bios based on the given search terms.
		 *
		 * This function fetches the specified bio records and returns a vector of bio HTML strings.
		 * It uses the QRZ API client to fetch the bios for each search term.
		 *
		 * @param searchTerms The set of search terms used to fetch the bios.
		 * @return A vector of strings representing the fetched bios.
		 *
		 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
		 */
		std::vector<std::string> fetchBios(const std::set<std::string> &searchTerms);

		/**
		 * @brief Refreshes the access token by fetching a new token from the QRZ API
		 *
		 * This function refreshes the access token by fetching a new token from the QRZ API.
		 *
		 * @note This function assumes that the necessary APIs and client objects are properly initialized before calling this function.
		 */
		bool refreshToken();

		/**
		 * @brief Retrieves the user's callsign from the user.
		 *
		 * This function prompts the user to enter their callsign and reads the input from the console.
		 *
		 * @return A string representing the user's callsign.
		 */
		static std::string getUserCallsignFromUser();

		/**
		 * @brief Retrieves the password from the user.
		 *
		 * This function prompts the user to enter their password and reads the input from the console.
		 *
		 * @return A string representing the user's password.
		 */
		std::string getPasswordFromUser();

		/**
		 * @brief Resets the counter for failed API calls.
		 *
		 * This function resets the counter for failed API calls.
		 */
		void resetFailedCallCount();

		/**
		 * @brief Updates the application configuration based on the state of the QRZ API client.
		 *
		 * This function updates the application configuration by setting the callsign, session key, and session expiration
		 * from the QRZ API client. It then saves the configuration.
		 *
		 * @note This function assumes that the QRZ API client and application configuration objects are properly initialized.
		 */
		void updateConfigFromClientState();

		/**
		 * @brief Changes the visibility of the console cursor.
		 *
		 * This function sets the visibility of the console cursor based on the value of the parameter `show`.
		 * If `show` is `true`, the console cursor will be displayed. If `show` is `false`, the console cursor will be hidden.
		 *
		 * @param show A boolean value indicating whether to show or hide the console cursor.
		 */
		void showConsoleCursor(const bool show);

		/**
		 * @brief Erases the current line in the console.
		 *
		 * This function erases the current line in the console, allowing for output to be overwritten.
		 */
		static void eraseLine();

	private:
		std::set<std::string> invalidCallsigns;
		TableModel *tableModel;
	};
}

#endif //QRZ_APPCONTROLLER_H
